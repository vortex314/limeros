// MESSAGE SYS_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief SYS_EVENT message
 *
 * System event message
 */
struct SYS_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40015;
    static constexpr size_t LENGTH = 132;
    static constexpr size_t MIN_LENGTH = 132;
    static constexpr uint8_t CRC_EXTRA = 32;
    static constexpr auto NAME = "SYS_EVENT";


    uint64_t utc; /*< [us] UTC timestamp */
    uint64_t uptime; /*< [ms] Uptime */
    uint64_t free_heap; /*< [bytes] Free heap */
    uint64_t flash_size; /*< [bytes] Flash size */
    std::array<char, 50> cpu_board_type; /*<  CPU board type */
    std::array<char, 50> build_date_time; /*<  Build date and time */


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
        ss << "  uptime: " << uptime << std::endl;
        ss << "  free_heap: " << free_heap << std::endl;
        ss << "  flash_size: " << flash_size << std::endl;
        ss << "  cpu_board_type: \"" << to_string(cpu_board_type) << "\"" << std::endl;
        ss << "  build_date_time: \"" << to_string(build_date_time) << "\"" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << utc;                           // offset: 0
        map << uptime;                        // offset: 8
        map << free_heap;                     // offset: 16
        map << flash_size;                    // offset: 24
        map << cpu_board_type;                // offset: 32
        map << build_date_time;               // offset: 82
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> utc;                           // offset: 0
        map >> uptime;                        // offset: 8
        map >> free_heap;                     // offset: 16
        map >> flash_size;                    // offset: 24
        map >> cpu_board_type;                // offset: 32
        map >> build_date_time;               // offset: 82
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
