#ifndef service_list_descriptor_hpp__
#define service_list_descriptor_hpp__

struct service
{
    uint16_t service_id;
    uint8_t service_type;
};

// \note not tested yet
struct service_list_descriptor : descriptor
{
    explicit service_list_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        service_list.reserve(payload.size() / 3);
        while (!payload.eos())
        {
            uint16_t service_id = payload.read<uint16_t>();
            uint8_t service_type = payload.read<uint8_t>();

            service_list.emplace_back(service{
                service_id,
                service_type
            });
        }
    }

    std::vector<service> service_list;
};

#endif // service_list_descriptor_hpp__
