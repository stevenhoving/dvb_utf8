#ifndef dvb_utf8_encode_h__
#define dvb_utf8_encode_h__

#if DVB_UTF8_ENABLE_ENCODE

#include "dvb_utf8_char_traits.hpp"
#include "dvb_utf8_opcodes.hpp"

#include <string>

namespace dvb_utf8
{
// \todo add spec comment here about the 2 byte character encoding...
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
        DVB_DBG("warning, invalid ISO8859 encoding identifier\n");
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
        token = ENC_ID_ISO8859;
        stream.write(token);
        token = get_dvb_0x10_encoding_identifier(encoding);
        stream.write(token);
        break;
    case character_encoding::iso_8859_5:
        token = ENC_ID_ISO8859_5;
        stream.write(token);
        break;
    case character_encoding::iso_8859_6:
        token = ENC_ID_ISO8859_6;
        stream.write(token);
        break;
    case character_encoding::iso_8859_7:
        token = ENC_ID_ISO8859_7;
        stream.write(token);
        break;
    case character_encoding::iso_8859_8:
        token = ENC_ID_ISO8859_8;
        stream.write(token);
        break;
    case character_encoding::iso_8859_9:
        token = ENC_ID_ISO8859_9;
        stream.write(token);
        break;
    case character_encoding::iso_8859_10:
        token = ENC_ID_ISO8859_10;
        stream.write(token);
        break;
    case character_encoding::iso_8859_11:
        token = ENC_ID_ISO8859_11;
        stream.write(token);
        break;
    case character_encoding::iso_8859_13:
        token = ENC_ID_ISO8859_13;
        stream.write(token);
        break;
    case character_encoding::iso_8859_14:
        token = ENC_ID_ISO8859_14;
        stream.write(token);
        break;
    case character_encoding::iso_8859_15:
        token = ENC_ID_ISO8859_15;
        stream.write(token);
        break;
    case character_encoding::ucs2be:
        token = ENC_ID_UNICODE;
        stream.write(token);
        break;
    case character_encoding::gb18030:
        token = ENC_ID_GB18030;
        stream.write(token);
        break;
    case character_encoding::big5:
        token = ENC_ID_BIG5;
        stream.write(token);
        break;
    case character_encoding::utf8:
        token = ENC_ID_UTF8;
        stream.write(token);
        break;
    case character_encoding::utf16be:
        token = ENC_ID_UTF16BE;
        stream.write(token);
        break;
    case character_encoding::utf16le:
        token = ENC_ID_UTF16LE;
        stream.write(token);
        break;
    case character_encoding::ksx1001:
        token = ENC_ID_KSX1001;
        stream.write(token);
        break;
    default:
        DVB_DBG("warning, attempting to write unsupported encoding token type\n");
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
    case character_encoding::ucs2be:
        stream.write(ucs2be::from_utf8()(text));
        break;
    default:
        DVB_DBG("warning, attempting to write unsupported encoding type\n");
        break;
    }
}


// convert utf8 to dvb and visa versa
static
stream_buffer encode(const std::string &text, character_encoding encoding)
{
    auto result = stream_buffer();
    serialize_encoding(result, encoding);
    serialize_string(result, text, encoding);
    result.seek(0, SEEK_SET);
    return result;
}

} // namespace dvb_utf8

#endif // DVB_UTF8_ENABLE_ENCODE
#endif // dvb_utf8_encode_h__
