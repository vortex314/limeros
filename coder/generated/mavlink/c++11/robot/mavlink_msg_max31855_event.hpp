// MESSAGE MAX31855_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief MAX31855_EVENT message
 *
 * Max31855 thermocouple event message
 */
struct MAX31855_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40021;
    static constexpr size_t LENGTH = 12;
    static constexpr size_t MIN_LENGTH = 12;
    static constexpr uint8_t CRC_EXTRA = 226;
    static constexpr auto NAME = "MAX31855_EVENT";


    float thermocouple_temp; /*< [degC] Thermocouple temperature in Celsius */
    float internal_temp; /*< [degC] Internal temperature in Celsius */
    uint8_t fault; /*<  Fault detected */
    uint8_t fault_short_vcc; /*<  Short to VCC detected */
    uint8_t fault_short_gnd; /*<  Short to GND detected */
    uint8_t fault_open_tc; /*<  Open thermocouple detected */


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
        ss << "  thermocouple_temp: " << thermocouple_temp << std::endl;
        ss << "  internal_temp: " << internal_temp << std::endl;
        ss << "  fault: " << +fault << std::endl;
        ss << "  fault_short_vcc: " << +fault_short_vcc << std::endl;
        ss << "  fault_short_gnd: " << +fault_short_gnd << std::endl;
        ss << "  fault_open_tc: " << +fault_open_tc << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << thermocouple_temp;             // offset: 0
        map << internal_temp;                 // offset: 4
        map << fault;                         // offset: 8
        map << fault_short_vcc;               // offset: 9
        map << fault_short_gnd;               // offset: 10
        map << fault_open_tc;                 // offset: 11
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> thermocouple_temp;             // offset: 0
        map >> internal_temp;                 // offset: 4
        map >> fault;                         // offset: 8
        map >> fault_short_vcc;               // offset: 9
        map >> fault_short_gnd;               // offset: 10
        map >> fault_open_tc;                 // offset: 11
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
