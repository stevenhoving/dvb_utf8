#ifndef descriptor_hpp__
#define descriptor_hpp__

struct descriptor
{
    explicit descriptor(const dvb_utf8::stream_span &stream)
    {
        tag = stream.read<uint8_t>();
        length = stream.read<uint8_t>();

        payload = stream.read_buffer(length);
    }
    uint8_t tag;
    uint8_t length;
    dvb_utf8::stream_span payload;
};

#endif // descriptor_hpp__
