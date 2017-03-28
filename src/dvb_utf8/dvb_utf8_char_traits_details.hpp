#ifndef dvb_utf8_char_traits_details_h__
#define dvb_utf8_char_traits_details_h__

namespace dvb_utf8
{
namespace converter_factory
{
template<typename InputConverter, typename OutputConverter>
struct from_utf8
{
    std::vector<uint8_t> operator()(const std::string &text) const
    {
        if (text.empty())
            return std::vector<uint8_t>();

        std::vector<uint8_t> result;
        for (auto i = 0u; i < text.size();)
        {
            // totally fucked up.... redesign...

            // first convert from multibyte to unicode
            auto p = (const unsigned char*)&text[i];
            auto left = text.size() - i;
            auto codepoint = ucs4_t();
            int converted_right_count = OutputConverter()(p, left, codepoint);
            if (converted_right_count > 0)
            {
                i += converted_right_count;
            }
            else
            {
                throw std::runtime_error("from_utf8, invalid data");
                ++i;
            }

            // second, convert from unicode to <encoding X>
            unsigned char temp[8]; // could any converter generate a encoding bigger than 8?

            auto converted_left_count = InputConverter()(temp, codepoint, converted_right_count);
            if (converted_left_count > 0 && converted_left_count < sizeof(temp))
                result.insert(result.end(), &temp[0], &temp[converted_left_count]);
        }
        return result;
    }
};

template<typename InputConverter, typename OutputConverter>
struct to_utf8
{
    std::string operator()(const stream_span &stream) const
    {
        if (stream.empty())
            return "";

        const auto src = stream.data();
        const auto len = stream.size();

        std::string result;
        result.reserve(len);
        std::ptrdiff_t i;
        for (i = 0; i < len;)
        {
            ucs4_t pwc;
            const int converted_left = InputConverter()(&pwc, src + i, len - i);
            if (converted_left > 0)
            {
                i += converted_left;
            }
            else
            {
                stream.seek(len - i, SEEK_CUR);
                throw std::runtime_error("to_utf8, invalid data");
            }
            OutputConverter()(pwc, result);
        }

        stream.seek(i, SEEK_CUR);

        // weird edge case, trim the line ending...
        if (result[result.size() - 1] == '\0')
            result.resize(result.size() - 1);

        result.shrink_to_fit();

        return result;
    }
};

} // namespace converter_factory
} // namespace dvb_utf8

#endif // dvb_utf8_char_traits_details_h__
