// MESSAGE CUTTER_REQUEST support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief CUTTER_REQUEST message
 *
 * Request command for cutter
 */
struct CUTTER_REQUEST : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40005;
    static constexpr size_t LENGTH = 5;
    static constexpr size_t MIN_LENGTH = 5;
    static constexpr uint8_t CRC_EXTRA = 130;
    static constexpr auto NAME = "CUTTER_REQUEST";


    uint8_t enabled; /*<  Enable or disable the cutter */
    int32_t rpm; /*< [rpm] Speed command for the cutter in RPM */


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
        ss << "  enabled: " << +enabled << std::endl;
        ss << "  rpm: " << rpm << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << rpm;                           // offset: 0
        map << enabled;                       // offset: 4
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> rpm;                           // offset: 0
        map >> enabled;                       // offset: 4
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
