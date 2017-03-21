#ifndef dvb_parser_hpp__
#define dvb_parser_hpp__

#include <memory>
#include <array>

#include "dvb_utf8.h"
#include "dvb_description_tag.hpp"
#include "descriptors/descriptors.hpp"

namespace dvb_parse
{
struct short_section
{
    explicit short_section(const dvb_utf8::stream_span &stream)
    {
        tableId = stream.read<uint8_t>();
        auto flags = stream.read<uint8_t>();
        section_syntax_indicator = (flags >> 7) & 0x01;
        uint16_t section_length = (uint16_t)(flags & 0x0F) << 8 | stream.read<uint8_t>();
        section_length &= 0xFFF; // 12 bits

        payload = stream.read_buffer(section_length - 4); // excluding the crc32
        crc32 = stream.read<uint32_t>();

        printf("section length: 0x%X (%u)\n", section_length, section_length);
    }
    uint8_t tableId;
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

struct descriptor_container
{
    void read_descriptor(const dvb_utf8::stream_span &stream)
    {
        uint8_t descriptor_tag = stream.peek<uint8_t>();

        switch (descriptor_tag)
        {
        case VIDEO_STREAM_DESCRIPTOR: /* 0x02 */ break;
        case AUDIO_STREAM_DESCRIPTOR: /* 0x03 */ break;
        case HIERARCHY_DESCRIPTOR: /* 0x04 */ break;
        case REGISTRATION_DESCRIPTOR: /* 0x05 */ break;
        case DATA_STREAM_ALIGNMENT_DESCRIPTOR: /* 0x06 */ break;
        case TARGET_BACKGROUND_GRID_DESCRIPTOR: /* 0x07 */ break;
        case VIDEO_WINDOW_DESCRIPTOR: /* 0x08 */ break;
        case CA_DESCRIPTOR: /* 0x09 */ break;
        case ISO_639_LANGUAGE_DESCRIPTOR: /* 0x0A */ break;
        case SYSTEM_CLOCK_DESCRIPTOR: /* 0x0B */ break;
        case MULTIPLEX_BUFFER_UTILIZATION_DESCRIPTOR: /* 0x0C */ break;
        case COPYRIGHT_DESCRIPTOR: /* 0x0D */ break;
        case MAXIMUM_BITRATE_DESCRIPTOR: /* 0x0E */ break;
        case PRIVATE_DATA_INDICATOR_DESCRIPTOR: /* 0x0F */ break;
        case SMOOTHING_BUFFER_DESCRIPTOR: /* 0x10 */ break;
        case STD_DESCRIPTOR: /* 0x11 */ break;
        case IBP_DESCRIPTOR: /* 0x12 */ break;
        case CAROUSEL_IDENTIFIER_DESCRIPTOR: /* 0x13 */ break;

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
        case VBI_TELETEXT_DESCRIPTOR: /* 0x46 */ break;
        case BOUQUET_NAME_DESCRIPTOR: /* 0x47 */ break;
        case SERVICE_DESCRIPTOR: /*0x48*/        descriptors.emplace_back(std::make_unique<service_descriptor>(stream)); break;
        case COUNTRY_AVAILABILITY_DESCRIPTOR: /*0x49*/ descriptors.emplace_back(std::make_unique<country_availability_descriptor>(stream)); break;
        case LINKAGE_DESCRIPTOR: /* 0x4A */ break;
        case NVOD_REFERENCE_DESCRIPTOR: /* 0x4B */ break;
        case TIME_SHIFTED_SERVICE_DESCRIPTOR: /* 0x4C */ break;
        case SHORT_EVENT_DESCRIPTOR: /*0x4D*/    descriptors.emplace_back(std::make_unique<short_event_descriptor>(stream)); break;
        case EXTENDED_EVENT_DESCRIPTOR: /*0x4E*/ descriptors.emplace_back(std::make_unique<extended_event_descriptor>(stream)); break;
        case TIME_SHIFTED_EVENT_DESCRIPTOR: /* 0x4F */ break;
        case COMPONENT_DESCRIPTOR: /* 0x50 */ break;
        case MOSAIC_DESCRIPTOR: /* 0x51 */ break;
        case STREAM_IDENTIFIER_DESCRIPTOR: /* 0x52 */ break;
        case CA_IDENTIFIER_DESCRIPTOR: /* 0x53 */ break;
        case CONTENT_DESCRIPTOR: /*0x54*/        descriptors.emplace_back(std::make_unique<content_descriptor>(stream)); break;
        case PARENTAL_RATING_DESCRIPTOR: /*0x55*/descriptors.emplace_back(std::make_unique<parental_rating_descriptor>(stream)); break;
        case TELETEXT_DESCRIPTOR: /* 0x56 */ break;
        case TELEPHONE_DESCRIPTOR: /* 0x57 */ break;
        case LOCAL_TIME_OFFSET_DESCRIPTOR: /* 0x58 */ break;
        case SUBTITLING_DESCRIPTOR: /* 0x59 */ break;
        case TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x5A */ break;
        case MULTILINGUAL_NETWORK_NAME_DESCRIPTOR: /* 0x5B */ break;
        case MULTILINGUAL_BOUQUET_NAME_DESCRIPTOR: /* 0x5C */ break;
        case MULTILINGUAL_SERVICE_NAME_DESCRIPTOR: /* 0x5D */ break;
        case MULTILINGUAL_COMPONENT_DESCRIPTOR: /* 0x5E */ break;
        case PRIVATE_DATA_SPECIFIER_DESCRIPTOR: /* 0x5F */ break;
        case SERVICE_MOVE_DESCRIPTOR: /* 0x60 */ break;
        case SHORT_SMOOTHING_BUFFER_DESCRIPTOR: /* 0x61 */ break;
        case FREQUENCY_LIST_DESCRIPTOR: /* 0x62 */ break;
        case PARTIAL_TRANSPORT_STREAM_DESCRIPTOR: /* 0x63 */ break;
        case DATA_BROADCAST_DESCRIPTOR: /* 0x64 */ break;
        case SCRAMBLING_DESCRIPTOR: /* 0x65 */ break;
        case DATA_BROADCAST_ID_DESCRIPTOR: /* 0x66 */ break;
        case TRANSPORT_STREAM_DESCRIPTOR: /* 0x67 */ break;
        case DSNG_DESCRIPTOR: /* 0x68 */ break;
        case PDC_DESCRIPTOR: /* 0x69 */ break;
        case AC3_DESCRIPTOR: /* 0x6A */ break;
        case ANCILLARY_DATA_DESCRIPTOR: /* 0x6B */ break;
        case CELL_LIST_DESCRIPTOR: /* 0x6C */ break;
        case CELL_FREQUENCY_LINK_DESCRIPTOR: /* 0x6D */ break;
        case ANNOUNCEMENT_SUPPORT_DESCRIPTOR: /* 0x6E */ break;
        case APPLICATION_SIGNALLING_DESCRIPTOR: /* 0x6F */ break;
        case ADAPTATION_FIELD_DATA_DESCRIPTOR: /* 0x70 */ break;
        case SERVICE_IDENTIFIER_DESCRIPTOR: /* 0x71 */ break;
        case SERVICE_AVAILABILITY_DESCRIPTOR: /* 0x72 */ break;
        case DEFAULT_AUTHORITY_DESCRIPTOR: /* 0x73 */ break;
        case RELATED_CONTENT_DESCRIPTOR: /*0x74*/
            descriptors.emplace_back(std::make_unique<related_content_descriptor>(stream));
            break;
        case TVA_ID_DESCRIPTOR: /* 0x75 */ break;
        case CONTENT_IDENTIFIER_DESCRIPTOR: /* 0x76 */ break;
        case TIME_SLICE_FEC_IDENTIFIER_DESCRIPTOR: /* 0x77 */ break;
        case ECM_REPETITION_RATE_DESCRIPTOR: /* 0x78 */ break;
        case S2_SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x79 */ break;
        case ENHANCED_AC3_DESCRIPTOR: /* 0x7A */ break;
        case DTS_DESCRIPTOR: /* 0x7B */ break;
        case AAC_DESCRIPTOR: /* 0x7C */ break;
        case XAIT_LOCATION_DESCRIPTOR: /* 0x7D */ break;
        case FTA_CONTENT_MANAGEMENT_DESCRIPTOR: /* 0x7E */ break;
        case EXTENSION_DESCRIPTOR: /* 0x7F */ break;
        case LOGICAL_CHANNEL_DESCRIPTOR: /* 0x83 */ break;
        case HD_SIMULCAST_LOGICAL_CHANNEL_DESCRIPTOR: /* 0x88 */ break;
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
        serviceId = stream.read<uint16_t>();
        auto flags = stream.read<uint8_t>();
        auto status = stream.read<uint8_t>();

        eit_schedule_flag = (flags >> 1) & 0x01;
        eit_present_following_flag = flags & 0x01;

        running_status = (status >> 5) & 0x07;
        free_ca_mode = (status >> 4) & 0x01;
        descriptors_loop_length =
              ((status & 0x0F) << 8)
            | stream.read<uint8_t>();
        auto descriptor_stream = stream.read_buffer(descriptors_loop_length);
        while(!descriptor_stream.eos())
            read_descriptor(descriptor_stream);
    }

    uint16_t serviceId;
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
            descriptions.emplace_back(service_description(stream));
    }

    uint16_t original_network_id;
    std::vector<service_description> descriptions;
};

struct network_information_section : long_section
{
    explicit network_information_section(const dvb_utf8::stream_span &stream)
        : long_section(stream)
    {
        __debugbreak();
    }
};

} // namespace dvb_parse

#endif // dvb_parser_hpp__
