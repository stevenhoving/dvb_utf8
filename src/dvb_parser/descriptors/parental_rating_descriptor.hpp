#ifndef parental_rating_descriptor_hpp__
#define parental_rating_descriptor_hpp__

struct parental_rating
{
    uint32_t country_code; // : 24;
    uint8_t rating;
};

struct parental_rating_descriptor : descriptor
{
    explicit parental_rating_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        ratings.reserve(payload.size() / 4);
        while (!payload.eos())
        {
            uint32_t country_code =
                  (uint32_t)payload.read<uint8_t>() << 16
                | (uint32_t)payload.read<uint8_t>() << 8
                | (uint32_t)payload.read<uint8_t>();

            uint8_t rating = payload.read<uint8_t>();

            ratings.emplace_back(parental_rating{ country_code , rating });
        }
    }

    std::vector<parental_rating> ratings;
};
#endif // parental_rating_descriptor_hpp__
