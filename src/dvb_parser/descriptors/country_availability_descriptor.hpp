#ifndef country_availability_descriptor_hpp__
#define country_availability_descriptor_hpp__

struct country_availability_descriptor : descriptor
{
    explicit country_availability_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        country_availability_flag = stream.read<uint8_t>();
        country_code_list.reserve(stream.range_size() / 3);
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
#endif // country_availability_descriptor_hpp__
