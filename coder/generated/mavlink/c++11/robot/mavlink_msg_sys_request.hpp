// MESSAGE SYS_REQUEST support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief SYS_REQUEST message
 *
 * System request message
 */
struct SYS_REQUEST : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40013;
    static constexpr size_t LENGTH = 113;
    static constexpr size_t MIN_LENGTH = 113;
    static constexpr uint8_t CRC_EXTRA = 15;
    static constexpr auto NAME = "SYS_REQUEST";


    uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply */
    uint64_t set_time; /*< [us] Set system time */
    uint8_t reboot; /*<  Reboot the device */
    std::array<char, 100> console; /*<  Console command string */


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
        ss << "  set_time: " << set_time << std::endl;
        ss << "  reboot: " << +reboot << std::endl;
        ss << "  console: \"" << to_string(console) << "\"" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << set_time;                      // offset: 0
        map << req_id;                        // offset: 8
        map << reboot;                        // offset: 12
        map << console;                       // offset: 13
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> set_time;                      // offset: 0
        map >> req_id;                        // offset: 8
        map >> reboot;                        // offset: 12
        map >> console;                       // offset: 13
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
