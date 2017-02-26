#ifndef libiconv_h__
#define libiconv_h__

#pragma once

typedef long  ucs4_t;

#define RET_ILUNI      -1
//#define RET_ILSEQ      -2
#define RET_TOOSMALL   -3
//#define RET_TOOFEW(n) (-4-(n))

/* Return code if invalid input after a shift sequence of n bytes was read.
(xxx_mbtowc) */
#define RET_SHIFT_ILSEQ(n)  (-1-2*(n))
/* Return code if invalid. (xxx_mbtowc) */
#define RET_ILSEQ           RET_SHIFT_ILSEQ(0)
/* Return code if only a shift sequence of n bytes was read. (xxx_mbtowc) */
#define RET_TOOFEW(n)       (-2-2*(n))
/* Retrieve the n from the encoded RET_... value. */
#define DECODE_SHIFT_ILSEQ(r)  ((unsigned int)(RET_SHIFT_ILSEQ(0) - (r)) / 2)
#define DECODE_TOOFEW(r)       ((unsigned int)(RET_TOOFEW(0) - (r)) / 2)
/* Maximum value of n that may be used as argument to RET_SHIFT_ILSEQ or RET_TOOFEW. */
#define RET_COUNT_MAX       ((INT_MAX / 2) - 1)


/* General multi-byte encodings */
#include "utf16be.h"
#include "utf16le.h"
#include "ucs2be.h"
#include "ucs2le.h"

/* 8-bit encodings */
#include "iso8859_1.h"
#include "iso8859_2.h"
#include "iso8859_3.h"
#include "iso8859_4.h"
#include "iso8859_5.h"
#include "iso8859_6.h"
#include "iso8859_7.h"
#include "iso8859_8.h"
#include "iso8859_9.h"
#include "iso8859_10.h"
#include "iso8859_11.h"
#include "iso8859_13.h"
#include "iso8859_14.h"
#include "iso8859_15.h"

typedef struct {
    unsigned short  indx;  /* index into big table */
    unsigned short  used;  /* bitmask of used entries */
} Summary16;



#include "gb2312.h"
#include "gbk.h"

#include "ascii.h"
#include "ces_gbk.h"

#include "cp936.h"

#include "big5.h"
#include "ksc5601.h"
#include "iso6937.h"


#include "ces_big5.h"
#include "gb18030.h"
#include "euc_kr.h"


#endif // libiconv_h__