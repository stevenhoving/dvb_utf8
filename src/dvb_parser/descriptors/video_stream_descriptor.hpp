#ifndef video_stream_descriptor_hpp__
#define video_stream_descriptor_hpp__

// \todo test with real data
struct video_stream_descriptor : descriptor
{
    explicit video_stream_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        uint8_t temp = payload.read<uint8_t>();
        auto multipleFrameRateFlag = (temp >> 7) & 0x01;
        auto frameRateCode = (temp >> 3) & 0x0F;
        auto mpeg1OnlyFlag = (temp >> 2) & 0x01;
        auto constrainedParameterFlag = (temp >> 1) & 0x01;

        if (!mpeg1OnlyFlag) {
            auto profileAndLevelIndication = payload.read<uint8_t>();
            temp = payload.read<uint8_t>();
            auto chromaFormat = (temp >> 6) & 0x03;
            auto frameRateExtensionFlag = (temp >> 5) & 0x01;
        }
    }
};

#endif // video_stream_descriptor_hpp__
