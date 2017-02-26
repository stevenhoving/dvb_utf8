#ifndef dvb_utf8_opcodes_h__
#define dvb_utf8_opcodes_h__

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

namespace dvb_utf8
{
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
    ucs2be,         // ISO10646
    gb18030,        // ISO10646 Simplified Chinese
    big5,           // ISO10646 Big5 Traditional Chinese
    utf8,           // ISO10646 Basic Multilingual Plane in UTF8 encoding
    utf16be,
    utf16le,
    ksx1001,        // Korean Character Set
    invalid,
};
} // namespace dvb_utf8

#endif // dvb_utf8_opcodes_h__
