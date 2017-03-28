#ifndef tva_id_descriptor_hpp__
#define tva_id_descriptor_hpp__

struct tva_id
{
    explicit tva_id(const dvb_utf8::stream_span &stream)
    {
        id = stream.read<uint16_t>();
        runningStatus = stream.read<uint8_t>() & 0x03;
    }

    uint16_t id;
    uint8_t runningStatus;
};

// \todo test with real data
struct tva_id_descriptor : descriptor
{
    explicit tva_id_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        while (!payload.eos())
            identifiers.emplace_back(tva_id(payload));
    }

    std::vector<tva_id> identifiers;
};

#endif // tva_id_descriptor_hpp__
