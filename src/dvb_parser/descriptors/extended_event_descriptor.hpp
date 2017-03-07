#ifndef extended_event_descriptor_hpp__
#define extended_event_descriptor_hpp__

struct extended_event
{
    extended_event(const dvb_utf8::stream_buffer &stream)
    {
        auto item_description_length = stream.read<uint8_t>();
        item_description = dvb_utf8::decode(
            dvb_utf8::stream_buffer(stream.read_buffer(item_description_length)));

        auto item_length = stream.read<uint8_t>();
        item = dvb_utf8::decode(
            dvb_utf8::stream_buffer(stream.read_buffer(item_length)));

        printf("extended_event - desc: %s, item: %s\n", item_description.c_str(), item.c_str());
    }

    std::string item_description;
    std::string item;
};

struct extended_event_descriptor : descriptor
{
    extended_event_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        auto temp = stream.read<uint8_t>();

        descriptor_number = (temp & 0xF0) >> 4;
        last_descriptor_number = temp & 0x0F;
        iso_639_language_code =
              (uint32_t)stream.read<uint8_t>() << 16
            | (uint32_t)stream.read<uint8_t>() << 8
            | (uint32_t)stream.read<uint8_t>();

        auto length_of_items = stream.read<uint8_t>();
        stream.range_set(length_of_items);
        while (!stream.range_eos())
            items.emplace_back(extended_event(stream));
        stream.range_mark_end();

        auto text_length = stream.read<uint8_t>();
        text = dvb_utf8::decode(
            dvb_utf8::stream_buffer(stream.read_buffer(text_length)));

        printf("extended_event_descriptor: %s\n", text.c_str());
    }

    uint8_t descriptor_number;
    uint8_t last_descriptor_number;
    uint32_t iso_639_language_code;
    std::vector<extended_event> items;
    std::string text;
};
#endif // extended_event_descriptor_hpp__
