#ifndef vbi_data_descriptor_hpp__
#define vbi_data_descriptor_hpp__

struct vbi_data_line
{
    vbi_data_line(const dvb_utf8::stream_buffer &stream)
    {
        auto data = stream.read<uint8_t>();
        field_parity = (data >> 5) & 0x01;
        line_offset = data & 0x1F;
    }

    // todo replace this with just a normal uint8_t and with getter functions.
    uint8_t field_parity : 1;
    uint8_t line_offset : 5;
};

struct vbi_data_service
{
    vbi_data_service(const dvb_utf8::stream_buffer &stream)
    {
        data_service_id = stream.read<uint8_t>();
        uint8_t data_service_descriptor_length = stream.read<uint8_t>();

        switch (data_service_id)
        {
        case 0x01:
        case 0x02:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            for (unsigned int i = 0; i < data_service_descriptor_length; ++i)
                vbi_data_lines.emplace_back(vbi_data_line(stream));
            break;

        default:
            // \note skip 'reserved' data service ids
            stream.seek(data_service_descriptor_length, SEEK_CUR);
            break;
        }
    }

    uint8_t data_service_id;
    std::vector<vbi_data_line> vbi_data_lines;
};

struct vbi_data_descriptor : descriptor
{
    vbi_data_descriptor(const dvb_utf8::stream_buffer &stream)
        : descriptor(stream)
    {
        while (!stream.range_eos())
            services.emplace_back(vbi_data_service(stream));

        stream.range_mark_end();
    }

    std::vector<vbi_data_service> services;
};
#endif // vbi_data_descriptor_hpp__
