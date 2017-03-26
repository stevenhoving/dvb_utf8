#ifndef event_information_section_hpp__
#define event_information_section_hpp__

namespace dvb_parse
{

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

        while (!payload.eos())
            events.emplace_back(event(payload));
    }

    int transport_stream_id;
    int original_network_id;
    int segment_last_section_number;
    int last_table_id;

    std::vector<event> events;
};

} // namespace dvb_parse

#endif // event_information_section_hpp__
