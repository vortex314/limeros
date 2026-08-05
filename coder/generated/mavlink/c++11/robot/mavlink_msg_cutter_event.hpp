// MESSAGE CUTTER_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief CUTTER_EVENT message
 *
 * Telemetry emitted by the cutter
 */
struct CUTTER_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40007;
    static constexpr size_t LENGTH = 17;
    static constexpr size_t MIN_LENGTH = 17;
    static constexpr uint8_t CRC_EXTRA = 187;
    static constexpr auto NAME = "CUTTER_EVENT";


    uint8_t enabled; /*<  Cutter enabled or disabled */
    int32_t rpm; /*< [rpm] Current speed of the cutter in RPM */
    float current; /*< [A] Current drawn by the cutter in Amperes */
    float voltage; /*< [V] Voltage supplied to the cutter in Volts */
    float temperature; /*< [degC] Temperature of the cutter in Celsius */


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
        ss << "  current: " << current << std::endl;
        ss << "  voltage: " << voltage << std::endl;
        ss << "  temperature: " << temperature << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << rpm;                           // offset: 0
        map << current;                       // offset: 4
        map << voltage;                       // offset: 8
        map << temperature;                   // offset: 12
        map << enabled;                       // offset: 16
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> rpm;                           // offset: 0
        map >> current;                       // offset: 4
        map >> voltage;                       // offset: 8
        map >> temperature;                   // offset: 12
        map >> enabled;                       // offset: 16
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
