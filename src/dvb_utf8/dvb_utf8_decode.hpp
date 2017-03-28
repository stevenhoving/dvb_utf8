#ifndef dvb_utf8_decode_h__
#define dvb_utf8_decode_h__

#include "dvb_utf8_stream_span.hpp"
#include "dvb_utf8_char_traits.hpp"
#include "dvb_utf8_opcodes.hpp"

#include <string>

namespace dvb_utf8
{
static
character_encoding get_encoding_dvb_0x10(const uint16_t identifier)
{
    switch (identifier)
    {
    case 0x01: return character_encoding::iso_8859_1;
    case 0x02: return character_encoding::iso_8859_2;
    case 0x03: return character_encoding::iso_8859_3;
    case 0x04: return character_encoding::iso_8859_4;
    case 0x05: return character_encoding::iso_8859_5;
    case 0x06: return character_encoding::iso_8859_6;
    case 0x07: return character_encoding::iso_8859_7;
    case 0x08: return character_encoding::iso_8859_8;
    case 0x09: return character_encoding::iso_8859_9;
    case 0x0A: return character_encoding::iso_8859_10;
    case 0x0B: return character_encoding::iso_8859_11;
    case 0x0D: return character_encoding::iso_8859_13;
    case 0x0E: return character_encoding::iso_8859_14;
    case 0x0F: return character_encoding::iso_8859_15;
    case 0x0C:
        DVB_DBG("warning, ISO8859-12 unsupported\n");
        return character_encoding::invalid;
    case 0x00: // reserved
    default:
        DVB_DBG("warning, invalid ISO8859 encoding identifier\n");
        return character_encoding::invalid;
    }
}

static
character_encoding deserialize_encoding(const stream_span &stream)
{
    uint8_t character_code_table_id = stream.read<uint8_t>();

    character_encoding encoding = character_encoding::invalid;

    /**
    * From page 126 (page 126 of the PDF) of the dvb A038 January 2017 spec:
    *
    *      "If the first byte of the text field has a value in the range "0x20"
    *      to "0xFF" then this and all subsequent bytes in the text item are
    *      coded using the default character coding table (table 00 - Latin
    *      alphabet) of figure A.1."
    */
    if (character_code_table_id >= 0x20 && character_code_table_id <= 0xFF)
    {
        encoding = character_encoding::iso_6937;
        stream.seek(-1, SEEK_CUR);
    }
    else
    {
        switch (character_code_table_id)
        {
        case ENC_ID_ISO8859_5: // ISO8859-5
            encoding = character_encoding::iso_8859_5;
            break;
        case ENC_ID_ISO8859_6: // ISO8859-6
            encoding = character_encoding::iso_8859_6;
            break;
        case ENC_ID_ISO8859_7: // ISO8859-7
            encoding = character_encoding::iso_8859_7;
            break;
        case ENC_ID_ISO8859_8: // ISO8859-8
            encoding = character_encoding::iso_8859_8;
            break;
        case ENC_ID_ISO8859_9: // ISO8859-9
            encoding = character_encoding::iso_8859_9;
            break;
        case ENC_ID_ISO8859_10: // ISO8859-10
            encoding = character_encoding::iso_8859_10;
            break;
        case ENC_ID_ISO8859_11: // ISO8859-11
            encoding = character_encoding::iso_8859_11;
            break;
        case ENC_ID_ISO8859_13: // ISO8859-13
            encoding = character_encoding::iso_8859_13;
            break;
        case ENC_ID_ISO8859_14: // ISO8859-14
            encoding = character_encoding::iso_8859_14;
            break;
        case ENC_ID_ISO8859_15: // ISO8859-15
            encoding = character_encoding::iso_8859_15;
            break;
        case 0x08: // reserved
        case 0x0C: // reserved
        case 0x0D: // reserved
        case 0x0E: // reserved
        case 0x0F: // reserved
            DVB_DBG("Invalid dvb text field character encoding identifier (%u)\n",
                character_code_table_id);
            encoding = character_encoding::invalid;
            break;
            /**
            * From page 127 (page 127 of the PDF) of the dvb A038 January 2017 spec:
            *
            *     "If the first byte of the text field has a value "0x10" then the
            *      following two bytes carry a 16-bit value (uimsbf) N to indicate that
            *      the remaining data of the text field is coded using the character code
            *      table specified in table A.4."
            */
        case ENC_ID_ISO8859: { // ISO8859
            uint16_t n = stream.read<uint8_t>() << 8 | stream.read<uint8_t>(); // character code table
            encoding = get_encoding_dvb_0x10(n);
        } break;
        case ENC_ID_UNICODE: // ISO10646 (BMP)
            encoding = character_encoding::ucs2be;
            break;
        case ENC_ID_KSX1001: // KSX1001-2004
                             // EUC-KR, windows-949
            encoding = character_encoding::ksx1001;
            break;
        case ENC_ID_GB18030: // GB-2312-1980
                             // GB-18030
            encoding = character_encoding::gb18030;
            break;
        case ENC_ID_BIG5: // Big5 subset of ISO10646
            encoding = character_encoding::big5;
            break;
        case ENC_ID_UTF8: // UTF-8 encoding of ISO10646 (BMP)
            encoding = character_encoding::utf8;
            break;
        case ENC_ID_UTF16BE:
            encoding = character_encoding::utf16be;
            break;
        case ENC_ID_UTF16LE:
            encoding = character_encoding::utf16le;
            break;
        case 0x18: // reserved
        case 0x19: // reserved
        case 0x1A: // reserved
        case 0x1B: // reserved
        case 0x1C: // reserved
        case 0x1D: // reserved
        case 0x1E: // reserved
        default:
            DVB_DBG("Invalid dvb text field character encoding identifier (%u)\n",
                character_code_table_id);
            encoding = character_encoding::invalid;
            break;
        /**
        * From page 127 (page 127 of the PDF) of the dvb A038 January 2017 spec:
        *
        *     "If the first byte of the text field has value "0x1F" then the following
        *      byte carries an 8-bit value (uimsbf) containing the encoding_type_id.
        *      This value indicates the encoding scheme of the string. Allocations of
        *      the value of this field are found in TS 101 162 [i.1]."
        */
        case ENC_ID_FREESAT:
            encoding = character_encoding::freesat;
        break;
        }
    }
    return encoding;
}

static
std::string deserialize_string(const stream_span &data, const character_encoding encoding)
{
    switch (encoding)
    {
    case character_encoding::iso_6937:
        return iso6937::to_utf8()(data);
    case character_encoding::iso_8859_1:
        return iso8859_1::to_utf8()(data);
    case character_encoding::iso_8859_2:
        return iso8859_2::to_utf8()(data);
    case character_encoding::iso_8859_3:
        return iso8859_3::to_utf8()(data);
    case character_encoding::iso_8859_4:
        return iso8859_4::to_utf8()(data);
    case character_encoding::iso_8859_5:
        return iso8859_5::to_utf8()(data);
    case character_encoding::iso_8859_6:
        return iso8859_6::to_utf8()(data);
    case character_encoding::iso_8859_7:
        return iso8859_7::to_utf8()(data);
    case character_encoding::iso_8859_8:
        return iso8859_8::to_utf8()(data);
    case character_encoding::iso_8859_9:
        return iso8859_9::to_utf8()(data);
    case character_encoding::iso_8859_10:
        return iso8859_10::to_utf8()(data);
    case character_encoding::iso_8859_11:
        return iso8859_11::to_utf8()(data);
    case character_encoding::iso_8859_13:
        return iso8859_13::to_utf8()(data);
    case character_encoding::iso_8859_14:
        return iso8859_14::to_utf8()(data);
    case character_encoding::iso_8859_15:
        return iso8859_15::to_utf8()(data);
    case character_encoding::gb18030:
        return gb18030::to_utf8()(data);
    case character_encoding::big5:
        return big5::to_utf8()(data);
    case character_encoding::ksx1001:
        return ksx1001::to_utf8()(data);
    case character_encoding::utf8:
        return utf8::to_utf8()(data);
    case character_encoding::utf16be:
        return utf16be::to_utf8()(data);
    case character_encoding::utf16le:
        return utf16le::to_utf8()(data);
    case character_encoding::ucs2be:
        return ucs2be::to_utf8()(data);
    case character_encoding::freesat:
        return freesat::to_utf8()(data);
        break;
    default:
        DVB_DBG("warning, invalid or unsupported encoding type\n");
        break;
    }

    return "";
}

// \todo add table overwrite option (quirk's of the system)
static
std::string decode(const stream_span &data, const character_encoding overwrite = character_encoding::invalid)
{
    if (data.empty())
        return "";

    auto encoding = deserialize_encoding(data);
    if (overwrite != character_encoding::invalid)
        encoding = overwrite;

    return deserialize_string(data, encoding);
}

} // namespace dvb_utf8

#endif // dvb_utf8_decode_h__
