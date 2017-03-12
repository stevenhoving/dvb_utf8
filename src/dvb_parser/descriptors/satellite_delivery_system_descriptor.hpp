#ifndef satellite_delivery_system_descriptor_hpp__
#define satellite_delivery_system_descriptor_hpp__

/* \todo the member variable flags contain multiple flags, see spec regarding
 * this.
 */
struct satellite_delivery_system_descriptor : descriptor
{
    explicit satellite_delivery_system_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        frequency = stream.read<uint32_t>();
        orbital_position = stream.read<uint16_t>();
        flags = stream.read<uint8_t>();
        uint32_t temp = stream.read<uint32_t>();
        fec_inner = temp & 0xF;
        symbol_rate = temp >> 4;
    }
    uint32_t frequency;
    uint16_t orbital_position;
    uint8_t flags;
    uint8_t fec_inner;
    uint32_t symbol_rate;
};

#endif // satellite_delivery_system_descriptor_hpp__
