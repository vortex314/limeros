// MESSAGE ENDPOINT_ANNOUNCE_REPLY support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief ENDPOINT_ANNOUNCE_REPLY message
 *
 * Endpoint announce reply message for service discovery
 */
struct ENDPOINT_ANNOUNCE_REPLY : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40026;
    static constexpr size_t LENGTH = 8;
    static constexpr size_t MIN_LENGTH = 8;
    static constexpr uint8_t CRC_EXTRA = 236;
    static constexpr auto NAME = "ENDPOINT_ANNOUNCE_REPLY";


    uint64_t utc; /*< [us] Timestamp in milliseconds since epoch */


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
        ss << "  utc: " << utc << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << utc;                           // offset: 0
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> utc;                           // offset: 0
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
