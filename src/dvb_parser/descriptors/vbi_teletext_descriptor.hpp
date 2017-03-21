#ifndef vbi_teletext_descriptor_hpp__
#define vbi_teletext_descriptor_hpp__

struct vbi_teletext_item
{
    uint32_t iso_639_language_code;
    uint8_t teletext_type;
    uint8_t teletext_magazine_number;
    uint8_t teletext_page_number;
};

struct vbi_teletext_descriptor : descriptor
{
    explicit vbi_teletext_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        while (!payload.eos())
        {
            uint32_t iso_639_language_code =
                  (uint32_t)payload.read<uint8_t>() << 16
                | (uint32_t)payload.read<uint8_t>() << 8
                | (uint32_t)payload.read<uint8_t>();

            uint8_t teletext_temp = payload.read<uint8_t>();
            uint8_t teletext_type = (teletext_temp >> 3) & 0x1F;
            uint8_t teletext_magazine_number = teletext_temp & 0x5;
            uint8_t teletext_page_number = payload.read<uint8_t>();
            items.push_back({
                iso_639_language_code,
                teletext_type,
                teletext_magazine_number,
                teletext_page_number
            });
        }
    }

    std::vector<vbi_teletext_item> items;
};
#endif // vbi_teletext_descriptor_hpp__