#ifndef dvb_parser_h__
#define dvb_parser_h__

#include <memory>

#include "dvb_utf8.h"
#include "dvb_description_tag.h"
#include "dvb_descriptors.h"

namespace dvb_parse
{
struct short_section
{
    short_section(const dvb_utf8::stream_buffer &stream)
    {
        tableId = stream.read<uint8_t>();
        auto flags = stream.read<uint8_t>();
        section_syntax_indicator = (flags >> 7) & 0x01;
        section_length = (uint16_t)(flags & 0x0F) << 8 | stream.read<uint8_t>();
        section_length &= 0xFFF; // 12 bits

        section_start = stream.tell();
        // this is including the 4 bytes crc32
        section_stream_end = stream.tell() + section_length;

        printf("section length: 0x%X (%u)\n", section_length, section_length);
    }
    int tableId;
    int section_syntax_indicator;
    int section_length;

    // helper variables
    int section_start;
    int section_stream_end;
};

struct long_section : short_section
{
    long_section(const dvb_utf8::stream_buffer &stream)
        : short_section(stream)
    {
        table_id_extension = stream.read<uint16_t>();
        auto flags = stream.read<uint8_t>();
        version_number = (flags >> 1) & 0x1f;
        current_next_indicator = flags & 0x01;
        section_number = stream.read<uint8_t>();
        last_section_number = stream.read<uint8_t>();
    }
    int table_id_extension;
    int version_number;
    int current_next_indicator;
    int section_number;
    int last_section_number;
};

struct long_crc_section : long_section
{
    long_crc_section(const dvb_utf8::stream_buffer &data)
        : long_section(data)
        , crc32(0)
    {
        crc32_pos = section_start + section_length - 4;
    }

    /* this is kinda fucked up, usually the crc is located at the end of the stream
    * the problem with my design is that it is a streamed parsing design. So we could check
    * if we are reading the crc from the correct location and then throw a error if we don't
    * and by doing so protect against incorrectly read dvb things
    */
    void read_crc(const dvb_utf8::stream_buffer &stream)
    {
        if (stream.tell() != crc32_pos)
            throw std::runtime_error("stream not in the correct position to read crc32, parsing the data went wrong");
        crc32 = stream.read<uint32_t>();
    }

    uint32_t crc32;
    int crc32_pos;
};

struct descriptor_container
{
    void read_descriptor(const dvb_utf8::stream_buffer &stream)
    {
        uint8_t descriptor_tag = stream.peek<uint8_t>();
        switch (descriptor_tag)
        {
        case VBI_DATA_DESCRIPTOR: // 0x45
            printf("vbi_data_descriptor\n");
            descriptors.emplace_back(std::make_unique<vbi_data_descriptor>(stream));
            break;
        case SERVICE_DESCRIPTOR: // 0x48
            printf("service_descriptor\n");
            descriptors.emplace_back(std::make_unique<service_descriptor>(stream));
            break;
        case COUNTRY_AVAILABILITY_DESCRIPTOR: // 0x49
            printf("country_availability_descriptor\n");
            descriptors.emplace_back(std::make_unique<country_availability_descriptor>(stream));
            break;
        case SHORT_EVENT_DESCRIPTOR: // 0x4D
            printf("short_event_descriptor\n");
            descriptors.emplace_back(std::make_unique<short_event_descriptor>(stream));
            break;
        case EXTENDED_EVENT_DESCRIPTOR: // 0x4E
            printf("extended_event_descriptor\n");
            descriptors.emplace_back(std::make_unique<extended_event_descriptor>(stream));
            break;
        case CONTENT_DESCRIPTOR: // 0x54
            printf("content_descriptor\n");
            descriptors.emplace_back(std::make_unique<content_descriptor>(stream));
            break;
        case PARENTAL_RATING_DESCRIPTOR: // 0x55
            printf("parental_rating_descriptor\n");
            descriptors.emplace_back(std::make_unique<parental_rating_descriptor>(stream));
            break;
        case RELATED_CONTENT_DESCRIPTOR: // 0x74
            printf("related_content_descriptor\n");
            descriptors.emplace_back(std::make_unique<related_content_descriptor>(stream));
            break;
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
    event(const dvb_utf8::stream_buffer &stream)
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

        auto pos_end = stream.tell() + event_length;

        while (stream.tell() < pos_end)
            read_descriptor(stream);
    }

    uint16_t event_id;
    uint64_t start_time;
    uint32_t duration;
    uint8_t status;
    uint8_t running_status;
    bool free_ca;
    uint16_t event_length;
};

struct event_information_section : long_crc_section
{
    event_information_section(const dvb_utf8::stream_buffer &stream)
        : long_crc_section(stream)
    {
        transport_stream_id = stream.read<uint16_t>();
        original_network_id = stream.read<uint16_t>();
        segment_last_section_number = stream.read<uint8_t>();
        last_table_id = stream.read<uint8_t>();

        while (stream.tell() < (size_t)section_stream_end-4)
            events.emplace_back(event(stream));

        read_crc(stream);
    }

    int transport_stream_id;
    int original_network_id;
    int segment_last_section_number;
    int last_table_id;

    std::vector<event> events;
};

struct service_description : descriptor_container
{
    service_description(const dvb_utf8::stream_buffer &stream)
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

        // \todo double check, but my gut feeling is that this is wrong.
        if (descriptors_loop_length > 5)
            descriptors_loop_length -= 5;

        auto pos_end = stream.tell() + descriptors_loop_length;

        while (stream.tell() < pos_end)
            read_descriptor(stream);
    }

    uint16_t serviceId;
    uint8_t eit_schedule_flag;
    uint8_t eit_present_following_flag;
    uint8_t running_status;
    uint8_t free_ca_mode;
    uint16_t descriptors_loop_length;
};

struct service_description_section : long_crc_section
{
    service_description_section(const dvb_utf8::stream_buffer &stream)
        : long_crc_section(stream)
    {
        original_network_id = stream.read<uint16_t>();
        uint8_t reserved = stream.read<uint8_t>();

        while (stream.tell() < section_stream_end - 4)
            descriptions.emplace_back(service_description(stream));

        read_crc(stream);
    }

    uint16_t original_network_id;
    std::vector<service_description> descriptions;
};

} // namespace dvb_parse

#endif // dvb_parser_h__
