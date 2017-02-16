#ifndef dvb_utf8_h__
#define dvb_utf8_h__

#include "dvb_utf8_stream_buffer.h"
#include "dvb_utf8_char_traits.h"

#include <string>

namespace dvb_utf8
{

/**
 * From page 126 (page 126 of the PDF) of the dvb A038 January 2017 spec,
 * section 'A.2 Selection of character table'.
 */
#define ISO8859_5           0x01    // Latin/Cyrillic
#define ISO8859_6           0x02    // Latin/Arabic
#define ISO8859_7           0x03    // Latin/Greek
#define ISO8859_8           0x04    // Latin/Hebrew
#define ISO8859_9           0x05    // Latin 5
#define ISO8859_10          0x06    // Latin 6
#define ISO8859_11          0x07    // Latin/Thai
#define ISO8859_12          0x08    // Reserved
#define ISO8859_13          0x09    // Latin 7
#define ISO8859_14          0x0A    // Latin 8 (Celtic)
#define ISO8859_15          0x0B    // Latin 9
#define ISO8859_xx          0x10    // encoded in next two bytes
#define UNICODE_ENCODING    0x11    // ISO10646 Basic Multilingual Plane
#define KSX1001_ENCODING    0x12    // KSX1001 Korean
#define GB18030_ENCODING    0x13    // ISO10646 Simplified Chinese
#define BIG5_ENCODING       0x14    // ISO10646 Big5 Traditional Chinese
#define UTF8_ENCODING       0x15    // ISO10646 Basic Multilingual Plane in
                                    // UTF8 encoding
#define UTF16BE_ENCODING    0x16
#define UTF16LE_ENCODING    0x17

enum class character_encoding
{
    iso_6937,       // table 0: Latin
    iso_8859_1,     // West European
    iso_8859_2,     // East European
    iso_8859_3,     // South European
    iso_8859_4,     // North and North - East European
    iso_8859_5,     // Latin/Cyrillic
    iso_8859_6,     // Latin/Arabic
    iso_8859_7,     // Latin/Greek
    iso_8859_8,     // Latin/Hebrew
    iso_8859_9,     // West European & Turkish (Latin 5)
    iso_8859_10,    // North European (Latin 6)
    iso_8859_11,    // Thai (Latin/Thai)
    iso_8859_13,    // Baltic (Latin 7)
    iso_8859_14,    // Celtic (Latin 8)
    iso_8859_15,    // West European (Latin 9)
    ucs2_bmp,        // ISO10646
    gb18030,        // ISO10646 Simplified Chinese
    big5,           // ISO10646 Big5 Traditional Chinese
    utf8,           // ISO10646 Basic Multilingual Plane in UTF8 encoding
    utf16be,
    utf16le,
    ksx1001,        // Korean Character Set
    invalid,
};

static
character_encoding get_encoding_dvb_0x10(uint16_t identifier)
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
        printf("warning, ISO8859-12 unsupported\n");
        return character_encoding::invalid;
    case 0x00: // reserved
    default:
        printf("warning, invalid ISO8859 encoding identifier\n");
        return character_encoding::invalid;
    }
}

// \note add spec comment here about the 2 byte character encoding...
static
uint8_t get_dvb_0x10_encoding_identifier(character_encoding encoding)
{
    switch (encoding)
    {
    case character_encoding::iso_8859_1: return 0x01;
    case character_encoding::iso_8859_2: return 0x02;
    case character_encoding::iso_8859_3: return 0x03;
    case character_encoding::iso_8859_4: return 0x04;
    case character_encoding::iso_8859_5: return 0x05;
    case character_encoding::iso_8859_6: return 0x06;
    case character_encoding::iso_8859_7: return 0x07;
    case character_encoding::iso_8859_8: return 0x08;
    case character_encoding::iso_8859_9: return 0x09;
    case character_encoding::iso_8859_10: return 0x0A;
    case character_encoding::iso_8859_11: return 0x0B;
    case character_encoding::iso_8859_13: return 0x0D;
    case character_encoding::iso_8859_14: return 0x0E;
    case character_encoding::iso_8859_15: return 0x0F;
    default:
        printf("warning, invalid ISO8859 encoding identifier\n");
        return 0x00; // \todo fix invalid... throw or something...
    }
}

static
void serialize_encoding(stream_buffer &stream, character_encoding encoding)
{
    uint8_t token;

    switch (encoding)
    {
    case character_encoding::iso_6937:
/**
 * From page 126 (page 126 of the PDF) of the dvb A038 January 2017 spec:
 *
 *      "If the first byte of the text field has a value in the range "0x20" to
 *      "0xFF" then this and all subsequent bytes in the text item are coded
 *      using the default character coding table (table 00 - Latin alphabet) of
 *      figure A.1."
 */
        break;
    case character_encoding::iso_8859_1:
    case character_encoding::iso_8859_2:
    case character_encoding::iso_8859_3:
    case character_encoding::iso_8859_4:
        // \note we could make encoding a lot easier by always using the
        // ISO8859_xx token. And on a more personal note, this code is a bit 
        // clumsy.
        token = ISO8859_xx;
        stream.write(token);
        token = get_dvb_0x10_encoding_identifier(encoding);
        stream.write(token);
        break;
    case character_encoding::iso_8859_5:
        token = ISO8859_5;
        stream.write(token);
        break;
    case character_encoding::iso_8859_6:
        token = ISO8859_6;
        stream.write(token);
        break;
    case character_encoding::iso_8859_7:
        token = ISO8859_7;
        stream.write(token);
        break;
    case character_encoding::iso_8859_8:
        token = ISO8859_8;
        stream.write(token);
        break;
    case character_encoding::iso_8859_9:
        token = ISO8859_9;
        stream.write(token);
        break;
    case character_encoding::iso_8859_10:
        token = ISO8859_10;
        stream.write(token);
        break;
    case character_encoding::iso_8859_11:
        token = ISO8859_11;
        stream.write(token);
        break;
    case character_encoding::iso_8859_13:
        token = ISO8859_13;
        stream.write(token);
        break;
    case character_encoding::iso_8859_14:
        token = ISO8859_14;
        stream.write(token);
        break;
    case character_encoding::iso_8859_15:
        token = ISO8859_15;
        stream.write(token);
        break;
    case character_encoding::ucs2_bmp:
        token = UNICODE_ENCODING;
        stream.write(token);
        break;
    case character_encoding::gb18030:
        token = GB18030_ENCODING;
        stream.write(token);
        break;
    case character_encoding::big5:
        token = BIG5_ENCODING;
        stream.write(token);
        break;
    case character_encoding::utf8:
        token = UTF8_ENCODING;
        stream.write(token);
        break;
    case character_encoding::utf16be:
        token = UTF16BE_ENCODING;
        stream.write(token);
        break;
    case character_encoding::utf16le:
        token = UTF16LE_ENCODING;
        stream.write(token);
        break;
    case character_encoding::ksx1001:
        token = KSX1001_ENCODING;
        stream.write(token);
        break;
    default:
        printf("warning, attempting to write unsupported encoding token type\n");
        break;
    }
};

static
void serialize_string(stream_buffer &stream, const std::string &text, character_encoding encoding)
{
    switch (encoding)
    {
    case character_encoding::iso_6937:
        stream.write(iso6937::from_utf8()(text));
        break;
    case character_encoding::iso_8859_1:
        stream.write(iso8859_1::from_utf8()(text));
        break;
    case character_encoding::iso_8859_2:
        stream.write(iso8859_2::from_utf8()(text));
        break;
    case character_encoding::iso_8859_3:
        stream.write(iso8859_3::from_utf8()(text));
        break;
    case character_encoding::iso_8859_4:
        stream.write(iso8859_4::from_utf8()(text));
        break;
    case character_encoding::iso_8859_5:
        stream.write(iso8859_5::from_utf8()(text));
        break;
    case character_encoding::iso_8859_6:
        stream.write(iso8859_6::from_utf8()(text));
        break;
    case character_encoding::iso_8859_7:
        stream.write(iso8859_7::from_utf8()(text));
        break;
    case character_encoding::iso_8859_8:
        stream.write(iso8859_8::from_utf8()(text));
        break;
    case character_encoding::iso_8859_9:
        stream.write(iso8859_9::from_utf8()(text));
        break;
    case character_encoding::iso_8859_10:
        stream.write(iso8859_10::from_utf8()(text));
        break;
    case character_encoding::iso_8859_11:
        stream.write(iso8859_11::from_utf8()(text));
        break;
    case character_encoding::iso_8859_13:
        stream.write(iso8859_13::from_utf8()(text));
        break;
    case character_encoding::iso_8859_14:
        stream.write(iso8859_14::from_utf8()(text));
        break;
    case character_encoding::iso_8859_15:
        stream.write(iso8859_15::from_utf8()(text));
        break;
    case character_encoding::gb18030:
        stream.write(gb18030::from_utf8()(text));
        break;
    case character_encoding::big5:
        stream.write(big5::from_utf8()(text));
        break;
    case character_encoding::ksx1001:
        stream.write(ksx1001::from_utf8()(text));
        break;
    case character_encoding::utf8:
        stream.write(utf8::from_utf8()(text));
        break;
    case character_encoding::utf16be:
        stream.write(utf16be::from_utf8()(text));
        break;
    case character_encoding::utf16le:
        stream.write(utf16le::from_utf8()(text));
        break;
    default:
        printf("warning, attempting to write unsupported encoding type\n");
        break;
    }
}

static
character_encoding deserialize_encoding(stream_buffer &stream)
{
    uint8_t character_code_table_id = stream.read<uint8_t>(1);

    character_encoding encoding = character_encoding::invalid;

/**
 * From page 126 (page 126 of the PDF) of the dvb A038 January 2017 spec:
 *
 *      "If the first byte of the text field has a value in the range "0x20" to
 *      "0xFF" then this and all subsequent bytes in the text item are coded
 *      using the default character coding table (table 00 - Latin alphabet) of
 *      figure A.1."
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
        case 0x01: // ISO8859-5
            encoding = character_encoding::iso_8859_5;
            break;
        case 0x02: // ISO8859-6
            encoding = character_encoding::iso_8859_6;
            break;
        case 0x03: // ISO8859-7
            encoding = character_encoding::iso_8859_7;
            break;
        case 0x04: // ISO8859-8
            encoding = character_encoding::iso_8859_8;
            break;
        case 0x05: // ISO8859-9
            encoding = character_encoding::iso_8859_9;
            break;
        case 0x06: // ISO8859-10
            encoding = character_encoding::iso_8859_10;
            break;
        case 0x07: // ISO8859-11
            encoding = character_encoding::iso_8859_11;
            break;
        case 0x09: // ISO8859-13
            encoding = character_encoding::iso_8859_13;
            break;
        case 0x0A: // ISO8859-14
            encoding = character_encoding::iso_8859_14;
            break;
        case 0x0B: // ISO8859-15
            encoding = character_encoding::iso_8859_15;
            break;
        case 0x08: // reserved
        case 0x0C: // reserved
        case 0x0D: // reserved
        case 0x0E: // reserved
        case 0x0F: // reserved
            printf("Panic! Warning! because we are using any of the the reserved stuff\n");
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
        case 0x10: { // ISO8859
            uint16_t n = stream.read<uint8_t>(1) << 8 | stream.read<uint8_t>(1); // character code table
            encoding = get_encoding_dvb_0x10(n);
        } break;
        case 0x11: // ISO10646 (BMP)
            encoding = character_encoding::ucs2_bmp;
            break;
        case 0x12: // KSX1001-2004
                   // EUC-KR, windows-949
            encoding = character_encoding::ksx1001;
            break;
        case 0x13: // GB-2312-1980
            encoding = character_encoding::gb18030;
            break;
        case 0x14: // Big5 subset of ISO10646
            encoding = character_encoding::big5;
            break;
        case 0x15: // UTF-8 encoding of ISO10646 (BMP)
            encoding = character_encoding::utf8;
            break;
        case 0x16:
            encoding = character_encoding::utf16be;
            break;
        case 0x17:
            encoding = character_encoding::utf16le;
            break;
        case 0x18: // reserved
        case 0x19: // reserved
        case 0x1A: // reserved
        case 0x1B: // reserved
        case 0x1C: // reserved
        case 0x1D: // reserved
        case 0x1E: // reserved
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
        case 0x1F: { // Described by encoding_type_id
            auto encoding_type_id = stream.read<uint8_t>(1);
            //uint8_t encoding_type_id = data[index++]; // \todo shizzle found in 'TS 101 162' (what ever that means).

            // \todo until we implemented the freeset huffman decoding we mark
            // it as invalid.
            encoding = character_encoding::invalid;
        } break;
        }
    }
    return encoding;
}

static
std::string deserialize_string(stream_buffer &data, character_encoding encoding)
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
    case character_encoding::ucs2_bmp:
        return ucs2be::to_utf8()(data);
    default:
        printf("warning, invalid or unsupported encoding type\n");
        break;
    }

    return "";
}

// convert utf8 to dvb and visa versa
static
stream_buffer encode(const std::string &text, character_encoding encoding)
{
    auto result = stream_buffer();
    serialize_encoding(result, encoding);
    serialize_string(result, text, encoding);
    result.rewind();
    return result;
}

// \todo add table overwrite option
static
std::string decode(stream_buffer &data)
{
    auto encoding = deserialize_encoding(data);
    return deserialize_string(data, encoding);
}

} // namespace dvb_utf8

#endif // dvb_utf8_h__
