#ifndef FREESAT_H
#define FREESAT_H

#include "dvb_utf8_config.hpp"
#include "dvb_utf8_stream_span.hpp"
#include "dvb_utf8_stream_buffer.hpp"
#include <string>

namespace dvb_utf8
{
namespace freesat
{
std::string decode(const dvb_utf8::stream_span &compressed);

#if DVB_UTF8_ENABLE_ENCODE
dvb_utf8::stream_buffer encode(const std::string &text, const int tableid = 1);
#endif // DVB_UTF8_ENABLE_ENCODE

} // namespace freesat
} // namespace dvb_utf8

#endif // #ifndef FREESAT_H
