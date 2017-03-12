#ifndef short_event_descriptor_hpp__
#define short_event_descriptor_hpp__

struct short_event_descriptor : descriptor
{
    explicit short_event_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        iso_639_language_code =
              (uint32_t)stream.read<uint8_t>() << 16
            | (uint32_t)stream.read<uint8_t>() << 8
            | (uint32_t)stream.read<uint8_t>();

        auto event_name_length = stream.read<uint8_t>();
        event_name = dvb_utf8::decode(
            dvb_utf8::stream_buffer(stream.read_buffer(event_name_length)));

        auto text_length = stream.read<uint8_t>();
        text = dvb_utf8::decode(
            dvb_utf8::stream_buffer(stream.read_buffer(text_length)));

        //printf("event name: %s\n", event_name.c_str());
        //printf("event text: %s\n", text.c_str());
    }

    uint32_t iso_639_language_code;
    std::string event_name;
    std::string text;
};

#endif // short_event_descriptor_hpp__
