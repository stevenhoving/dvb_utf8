#ifndef cable_delivery_system_descriptor_hpp__
#define cable_delivery_system_descriptor_hpp__

struct cable_delivery_system_descriptor : descriptor
{
    explicit cable_delivery_system_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        frequency = payload.read<uint32_t>();
        fec_outer = payload.read<uint16_t>(); // \note only the lower 4 bits
        modulation = payload.read<uint8_t>();
        uint32_t temp = payload.read<uint32_t>();
        symbol_rate = temp >> 4;
        fec_inner = temp & 0xF;
    }

    uint32_t frequency;
    uint16_t fec_outer;
    uint8_t modulation;
    uint32_t symbol_rate;
    uint8_t fec_inner;
};

#endif // cable_delivery_system_descriptor_hpp__
