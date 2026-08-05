// MESSAGE PS4_REQUEST support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief PS4_REQUEST message
 *
 * PS4 controller output command
 */
struct PS4_REQUEST : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40012;
    static constexpr size_t LENGTH = 32;
    static constexpr size_t MIN_LENGTH = 32;
    static constexpr uint8_t CRC_EXTRA = 140;
    static constexpr auto NAME = "PS4_REQUEST";


    uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply */
    int32_t rumble_small; /*<  Small rumble intensity */
    int32_t rumble_large; /*<  Large rumble intensity */
    int32_t led_red; /*<  LED red channel */
    int32_t led_green; /*<  LED green channel */
    int32_t led_blue; /*<  LED blue channel */
    int32_t led_flash_on; /*<  LED flash on duration */
    int32_t led_flash_off; /*<  LED flash off duration */


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
        ss << "  rumble_small: " << rumble_small << std::endl;
        ss << "  rumble_large: " << rumble_large << std::endl;
        ss << "  led_red: " << led_red << std::endl;
        ss << "  led_green: " << led_green << std::endl;
        ss << "  led_blue: " << led_blue << std::endl;
        ss << "  led_flash_on: " << led_flash_on << std::endl;
        ss << "  led_flash_off: " << led_flash_off << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << req_id;                        // offset: 0
        map << rumble_small;                  // offset: 4
        map << rumble_large;                  // offset: 8
        map << led_red;                       // offset: 12
        map << led_green;                     // offset: 16
        map << led_blue;                      // offset: 20
        map << led_flash_on;                  // offset: 24
        map << led_flash_off;                 // offset: 28
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> req_id;                        // offset: 0
        map >> rumble_small;                  // offset: 4
        map >> rumble_large;                  // offset: 8
        map >> led_red;                       // offset: 12
        map >> led_green;                     // offset: 16
        map >> led_blue;                      // offset: 20
        map >> led_flash_on;                  // offset: 24
        map >> led_flash_off;                 // offset: 28
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
