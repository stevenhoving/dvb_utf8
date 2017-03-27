#ifndef service_description_section_hpp__
#define service_description_section_hpp__

namespace dvb_parse
{
struct service_description_section : long_section
{
    explicit service_description_section(const dvb_utf8::stream_span &stream)
        : long_section(stream)
    {
        original_network_id = payload.read<uint16_t>();
        uint8_t reserved = payload.read<uint8_t>();
        (void)reserved;

        while (!payload.eos())
            descriptions.emplace_back(service_description(payload));
    }

    uint16_t original_network_id;
    std::vector<service_description> descriptions;
};
} //  namespace dvb_parse


#endif // service_description_section_hpp__
