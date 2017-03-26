#ifndef service_descriptor_hpp__
#define service_descriptor_hpp__

enum class service_types : uint8_t
{
    digital_television_service = 0x01,
    digital_radio_sound_service = 0x02,
    teletext_service = 0x03,
    nvod_reference_service = 0x04,
    nvod_time_shifted_service,
    mosaic_service = 0x06,
    rm_radio_service = 0x07,
    dvb_rsm_service = 0x08,
    advanced_codec_digital_radio_sound_service = 0x0A,
    h264_avc_mosaic_service = 0x0B,
    data_broadcast_service = 0x0C,
    common_interface_reserved = 0x0D,
    rcs_map = 0x0E,
    rcs_fls = 0x0F,
    dvb_mhp_service = 0x10,
    mpeg2_hd_digital_television_service = 0x11,
    // h264 sd tv
    h264_avc_sd_digital_tvservice = 0x16,
    h264_avc_sd_nvod_time_shifted_service = 0x17,
    h264_avc_sd_nvod_reference_service = 0x18,
    // h264 hd tv
    h264_avc_hd_digital_tv_service = 0x19,
    h264_avc_hd_nvod_time_shifted_service = 0x1A,
    h264_avc_hd_nvod_reference_service = 0x1B,
    // h264 3d tv
    h264_avc_frame_compatible_plano_stereoscopic_hd_digital_tv_service = 0x1c,
    h264_avc_frame_compatible_plano_stereoscopic_hd_nvod_time_shifted_service = 0x1d,
    h264_avc_frame_compatible_plano_stereoscopic_hd_nvod_reference_service = 0x1e,
    // h265 tv
    hevc_digital_television_service = 0x1f,
    // h265 uhd tv
    hevc_uhd_digital_television_service = 0x20,
};

struct service_descriptor : descriptor
{
    explicit service_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        service_type = static_cast<service_types>(payload.read<uint8_t>());
        uint8_t service_provider_name_length = payload.read<uint8_t>();

        service_provider_name = dvb_utf8::decode(
            payload.read_buffer(service_provider_name_length));

        uint8_t service_name_length = payload.read<uint8_t>();

        service_name = dvb_utf8::decode(
            payload.read_buffer(service_name_length));

        DVB_PARSER_DBG("service_descriptor - provider name: %s, name: %s, service type: 0x%X\n",
            service_provider_name.c_str(),
            service_name.c_str(),
            service_type
        );
    }

    service_types service_type;
    std::string service_provider_name;
    std::string service_name;
};
#endif // service_descriptor_hpp__
