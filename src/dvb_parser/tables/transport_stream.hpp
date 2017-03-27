#ifndef transport_stream_hpp__
#define transport_stream_hpp__

namespace dvb_parse
{

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

} // namespace dvb_parse

#endif // transport_stream_hpp__
