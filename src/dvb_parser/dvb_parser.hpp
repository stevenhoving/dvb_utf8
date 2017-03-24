#ifndef dvb_parser_hpp__
#define dvb_parser_hpp__

#include <memory>
#include <array>

#include "dvb_utf8.h"
#include "dvb_description_tag.hpp"
#include "dvb_crc32.hpp"
#include "descriptors/descriptors.hpp"

namespace dvb_parse
{
struct short_section
{
    explicit short_section(const dvb_utf8::stream_span &stream)
    {
        table_id = stream.read<uint8_t>();
        auto flags = stream.read<uint8_t>();
        section_syntax_indicator = (flags >> 7) & 0x01;
        uint16_t section_length = (uint16_t)(flags & 0x0F) << 8 | stream.read<uint8_t>();
        section_length &= 0xFFF; // 12 bits

        // \note we do a bit of moving around here because for checking the
        // crc32 we need the complete section (including table_id, flags and
        // length).
        stream.seek(-3, SEEK_CUR); // move the read cursor to the start of the section
        payload = stream.read_buffer((section_length - 4) + 3); // exclude the crc32 (but include table_id, flags and length)
        payload.seek(3, SEEK_SET); // move the read cursor to the start of the section payload (skipping over table_id, flags and length)

        crc32 = stream.read<uint32_t>();

        if (dvb_validate_crc32(payload, crc32))
            DVB_PARSER_DBG("payload crc is good\n");
        else
            DVB_PARSER_DBG("payload crc is bad\n");

        DVB_PARSER_DBG("section length: 0x%X (%u)\n", section_length, section_length);
    }
    uint8_t table_id;
    uint8_t section_syntax_indicator;
    uint32_t crc32;

    // helper variables
    dvb_utf8::stream_span payload;
};

struct long_section : short_section
{
    explicit long_section(const dvb_utf8::stream_span &stream)
        : short_section(stream)
    {
        table_id_extension = payload.read<uint16_t>();
        auto flags = payload.read<uint8_t>();
        version_number = (flags >> 1) & 0x1f;
        current_next_indicator = flags & 0x01;
        section_number = payload.read<uint8_t>();
        last_section_number = payload.read<uint8_t>();
    }
    int table_id_extension;
    int version_number;
    int current_next_indicator;
    int section_number;
    int last_section_number;
};

struct unknown_descriptor : descriptor
{
    explicit unknown_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        printf("Unknown or unsupported descriptor token\n");
    }
};

    {
    }

};

void handle_dummy(const dvb_utf8::stream_span &stream, uint8_t descriptor_tag)
{
    stream.seek(0, SEEK_END); printf("unhandled descriptor: %u (0x%X)\n", descriptor_tag, descriptor_tag);
}

struct descriptor_container
{
    void read_descriptor(const dvb_utf8::stream_span &stream)
    {
        uint8_t descriptor_tag = stream.peek<uint8_t>();

        switch (descriptor_tag)
        {
        case VIDEO_STREAM_DESCRIPTOR: /* 0x02 */ handle_dummy(stream, descriptor_tag); break;
        case AUDIO_STREAM_DESCRIPTOR: /* 0x03 */ handle_dummy(stream, descriptor_tag); break;
        case HIERARCHY_DESCRIPTOR: /* 0x04 */ handle_dummy(stream, descriptor_tag); break;
        case REGISTRATION_DESCRIPTOR: /* 0x05 */ handle_dummy(stream, descriptor_tag); break;
        case DATA_STREAM_ALIGNMENT_DESCRIPTOR: /* 0x06 */ handle_dummy(stream, descriptor_tag); break;
        case TARGET_BACKGROUND_GRID_DESCRIPTOR: /* 0x07 */ handle_dummy(stream, descriptor_tag); break;
        case VIDEO_WINDOW_DESCRIPTOR: /* 0x08 */ handle_dummy(stream, descriptor_tag); break;
        case CA_DESCRIPTOR: /* 0x09 */ handle_dummy(stream, descriptor_tag); break;
        case ISO_639_LANGUAGE_DESCRIPTOR: /* 0x0A */ handle_dummy(stream, descriptor_tag); break;
        case SYSTEM_CLOCK_DESCRIPTOR: /* 0x0B */ handle_dummy(stream, descriptor_tag); break;
        case MULTIPLEX_BUFFER_UTILIZATION_DESCRIPTOR: /* 0x0C */ handle_dummy(stream, descriptor_tag); break;
        case COPYRIGHT_DESCRIPTOR: /* 0x0D */ handle_dummy(stream, descriptor_tag); break;
        case MAXIMUM_BITRATE_DESCRIPTOR: /* 0x0E */ handle_dummy(stream, descriptor_tag); break;
        case PRIVATE_DATA_INDICATOR_DESCRIPTOR: /* 0x0F */ handle_dummy(stream, descriptor_tag); break;
        case SMOOTHING_BUFFER_DESCRIPTOR: /* 0x10 */ handle_dummy(stream, descriptor_tag); break;
        case STD_DESCRIPTOR: /* 0x11 */ handle_dummy(stream, descriptor_tag); break;
        case IBP_DESCRIPTOR: /* 0x12 */ handle_dummy(stream, descriptor_tag); break;
        case CAROUSEL_IDENTIFIER_DESCRIPTOR: /* 0x13 */ handle_dummy(stream, descriptor_tag); break;

        case NETWORK_NAME_DESCRIPTOR: /* 0x40 */
            descriptors.emplace_back(std::make_unique<network_name_descriptor>(stream));
            break;
        case SERVICE_LIST_DESCRIPTOR: /* 0x41 */
            descriptors.emplace_back(std::make_unique<service_list_descriptor>(stream));
            break;
        case STUFFING_DESCRIPTOR: /* 0x42 */
            descriptors.emplace_back(std::make_unique<stuffing_descriptor>(stream));
            break;
        case SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x43 */
            descriptors.emplace_back(std::make_unique<satellite_delivery_system_descriptor>(stream));
            break;
        case CABLE_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x44 */
            descriptors.emplace_back(std::make_unique<cable_delivery_system_descriptor>(stream));
            break;
        case VBI_DATA_DESCRIPTOR: /*0x45*/
            descriptors.emplace_back(std::make_unique<vbi_data_descriptor>(stream));
            break;
        case VBI_TELETEXT_DESCRIPTOR: /* 0x46 */ handle_dummy(stream, descriptor_tag); break;
        case BOUQUET_NAME_DESCRIPTOR: /* 0x47 */ handle_dummy(stream, descriptor_tag); break;
        case SERVICE_DESCRIPTOR: /*0x48*/        descriptors.emplace_back(std::make_unique<service_descriptor>(stream)); break;
        case COUNTRY_AVAILABILITY_DESCRIPTOR: /*0x49*/ 
        case LINKAGE_DESCRIPTOR: /* 0x4A */ descriptors.emplace_back(std::make_unique<linkage_descriptor>(stream)); break;
        case NVOD_REFERENCE_DESCRIPTOR: /* 0x4B */ handle_dummy(stream, descriptor_tag); break;
        case TIME_SHIFTED_SERVICE_DESCRIPTOR: /* 0x4C */ handle_dummy(stream, descriptor_tag); break;
        case SHORT_EVENT_DESCRIPTOR: /*0x4D*/    descriptors.emplace_back(std::make_unique<short_event_descriptor>(stream)); break;
        case EXTENDED_EVENT_DESCRIPTOR: /*0x4E*/ descriptors.emplace_back(std::make_unique<extended_event_descriptor>(stream)); break;
        case TIME_SHIFTED_EVENT_DESCRIPTOR: /* 0x4F */ handle_dummy(stream, descriptor_tag); break;
        case COMPONENT_DESCRIPTOR: /* 0x50 */ handle_dummy(stream, descriptor_tag); break;
        case MOSAIC_DESCRIPTOR: /* 0x51 */ handle_dummy(stream, descriptor_tag); break;
        case STREAM_IDENTIFIER_DESCRIPTOR: /* 0x52 */ handle_dummy(stream, descriptor_tag); break;
        case CA_IDENTIFIER_DESCRIPTOR: /* 0x53 */ handle_dummy(stream, descriptor_tag); break;
        case CONTENT_DESCRIPTOR: /*0x54*/        descriptors.emplace_back(std::make_unique<content_descriptor>(stream)); break;
        case PARENTAL_RATING_DESCRIPTOR: /*0x55*/descriptors.emplace_back(std::make_unique<parental_rating_descriptor>(stream)); break;
        case TELETEXT_DESCRIPTOR: /* 0x56 */ handle_dummy(stream, descriptor_tag); break;
        case TELEPHONE_DESCRIPTOR: /* 0x57 */ handle_dummy(stream, descriptor_tag); break;
        case LOCAL_TIME_OFFSET_DESCRIPTOR: /* 0x58 */ handle_dummy(stream, descriptor_tag); break;
        case SUBTITLING_DESCRIPTOR: /* 0x59 */ handle_dummy(stream, descriptor_tag); break;
        case TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x5A */ handle_dummy(stream, descriptor_tag); break;
        case MULTILINGUAL_NETWORK_NAME_DESCRIPTOR: /* 0x5B */ handle_dummy(stream, descriptor_tag); break;
        case MULTILINGUAL_BOUQUET_NAME_DESCRIPTOR: /* 0x5C */ handle_dummy(stream, descriptor_tag); break;
        case MULTILINGUAL_SERVICE_NAME_DESCRIPTOR: /* 0x5D */ handle_dummy(stream, descriptor_tag); break;
        case MULTILINGUAL_COMPONENT_DESCRIPTOR: /* 0x5E */ handle_dummy(stream, descriptor_tag); break;
        case PRIVATE_DATA_SPECIFIER_DESCRIPTOR: /* 0x5F */ handle_dummy(stream, descriptor_tag); break;
        case SERVICE_MOVE_DESCRIPTOR: /* 0x60 */ handle_dummy(stream, descriptor_tag); break;
        case SHORT_SMOOTHING_BUFFER_DESCRIPTOR: /* 0x61 */ handle_dummy(stream, descriptor_tag); break;
        case FREQUENCY_LIST_DESCRIPTOR: /* 0x62 */ handle_dummy(stream, descriptor_tag); break;
        case PARTIAL_TRANSPORT_STREAM_DESCRIPTOR: /* 0x63 */ handle_dummy(stream, descriptor_tag); break;
        case DATA_BROADCAST_DESCRIPTOR: /* 0x64 */ handle_dummy(stream, descriptor_tag); break;
        case SCRAMBLING_DESCRIPTOR: /* 0x65 */ handle_dummy(stream, descriptor_tag); break;
        case DATA_BROADCAST_ID_DESCRIPTOR: /* 0x66 */ handle_dummy(stream, descriptor_tag); break;
        case TRANSPORT_STREAM_DESCRIPTOR: /* 0x67 */ handle_dummy(stream, descriptor_tag); break;
        case DSNG_DESCRIPTOR: /* 0x68 */ handle_dummy(stream, descriptor_tag); break;
        case PDC_DESCRIPTOR: /* 0x69 */ handle_dummy(stream, descriptor_tag); break;
        case AC3_DESCRIPTOR: /* 0x6A */ handle_dummy(stream, descriptor_tag); break;
        case ANCILLARY_DATA_DESCRIPTOR: /* 0x6B */ handle_dummy(stream, descriptor_tag); break;
        case CELL_LIST_DESCRIPTOR: /* 0x6C */ handle_dummy(stream, descriptor_tag); break;
        case CELL_FREQUENCY_LINK_DESCRIPTOR: /* 0x6D */ handle_dummy(stream, descriptor_tag); break;
        case ANNOUNCEMENT_SUPPORT_DESCRIPTOR: /* 0x6E */ handle_dummy(stream, descriptor_tag); break;
        case APPLICATION_SIGNALLING_DESCRIPTOR: /* 0x6F */ handle_dummy(stream, descriptor_tag); break;
        case ADAPTATION_FIELD_DATA_DESCRIPTOR: /* 0x70 */ handle_dummy(stream, descriptor_tag); break;
        case SERVICE_IDENTIFIER_DESCRIPTOR: /* 0x71 */ handle_dummy(stream, descriptor_tag); break;
        case SERVICE_AVAILABILITY_DESCRIPTOR: /* 0x72 */ handle_dummy(stream, descriptor_tag); break;
        case DEFAULT_AUTHORITY_DESCRIPTOR: /* 0x73 */ handle_dummy(stream, descriptor_tag); break;
        case RELATED_CONTENT_DESCRIPTOR: /*0x74*/
            descriptors.emplace_back(std::make_unique<related_content_descriptor>(stream));
            break;
        case TVA_ID_DESCRIPTOR: /* 0x75 */ handle_dummy(stream, descriptor_tag); break;
        case CONTENT_IDENTIFIER_DESCRIPTOR: /* 0x76 */ handle_dummy(stream, descriptor_tag); break;
        case TIME_SLICE_FEC_IDENTIFIER_DESCRIPTOR: /* 0x77 */ handle_dummy(stream, descriptor_tag); break;
        case ECM_REPETITION_RATE_DESCRIPTOR: /* 0x78 */ handle_dummy(stream, descriptor_tag); break;
        case S2_SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x79 */ handle_dummy(stream, descriptor_tag); break;
        case ENHANCED_AC3_DESCRIPTOR: /* 0x7A */ handle_dummy(stream, descriptor_tag); break;
        case DTS_DESCRIPTOR: /* 0x7B */ handle_dummy(stream, descriptor_tag); break;
        case AAC_DESCRIPTOR: /* 0x7C */ handle_dummy(stream, descriptor_tag); break;
        case XAIT_LOCATION_DESCRIPTOR: /* 0x7D */ handle_dummy(stream, descriptor_tag); break;
        case FTA_CONTENT_MANAGEMENT_DESCRIPTOR: /* 0x7E */ handle_dummy(stream, descriptor_tag); break;
        case EXTENSION_DESCRIPTOR: /* 0x7F */ handle_dummy(stream, descriptor_tag); break;
        case 0x80: stream.seek(SEEK_END, 0); break; // user defined
        case LOGICAL_CHANNEL_DESCRIPTOR: /* 0x83 */ handle_dummy(stream, descriptor_tag); break;
        case HD_SIMULCAST_LOGICAL_CHANNEL_DESCRIPTOR: /* 0x88 */ handle_dummy(stream, descriptor_tag); break;
        default:
            printf("unimplemented descriptor tag: 0x%X (%u)\n", descriptor_tag, descriptor_tag);
            __debugbreak();
        break;
        }
    }

    std::vector<std::unique_ptr<descriptor>> descriptors;
};

struct event : descriptor_container
{
    explicit event(const dvb_utf8::stream_span &stream)
    {
        event_id = stream.read<uint16_t>();
        start_time =
              (uint64_t)stream.read<uint8_t>() << 32
            | (uint64_t)stream.read<uint8_t>() << 24
            | (uint64_t)stream.read<uint8_t>() << 16
            | (uint64_t)stream.read<uint8_t>() << 8
            | (uint64_t)stream.read<uint8_t>();
        duration =
              (uint32_t)stream.read<uint8_t>() << 16
            | (uint32_t)stream.read<uint8_t>() << 8
            | (uint32_t)stream.read<uint8_t>();

        status = stream.read<uint8_t>();
        running_status = status >> 5;
        free_ca = (status & 0x10) == 0x10;
        event_length = (uint16_t)(status & 0xF) << 8 | stream.read<uint8_t>();

        auto descriptor_stream = stream.read_buffer(event_length);
        while (!descriptor_stream.eos())
            read_descriptor(descriptor_stream);
    }

    uint16_t event_id;
    uint64_t start_time;
    uint32_t duration;
    uint8_t status;
    uint8_t running_status;
    bool free_ca;
    uint16_t event_length;
};

struct event_information_section : long_section
{
    explicit event_information_section(const dvb_utf8::stream_span &stream)
        : long_section(stream)
    {
        transport_stream_id = payload.read<uint16_t>();
        original_network_id = payload.read<uint16_t>();
        segment_last_section_number = payload.read<uint8_t>();
        last_table_id = payload.read<uint8_t>();

        while(!payload.eos())
            events.emplace_back(event(payload));
    }

    int transport_stream_id;
    int original_network_id;
    int segment_last_section_number;
    int last_table_id;

    std::vector<event> events;
};

struct service_description : descriptor_container
{
    explicit service_description(const dvb_utf8::stream_span &stream)
    {
        service_id = stream.read<uint16_t>();
        auto flags = stream.read<uint8_t>();
        auto status = stream.read<uint8_t>();

        eit_schedule_flag = (flags >> 1) & 0x01;
        eit_present_following_flag = flags & 0x01;

        running_status = (status >> 5) & 0x07;
        free_ca_mode = (status >> 4) & 0x01;
        descriptors_loop_length = ((status & 0x0F) << 8) | stream.read<uint8_t>();
        auto descriptor_stream = stream.read_buffer(descriptors_loop_length);
        while(!descriptor_stream.eos())
            read_descriptor(descriptor_stream);
    }

    uint16_t service_id;
    uint8_t eit_schedule_flag;
    uint8_t eit_present_following_flag;
    uint8_t running_status;
    uint8_t free_ca_mode;
    uint16_t descriptors_loop_length;
};

struct service_description_section : long_section
{
    explicit service_description_section(const dvb_utf8::stream_span &stream)
        : long_section(stream)
    {
        original_network_id = payload.read<uint16_t>();
        uint8_t reserved = payload.read<uint8_t>();
        (void)reserved;

        while (!payload.eos())
            descriptions.emplace_back(service_description(payload));
    }

    uint16_t original_network_id;
    std::vector<service_description> descriptions;
};

struct transport_stream : descriptor_container
{
    transport_stream(const dvb_utf8::stream_span &stream)
    {
        uint16_t transport_stream_id = stream.read<uint16_t>();
        uint16_t original_network_ud = stream.read<uint16_t>();
        uint8_t temp = stream.read<uint8_t>();
        uint32_t transport_descriptors_length = (temp & 0x0F) << 8 | stream.read<uint8_t>();
        auto transport_descriptors_stream = stream.read_buffer(transport_descriptors_length);
        while (!transport_descriptors_stream.eos())
            read_descriptor(transport_descriptors_stream);
    }
};


struct network_information_section : long_section, descriptor_container
{
    explicit network_information_section(const dvb_utf8::stream_span &stream)
        : long_section(stream)
    {
        uint8_t temp = payload.read<uint8_t>();
        uint32_t network_descriptors_length = (temp & 0x0F) << 8 | payload.read<uint8_t>();
        auto descriptor_stream = payload.read_buffer(network_descriptors_length);
        while (!descriptor_stream.eos())
            read_descriptor(descriptor_stream);

        temp = payload.read<uint8_t>();
        uint32_t transport_stream_loop_length = (temp & 0x0F) << 8 | payload.read<uint8_t>();
        auto transport_stream_stream = payload.read_buffer(transport_stream_loop_length);
        while (!transport_stream_stream.eos())
            transport_streams.emplace_back(transport_stream(transport_stream_stream));
    }

    std::vector<transport_stream> transport_streams;
};

} // namespace dvb_parse

#endif // dvb_parser_hpp__
