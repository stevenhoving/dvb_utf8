#ifndef dvb_utf8_config_h__
#define dvb_utf8_config_h__

#define DVB_UTF8_ENABLE_ENCODE 1
#define DVB_UTF8_ENABLE_DBG_LOG 0

#if DVB_UTF8_ENABLE_DBG_LOG
#define DVB_DBG(x, ...) do {printf(x, ##__VA_ARGS__);} while(0)
#else
#define DVB_DBG(x, ...) do {} while(0)
#endif

#endif // dvb_utf8_config_h__
