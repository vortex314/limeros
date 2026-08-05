// MESSAGE US_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief US_EVENT message
 *
 * Ultrasonic sensor event message
 */
struct US_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40010;
    static constexpr size_t LENGTH = 12;
    static constexpr size_t MIN_LENGTH = 12;
    static constexpr uint8_t CRC_EXTRA = 169;
    static constexpr auto NAME = "US_EVENT";


    float distance; /*< [m] Distance in meters */
    float temperature; /*< [degC] Temperature in Celsius */
    int32_t status; /*<  Status code, 0 if no error */


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
        ss << "  distance: " << distance << std::endl;
        ss << "  temperature: " << temperature << std::endl;
        ss << "  status: " << status << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << distance;                      // offset: 0
        map << temperature;                   // offset: 4
        map << status;                        // offset: 8
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> distance;                      // offset: 0
        map >> temperature;                   // offset: 4
        map >> status;                        // offset: 8
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
