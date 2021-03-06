#ifndef descriptors_hpp__
#define descriptors_hpp__

#include <vector>
#include <cstdint>
#include <memory>

#include "descriptor.hpp"
#include "descriptor_container.hpp"
#include "descriptor_tag.hpp"

#include "aac_descriptor.hpp"
#include "ac3_descriptor.hpp"
#include "adaptation_field_data_descriptor.hpp"
#include "ancillary_data_descriptor.hpp"
#include "announcement_support_descriptor.hpp"
#include "application_descriptor.hpp"
#include "application_icons_descriptor.hpp"
#include "application_name_descriptor.hpp"
#include "application_signalling_descriptor.hpp"
#include "application_storage_descriptor.hpp"
#include "application_usage_descriptor.hpp"
#include "audio_stream_descriptor.hpp"
#include "bouquet_name_descriptor.hpp"
#include "ca_descriptor.hpp"
#include "ca_identifier_descriptor.hpp"
#include "cable_delivery_system_descriptor.hpp"
#include "caching_priority_descriptor.hpp"
#include "carousel_identifier_descriptor.hpp"
#include "cell_frequency_link_descriptor.hpp"
#include "cell_list_descriptor.hpp"
#include "component_descriptor.hpp"
#include "compressed_module_descriptor.hpp"
#include "content_descriptor.hpp"
#include "content_identifier_descriptor.hpp"
#include "content_type_descriptor.hpp"
#include "country_availability_descriptor.hpp"
#include "cp_descriptor.hpp"
#include "cp_identifier_descriptor.hpp"
#include "cpcm_delivery_signalling_descriptor.hpp"
#include "crc32_descriptor.hpp"
#include "data_broadcast_descriptor.hpp"
#include "data_broadcast_id_descriptor.hpp"
#include "default_authority_descriptor.hpp"
#include "delegated_application_descriptor.hpp"
#include "dii_location_descriptor.hpp"
#include "dsng_descriptor.hpp"
#include "dts_descriptor.hpp"
#include "dvb_html_application_boundary_descriptor.hpp"
#include "dvb_html_application_descriptor.hpp"
#include "dvb_html_application_location_descriptor.hpp"
#include "dvb_j_application_descriptor.hpp"
#include "dvb_j_application_location_descriptor.hpp"
#include "ecm_repetition_rate_descriptor.hpp"
#include "enhanced_ac3_descriptor.hpp"
#include "est_download_time_descriptor.hpp"
#include "extended_event_descriptor.hpp"
#include "extension_descriptor.hpp"
#include "external_application_authorisation_descriptor.hpp"
#include "frequency_list_descriptor.hpp"
#include "fta_content_management_descriptor.hpp"
#include "graphics_constraints_descriptor.hpp"
#include "group_link_descriptor.hpp"
#include "image_icon_descriptor.hpp"
#include "info_descriptor.hpp"
#include "ip_signaling_descriptor.hpp"
#include "iso639_language_descriptor.hpp"
#include "label_descriptor.hpp"
#include "linkage_descriptor.hpp"
#include "local_time_offset_descriptor.hpp"
#include "location_descriptor.hpp"
#include "logical_channel_descriptor.hpp"
#include "module_link_descriptor.hpp"
#include "mosaic_descriptor.hpp"
#include "multilingual_bouquet_name_descriptor.hpp"
#include "multilingual_component_descriptor.hpp"
#include "multilingual_network_name_descriptor.hpp"
#include "multilingual_service_name_descriptor.hpp"
#include "name_descriptor.hpp"
#include "network_name_descriptor.hpp"
#include "nvod_reference_descriptor.hpp"
#include "parental_rating_descriptor.hpp"
#include "pdc_descriptor.hpp"
#include "plugin_descriptor.hpp"
#include "prefetch_descriptor.hpp"
#include "private_data_specifier_descriptor.hpp"
#include "registration_descriptor.hpp"
#include "related_content_descriptor.hpp"
#include "s2_satellite_delivery_system_descriptor.hpp"
#include "satellite_delivery_system_descriptor.hpp"
#include "scrambling_descriptor.hpp"
#include "service_availability_descriptor.hpp"
#include "service_descriptor.hpp"
#include "service_identifier_descriptor.hpp"
#include "service_list_descriptor.hpp"
#include "service_move_descriptor.hpp"
#include "short_event_descriptor.hpp"
#include "short_smoothing_buffer_descriptor.hpp"
#include "simple_application_boundary_descriptor.hpp"
#include "simple_application_location_descriptor.hpp"
#include "stream_identifier_descriptor.hpp"
#include "stuffing_descriptor.hpp"
#include "subtitling_descriptor.hpp"
#include "t2_delivery_system_descriptor.hpp"
#include "target_background_grid_descriptor.hpp"
#include "telephone_descriptor.hpp"
#include "teletext_descriptor.hpp"
#include "terrestrial_delivery_system_descriptor.hpp"
#include "time_shifted_service_descriptor.hpp"
#include "time_slice_fec_identifier_descriptor.hpp"
#include "transport_protocol_descriptor.hpp"
#include "transport_stream_descriptor.hpp"
#include "tva_id_descriptor.hpp"
#include "vbi_data_descriptor.hpp"
#include "vbi_teletext_descriptor.hpp"
#include "video_stream_descriptor.hpp"
#include "video_window_descriptor.hpp"
#include "xait_location_descriptor.hpp"

#endif // descriptors_hpp__
