#ifndef dvb_descriptors_hpp__
#define dvb_descriptors_hpp__

#include "descriptors/descriptors.hpp"




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

#endif // dvb_descriptors_hpp__
