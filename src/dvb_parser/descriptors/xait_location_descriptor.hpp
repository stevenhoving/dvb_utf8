#ifndef xait_location_descriptor_hpp__
#define xait_location_descriptor_hpp__

// \todo test with real data
struct xait_location_descriptor : descriptor
{
    explicit xait_location_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        auto original_network_id = payload.read<uint16_t>();
        auto service_id = payload.read<uint16_t>();
        uint8_t temp = payload.read<uint8_t>();
        auto versionNumber = (temp >> 3) & 0x1f;
        auto updatePolicy = temp & 0x03;
    }
};

#endif // xait_location_descriptor_hpp__
