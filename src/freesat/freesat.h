#ifndef FREESAT_H
#define FREESAT_H

#include "../dvb_utf8_stream_buffer.h"

#include <string>

std::string freesat_huffman_decode(dvb_utf8::stream_buffer &compressed);

#endif
