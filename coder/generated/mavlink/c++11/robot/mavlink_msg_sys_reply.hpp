// MESSAGE SYS_REPLY support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief SYS_REPLY message
 *
 * System reply message
 */
struct SYS_REPLY : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40014;
    static constexpr size_t LENGTH = 108;
    static constexpr size_t MIN_LENGTH = 108;
    static constexpr uint8_t CRC_EXTRA = 129;
    static constexpr auto NAME = "SYS_REPLY";


    uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply */
    int32_t rc; /*<  Return code */
    std::array<char, 100> message; /*<  Result message */


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
        ss << "  rc: " << rc << std::endl;
        ss << "  message: \"" << to_string(message) << "\"" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << req_id;                        // offset: 0
        map << rc;                            // offset: 4
        map << message;                       // offset: 8
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> req_id;                        // offset: 0
        map >> rc;                            // offset: 4
        map >> message;                       // offset: 8
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
