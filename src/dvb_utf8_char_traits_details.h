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
        std::vector<uint8_t> result;
        for (auto i = 0u; i < text.size();)
        {
            // totally fucked up.... redesign...

            // first convert from multibyte to unicode
            auto p = (const unsigned char*)&text[i];
            auto left = text.size() - i;
            auto codepoint = ucs4_t();
            int converted_right = OutputConverter()(p, left, codepoint);
            if (converted_right > 0)
            {
                i += converted_right;
            }
            else
            {
                throw std::runtime_error("from_utf8, invalid data");
                ++i;
            }

            // second, convert from unicode to <encoding X>
            unsigned char temp[8]; // could any converter generate a encoding bigger than 8?

            auto converted_left = InputConverter()(temp, codepoint, converted_right);
            if (converted_left > 0 && converted_left < sizeof(temp))
                result.insert(result.end(), &temp[0], &temp[converted_left]);
        }
        return result;
    }
};

template<typename InputConverter, typename OutputConverter>
struct to_utf8
{
    std::string operator()(stream_buffer &data) const
    {
        auto src = &data.data()[data.tell()];
        auto len = data.size() - data.tell();
        std::string result;
        for (std::size_t i = 0; i < len;)
        {
            ucs4_t pwc;
            int converted_left = InputConverter()(&pwc, src + i, len - i);
            if (converted_left > 0)
            {
                i += converted_left;
            }
            else
            {
                throw std::runtime_error("to_utf8, invalid data");
                ++i;
            }
            OutputConverter()(pwc, result);
        }

        // weird edge case, trim the line ending...
        if (result[result.size() - 1] == '\0')
            result.resize(result.size() - 1);

        result.shrink_to_fit();

        return result;
    }
};

} // namespace factory
} // namespace dvb_utf8

#endif // dvb_utf8_char_traits_details_h__
