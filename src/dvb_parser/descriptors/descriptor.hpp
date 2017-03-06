#ifndef descriptor_hpp__
#define descriptor_hpp__

struct descriptor
{
    descriptor(const dvb_utf8::stream_buffer &stream)
    {
        tag = stream.read<uint8_t>();
        length = stream.read<uint8_t>();

        // \note hack to allow for easier sub stream processing.
        stream.range_set(length);
    }
    uint8_t tag;
    uint8_t length;
};
#endif // descriptor_hpp__
