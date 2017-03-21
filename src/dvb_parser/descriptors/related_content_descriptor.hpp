#ifndef related_content_descriptor_hpp__
#define related_content_descriptor_hpp__

/*!
 * At least 2 descriptors have this basic form.
 */
struct basic_text_descriptor_base : descriptor
{
    explicit basic_text_descriptor_base(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        text = dvb_utf8::decode(payload);
    }
    std::string text;
};

using related_content_descriptor = basic_text_descriptor_base;
using network_name_descriptor = basic_text_descriptor_base;

#endif // related_content_descriptor_hpp__
