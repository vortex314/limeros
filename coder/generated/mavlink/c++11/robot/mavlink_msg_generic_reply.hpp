// MESSAGE GENERIC_REPLY support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief GENERIC_REPLY message
 *
 * Reply for hoverboard request
 */
struct GENERIC_REPLY : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40003;
    static constexpr size_t LENGTH = 112;
    static constexpr size_t MIN_LENGTH = 112;
    static constexpr uint8_t CRC_EXTRA = 137;
    static constexpr auto NAME = "GENERIC_REPLY";


    uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply */
    uint32_t error_code; /*<  Error code, 0 if no error */
    std::array<char, 100> message; /*<  Error message or additional information */
    uint32_t msg_type; /*<  Message type identifier, the original request */


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
        ss << "  error_code: " << error_code << std::endl;
        ss << "  message: \"" << to_string(message) << "\"" << std::endl;
        ss << "  msg_type: " << msg_type << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << req_id;                        // offset: 0
        map << error_code;                    // offset: 4
        map << msg_type;                      // offset: 8
        map << message;                       // offset: 12
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> req_id;                        // offset: 0
        map >> error_code;                    // offset: 4
        map >> msg_type;                      // offset: 8
        map >> message;                       // offset: 12
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
