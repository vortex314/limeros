// MESSAGE PING_REPLY support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief PING_REPLY message
 *
 * Ping reply message
 */
struct PING_REPLY : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40017;
    static constexpr size_t LENGTH = 12;
    static constexpr size_t MIN_LENGTH = 12;
    static constexpr uint8_t CRC_EXTRA = 47;
    static constexpr auto NAME = "PING_REPLY";


    uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply */
    uint64_t timestamp; /*< [ms] Timestamp in milliseconds since epoch */


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
        ss << "  req_id: " << req_id << std::endl;
        ss << "  timestamp: " << timestamp << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << timestamp;                     // offset: 0
        map << req_id;                        // offset: 8
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> timestamp;                     // offset: 0
        map >> req_id;                        // offset: 8
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
