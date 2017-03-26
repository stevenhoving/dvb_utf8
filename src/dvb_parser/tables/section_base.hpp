#ifndef section_base_hpp__
#define section_base_hpp__

#include "dvb_crc32.hpp"

namespace dvb_parse
{
/* \note very important, a short_section should not handle the crc32, so this
 * a temporary solution to make parsing the payload of a section easier */
struct short_section
{
    explicit short_section(const dvb_utf8::stream_span &stream)
    {
        table_id = stream.read<uint8_t>();
        auto flags = stream.read<uint8_t>();
        section_syntax_indicator = (flags >> 7) & 0x01;
        uint16_t section_length = (uint16_t)(flags & 0x0F) << 8 | stream.read<uint8_t>();
        section_length &= 0xFFF; // 12 bits

        // \note we do a bit of moving around here because for checking the
        // crc32 we need the complete section (including table_id, flags and
        // length).
        stream.seek(-3, SEEK_CUR); // move the read cursor to the start of the section
        payload = stream.read_buffer((section_length - 4) + 3); // exclude the crc32 (but include table_id, flags and length)
        payload.seek(3, SEEK_SET); // move the read cursor to the start of the section payload (skipping over table_id, flags and length)

        crc32 = stream.read<uint32_t>();

        if (dvb_validate_crc32(payload, crc32))
            DVB_PARSER_DBG("payload crc is good\n");
        else
            DVB_PARSER_DBG("payload crc is bad\n");

        DVB_PARSER_DBG("section length: 0x%X (%u)\n", section_length, section_length);
    }
    uint8_t table_id;
    uint8_t section_syntax_indicator;
    uint32_t crc32;
protected:
    // helper variable to help parsing the dvb data
    dvb_utf8::stream_span payload;
};

struct long_section : short_section
{
    explicit long_section(const dvb_utf8::stream_span &stream)
        : short_section(stream)
    {
        table_id_extension = payload.read<uint16_t>();
        auto flags = payload.read<uint8_t>();
        version_number = (flags >> 1) & 0x1f;
        current_next_indicator = flags & 0x01;
        section_number = payload.read<uint8_t>();
        last_section_number = payload.read<uint8_t>();
    }
    int table_id_extension;
    int version_number;
    int current_next_indicator;
    int section_number;
    int last_section_number;
};

} // namespace dvb_parse
#endif // section_base_hpp__
