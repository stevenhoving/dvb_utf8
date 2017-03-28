#ifndef video_window_descriptor_hpp__
#define video_window_descriptor_hpp__

// \todo test with real data
struct video_window_descriptor : descriptor
{
    explicit video_window_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        uint32_t temp = payload.read<uint32_t>();
        auto horizontalOffset = (temp >> 18) & 0x3fff;
        auto verticalOffset = (temp >> 4) & 0x3fff;
        auto windowPriority = temp & 0x000F;
    }
};

#endif // video_window_descriptor_hpp__
