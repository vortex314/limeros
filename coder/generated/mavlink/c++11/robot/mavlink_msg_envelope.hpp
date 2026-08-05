// MESSAGE ENVELOPE support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief ENVELOPE message
 *
 * Envelope message for encapsulating other messages
 */
struct ENVELOPE : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40024;
    static constexpr size_t LENGTH = 275;
    static constexpr size_t MIN_LENGTH = 275;
    static constexpr uint8_t CRC_EXTRA = 76;
    static constexpr auto NAME = "ENVELOPE";


    uint32_t src; /*<  Source endpoint name */
    uint32_t dst; /*<  Destination endpoint name */
    uint32_t msg_type; /*<  Message type name */
    uint32_t request_id; /*<  Request ID for matching request/reply */
    uint32_t instance_id; /*<  Instance ID for matching request/reply */
    std::array<uint8_t, 255> payload; /*<  Serialized payload of the message */


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
        ss << "  dst: " << dst << std::endl;
        ss << "  msg_type: " << msg_type << std::endl;
        ss << "  request_id: " << request_id << std::endl;
        ss << "  instance_id: " << instance_id << std::endl;
        ss << "  payload: [" << to_string(payload) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << src;                           // offset: 0
        map << dst;                           // offset: 4
        map << msg_type;                      // offset: 8
        map << request_id;                    // offset: 12
        map << instance_id;                   // offset: 16
        map << payload;                       // offset: 20
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> src;                           // offset: 0
        map >> dst;                           // offset: 4
        map >> msg_type;                      // offset: 8
        map >> request_id;                    // offset: 12
        map >> instance_id;                   // offset: 16
        map >> payload;                       // offset: 20
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
