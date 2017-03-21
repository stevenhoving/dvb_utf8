#ifndef extended_event_descriptor_hpp__
#define extended_event_descriptor_hpp__

struct extended_event
{
    explicit extended_event(const dvb_utf8::stream_span &stream)
    {
        auto item_description_length = stream.read<uint8_t>();
        item_description = dvb_utf8::decode(
            stream.read_buffer(item_description_length));

        auto item_length = stream.read<uint8_t>();
        item = dvb_utf8::decode(
            stream.read_buffer(item_length));

        printf("extended_event - desc: %s, item: %s\n", item_description.c_str(), item.c_str());
    }

    std::string item_description;
    std::string item;
};

struct extended_event_descriptor : descriptor
{
    explicit extended_event_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        const auto temp = payload.read<uint8_t>();

        descriptor_number = (temp & 0xF0) >> 4;
        last_descriptor_number = temp & 0x0F;
        iso_639_language_code =
              (uint32_t)payload.read<uint8_t>() << 16
            | (uint32_t)payload.read<uint8_t>() << 8
            | (uint32_t)payload.read<uint8_t>();

        const auto length_of_items = payload.read<uint8_t>();
        const auto item_stream = payload.read_buffer(length_of_items);
        while (!item_stream.eos())
            items.emplace_back(extended_event(item_stream));

        const auto text_length = payload.read<uint8_t>();
        text = dvb_utf8::decode(
            payload.read_buffer(text_length));

        DVB_PARSER_DBG("extended_event_descriptor: %s\n", text.c_str());
    }

    uint8_t descriptor_number;
    uint8_t last_descriptor_number;
    uint32_t iso_639_language_code;
    std::vector<extended_event> items;
    std::string text;
};
#endif // extended_event_descriptor_hpp__
