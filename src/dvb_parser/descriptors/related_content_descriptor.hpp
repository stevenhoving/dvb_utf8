#ifndef related_content_descriptor_hpp__
#define related_content_descriptor_hpp__

/*!
 * At least 2 descriptors have this basic form.
 */
struct basic_text_descriptor_base : descriptor
{
    explicit basic_text_descriptor_base(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        text = dvb_utf8::decode(
            dvb_utf8::stream_buffer(stream.read_buffer(length)));
    }
    std::string text;
};

using related_content_descriptor = basic_text_descriptor_base;
using network_name_descriptor = basic_text_descriptor_base;

#endif // related_content_descriptor_hpp__
