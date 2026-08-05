// MESSAGE PS4_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief PS4_EVENT message
 *
 * PS4 controller state event
 */
struct PS4_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40011;
    static constexpr size_t LENGTH = 168;
    static constexpr size_t MIN_LENGTH = 168;
    static constexpr uint8_t CRC_EXTRA = 12;
    static constexpr auto NAME = "PS4_EVENT";


    uint8_t button_left; /*<  Left button pressed */
    uint8_t button_right; /*<  Right button pressed */
    uint8_t button_up; /*<  Up button pressed */
    uint8_t button_down; /*<  Down button pressed */
    uint8_t button_square; /*<  Square button pressed */
    uint8_t button_cross; /*<  Cross button pressed */
    uint8_t button_circle; /*<  Circle button pressed */
    uint8_t button_triangle; /*<  Triangle button pressed */
    uint8_t button_left_shoulder; /*<  Left shoulder button pressed */
    uint8_t button_right_shoulder; /*<  Right shoulder button pressed */
    uint8_t button_left_trigger; /*<  Left trigger pressed */
    uint8_t button_right_trigger; /*<  Right trigger pressed */
    uint8_t button_left_joystick; /*<  Left joystick button pressed */
    uint8_t button_right_joystick; /*<  Right joystick button pressed */
    uint8_t button_share; /*<  Share button pressed */
    uint8_t button_options; /*<  Options button pressed */
    uint8_t button_touchpad; /*<  Touchpad button pressed */
    uint8_t button_ps; /*<  PS button pressed */
    int32_t axis_lx; /*<  Left stick X axis */
    int32_t axis_ly; /*<  Left stick Y axis */
    int32_t axis_rx; /*<  Right stick X axis */
    int32_t axis_ry; /*<  Right stick Y axis */
    int32_t gyro_x; /*< [deg_s] Gyroscope X axis */
    int32_t gyro_y; /*< [deg_s] Gyroscope Y axis */
    int32_t gyro_z; /*< [deg_s] Gyroscope Z axis */
    int32_t accel_x; /*< [m_s2] Accelerometer X axis */
    int32_t accel_y; /*< [m_s2] Accelerometer Y axis */
    int32_t accel_z; /*< [m_s2] Accelerometer Z axis */
    uint8_t connected; /*<  Controller connected */
    int32_t battery_level; /*< [%] Battery level */
    uint8_t bluetooth; /*<  Connected over Bluetooth */
    std::array<char, 100> debug; /*<  Debug string */
    int32_t temp; /*< [degC] Temperature */


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
        ss << "  button_left: " << +button_left << std::endl;
        ss << "  button_right: " << +button_right << std::endl;
        ss << "  button_up: " << +button_up << std::endl;
        ss << "  button_down: " << +button_down << std::endl;
        ss << "  button_square: " << +button_square << std::endl;
        ss << "  button_cross: " << +button_cross << std::endl;
        ss << "  button_circle: " << +button_circle << std::endl;
        ss << "  button_triangle: " << +button_triangle << std::endl;
        ss << "  button_left_shoulder: " << +button_left_shoulder << std::endl;
        ss << "  button_right_shoulder: " << +button_right_shoulder << std::endl;
        ss << "  button_left_trigger: " << +button_left_trigger << std::endl;
        ss << "  button_right_trigger: " << +button_right_trigger << std::endl;
        ss << "  button_left_joystick: " << +button_left_joystick << std::endl;
        ss << "  button_right_joystick: " << +button_right_joystick << std::endl;
        ss << "  button_share: " << +button_share << std::endl;
        ss << "  button_options: " << +button_options << std::endl;
        ss << "  button_touchpad: " << +button_touchpad << std::endl;
        ss << "  button_ps: " << +button_ps << std::endl;
        ss << "  axis_lx: " << axis_lx << std::endl;
        ss << "  axis_ly: " << axis_ly << std::endl;
        ss << "  axis_rx: " << axis_rx << std::endl;
        ss << "  axis_ry: " << axis_ry << std::endl;
        ss << "  gyro_x: " << gyro_x << std::endl;
        ss << "  gyro_y: " << gyro_y << std::endl;
        ss << "  gyro_z: " << gyro_z << std::endl;
        ss << "  accel_x: " << accel_x << std::endl;
        ss << "  accel_y: " << accel_y << std::endl;
        ss << "  accel_z: " << accel_z << std::endl;
        ss << "  connected: " << +connected << std::endl;
        ss << "  battery_level: " << battery_level << std::endl;
        ss << "  bluetooth: " << +bluetooth << std::endl;
        ss << "  debug: \"" << to_string(debug) << "\"" << std::endl;
        ss << "  temp: " << temp << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << axis_lx;                       // offset: 0
        map << axis_ly;                       // offset: 4
        map << axis_rx;                       // offset: 8
        map << axis_ry;                       // offset: 12
        map << gyro_x;                        // offset: 16
        map << gyro_y;                        // offset: 20
        map << gyro_z;                        // offset: 24
        map << accel_x;                       // offset: 28
        map << accel_y;                       // offset: 32
        map << accel_z;                       // offset: 36
        map << battery_level;                 // offset: 40
        map << temp;                          // offset: 44
        map << button_left;                   // offset: 48
        map << button_right;                  // offset: 49
        map << button_up;                     // offset: 50
        map << button_down;                   // offset: 51
        map << button_square;                 // offset: 52
        map << button_cross;                  // offset: 53
        map << button_circle;                 // offset: 54
        map << button_triangle;               // offset: 55
        map << button_left_shoulder;          // offset: 56
        map << button_right_shoulder;         // offset: 57
        map << button_left_trigger;           // offset: 58
        map << button_right_trigger;          // offset: 59
        map << button_left_joystick;          // offset: 60
        map << button_right_joystick;         // offset: 61
        map << button_share;                  // offset: 62
        map << button_options;                // offset: 63
        map << button_touchpad;               // offset: 64
        map << button_ps;                     // offset: 65
        map << connected;                     // offset: 66
        map << bluetooth;                     // offset: 67
        map << debug;                         // offset: 68
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> axis_lx;                       // offset: 0
        map >> axis_ly;                       // offset: 4
        map >> axis_rx;                       // offset: 8
        map >> axis_ry;                       // offset: 12
        map >> gyro_x;                        // offset: 16
        map >> gyro_y;                        // offset: 20
        map >> gyro_z;                        // offset: 24
        map >> accel_x;                       // offset: 28
        map >> accel_y;                       // offset: 32
        map >> accel_z;                       // offset: 36
        map >> battery_level;                 // offset: 40
        map >> temp;                          // offset: 44
        map >> button_left;                   // offset: 48
        map >> button_right;                  // offset: 49
        map >> button_up;                     // offset: 50
        map >> button_down;                   // offset: 51
        map >> button_square;                 // offset: 52
        map >> button_cross;                  // offset: 53
        map >> button_circle;                 // offset: 54
        map >> button_triangle;               // offset: 55
        map >> button_left_shoulder;          // offset: 56
        map >> button_right_shoulder;         // offset: 57
        map >> button_left_trigger;           // offset: 58
        map >> button_right_trigger;          // offset: 59
        map >> button_left_joystick;          // offset: 60
        map >> button_right_joystick;         // offset: 61
        map >> button_share;                  // offset: 62
        map >> button_options;                // offset: 63
        map >> button_touchpad;               // offset: 64
        map >> button_ps;                     // offset: 65
        map >> connected;                     // offset: 66
        map >> bluetooth;                     // offset: 67
        map >> debug;                         // offset: 68
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
