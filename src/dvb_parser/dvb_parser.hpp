#ifndef dvb_parser_hpp__
#define dvb_parser_hpp__

#include <memory>
#include <array>

#include "dvb_utf8.h"
#include "descriptors/descriptors.hpp"
#include "tables/tables.hpp"

namespace dvb_parse
{



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
