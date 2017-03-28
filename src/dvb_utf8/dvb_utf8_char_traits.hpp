#ifndef dvb_utf8_char_traits_h__
#define dvb_utf8_char_traits_h__

#include "dvb_utf8_char_traits_details.hpp"
#include "dvb_utf8_stream_span.hpp"

#include "libiconv/libiconv.h"

#include <string>

namespace dvb_utf8
{
inline
void unicode_to_utf8(const ucs4_t c, std::string &dst)
{
    if (c < 0x80)
    {
        dst += static_cast<char>(c);
    }
    else if (c < 0x800)
    {
        dst += static_cast<char>(0xc0 | (c >> 6));
        dst += static_cast<char>(0x80 | (c & 0x3f));
    }
    else if (c < 0x10000)
    {
        dst += static_cast<char>(0xe0 | (c >> 12));
        dst += static_cast<char>(0x80 | ((c >> 6) & 0x3f));
        dst += static_cast<char>(0x80 | (c & 0x3f));
    }
    else if (c < 0x200000)
    {
        dst += static_cast<char>(0xf0 | (c >> 18));
        dst += static_cast<char>(0x80 | ((c >> 12) & 0x3f));
        dst += static_cast<char>(0x80 | ((c >> 6) & 0x3f));
        dst += static_cast<char>(0x80 | (c & 0x3f));
    }
    else
    {
        throw std::runtime_error("unicode_to_utf8, invalid code point");
    }
}

static inline
int utf8_to_unicode(const unsigned char *src, const int src_len, ucs4_t &dst)
{
    const unsigned char c = src[0];

    if (c < 0x80)
    {
        dst = c;
        return 1;
    }
    else if (c < 0xc2)
    {
        return RET_ILSEQ;
    }
    else if (c < 0xe0)
    {
        if (src_len < 2)
            return RET_TOOFEW(0);

        if (!((src[1] ^ 0x80) < 0x40))
            return RET_ILSEQ;

        dst = ((ucs4_t)(c & 0x1f) << 6) | (ucs4_t)(src[1] ^ 0x80);
        return 2;
    }
    else if (c < 0xf0)
    {
        if (src_len < 3)
            return RET_TOOFEW(0);

        if (!((src[1] ^ 0x80) < 0x40 && (src[2] ^ 0x80) < 0x40
            && (c >= 0xe1 || src[1] >= 0xa0)
            && (c != 0xed || src[1] < 0xa0)))
            return RET_ILSEQ;

        dst = ((ucs4_t)(c & 0x0f) << 12)
            | ((ucs4_t)(src[1] ^ 0x80) << 6)
            | (ucs4_t)(src[2] ^ 0x80);
        return 3;
    }
    else if (c < 0xf8 && sizeof(ucs4_t) * 8 >= 32)
    {
        if (src_len < 4)
            return RET_TOOFEW(0);

        if (!((src[1] ^ 0x80) < 0x40 && (src[2] ^ 0x80) < 0x40
            && (src[3] ^ 0x80) < 0x40
            && (c >= 0xf1 || src[1] >= 0x90)
            && (c < 0xf4 || (c == 0xf4 && src[1] < 0x90))))
            return RET_ILSEQ;

        dst = ((ucs4_t)(c & 0x07) << 18)
            | ((ucs4_t)(src[1] ^ 0x80) << 12)
            | ((ucs4_t)(src[2] ^ 0x80) << 6)
            | (ucs4_t)(src[3] ^ 0x80);
        return 4;
    }
    else
        return RET_ILSEQ;
}

/************************************************************************/
/* intermediate converters                                              */
/************************************************************************/
namespace intermediate
{
namespace dummy
{
    struct to_utf8
    {
        void operator()(const ucs4_t c, std::string &dst)
        {
            dst += static_cast<unsigned char>(c);
        }
    };

    struct from_utf8
    {
        int operator()(const unsigned char *src, const int src_len, ucs4_t &dst)
        {
            dst = *src;
            return 1;
        }
    };
}

namespace unicode
{
    struct to_utf8
    {
        void operator()(const ucs4_t c, std::string &dst)
        {
            unicode_to_utf8(c, dst);
        }
    };

    struct from_utf8
    {
        int operator()(const unsigned char *src, const int src_len, ucs4_t &dst)
        {
            return utf8_to_unicode(src, src_len, dst);
        }
    };
}

} // namespace intermediate

/************************************************************************/
/* character encoding converters                                        */
/************************************************************************/
namespace dummy_input
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            *r = (unsigned char)wc;
            return 1;
        }
    };

    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            *pwc = (ucs4_t)*s;
            return 1;
        }
    };
}

namespace gb18030
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return gb18030_wctomb(r, wc, n);
        }
    };

    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return gb18030_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;

} // namespace GB18030

namespace big5
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return big5_wctomb(r, wc, n);
        }
    };

    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return big5_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace ksx1001
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            /* \note I put my trust in 'libiconv' and believe this also
             * converts ksx1001.
             */
            return euc_kr_wctomb(r, wc, n);
        }
    };

    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return euc_kr_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso6937
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            // my own botched support for iso6937 (not finished yet)
            return iso6937_wctomb(r, wc, n);
        }
    };

    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            // my own botched support for iso6937
            return iso6937_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_1
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_1_wctomb(r, wc, n);
        }
    };

    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_1_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_2
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_2_wctomb(r, wc, n);
        }
    };

    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_2_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_3
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_3_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_3_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_4
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_4_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_4_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_5
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_5_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_5_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_6
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_6_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_6_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_7
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_7_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_7_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_8
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_8_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_8_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_9
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_9_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_9_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_10
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_10_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_10_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_11
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_11_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_11_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_13
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_13_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_13_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_14
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_14_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_14_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace iso8859_15
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return iso8859_15_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return iso8859_15_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace utf8
{
    using to_utf8 = converter_factory::to_utf8<dummy_input::mbtowc, intermediate::dummy::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<dummy_input::wctomb, intermediate::dummy::from_utf8>;
}

namespace utf16be
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return utf16be_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return utf16be_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

namespace utf16le
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return utf16le_wctomb(r, wc, n);
        }
    };
    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return utf16le_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

// unicode Basic Multilingual Plane (BMP)
namespace ucs2be
{
    struct wctomb
    {
        int operator()(unsigned char *r, ucs4_t wc, size_t n)
        {
            return ucs2be_wctomb(r, wc, n);
        }
    };

    struct mbtowc
    {
        int operator()(ucs4_t *pwc, const unsigned char *s, size_t n)
        {
            return ucs2be_mbtowc(pwc, s, n);
        }
    };

    using to_utf8 = converter_factory::to_utf8<mbtowc, intermediate::unicode::to_utf8>;
    using from_utf8 = converter_factory::from_utf8<wctomb, intermediate::unicode::from_utf8>;
}

} // namespace dvb_utf8

#endif // dvb_utf8_char_traits_h__
