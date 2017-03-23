#ifndef linkage_descriptor_hpp__
#define linkage_descriptor_hpp__

struct mobile_hand_over_info
{
    enum class origin_types : uint8_t
    {
        NIT = 0x00,
        SDT = 0x01
    };

    enum class hand_over_types : uint8_t
    {
        to_identical_service_neighbouring_country = 0x01,
        to_local_variation_same_service = 0x02,
        to_associated_service = 0x03
    };

    explicit mobile_hand_over_info(const dvb_utf8::stream_span &stream)
    {
        uint8_t temp = stream.read<uint8_t>();
        hand_over_type = static_cast<hand_over_types>((temp & 0xF0) >> 4);
        origin_type = static_cast<origin_types>(temp & 0x01);

        if (hand_over_type == hand_over_types::to_identical_service_neighbouring_country
         || hand_over_type == hand_over_types::to_local_variation_same_service
         || hand_over_type == hand_over_types::to_associated_service)
            network_id = stream.read<uint16_t>();
        else
            network_id = 0;

        if (origin_type == origin_types::SDT)
            initial_service_id = stream.read<uint16_t>();
        else
            initial_service_id = 0;
    }

    hand_over_types hand_over_type;
    origin_types origin_type;
    uint16_t network_id;
    uint16_t initial_service_id;
};

struct event_linkage_info
{
    explicit event_linkage_info(const dvb_utf8::stream_span &stream)
    {
        target_event_id = stream.read<uint16_t>();
        uint8_t temp = stream.read<uint8_t>();
        target_listed   = (target_listed >> 7) & 0x01;
        event_simulcast = (target_listed >> 6) & 0x01;
    }
    uint16_t target_event_id;
    uint8_t target_listed;
    uint8_t event_simulcast;
};

struct extended_event_linkage_info_entry
{
    explicit extended_event_linkage_info_entry(const dvb_utf8::stream_span &stream)
    {
        target_event_id = stream.read<uint16_t>();
        uint8_t flags = stream.read<uint8_t>();
        // \note this is probably wrong...!!!!
        target_listed            = (flags >> 7) & 1;
        event_simulcast          = (flags >> 6) & 1;
        link_type                = (flags >> 4) & 3;
        target_id_type           = (flags >> 2) & 3;
        original_network_id_flag = (flags >> 1) & 1;
        service_id_flag          = (flags >> 0) & 1;
        if (target_id_type == 3)
        {
            user_defined_id = stream.read<uint16_t>();
        }
        else
        {
            if (target_id_type == 1)
                target_transport_stream_id = stream.read<uint16_t>();

            if (original_network_id_flag == 1)
                target_original_transport_id = stream.read<uint16_t>();

            if (service_id_flag == 1)
                target_service_id = stream.read<uint16_t>();
        }
    }
    uint16_t target_event_id;
    uint8_t target_listed;
    uint8_t event_simulcast;
    uint8_t link_type;
    uint8_t target_id_type;
    uint8_t original_network_id_flag;
    uint8_t service_id_flag;
    uint16_t user_defined_id; // set if target_id_type == 3
    uint16_t target_transport_stream_id; // set if target_id_type == 1
    uint16_t target_original_transport_id; // set if original_network_id_flag == 1
    uint16_t target_service_id; // set if target_service_flag == 1
};

struct extended_event_linkage_info
{
    explicit extended_event_linkage_info(const dvb_utf8::stream_span &stream)
    {
        uint8_t loop_length = stream.read<uint8_t>();
        auto payload = stream.read_buffer(loop_length);
        while (!payload.eos())
        {
            items.emplace_back(extended_event_linkage_info_entry(payload));
        }
    }

    std::vector<extended_event_linkage_info_entry> items;
};

struct linkage_descriptor : descriptor
{
    explicit linkage_descriptor(const dvb_utf8::stream_span &stream)
        : descriptor(stream)
    {
        uint16_t transport_stream_id = payload.read<uint16_t>();
        uint16_t original_network_id = payload.read<uint16_t>();
        uint16_t service_id = payload.read<uint16_t>();
        uint8_t linkage_type = payload.read<uint8_t>();
        if (linkage_type == 0x80)
        {
            // parse mobile_hand_over_info
            auto mobile_hand_over_info_ = mobile_hand_over_info(payload);
        }
        else if (linkage_type == 0x0D)
        {
             // parse event_linkage_info
            auto event_linkage_info_ = event_linkage_info(payload);
        }
        else if (linkage_type >= 0x0E && linkage_type <= 0x1F)
        {
            // parse extended_event_linkage_info
            auto extended_event_linkage_info_ = extended_event_linkage_info(payload);
        }

        private_data = payload.read_buffer(payload.size() - payload.tell());
    }

    dvb_utf8::stream_span private_data;
};

#endif // linkage_descriptor_hpp__
