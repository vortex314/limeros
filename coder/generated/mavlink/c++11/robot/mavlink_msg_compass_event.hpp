// MESSAGE COMPASS_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief COMPASS_EVENT message
 *
 * Compass event message
 */
struct COMPASS_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40008;
    static constexpr size_t LENGTH = 36;
    static constexpr size_t MIN_LENGTH = 36;
    static constexpr uint8_t CRC_EXTRA = 154;
    static constexpr auto NAME = "COMPASS_EVENT";


    float heading; /*< [deg] Heading in degrees */
    float pitch; /*< [deg] Pitch in degrees */
    float roll; /*< [deg] Roll in degrees */
    float mag_x; /*< [T] Magnetometer X axis in uT */
    float mag_y; /*< [T] Magnetometer Y axis in uT */
    float mag_z; /*< [T] Magnetometer Z axis in uT */
    float accel_x; /*< [m_s2] Accelerometer X axis in m/s^2 */
    float accel_y; /*< [m_s2] Accelerometer Y axis in m/s^2 */
    float accel_z; /*< [m_s2] Accelerometer Z axis in m/s^2 */


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
        ss << "  heading: " << heading << std::endl;
        ss << "  pitch: " << pitch << std::endl;
        ss << "  roll: " << roll << std::endl;
        ss << "  mag_x: " << mag_x << std::endl;
        ss << "  mag_y: " << mag_y << std::endl;
        ss << "  mag_z: " << mag_z << std::endl;
        ss << "  accel_x: " << accel_x << std::endl;
        ss << "  accel_y: " << accel_y << std::endl;
        ss << "  accel_z: " << accel_z << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << heading;                       // offset: 0
        map << pitch;                         // offset: 4
        map << roll;                          // offset: 8
        map << mag_x;                         // offset: 12
        map << mag_y;                         // offset: 16
        map << mag_z;                         // offset: 20
        map << accel_x;                       // offset: 24
        map << accel_y;                       // offset: 28
        map << accel_z;                       // offset: 32
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> heading;                       // offset: 0
        map >> pitch;                         // offset: 4
        map >> roll;                          // offset: 8
        map >> mag_x;                         // offset: 12
        map >> mag_y;                         // offset: 16
        map >> mag_z;                         // offset: 20
        map >> accel_x;                       // offset: 24
        map >> accel_y;                       // offset: 28
        map >> accel_z;                       // offset: 32
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
