// MESSAGE HEATING_REQUEST support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief HEATING_REQUEST message
 *
 * Heating request message
 */
struct HEATING_REQUEST : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40023;
    static constexpr size_t LENGTH = 18;
    static constexpr size_t MIN_LENGTH = 18;
    static constexpr uint8_t CRC_EXTRA = 74;
    static constexpr auto NAME = "HEATING_REQUEST";


    float setpoint_c; /*< [degC] Setpoint temperature in Celsius */
    uint8_t enabled; /*<  Enable or disable heating */
    float kp; /*<  Proportional gain for PID controller */
    float ki; /*<  Integral gain for PID controller */
    float kd; /*<  Derivative gain for PID controller */
    uint8_t reset_integral; /*<  Reset the integral term of the PID controller */


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
        ss << "  setpoint_c: " << setpoint_c << std::endl;
        ss << "  enabled: " << +enabled << std::endl;
        ss << "  kp: " << kp << std::endl;
        ss << "  ki: " << ki << std::endl;
        ss << "  kd: " << kd << std::endl;
        ss << "  reset_integral: " << +reset_integral << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << setpoint_c;                    // offset: 0
        map << kp;                            // offset: 4
        map << ki;                            // offset: 8
        map << kd;                            // offset: 12
        map << enabled;                       // offset: 16
        map << reset_integral;                // offset: 17
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> setpoint_c;                    // offset: 0
        map >> kp;                            // offset: 4
        map >> ki;                            // offset: 8
        map >> kd;                            // offset: 12
        map >> enabled;                       // offset: 16
        map >> reset_integral;                // offset: 17
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
