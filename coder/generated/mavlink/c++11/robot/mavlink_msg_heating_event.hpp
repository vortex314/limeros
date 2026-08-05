// MESSAGE HEATING_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief HEATING_EVENT message
 *
 * Heating event message
 */
struct HEATING_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40022;
    static constexpr size_t LENGTH = 23;
    static constexpr size_t MIN_LENGTH = 23;
    static constexpr uint8_t CRC_EXTRA = 82;
    static constexpr auto NAME = "HEATING_EVENT";


    float temperature_c; /*< [degC] Current temperature in Celsius */
    float setpoint_c; /*< [degC] Setpoint temperature in Celsius */
    uint8_t enabled; /*<  Heating enabled or disabled */
    float output_pct; /*< [%] Output percentage of the heating element */
    uint8_t heater_on; /*<  Heater is currently on or off */
    uint8_t fault; /*<  Fault detected in the heating system */
    uint64_t timestamp_ms; /*< [ms] Timestamp in milliseconds since epoch */


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
        ss << "  temperature_c: " << temperature_c << std::endl;
        ss << "  setpoint_c: " << setpoint_c << std::endl;
        ss << "  enabled: " << +enabled << std::endl;
        ss << "  output_pct: " << output_pct << std::endl;
        ss << "  heater_on: " << +heater_on << std::endl;
        ss << "  fault: " << +fault << std::endl;
        ss << "  timestamp_ms: " << timestamp_ms << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << timestamp_ms;                  // offset: 0
        map << temperature_c;                 // offset: 8
        map << setpoint_c;                    // offset: 12
        map << output_pct;                    // offset: 16
        map << enabled;                       // offset: 20
        map << heater_on;                     // offset: 21
        map << fault;                         // offset: 22
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> timestamp_ms;                  // offset: 0
        map >> temperature_c;                 // offset: 8
        map >> setpoint_c;                    // offset: 12
        map >> output_pct;                    // offset: 16
        map >> enabled;                       // offset: 20
        map >> heater_on;                     // offset: 21
        map >> fault;                         // offset: 22
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
