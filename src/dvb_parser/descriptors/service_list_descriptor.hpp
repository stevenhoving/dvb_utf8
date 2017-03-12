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
    explicit service_list_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        service_list.reserve(stream.range_size() / 3);
        while (!stream.range_eos())
        {
            uint16_t service_id = stream.read<uint16_t>();
            uint8_t service_type = stream.read<uint8_t>();

            service_list.emplace_back(service{
                service_id,
                service_type
            });
        }
        stream.range_mark_end();
    }

    std::vector<service> service_list;
};

#endif // service_list_descriptor_hpp__
