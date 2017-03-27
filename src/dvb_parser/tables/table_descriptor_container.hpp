#ifndef table_descriptor_container_hpp__
#define table_descriptor_container_hpp__

#include "dvb_description_tag.hpp"
#include <memory>

namespace dvb_parse
{

void handle_unhandled_descriptor(const dvb_utf8::stream_span &stream, uint8_t descriptor_tag)
{
    stream.seek(0, SEEK_END);
    printf("unhandled descriptor: %u (0x%X)\n", descriptor_tag, descriptor_tag);
}

struct descriptor_container
{
    void read_descriptor(const dvb_utf8::stream_span &stream)
    {
        const uint8_t descriptor_tag = stream.peek<uint8_t>();
        switch (descriptor_tag)
        {
        case VIDEO_STREAM_DESCRIPTOR: /* 0x02 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case AUDIO_STREAM_DESCRIPTOR: /* 0x03 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case HIERARCHY_DESCRIPTOR: /* 0x04 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case REGISTRATION_DESCRIPTOR: /* 0x05 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case DATA_STREAM_ALIGNMENT_DESCRIPTOR: /* 0x06 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case TARGET_BACKGROUND_GRID_DESCRIPTOR: /* 0x07 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case VIDEO_WINDOW_DESCRIPTOR: /* 0x08 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case CA_DESCRIPTOR: /* 0x09 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case ISO_639_LANGUAGE_DESCRIPTOR: /* 0x0A */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SYSTEM_CLOCK_DESCRIPTOR: /* 0x0B */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case MULTIPLEX_BUFFER_UTILIZATION_DESCRIPTOR: /* 0x0C */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case COPYRIGHT_DESCRIPTOR: /* 0x0D */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case MAXIMUM_BITRATE_DESCRIPTOR: /* 0x0E */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case PRIVATE_DATA_INDICATOR_DESCRIPTOR: /* 0x0F */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SMOOTHING_BUFFER_DESCRIPTOR: /* 0x10 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case STD_DESCRIPTOR: /* 0x11 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case IBP_DESCRIPTOR: /* 0x12 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case CAROUSEL_IDENTIFIER_DESCRIPTOR: /* 0x13 */ handle_unhandled_descriptor(stream, descriptor_tag); break;

        case NETWORK_NAME_DESCRIPTOR: /* 0x40 */
            descriptors.emplace_back(std::make_unique<network_name_descriptor>(stream));
            break;
        case SERVICE_LIST_DESCRIPTOR: /* 0x41 */
            descriptors.emplace_back(std::make_unique<service_list_descriptor>(stream));
            break;
        case STUFFING_DESCRIPTOR: /* 0x42 */
            descriptors.emplace_back(std::make_unique<stuffing_descriptor>(stream));
            break;
        case SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x43 */
            descriptors.emplace_back(std::make_unique<satellite_delivery_system_descriptor>(stream));
            break;
        case CABLE_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x44 */
            descriptors.emplace_back(std::make_unique<cable_delivery_system_descriptor>(stream));
            break;
        case VBI_DATA_DESCRIPTOR: /*0x45*/
            descriptors.emplace_back(std::make_unique<vbi_data_descriptor>(stream));
            break;
        case VBI_TELETEXT_DESCRIPTOR: /* 0x46 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case BOUQUET_NAME_DESCRIPTOR: /* 0x47 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SERVICE_DESCRIPTOR: /*0x48*/        descriptors.emplace_back(std::make_unique<service_descriptor>(stream)); break;
        case COUNTRY_AVAILABILITY_DESCRIPTOR: /*0x49*/ 
        case LINKAGE_DESCRIPTOR: /* 0x4A */ descriptors.emplace_back(std::make_unique<linkage_descriptor>(stream)); break;
        case NVOD_REFERENCE_DESCRIPTOR: /* 0x4B */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case TIME_SHIFTED_SERVICE_DESCRIPTOR: /* 0x4C */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SHORT_EVENT_DESCRIPTOR: /*0x4D*/    descriptors.emplace_back(std::make_unique<short_event_descriptor>(stream)); break;
        case EXTENDED_EVENT_DESCRIPTOR: /*0x4E*/ descriptors.emplace_back(std::make_unique<extended_event_descriptor>(stream)); break;
        case TIME_SHIFTED_EVENT_DESCRIPTOR: /* 0x4F */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case COMPONENT_DESCRIPTOR: /* 0x50 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case MOSAIC_DESCRIPTOR: /* 0x51 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case STREAM_IDENTIFIER_DESCRIPTOR: /* 0x52 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case CA_IDENTIFIER_DESCRIPTOR: /* 0x53 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case CONTENT_DESCRIPTOR: /*0x54*/        descriptors.emplace_back(std::make_unique<content_descriptor>(stream)); break;
        case PARENTAL_RATING_DESCRIPTOR: /*0x55*/descriptors.emplace_back(std::make_unique<parental_rating_descriptor>(stream)); break;
        case TELETEXT_DESCRIPTOR: /* 0x56 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case TELEPHONE_DESCRIPTOR: /* 0x57 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case LOCAL_TIME_OFFSET_DESCRIPTOR: /* 0x58 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SUBTITLING_DESCRIPTOR: /* 0x59 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x5A */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case MULTILINGUAL_NETWORK_NAME_DESCRIPTOR: /* 0x5B */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case MULTILINGUAL_BOUQUET_NAME_DESCRIPTOR: /* 0x5C */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case MULTILINGUAL_SERVICE_NAME_DESCRIPTOR: /* 0x5D */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case MULTILINGUAL_COMPONENT_DESCRIPTOR: /* 0x5E */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case PRIVATE_DATA_SPECIFIER_DESCRIPTOR: /* 0x5F */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SERVICE_MOVE_DESCRIPTOR: /* 0x60 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SHORT_SMOOTHING_BUFFER_DESCRIPTOR: /* 0x61 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case FREQUENCY_LIST_DESCRIPTOR: /* 0x62 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case PARTIAL_TRANSPORT_STREAM_DESCRIPTOR: /* 0x63 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case DATA_BROADCAST_DESCRIPTOR: /* 0x64 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SCRAMBLING_DESCRIPTOR: /* 0x65 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case DATA_BROADCAST_ID_DESCRIPTOR: /* 0x66 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case TRANSPORT_STREAM_DESCRIPTOR: /* 0x67 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case DSNG_DESCRIPTOR: /* 0x68 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case PDC_DESCRIPTOR: /* 0x69 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case AC3_DESCRIPTOR: /* 0x6A */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case ANCILLARY_DATA_DESCRIPTOR: /* 0x6B */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case CELL_LIST_DESCRIPTOR: /* 0x6C */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case CELL_FREQUENCY_LINK_DESCRIPTOR: /* 0x6D */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case ANNOUNCEMENT_SUPPORT_DESCRIPTOR: /* 0x6E */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case APPLICATION_SIGNALLING_DESCRIPTOR: /* 0x6F */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case ADAPTATION_FIELD_DATA_DESCRIPTOR: /* 0x70 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SERVICE_IDENTIFIER_DESCRIPTOR: /* 0x71 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case SERVICE_AVAILABILITY_DESCRIPTOR: /* 0x72 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case DEFAULT_AUTHORITY_DESCRIPTOR: /* 0x73 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case RELATED_CONTENT_DESCRIPTOR: /*0x74*/
            descriptors.emplace_back(std::make_unique<related_content_descriptor>(stream));
            break;
        case TVA_ID_DESCRIPTOR: /* 0x75 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case CONTENT_IDENTIFIER_DESCRIPTOR: /* 0x76 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case TIME_SLICE_FEC_IDENTIFIER_DESCRIPTOR: /* 0x77 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case ECM_REPETITION_RATE_DESCRIPTOR: /* 0x78 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case S2_SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR: /* 0x79 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case ENHANCED_AC3_DESCRIPTOR: /* 0x7A */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case DTS_DESCRIPTOR: /* 0x7B */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case AAC_DESCRIPTOR: /* 0x7C */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case XAIT_LOCATION_DESCRIPTOR: /* 0x7D */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case FTA_CONTENT_MANAGEMENT_DESCRIPTOR: /* 0x7E */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case EXTENSION_DESCRIPTOR: /* 0x7F */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case LOGICAL_CHANNEL_DESCRIPTOR: /* 0x83 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        case HD_SIMULCAST_LOGICAL_CHANNEL_DESCRIPTOR: /* 0x88 */ handle_unhandled_descriptor(stream, descriptor_tag); break;
        default:
            handle_unhandled_descriptor(stream, descriptor_tag); break;
        break;
        }
    }

    std::vector<std::unique_ptr<descriptor>> descriptors;
};
} // namespace dvb_parse

#endif // table_descriptor_container_hpp__
