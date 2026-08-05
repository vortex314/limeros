// MESSAGE DEVICE_ALIVE_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief DEVICE_ALIVE_EVENT message
 *
 * Device alive event message
 */
struct DEVICE_ALIVE_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40020;
    static constexpr size_t LENGTH = 108;
    static constexpr size_t MIN_LENGTH = 108;
    static constexpr uint8_t CRC_EXTRA = 131;
    static constexpr auto NAME = "DEVICE_ALIVE_EVENT";


    std::array<char, 50> device; /*<  Device name */
    std::array<char, 50> endpoint; /*<  Endpoint name */
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
        ss << "  device: \"" << to_string(device) << "\"" << std::endl;
        ss << "  endpoint: \"" << to_string(endpoint) << "\"" << std::endl;
        ss << "  timestamp: " << timestamp << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << timestamp;                     // offset: 0
        map << device;                        // offset: 8
        map << endpoint;                      // offset: 58
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> timestamp;                     // offset: 0
        map >> device;                        // offset: 8
        map >> endpoint;                      // offset: 58
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
