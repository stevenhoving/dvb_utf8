#ifndef dvb_utf8_opcodes_h__
#define dvb_utf8_opcodes_h__

/**
* From page 126 (page 126 of the PDF) of the dvb A038 January 2017 spec,
* section 'A.2 Selection of character table'.
*/
#define ENC_ID_ISO8859_5    0x01    // Latin/Cyrillic
#define ENC_ID_ISO8859_6    0x02    // Latin/Arabic
#define ENC_ID_ISO8859_7    0x03    // Latin/Greek
#define ENC_ID_ISO8859_8    0x04    // Latin/Hebrew
#define ENC_ID_ISO8859_9    0x05    // Latin 5
#define ENC_ID_ISO8859_10   0x06    // Latin 6
#define ENC_ID_ISO8859_11   0x07    // Latin/Thai
#define ENC_ID_ISO8859_12   0x08    // Reserved
#define ENC_ID_ISO8859_13   0x09    // Latin 7
#define ENC_ID_ISO8859_14   0x0A    // Latin 8 (Celtic)
#define ENC_ID_ISO8859_15   0x0B    // Latin 9
#define ENC_ID_ISO8859      0x10    // encoded in next two bytes
#define ENC_ID_UNICODE      0x11    // ISO10646 Basic Multilingual Plane
#define ENC_ID_KSX1001      0x12    // KSX1001 Korean
#define ENC_ID_GB18030      0x13    // ISO10646 Simplified Chinese
#define ENC_ID_BIG5         0x14    // ISO10646 Big5 Traditional Chinese
#define ENC_ID_UTF8         0x15    // ISO10646 Basic Multilingual Plane in
                                    // UTF8 encoding
#define ENC_ID_UTF16BE      0x16
#define ENC_ID_UTF16LE      0x17
#define ENC_ID_FREESAT      0x1F    // Freesat huffman encoding

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
    freesat,
    invalid,
};
} // namespace dvb_utf8

#endif // dvb_utf8_opcodes_h__
