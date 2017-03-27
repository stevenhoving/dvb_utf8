#ifndef dvb_utf8_utilities_h__
#define dvb_utf8_utilities_h__

#include "dvb_utf8_stream_span.h"
#include <cstdio>

namespace dvb_utf8
{

static
void print_hexview(const stream_span &stream)
{
    auto data = &stream.data()[stream.tell()];
    auto size = stream.size() - stream.tell();
    for (int i = 0; i < size; ++i)
        printf("%02X ", data[i]);
    printf("\n");
}

static
void print_stringview(const stream_span &stream)
{
    auto data = &stream.data()[stream.tell()];
    auto size = stream.size() - stream.tell();
    for (int i = 0; i < size; ++i)
    {
        int letter = data[i];
        if (isprint(letter))
            printf("%c", letter);
        else
            printf(".");
    }
    printf("\n");
}

} // namespace dvb_utf8

#endif // dvb_utf8_utilities_h__
