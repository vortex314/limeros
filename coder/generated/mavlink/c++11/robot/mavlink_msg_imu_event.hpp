// MESSAGE IMU_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief IMU_EVENT message
 *
 * IMU event message
 */
struct IMU_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40009;
    static constexpr size_t LENGTH = 24;
    static constexpr size_t MIN_LENGTH = 24;
    static constexpr uint8_t CRC_EXTRA = 106;
    static constexpr auto NAME = "IMU_EVENT";


    float gyro_x; /*< [deg_s] Gyroscope X axis in deg/s */
    float gyro_y; /*< [deg_s] Gyroscope Y axis in deg/s */
    float gyro_z; /*< [deg_s] Gyroscope Z axis in deg/s */
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
        ss << "  gyro_x: " << gyro_x << std::endl;
        ss << "  gyro_y: " << gyro_y << std::endl;
        ss << "  gyro_z: " << gyro_z << std::endl;
        ss << "  accel_x: " << accel_x << std::endl;
        ss << "  accel_y: " << accel_y << std::endl;
        ss << "  accel_z: " << accel_z << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << gyro_x;                        // offset: 0
        map << gyro_y;                        // offset: 4
        map << gyro_z;                        // offset: 8
        map << accel_x;                       // offset: 12
        map << accel_y;                       // offset: 16
        map << accel_z;                       // offset: 20
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> gyro_x;                        // offset: 0
        map >> gyro_y;                        // offset: 4
        map >> gyro_z;                        // offset: 8
        map >> accel_x;                       // offset: 12
        map >> accel_y;                       // offset: 16
        map >> accel_z;                       // offset: 20
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
