#ifndef transport_stream_descriptor_hpp__
#define transport_stream_descriptor_hpp__

// \todo test with real data
struct transport_stream_descriptor : descriptor
{
    explicit transport_stream_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        dvb_transport_stream_id = std::string((char *)payload.data(), payload.size());
        payload.seek(0, SEEK_END);
    }

    std::string dvb_transport_stream_id;
};

#endif // transport_stream_descriptor_hpp__
