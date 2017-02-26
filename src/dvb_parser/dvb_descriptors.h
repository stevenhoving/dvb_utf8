#ifndef dvb_descriptors_h__
#define dvb_descriptors_h__

struct descriptor
{
    descriptor(const dvb_utf8::stream_buffer &stream)
    {
        tag = stream.read<uint8_t>();
        length = stream.read<uint8_t>();

        stream.range_set(length);
    }
    uint8_t tag;
    uint8_t length;
};

struct short_event_descriptor : descriptor
{
    short_event_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        iso_639_language_code =
              (uint32_t)stream.read<uint8_t>() << 16
            | (uint32_t)stream.read<uint8_t>() << 8
            | (uint32_t)stream.read<uint8_t>();

        auto event_name_length = stream.read<uint8_t>();
        if (event_name_length)
            event_name = dvb_utf8::decode(
                dvb_utf8::stream_buffer(stream.read_buffer(event_name_length)));

        auto text_length = stream.read<uint8_t>();
        if (text_length)
            text = dvb_utf8::decode(
                dvb_utf8::stream_buffer(stream.read_buffer(text_length)));

        printf("event name: %s\n", event_name.c_str());
        printf("event text: %s\n", text.c_str());
    }

    uint32_t iso_639_language_code;
    std::string event_name;
    std::string text;
};

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

struct vbi_data_line
{
    vbi_data_line(const dvb_utf8::stream_buffer &stream)
    {
        auto data = stream.read<uint8_t>();
        fieldParity = (data >> 5) & 0x01;
        lineOffset = data & 0x1F;
    }

    // todo replace this with just a normal uint8_t and with getter functions.
    uint8_t fieldParity : 1;
    uint8_t lineOffset : 5;
};

struct vbi_data_service
{
    vbi_data_service(const dvb_utf8::stream_buffer &stream)
    {
        data_service_id = stream.read<uint8_t>();
        data_service_descriptor_length = stream.read<uint8_t>();

        switch (data_service_id)
        {
        case 0x01:
        case 0x02:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            for (unsigned int i = 0; i < data_service_descriptor_length; ++i)
                vbi_data_lines.emplace_back(vbi_data_line(stream));
            break;

        default:
            // skip stuff that is 'reserved'
            stream.seek(data_service_descriptor_length, SEEK_CUR);
            printf("vbi_data_service, reserved id\n");
            break;
        }
    }

    uint8_t data_service_id;
    uint8_t data_service_descriptor_length;
    std::vector<vbi_data_line> vbi_data_lines;
};

struct vbi_data_descriptor : descriptor
{
    vbi_data_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        while (!stream.range_eos())
            services.emplace_back(vbi_data_service(stream));

        stream.range_mark_end();
    }

    std::vector<vbi_data_service> services;
};

struct country_availability_descriptor : descriptor
{
    country_availability_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        country_availability_flag = stream.read<uint8_t>();
        while (!stream.range_eos())
        {
            uint32_t country_code =
                  (uint32_t)stream.read<uint8_t>() << 16
                | (uint32_t)stream.read<uint8_t>() << 8
                | (uint32_t)stream.read<uint8_t>();

            country_code_list.emplace_back(country_code);
        }

        stream.range_mark_end();
    }

    uint8_t country_availability_flag;          // \note 7 bits
    std::vector<uint32_t> country_code_list;
};

struct extended_event
{
    extended_event(const dvb_utf8::stream_buffer &stream)
    {
        auto item_description_length = stream.read<uint8_t>();
        if (item_description_length)
            item_description = dvb_utf8::decode(
                dvb_utf8::stream_buffer(stream.read_buffer(item_description_length)));

        auto item_length = stream.read<uint8_t>();
        if (item_length)
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

        auto text_length = stream.read<uint8_t>();
        if (text_length)
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

struct parental_rating
{
    uint32_t country_code; // : 24;
    uint8_t rating;
};

struct parental_rating_descriptor : descriptor
{
    parental_rating_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        ratings.reserve(stream.range_size() / 4);
        while (!stream.range_eos())
        {
            uint32_t country_code =
                  (uint32_t)stream.read<uint8_t>() << 16
                | (uint32_t)stream.read<uint8_t>() << 8
                | (uint32_t)stream.read<uint8_t>();

            uint8_t rating = stream.read<uint8_t>();

            ratings.emplace_back(parental_rating{ country_code , rating });
        }
        stream.range_mark_end();
    }

    std::vector<parental_rating> ratings;
};

struct content_descriptor_item
{
    uint8_t content_nibble_level_1 : 4;
    uint8_t content_nibble_level_2 : 4;
    uint8_t user_byte;
};

struct content_descriptor : descriptor
{
    content_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        while (!stream.range_eos())
        {
            uint8_t content_nibble = stream.read<uint8_t>();
            uint8_t user_byte = stream.read<uint8_t>();

            items.emplace_back(content_descriptor_item{
                uint8_t((content_nibble & 0xF0) >> 4),
                uint8_t(content_nibble & 0x0F),
                user_byte
            });
        }
        stream.range_mark_end();
    }
    std::vector<content_descriptor_item> items;
};

#endif // dvb_descriptors_h__
