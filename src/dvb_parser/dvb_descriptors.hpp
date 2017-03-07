#ifndef dvb_descriptors_hpp__
#define dvb_descriptors_hpp__

#include "descriptors/descriptors.hpp"



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
