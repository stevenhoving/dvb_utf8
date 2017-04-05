#ifndef dvb_utf8_utilities_h__
#define dvb_utf8_utilities_h__

#include "dvb_utf8_stream_span.hpp"
#include "dvb_utf8_stream_buffer.hpp"
#include <cstdio>

namespace dvb_utf8
{

static
void print_hexview(const uint8_t *data, int size)
{
    for (int i = 0; i < size; ++i)
        printf("%02X ", data[i]);
    printf("\n");
}

static
void print_hexview(const std::string &stream)
{
    auto data = stream.data();
    auto size = stream.size();
    print_hexview((const uint8_t *)data, size);
}

static
void print_hexview(const stream_span &stream)
{
    auto data = &stream.data()[stream.tell()];
    auto size = stream.size() - stream.tell();
    print_hexview(data, size);
}

static
void print_hexview(const stream_buffer &stream)
{
    auto data = &stream.data()[stream.tell()];
    auto size = stream.size() - stream.tell();
    print_hexview(data, size);
}

static
void print_stringview(const char *data, int size)
{
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

static
void print_stringview(const std::string &stream)
{
    auto data = stream.data();
    auto size = stream.size();
    print_stringview((const char *)data, size);
}

static
void print_stringview(const stream_span &stream)
{
    auto data = &stream.data()[stream.tell()];
    auto size = stream.size() - stream.tell();
    print_stringview((const char *)data, size);
}


} // namespace dvb_utf8

#endif // dvb_utf8_utilities_h__
