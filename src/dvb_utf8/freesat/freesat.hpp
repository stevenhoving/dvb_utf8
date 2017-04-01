#ifndef FREESAT_H
#define FREESAT_H

#include "dvb_utf8_config.hpp"
#include "dvb_utf8_stream_span.hpp"
#include "dvb_utf8_stream_buffer.hpp"
#include <string>

std::string freesat_huffman_decode(const dvb_utf8::stream_span &compressed);
dvb_utf8::stream_buffer freesat_huffman_encode(const std::string &text, const int tableid = 0);

#endif
