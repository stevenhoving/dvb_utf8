#ifndef related_content_descriptor_hpp__
#define related_content_descriptor_hpp__

struct related_content_descriptor : descriptor
{
    related_content_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        content_description = dvb_utf8::decode(
            dvb_utf8::stream_buffer(stream.read_buffer(length)));

        printf("related_content_descriptor: %s\n", content_description.c_str());
    }
    std::string content_description;
};
#endif // related_content_descriptor_hpp__
