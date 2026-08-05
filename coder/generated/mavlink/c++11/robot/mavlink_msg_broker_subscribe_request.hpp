// MESSAGE BROKER_SUBSCRIBE_REQUEST support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief BROKER_SUBSCRIBE_REQUEST message
 *
 * Subscribe to a message type from a source endpoint
 */
struct BROKER_SUBSCRIBE_REQUEST : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40018;
    static constexpr size_t LENGTH = 8;
    static constexpr size_t MIN_LENGTH = 8;
    static constexpr uint8_t CRC_EXTRA = 33;
    static constexpr auto NAME = "BROKER_SUBSCRIBE_REQUEST";


    uint32_t src; /*<  Source endpoint ID; 0 to subscribe to all sources */
    uint32_t msg_type; /*<  Message type ID; 0 to subscribe to all message types */


    inline std::string get_name(void) const override
    {
            return NAME;
    }

    inline Info get_message_info(void) const override
    {
            return { MSG_ID, LENGTH, MIN_LENGTH, CRC_EXTRA };
    }

    inline std::string to_yaml(void) const override
    {
        std::stringstream ss;

        ss << NAME << ":" << std::endl;
        ss << "  src: " << src << std::endl;
        ss << "  msg_type: " << msg_type << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << src;                           // offset: 0
        map << msg_type;                      // offset: 4
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> src;                           // offset: 0
        map >> msg_type;                      // offset: 4
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
