#ifndef service_description_hpp__
#define service_description_hpp__
namespace dvb_parse
{

struct service_description : descriptor_container
{
    explicit service_description(const dvb_utf8::stream_span &stream)
    {
        service_id = stream.read<uint16_t>();
        auto flags = stream.read<uint8_t>();
        auto status = stream.read<uint8_t>();

        eit_schedule_flag = (flags >> 1) & 0x01;
        eit_present_following_flag = flags & 0x01;

        running_status = (status >> 5) & 0x07;
        free_ca_mode = (status >> 4) & 0x01;
        descriptors_loop_length = ((status & 0x0F) << 8) | stream.read<uint8_t>();
        auto descriptor_stream = stream.read_buffer(descriptors_loop_length);
        while(!descriptor_stream.eos())
            read_descriptor(descriptor_stream);
    }

    uint16_t service_id;
    uint8_t eit_schedule_flag;
    uint8_t eit_present_following_flag;
    uint8_t running_status;
    uint8_t free_ca_mode;
    uint16_t descriptors_loop_length;
};

} // namespace dvb_parse

#endif // service_description_hpp__
