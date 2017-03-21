#ifndef service_descriptor_hpp__
#define service_descriptor_hpp__

struct service_descriptor : descriptor
{
    explicit service_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        service_type = payload.read<uint8_t>();
        uint8_t service_provider_name_length = payload.read<uint8_t>();

        service_provider_name = dvb_utf8::decode(
            payload.read_buffer(service_provider_name_length));

        uint8_t service_name_length = payload.read<uint8_t>();

        service_name = dvb_utf8::decode(
            payload.read_buffer(service_name_length));

        printf("service_descriptor - provider name: %s, name: %s\n",
            service_provider_name.c_str(),
            service_name.c_str()
        );
    }
    uint8_t service_type;
    std::string service_provider_name;
    std::string service_name;
};
#endif // service_descriptor_hpp__
