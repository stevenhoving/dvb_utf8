#ifndef descriptor_hpp__
#define descriptor_hpp__

#include "dvb_parser_config.h"

#if DVB_PARSER_DEBUG_LOG
#define DVB_PARSER_DBG(x, ...) do{printf(x, ##__VA_ARGS__);}while(0)
#else
#define DVB_PARSER_DBG(x, ...) do{}while(0)
#endif // DVB_PARSER_DEBUG_LOG

struct descriptor
{
    explicit descriptor(const dvb_utf8::stream_span &stream)
    {
        tag = stream.read<uint8_t>();
        length = stream.read<uint8_t>();

        payload = stream.read_buffer(length);
    }
    uint8_t tag;
    uint8_t length;
    dvb_utf8::stream_span payload;
};

#endif // descriptor_hpp__
