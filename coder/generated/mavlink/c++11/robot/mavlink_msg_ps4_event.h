#pragma once
// MESSAGE PS4_EVENT PACKING

#define MAVLINK_MSG_ID_PS4_EVENT 40011


typedef struct __mavlink_ps4_event_t {
 int32_t axis_lx; /*<  Left stick X axis*/
 int32_t axis_ly; /*<  Left stick Y axis*/
 int32_t axis_rx; /*<  Right stick X axis*/
 int32_t axis_ry; /*<  Right stick Y axis*/
 int32_t gyro_x; /*< [deg_s] Gyroscope X axis*/
 int32_t gyro_y; /*< [deg_s] Gyroscope Y axis*/
 int32_t gyro_z; /*< [deg_s] Gyroscope Z axis*/
 int32_t accel_x; /*< [m_s2] Accelerometer X axis*/
 int32_t accel_y; /*< [m_s2] Accelerometer Y axis*/
 int32_t accel_z; /*< [m_s2] Accelerometer Z axis*/
 int32_t battery_level; /*< [%] Battery level*/
 int32_t temp; /*< [degC] Temperature*/
 uint8_t button_left; /*<  Left button pressed*/
 uint8_t button_right; /*<  Right button pressed*/
 uint8_t button_up; /*<  Up button pressed*/
 uint8_t button_down; /*<  Down button pressed*/
 uint8_t button_square; /*<  Square button pressed*/
 uint8_t button_cross; /*<  Cross button pressed*/
 uint8_t button_circle; /*<  Circle button pressed*/
 uint8_t button_triangle; /*<  Triangle button pressed*/
 uint8_t button_left_shoulder; /*<  Left shoulder button pressed*/
 uint8_t button_right_shoulder; /*<  Right shoulder button pressed*/
 uint8_t button_left_trigger; /*<  Left trigger pressed*/
 uint8_t button_right_trigger; /*<  Right trigger pressed*/
 uint8_t button_left_joystick; /*<  Left joystick button pressed*/
 uint8_t button_right_joystick; /*<  Right joystick button pressed*/
 uint8_t button_share; /*<  Share button pressed*/
 uint8_t button_options; /*<  Options button pressed*/
 uint8_t button_touchpad; /*<  Touchpad button pressed*/
 uint8_t button_ps; /*<  PS button pressed*/
 uint8_t connected; /*<  Controller connected*/
 uint8_t bluetooth; /*<  Connected over Bluetooth*/
 char debug[100]; /*<  Debug string*/
} mavlink_ps4_event_t;

#define MAVLINK_MSG_ID_PS4_EVENT_LEN 168
#define MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN 168
#define MAVLINK_MSG_ID_40011_LEN 168
#define MAVLINK_MSG_ID_40011_MIN_LEN 168

#define MAVLINK_MSG_ID_PS4_EVENT_CRC 12
#define MAVLINK_MSG_ID_40011_CRC 12

#define MAVLINK_MSG_PS4_EVENT_FIELD_DEBUG_LEN 100

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_PS4_EVENT { \
    40011, \
    "PS4_EVENT", \
    33, \
    {  { "button_left", NULL, MAVLINK_TYPE_UINT8_T, 0, 48, offsetof(mavlink_ps4_event_t, button_left) }, \
         { "button_right", NULL, MAVLINK_TYPE_UINT8_T, 0, 49, offsetof(mavlink_ps4_event_t, button_right) }, \
         { "button_up", NULL, MAVLINK_TYPE_UINT8_T, 0, 50, offsetof(mavlink_ps4_event_t, button_up) }, \
         { "button_down", NULL, MAVLINK_TYPE_UINT8_T, 0, 51, offsetof(mavlink_ps4_event_t, button_down) }, \
         { "button_square", NULL, MAVLINK_TYPE_UINT8_T, 0, 52, offsetof(mavlink_ps4_event_t, button_square) }, \
         { "button_cross", NULL, MAVLINK_TYPE_UINT8_T, 0, 53, offsetof(mavlink_ps4_event_t, button_cross) }, \
         { "button_circle", NULL, MAVLINK_TYPE_UINT8_T, 0, 54, offsetof(mavlink_ps4_event_t, button_circle) }, \
         { "button_triangle", NULL, MAVLINK_TYPE_UINT8_T, 0, 55, offsetof(mavlink_ps4_event_t, button_triangle) }, \
         { "button_left_shoulder", NULL, MAVLINK_TYPE_UINT8_T, 0, 56, offsetof(mavlink_ps4_event_t, button_left_shoulder) }, \
         { "button_right_shoulder", NULL, MAVLINK_TYPE_UINT8_T, 0, 57, offsetof(mavlink_ps4_event_t, button_right_shoulder) }, \
         { "button_left_trigger", NULL, MAVLINK_TYPE_UINT8_T, 0, 58, offsetof(mavlink_ps4_event_t, button_left_trigger) }, \
         { "button_right_trigger", NULL, MAVLINK_TYPE_UINT8_T, 0, 59, offsetof(mavlink_ps4_event_t, button_right_trigger) }, \
         { "button_left_joystick", NULL, MAVLINK_TYPE_UINT8_T, 0, 60, offsetof(mavlink_ps4_event_t, button_left_joystick) }, \
         { "button_right_joystick", NULL, MAVLINK_TYPE_UINT8_T, 0, 61, offsetof(mavlink_ps4_event_t, button_right_joystick) }, \
         { "button_share", NULL, MAVLINK_TYPE_UINT8_T, 0, 62, offsetof(mavlink_ps4_event_t, button_share) }, \
         { "button_options", NULL, MAVLINK_TYPE_UINT8_T, 0, 63, offsetof(mavlink_ps4_event_t, button_options) }, \
         { "button_touchpad", NULL, MAVLINK_TYPE_UINT8_T, 0, 64, offsetof(mavlink_ps4_event_t, button_touchpad) }, \
         { "button_ps", NULL, MAVLINK_TYPE_UINT8_T, 0, 65, offsetof(mavlink_ps4_event_t, button_ps) }, \
         { "axis_lx", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_ps4_event_t, axis_lx) }, \
         { "axis_ly", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_ps4_event_t, axis_ly) }, \
         { "axis_rx", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_ps4_event_t, axis_rx) }, \
         { "axis_ry", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_ps4_event_t, axis_ry) }, \
         { "gyro_x", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_ps4_event_t, gyro_x) }, \
         { "gyro_y", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_ps4_event_t, gyro_y) }, \
         { "gyro_z", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_ps4_event_t, gyro_z) }, \
         { "accel_x", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_ps4_event_t, accel_x) }, \
         { "accel_y", NULL, MAVLINK_TYPE_INT32_T, 0, 32, offsetof(mavlink_ps4_event_t, accel_y) }, \
         { "accel_z", NULL, MAVLINK_TYPE_INT32_T, 0, 36, offsetof(mavlink_ps4_event_t, accel_z) }, \
         { "connected", NULL, MAVLINK_TYPE_UINT8_T, 0, 66, offsetof(mavlink_ps4_event_t, connected) }, \
         { "battery_level", NULL, MAVLINK_TYPE_INT32_T, 0, 40, offsetof(mavlink_ps4_event_t, battery_level) }, \
         { "bluetooth", NULL, MAVLINK_TYPE_UINT8_T, 0, 67, offsetof(mavlink_ps4_event_t, bluetooth) }, \
         { "debug", NULL, MAVLINK_TYPE_CHAR, 100, 68, offsetof(mavlink_ps4_event_t, debug) }, \
         { "temp", NULL, MAVLINK_TYPE_INT32_T, 0, 44, offsetof(mavlink_ps4_event_t, temp) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_PS4_EVENT { \
    "PS4_EVENT", \
    33, \
    {  { "button_left", NULL, MAVLINK_TYPE_UINT8_T, 0, 48, offsetof(mavlink_ps4_event_t, button_left) }, \
         { "button_right", NULL, MAVLINK_TYPE_UINT8_T, 0, 49, offsetof(mavlink_ps4_event_t, button_right) }, \
         { "button_up", NULL, MAVLINK_TYPE_UINT8_T, 0, 50, offsetof(mavlink_ps4_event_t, button_up) }, \
         { "button_down", NULL, MAVLINK_TYPE_UINT8_T, 0, 51, offsetof(mavlink_ps4_event_t, button_down) }, \
         { "button_square", NULL, MAVLINK_TYPE_UINT8_T, 0, 52, offsetof(mavlink_ps4_event_t, button_square) }, \
         { "button_cross", NULL, MAVLINK_TYPE_UINT8_T, 0, 53, offsetof(mavlink_ps4_event_t, button_cross) }, \
         { "button_circle", NULL, MAVLINK_TYPE_UINT8_T, 0, 54, offsetof(mavlink_ps4_event_t, button_circle) }, \
         { "button_triangle", NULL, MAVLINK_TYPE_UINT8_T, 0, 55, offsetof(mavlink_ps4_event_t, button_triangle) }, \
         { "button_left_shoulder", NULL, MAVLINK_TYPE_UINT8_T, 0, 56, offsetof(mavlink_ps4_event_t, button_left_shoulder) }, \
         { "button_right_shoulder", NULL, MAVLINK_TYPE_UINT8_T, 0, 57, offsetof(mavlink_ps4_event_t, button_right_shoulder) }, \
         { "button_left_trigger", NULL, MAVLINK_TYPE_UINT8_T, 0, 58, offsetof(mavlink_ps4_event_t, button_left_trigger) }, \
         { "button_right_trigger", NULL, MAVLINK_TYPE_UINT8_T, 0, 59, offsetof(mavlink_ps4_event_t, button_right_trigger) }, \
         { "button_left_joystick", NULL, MAVLINK_TYPE_UINT8_T, 0, 60, offsetof(mavlink_ps4_event_t, button_left_joystick) }, \
         { "button_right_joystick", NULL, MAVLINK_TYPE_UINT8_T, 0, 61, offsetof(mavlink_ps4_event_t, button_right_joystick) }, \
         { "button_share", NULL, MAVLINK_TYPE_UINT8_T, 0, 62, offsetof(mavlink_ps4_event_t, button_share) }, \
         { "button_options", NULL, MAVLINK_TYPE_UINT8_T, 0, 63, offsetof(mavlink_ps4_event_t, button_options) }, \
         { "button_touchpad", NULL, MAVLINK_TYPE_UINT8_T, 0, 64, offsetof(mavlink_ps4_event_t, button_touchpad) }, \
         { "button_ps", NULL, MAVLINK_TYPE_UINT8_T, 0, 65, offsetof(mavlink_ps4_event_t, button_ps) }, \
         { "axis_lx", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_ps4_event_t, axis_lx) }, \
         { "axis_ly", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_ps4_event_t, axis_ly) }, \
         { "axis_rx", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_ps4_event_t, axis_rx) }, \
         { "axis_ry", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_ps4_event_t, axis_ry) }, \
         { "gyro_x", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_ps4_event_t, gyro_x) }, \
         { "gyro_y", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_ps4_event_t, gyro_y) }, \
         { "gyro_z", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_ps4_event_t, gyro_z) }, \
         { "accel_x", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_ps4_event_t, accel_x) }, \
         { "accel_y", NULL, MAVLINK_TYPE_INT32_T, 0, 32, offsetof(mavlink_ps4_event_t, accel_y) }, \
         { "accel_z", NULL, MAVLINK_TYPE_INT32_T, 0, 36, offsetof(mavlink_ps4_event_t, accel_z) }, \
         { "connected", NULL, MAVLINK_TYPE_UINT8_T, 0, 66, offsetof(mavlink_ps4_event_t, connected) }, \
         { "battery_level", NULL, MAVLINK_TYPE_INT32_T, 0, 40, offsetof(mavlink_ps4_event_t, battery_level) }, \
         { "bluetooth", NULL, MAVLINK_TYPE_UINT8_T, 0, 67, offsetof(mavlink_ps4_event_t, bluetooth) }, \
         { "debug", NULL, MAVLINK_TYPE_CHAR, 100, 68, offsetof(mavlink_ps4_event_t, debug) }, \
         { "temp", NULL, MAVLINK_TYPE_INT32_T, 0, 44, offsetof(mavlink_ps4_event_t, temp) }, \
         } \
}
#endif

/**
 * @brief Pack a ps4_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param button_left  Left button pressed
 * @param button_right  Right button pressed
 * @param button_up  Up button pressed
 * @param button_down  Down button pressed
 * @param button_square  Square button pressed
 * @param button_cross  Cross button pressed
 * @param button_circle  Circle button pressed
 * @param button_triangle  Triangle button pressed
 * @param button_left_shoulder  Left shoulder button pressed
 * @param button_right_shoulder  Right shoulder button pressed
 * @param button_left_trigger  Left trigger pressed
 * @param button_right_trigger  Right trigger pressed
 * @param button_left_joystick  Left joystick button pressed
 * @param button_right_joystick  Right joystick button pressed
 * @param button_share  Share button pressed
 * @param button_options  Options button pressed
 * @param button_touchpad  Touchpad button pressed
 * @param button_ps  PS button pressed
 * @param axis_lx  Left stick X axis
 * @param axis_ly  Left stick Y axis
 * @param axis_rx  Right stick X axis
 * @param axis_ry  Right stick Y axis
 * @param gyro_x [deg_s] Gyroscope X axis
 * @param gyro_y [deg_s] Gyroscope Y axis
 * @param gyro_z [deg_s] Gyroscope Z axis
 * @param accel_x [m_s2] Accelerometer X axis
 * @param accel_y [m_s2] Accelerometer Y axis
 * @param accel_z [m_s2] Accelerometer Z axis
 * @param connected  Controller connected
 * @param battery_level [%] Battery level
 * @param bluetooth  Connected over Bluetooth
 * @param debug  Debug string
 * @param temp [degC] Temperature
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ps4_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t button_left, uint8_t button_right, uint8_t button_up, uint8_t button_down, uint8_t button_square, uint8_t button_cross, uint8_t button_circle, uint8_t button_triangle, uint8_t button_left_shoulder, uint8_t button_right_shoulder, uint8_t button_left_trigger, uint8_t button_right_trigger, uint8_t button_left_joystick, uint8_t button_right_joystick, uint8_t button_share, uint8_t button_options, uint8_t button_touchpad, uint8_t button_ps, int32_t axis_lx, int32_t axis_ly, int32_t axis_rx, int32_t axis_ry, int32_t gyro_x, int32_t gyro_y, int32_t gyro_z, int32_t accel_x, int32_t accel_y, int32_t accel_z, uint8_t connected, int32_t battery_level, uint8_t bluetooth, const char *debug, int32_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PS4_EVENT_LEN];
    _mav_put_int32_t(buf, 0, axis_lx);
    _mav_put_int32_t(buf, 4, axis_ly);
    _mav_put_int32_t(buf, 8, axis_rx);
    _mav_put_int32_t(buf, 12, axis_ry);
    _mav_put_int32_t(buf, 16, gyro_x);
    _mav_put_int32_t(buf, 20, gyro_y);
    _mav_put_int32_t(buf, 24, gyro_z);
    _mav_put_int32_t(buf, 28, accel_x);
    _mav_put_int32_t(buf, 32, accel_y);
    _mav_put_int32_t(buf, 36, accel_z);
    _mav_put_int32_t(buf, 40, battery_level);
    _mav_put_int32_t(buf, 44, temp);
    _mav_put_uint8_t(buf, 48, button_left);
    _mav_put_uint8_t(buf, 49, button_right);
    _mav_put_uint8_t(buf, 50, button_up);
    _mav_put_uint8_t(buf, 51, button_down);
    _mav_put_uint8_t(buf, 52, button_square);
    _mav_put_uint8_t(buf, 53, button_cross);
    _mav_put_uint8_t(buf, 54, button_circle);
    _mav_put_uint8_t(buf, 55, button_triangle);
    _mav_put_uint8_t(buf, 56, button_left_shoulder);
    _mav_put_uint8_t(buf, 57, button_right_shoulder);
    _mav_put_uint8_t(buf, 58, button_left_trigger);
    _mav_put_uint8_t(buf, 59, button_right_trigger);
    _mav_put_uint8_t(buf, 60, button_left_joystick);
    _mav_put_uint8_t(buf, 61, button_right_joystick);
    _mav_put_uint8_t(buf, 62, button_share);
    _mav_put_uint8_t(buf, 63, button_options);
    _mav_put_uint8_t(buf, 64, button_touchpad);
    _mav_put_uint8_t(buf, 65, button_ps);
    _mav_put_uint8_t(buf, 66, connected);
    _mav_put_uint8_t(buf, 67, bluetooth);
    _mav_put_char_array(buf, 68, debug, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PS4_EVENT_LEN);
#else
    mavlink_ps4_event_t packet;
    packet.axis_lx = axis_lx;
    packet.axis_ly = axis_ly;
    packet.axis_rx = axis_rx;
    packet.axis_ry = axis_ry;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;
    packet.battery_level = battery_level;
    packet.temp = temp;
    packet.button_left = button_left;
    packet.button_right = button_right;
    packet.button_up = button_up;
    packet.button_down = button_down;
    packet.button_square = button_square;
    packet.button_cross = button_cross;
    packet.button_circle = button_circle;
    packet.button_triangle = button_triangle;
    packet.button_left_shoulder = button_left_shoulder;
    packet.button_right_shoulder = button_right_shoulder;
    packet.button_left_trigger = button_left_trigger;
    packet.button_right_trigger = button_right_trigger;
    packet.button_left_joystick = button_left_joystick;
    packet.button_right_joystick = button_right_joystick;
    packet.button_share = button_share;
    packet.button_options = button_options;
    packet.button_touchpad = button_touchpad;
    packet.button_ps = button_ps;
    packet.connected = connected;
    packet.bluetooth = bluetooth;
    mav_array_assign_char(packet.debug, debug, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PS4_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PS4_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN, MAVLINK_MSG_ID_PS4_EVENT_LEN, MAVLINK_MSG_ID_PS4_EVENT_CRC);
}

/**
 * @brief Pack a ps4_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param button_left  Left button pressed
 * @param button_right  Right button pressed
 * @param button_up  Up button pressed
 * @param button_down  Down button pressed
 * @param button_square  Square button pressed
 * @param button_cross  Cross button pressed
 * @param button_circle  Circle button pressed
 * @param button_triangle  Triangle button pressed
 * @param button_left_shoulder  Left shoulder button pressed
 * @param button_right_shoulder  Right shoulder button pressed
 * @param button_left_trigger  Left trigger pressed
 * @param button_right_trigger  Right trigger pressed
 * @param button_left_joystick  Left joystick button pressed
 * @param button_right_joystick  Right joystick button pressed
 * @param button_share  Share button pressed
 * @param button_options  Options button pressed
 * @param button_touchpad  Touchpad button pressed
 * @param button_ps  PS button pressed
 * @param axis_lx  Left stick X axis
 * @param axis_ly  Left stick Y axis
 * @param axis_rx  Right stick X axis
 * @param axis_ry  Right stick Y axis
 * @param gyro_x [deg_s] Gyroscope X axis
 * @param gyro_y [deg_s] Gyroscope Y axis
 * @param gyro_z [deg_s] Gyroscope Z axis
 * @param accel_x [m_s2] Accelerometer X axis
 * @param accel_y [m_s2] Accelerometer Y axis
 * @param accel_z [m_s2] Accelerometer Z axis
 * @param connected  Controller connected
 * @param battery_level [%] Battery level
 * @param bluetooth  Connected over Bluetooth
 * @param debug  Debug string
 * @param temp [degC] Temperature
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ps4_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t button_left, uint8_t button_right, uint8_t button_up, uint8_t button_down, uint8_t button_square, uint8_t button_cross, uint8_t button_circle, uint8_t button_triangle, uint8_t button_left_shoulder, uint8_t button_right_shoulder, uint8_t button_left_trigger, uint8_t button_right_trigger, uint8_t button_left_joystick, uint8_t button_right_joystick, uint8_t button_share, uint8_t button_options, uint8_t button_touchpad, uint8_t button_ps, int32_t axis_lx, int32_t axis_ly, int32_t axis_rx, int32_t axis_ry, int32_t gyro_x, int32_t gyro_y, int32_t gyro_z, int32_t accel_x, int32_t accel_y, int32_t accel_z, uint8_t connected, int32_t battery_level, uint8_t bluetooth, const char *debug, int32_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PS4_EVENT_LEN];
    _mav_put_int32_t(buf, 0, axis_lx);
    _mav_put_int32_t(buf, 4, axis_ly);
    _mav_put_int32_t(buf, 8, axis_rx);
    _mav_put_int32_t(buf, 12, axis_ry);
    _mav_put_int32_t(buf, 16, gyro_x);
    _mav_put_int32_t(buf, 20, gyro_y);
    _mav_put_int32_t(buf, 24, gyro_z);
    _mav_put_int32_t(buf, 28, accel_x);
    _mav_put_int32_t(buf, 32, accel_y);
    _mav_put_int32_t(buf, 36, accel_z);
    _mav_put_int32_t(buf, 40, battery_level);
    _mav_put_int32_t(buf, 44, temp);
    _mav_put_uint8_t(buf, 48, button_left);
    _mav_put_uint8_t(buf, 49, button_right);
    _mav_put_uint8_t(buf, 50, button_up);
    _mav_put_uint8_t(buf, 51, button_down);
    _mav_put_uint8_t(buf, 52, button_square);
    _mav_put_uint8_t(buf, 53, button_cross);
    _mav_put_uint8_t(buf, 54, button_circle);
    _mav_put_uint8_t(buf, 55, button_triangle);
    _mav_put_uint8_t(buf, 56, button_left_shoulder);
    _mav_put_uint8_t(buf, 57, button_right_shoulder);
    _mav_put_uint8_t(buf, 58, button_left_trigger);
    _mav_put_uint8_t(buf, 59, button_right_trigger);
    _mav_put_uint8_t(buf, 60, button_left_joystick);
    _mav_put_uint8_t(buf, 61, button_right_joystick);
    _mav_put_uint8_t(buf, 62, button_share);
    _mav_put_uint8_t(buf, 63, button_options);
    _mav_put_uint8_t(buf, 64, button_touchpad);
    _mav_put_uint8_t(buf, 65, button_ps);
    _mav_put_uint8_t(buf, 66, connected);
    _mav_put_uint8_t(buf, 67, bluetooth);
    _mav_put_char_array(buf, 68, debug, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PS4_EVENT_LEN);
#else
    mavlink_ps4_event_t packet;
    packet.axis_lx = axis_lx;
    packet.axis_ly = axis_ly;
    packet.axis_rx = axis_rx;
    packet.axis_ry = axis_ry;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;
    packet.battery_level = battery_level;
    packet.temp = temp;
    packet.button_left = button_left;
    packet.button_right = button_right;
    packet.button_up = button_up;
    packet.button_down = button_down;
    packet.button_square = button_square;
    packet.button_cross = button_cross;
    packet.button_circle = button_circle;
    packet.button_triangle = button_triangle;
    packet.button_left_shoulder = button_left_shoulder;
    packet.button_right_shoulder = button_right_shoulder;
    packet.button_left_trigger = button_left_trigger;
    packet.button_right_trigger = button_right_trigger;
    packet.button_left_joystick = button_left_joystick;
    packet.button_right_joystick = button_right_joystick;
    packet.button_share = button_share;
    packet.button_options = button_options;
    packet.button_touchpad = button_touchpad;
    packet.button_ps = button_ps;
    packet.connected = connected;
    packet.bluetooth = bluetooth;
    mav_array_memcpy(packet.debug, debug, sizeof(char)*100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PS4_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PS4_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN, MAVLINK_MSG_ID_PS4_EVENT_LEN, MAVLINK_MSG_ID_PS4_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN, MAVLINK_MSG_ID_PS4_EVENT_LEN);
#endif
}

/**
 * @brief Pack a ps4_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param button_left  Left button pressed
 * @param button_right  Right button pressed
 * @param button_up  Up button pressed
 * @param button_down  Down button pressed
 * @param button_square  Square button pressed
 * @param button_cross  Cross button pressed
 * @param button_circle  Circle button pressed
 * @param button_triangle  Triangle button pressed
 * @param button_left_shoulder  Left shoulder button pressed
 * @param button_right_shoulder  Right shoulder button pressed
 * @param button_left_trigger  Left trigger pressed
 * @param button_right_trigger  Right trigger pressed
 * @param button_left_joystick  Left joystick button pressed
 * @param button_right_joystick  Right joystick button pressed
 * @param button_share  Share button pressed
 * @param button_options  Options button pressed
 * @param button_touchpad  Touchpad button pressed
 * @param button_ps  PS button pressed
 * @param axis_lx  Left stick X axis
 * @param axis_ly  Left stick Y axis
 * @param axis_rx  Right stick X axis
 * @param axis_ry  Right stick Y axis
 * @param gyro_x [deg_s] Gyroscope X axis
 * @param gyro_y [deg_s] Gyroscope Y axis
 * @param gyro_z [deg_s] Gyroscope Z axis
 * @param accel_x [m_s2] Accelerometer X axis
 * @param accel_y [m_s2] Accelerometer Y axis
 * @param accel_z [m_s2] Accelerometer Z axis
 * @param connected  Controller connected
 * @param battery_level [%] Battery level
 * @param bluetooth  Connected over Bluetooth
 * @param debug  Debug string
 * @param temp [degC] Temperature
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ps4_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t button_left,uint8_t button_right,uint8_t button_up,uint8_t button_down,uint8_t button_square,uint8_t button_cross,uint8_t button_circle,uint8_t button_triangle,uint8_t button_left_shoulder,uint8_t button_right_shoulder,uint8_t button_left_trigger,uint8_t button_right_trigger,uint8_t button_left_joystick,uint8_t button_right_joystick,uint8_t button_share,uint8_t button_options,uint8_t button_touchpad,uint8_t button_ps,int32_t axis_lx,int32_t axis_ly,int32_t axis_rx,int32_t axis_ry,int32_t gyro_x,int32_t gyro_y,int32_t gyro_z,int32_t accel_x,int32_t accel_y,int32_t accel_z,uint8_t connected,int32_t battery_level,uint8_t bluetooth,const char *debug,int32_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PS4_EVENT_LEN];
    _mav_put_int32_t(buf, 0, axis_lx);
    _mav_put_int32_t(buf, 4, axis_ly);
    _mav_put_int32_t(buf, 8, axis_rx);
    _mav_put_int32_t(buf, 12, axis_ry);
    _mav_put_int32_t(buf, 16, gyro_x);
    _mav_put_int32_t(buf, 20, gyro_y);
    _mav_put_int32_t(buf, 24, gyro_z);
    _mav_put_int32_t(buf, 28, accel_x);
    _mav_put_int32_t(buf, 32, accel_y);
    _mav_put_int32_t(buf, 36, accel_z);
    _mav_put_int32_t(buf, 40, battery_level);
    _mav_put_int32_t(buf, 44, temp);
    _mav_put_uint8_t(buf, 48, button_left);
    _mav_put_uint8_t(buf, 49, button_right);
    _mav_put_uint8_t(buf, 50, button_up);
    _mav_put_uint8_t(buf, 51, button_down);
    _mav_put_uint8_t(buf, 52, button_square);
    _mav_put_uint8_t(buf, 53, button_cross);
    _mav_put_uint8_t(buf, 54, button_circle);
    _mav_put_uint8_t(buf, 55, button_triangle);
    _mav_put_uint8_t(buf, 56, button_left_shoulder);
    _mav_put_uint8_t(buf, 57, button_right_shoulder);
    _mav_put_uint8_t(buf, 58, button_left_trigger);
    _mav_put_uint8_t(buf, 59, button_right_trigger);
    _mav_put_uint8_t(buf, 60, button_left_joystick);
    _mav_put_uint8_t(buf, 61, button_right_joystick);
    _mav_put_uint8_t(buf, 62, button_share);
    _mav_put_uint8_t(buf, 63, button_options);
    _mav_put_uint8_t(buf, 64, button_touchpad);
    _mav_put_uint8_t(buf, 65, button_ps);
    _mav_put_uint8_t(buf, 66, connected);
    _mav_put_uint8_t(buf, 67, bluetooth);
    _mav_put_char_array(buf, 68, debug, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PS4_EVENT_LEN);
#else
    mavlink_ps4_event_t packet;
    packet.axis_lx = axis_lx;
    packet.axis_ly = axis_ly;
    packet.axis_rx = axis_rx;
    packet.axis_ry = axis_ry;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;
    packet.battery_level = battery_level;
    packet.temp = temp;
    packet.button_left = button_left;
    packet.button_right = button_right;
    packet.button_up = button_up;
    packet.button_down = button_down;
    packet.button_square = button_square;
    packet.button_cross = button_cross;
    packet.button_circle = button_circle;
    packet.button_triangle = button_triangle;
    packet.button_left_shoulder = button_left_shoulder;
    packet.button_right_shoulder = button_right_shoulder;
    packet.button_left_trigger = button_left_trigger;
    packet.button_right_trigger = button_right_trigger;
    packet.button_left_joystick = button_left_joystick;
    packet.button_right_joystick = button_right_joystick;
    packet.button_share = button_share;
    packet.button_options = button_options;
    packet.button_touchpad = button_touchpad;
    packet.button_ps = button_ps;
    packet.connected = connected;
    packet.bluetooth = bluetooth;
    mav_array_assign_char(packet.debug, debug, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PS4_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PS4_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN, MAVLINK_MSG_ID_PS4_EVENT_LEN, MAVLINK_MSG_ID_PS4_EVENT_CRC);
}

/**
 * @brief Encode a ps4_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ps4_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ps4_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ps4_event_t* ps4_event)
{
    return mavlink_msg_ps4_event_pack(system_id, component_id, msg, ps4_event->button_left, ps4_event->button_right, ps4_event->button_up, ps4_event->button_down, ps4_event->button_square, ps4_event->button_cross, ps4_event->button_circle, ps4_event->button_triangle, ps4_event->button_left_shoulder, ps4_event->button_right_shoulder, ps4_event->button_left_trigger, ps4_event->button_right_trigger, ps4_event->button_left_joystick, ps4_event->button_right_joystick, ps4_event->button_share, ps4_event->button_options, ps4_event->button_touchpad, ps4_event->button_ps, ps4_event->axis_lx, ps4_event->axis_ly, ps4_event->axis_rx, ps4_event->axis_ry, ps4_event->gyro_x, ps4_event->gyro_y, ps4_event->gyro_z, ps4_event->accel_x, ps4_event->accel_y, ps4_event->accel_z, ps4_event->connected, ps4_event->battery_level, ps4_event->bluetooth, ps4_event->debug, ps4_event->temp);
}

/**
 * @brief Encode a ps4_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ps4_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ps4_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ps4_event_t* ps4_event)
{
    return mavlink_msg_ps4_event_pack_chan(system_id, component_id, chan, msg, ps4_event->button_left, ps4_event->button_right, ps4_event->button_up, ps4_event->button_down, ps4_event->button_square, ps4_event->button_cross, ps4_event->button_circle, ps4_event->button_triangle, ps4_event->button_left_shoulder, ps4_event->button_right_shoulder, ps4_event->button_left_trigger, ps4_event->button_right_trigger, ps4_event->button_left_joystick, ps4_event->button_right_joystick, ps4_event->button_share, ps4_event->button_options, ps4_event->button_touchpad, ps4_event->button_ps, ps4_event->axis_lx, ps4_event->axis_ly, ps4_event->axis_rx, ps4_event->axis_ry, ps4_event->gyro_x, ps4_event->gyro_y, ps4_event->gyro_z, ps4_event->accel_x, ps4_event->accel_y, ps4_event->accel_z, ps4_event->connected, ps4_event->battery_level, ps4_event->bluetooth, ps4_event->debug, ps4_event->temp);
}

/**
 * @brief Encode a ps4_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ps4_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ps4_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ps4_event_t* ps4_event)
{
    return mavlink_msg_ps4_event_pack_status(system_id, component_id, _status, msg,  ps4_event->button_left, ps4_event->button_right, ps4_event->button_up, ps4_event->button_down, ps4_event->button_square, ps4_event->button_cross, ps4_event->button_circle, ps4_event->button_triangle, ps4_event->button_left_shoulder, ps4_event->button_right_shoulder, ps4_event->button_left_trigger, ps4_event->button_right_trigger, ps4_event->button_left_joystick, ps4_event->button_right_joystick, ps4_event->button_share, ps4_event->button_options, ps4_event->button_touchpad, ps4_event->button_ps, ps4_event->axis_lx, ps4_event->axis_ly, ps4_event->axis_rx, ps4_event->axis_ry, ps4_event->gyro_x, ps4_event->gyro_y, ps4_event->gyro_z, ps4_event->accel_x, ps4_event->accel_y, ps4_event->accel_z, ps4_event->connected, ps4_event->battery_level, ps4_event->bluetooth, ps4_event->debug, ps4_event->temp);
}

/**
 * @brief Send a ps4_event message
 * @param chan MAVLink channel to send the message
 *
 * @param button_left  Left button pressed
 * @param button_right  Right button pressed
 * @param button_up  Up button pressed
 * @param button_down  Down button pressed
 * @param button_square  Square button pressed
 * @param button_cross  Cross button pressed
 * @param button_circle  Circle button pressed
 * @param button_triangle  Triangle button pressed
 * @param button_left_shoulder  Left shoulder button pressed
 * @param button_right_shoulder  Right shoulder button pressed
 * @param button_left_trigger  Left trigger pressed
 * @param button_right_trigger  Right trigger pressed
 * @param button_left_joystick  Left joystick button pressed
 * @param button_right_joystick  Right joystick button pressed
 * @param button_share  Share button pressed
 * @param button_options  Options button pressed
 * @param button_touchpad  Touchpad button pressed
 * @param button_ps  PS button pressed
 * @param axis_lx  Left stick X axis
 * @param axis_ly  Left stick Y axis
 * @param axis_rx  Right stick X axis
 * @param axis_ry  Right stick Y axis
 * @param gyro_x [deg_s] Gyroscope X axis
 * @param gyro_y [deg_s] Gyroscope Y axis
 * @param gyro_z [deg_s] Gyroscope Z axis
 * @param accel_x [m_s2] Accelerometer X axis
 * @param accel_y [m_s2] Accelerometer Y axis
 * @param accel_z [m_s2] Accelerometer Z axis
 * @param connected  Controller connected
 * @param battery_level [%] Battery level
 * @param bluetooth  Connected over Bluetooth
 * @param debug  Debug string
 * @param temp [degC] Temperature
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ps4_event_send(mavlink_channel_t chan, uint8_t button_left, uint8_t button_right, uint8_t button_up, uint8_t button_down, uint8_t button_square, uint8_t button_cross, uint8_t button_circle, uint8_t button_triangle, uint8_t button_left_shoulder, uint8_t button_right_shoulder, uint8_t button_left_trigger, uint8_t button_right_trigger, uint8_t button_left_joystick, uint8_t button_right_joystick, uint8_t button_share, uint8_t button_options, uint8_t button_touchpad, uint8_t button_ps, int32_t axis_lx, int32_t axis_ly, int32_t axis_rx, int32_t axis_ry, int32_t gyro_x, int32_t gyro_y, int32_t gyro_z, int32_t accel_x, int32_t accel_y, int32_t accel_z, uint8_t connected, int32_t battery_level, uint8_t bluetooth, const char *debug, int32_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PS4_EVENT_LEN];
    _mav_put_int32_t(buf, 0, axis_lx);
    _mav_put_int32_t(buf, 4, axis_ly);
    _mav_put_int32_t(buf, 8, axis_rx);
    _mav_put_int32_t(buf, 12, axis_ry);
    _mav_put_int32_t(buf, 16, gyro_x);
    _mav_put_int32_t(buf, 20, gyro_y);
    _mav_put_int32_t(buf, 24, gyro_z);
    _mav_put_int32_t(buf, 28, accel_x);
    _mav_put_int32_t(buf, 32, accel_y);
    _mav_put_int32_t(buf, 36, accel_z);
    _mav_put_int32_t(buf, 40, battery_level);
    _mav_put_int32_t(buf, 44, temp);
    _mav_put_uint8_t(buf, 48, button_left);
    _mav_put_uint8_t(buf, 49, button_right);
    _mav_put_uint8_t(buf, 50, button_up);
    _mav_put_uint8_t(buf, 51, button_down);
    _mav_put_uint8_t(buf, 52, button_square);
    _mav_put_uint8_t(buf, 53, button_cross);
    _mav_put_uint8_t(buf, 54, button_circle);
    _mav_put_uint8_t(buf, 55, button_triangle);
    _mav_put_uint8_t(buf, 56, button_left_shoulder);
    _mav_put_uint8_t(buf, 57, button_right_shoulder);
    _mav_put_uint8_t(buf, 58, button_left_trigger);
    _mav_put_uint8_t(buf, 59, button_right_trigger);
    _mav_put_uint8_t(buf, 60, button_left_joystick);
    _mav_put_uint8_t(buf, 61, button_right_joystick);
    _mav_put_uint8_t(buf, 62, button_share);
    _mav_put_uint8_t(buf, 63, button_options);
    _mav_put_uint8_t(buf, 64, button_touchpad);
    _mav_put_uint8_t(buf, 65, button_ps);
    _mav_put_uint8_t(buf, 66, connected);
    _mav_put_uint8_t(buf, 67, bluetooth);
    _mav_put_char_array(buf, 68, debug, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_EVENT, buf, MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN, MAVLINK_MSG_ID_PS4_EVENT_LEN, MAVLINK_MSG_ID_PS4_EVENT_CRC);
#else
    mavlink_ps4_event_t packet;
    packet.axis_lx = axis_lx;
    packet.axis_ly = axis_ly;
    packet.axis_rx = axis_rx;
    packet.axis_ry = axis_ry;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;
    packet.battery_level = battery_level;
    packet.temp = temp;
    packet.button_left = button_left;
    packet.button_right = button_right;
    packet.button_up = button_up;
    packet.button_down = button_down;
    packet.button_square = button_square;
    packet.button_cross = button_cross;
    packet.button_circle = button_circle;
    packet.button_triangle = button_triangle;
    packet.button_left_shoulder = button_left_shoulder;
    packet.button_right_shoulder = button_right_shoulder;
    packet.button_left_trigger = button_left_trigger;
    packet.button_right_trigger = button_right_trigger;
    packet.button_left_joystick = button_left_joystick;
    packet.button_right_joystick = button_right_joystick;
    packet.button_share = button_share;
    packet.button_options = button_options;
    packet.button_touchpad = button_touchpad;
    packet.button_ps = button_ps;
    packet.connected = connected;
    packet.bluetooth = bluetooth;
    mav_array_assign_char(packet.debug, debug, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_EVENT, (const char *)&packet, MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN, MAVLINK_MSG_ID_PS4_EVENT_LEN, MAVLINK_MSG_ID_PS4_EVENT_CRC);
#endif
}

/**
 * @brief Send a ps4_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ps4_event_send_struct(mavlink_channel_t chan, const mavlink_ps4_event_t* ps4_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ps4_event_send(chan, ps4_event->button_left, ps4_event->button_right, ps4_event->button_up, ps4_event->button_down, ps4_event->button_square, ps4_event->button_cross, ps4_event->button_circle, ps4_event->button_triangle, ps4_event->button_left_shoulder, ps4_event->button_right_shoulder, ps4_event->button_left_trigger, ps4_event->button_right_trigger, ps4_event->button_left_joystick, ps4_event->button_right_joystick, ps4_event->button_share, ps4_event->button_options, ps4_event->button_touchpad, ps4_event->button_ps, ps4_event->axis_lx, ps4_event->axis_ly, ps4_event->axis_rx, ps4_event->axis_ry, ps4_event->gyro_x, ps4_event->gyro_y, ps4_event->gyro_z, ps4_event->accel_x, ps4_event->accel_y, ps4_event->accel_z, ps4_event->connected, ps4_event->battery_level, ps4_event->bluetooth, ps4_event->debug, ps4_event->temp);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_EVENT, (const char *)ps4_event, MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN, MAVLINK_MSG_ID_PS4_EVENT_LEN, MAVLINK_MSG_ID_PS4_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_PS4_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ps4_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t button_left, uint8_t button_right, uint8_t button_up, uint8_t button_down, uint8_t button_square, uint8_t button_cross, uint8_t button_circle, uint8_t button_triangle, uint8_t button_left_shoulder, uint8_t button_right_shoulder, uint8_t button_left_trigger, uint8_t button_right_trigger, uint8_t button_left_joystick, uint8_t button_right_joystick, uint8_t button_share, uint8_t button_options, uint8_t button_touchpad, uint8_t button_ps, int32_t axis_lx, int32_t axis_ly, int32_t axis_rx, int32_t axis_ry, int32_t gyro_x, int32_t gyro_y, int32_t gyro_z, int32_t accel_x, int32_t accel_y, int32_t accel_z, uint8_t connected, int32_t battery_level, uint8_t bluetooth, const char *debug, int32_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, axis_lx);
    _mav_put_int32_t(buf, 4, axis_ly);
    _mav_put_int32_t(buf, 8, axis_rx);
    _mav_put_int32_t(buf, 12, axis_ry);
    _mav_put_int32_t(buf, 16, gyro_x);
    _mav_put_int32_t(buf, 20, gyro_y);
    _mav_put_int32_t(buf, 24, gyro_z);
    _mav_put_int32_t(buf, 28, accel_x);
    _mav_put_int32_t(buf, 32, accel_y);
    _mav_put_int32_t(buf, 36, accel_z);
    _mav_put_int32_t(buf, 40, battery_level);
    _mav_put_int32_t(buf, 44, temp);
    _mav_put_uint8_t(buf, 48, button_left);
    _mav_put_uint8_t(buf, 49, button_right);
    _mav_put_uint8_t(buf, 50, button_up);
    _mav_put_uint8_t(buf, 51, button_down);
    _mav_put_uint8_t(buf, 52, button_square);
    _mav_put_uint8_t(buf, 53, button_cross);
    _mav_put_uint8_t(buf, 54, button_circle);
    _mav_put_uint8_t(buf, 55, button_triangle);
    _mav_put_uint8_t(buf, 56, button_left_shoulder);
    _mav_put_uint8_t(buf, 57, button_right_shoulder);
    _mav_put_uint8_t(buf, 58, button_left_trigger);
    _mav_put_uint8_t(buf, 59, button_right_trigger);
    _mav_put_uint8_t(buf, 60, button_left_joystick);
    _mav_put_uint8_t(buf, 61, button_right_joystick);
    _mav_put_uint8_t(buf, 62, button_share);
    _mav_put_uint8_t(buf, 63, button_options);
    _mav_put_uint8_t(buf, 64, button_touchpad);
    _mav_put_uint8_t(buf, 65, button_ps);
    _mav_put_uint8_t(buf, 66, connected);
    _mav_put_uint8_t(buf, 67, bluetooth);
    _mav_put_char_array(buf, 68, debug, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_EVENT, buf, MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN, MAVLINK_MSG_ID_PS4_EVENT_LEN, MAVLINK_MSG_ID_PS4_EVENT_CRC);
#else
    mavlink_ps4_event_t *packet = (mavlink_ps4_event_t *)msgbuf;
    packet->axis_lx = axis_lx;
    packet->axis_ly = axis_ly;
    packet->axis_rx = axis_rx;
    packet->axis_ry = axis_ry;
    packet->gyro_x = gyro_x;
    packet->gyro_y = gyro_y;
    packet->gyro_z = gyro_z;
    packet->accel_x = accel_x;
    packet->accel_y = accel_y;
    packet->accel_z = accel_z;
    packet->battery_level = battery_level;
    packet->temp = temp;
    packet->button_left = button_left;
    packet->button_right = button_right;
    packet->button_up = button_up;
    packet->button_down = button_down;
    packet->button_square = button_square;
    packet->button_cross = button_cross;
    packet->button_circle = button_circle;
    packet->button_triangle = button_triangle;
    packet->button_left_shoulder = button_left_shoulder;
    packet->button_right_shoulder = button_right_shoulder;
    packet->button_left_trigger = button_left_trigger;
    packet->button_right_trigger = button_right_trigger;
    packet->button_left_joystick = button_left_joystick;
    packet->button_right_joystick = button_right_joystick;
    packet->button_share = button_share;
    packet->button_options = button_options;
    packet->button_touchpad = button_touchpad;
    packet->button_ps = button_ps;
    packet->connected = connected;
    packet->bluetooth = bluetooth;
    mav_array_assign_char(packet->debug, debug, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_EVENT, (const char *)packet, MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN, MAVLINK_MSG_ID_PS4_EVENT_LEN, MAVLINK_MSG_ID_PS4_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE PS4_EVENT UNPACKING


/**
 * @brief Get field button_left from ps4_event message
 *
 * @return  Left button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_left(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  48);
}

/**
 * @brief Get field button_right from ps4_event message
 *
 * @return  Right button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_right(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  49);
}

/**
 * @brief Get field button_up from ps4_event message
 *
 * @return  Up button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_up(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  50);
}

/**
 * @brief Get field button_down from ps4_event message
 *
 * @return  Down button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_down(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  51);
}

/**
 * @brief Get field button_square from ps4_event message
 *
 * @return  Square button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_square(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  52);
}

/**
 * @brief Get field button_cross from ps4_event message
 *
 * @return  Cross button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_cross(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  53);
}

/**
 * @brief Get field button_circle from ps4_event message
 *
 * @return  Circle button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_circle(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  54);
}

/**
 * @brief Get field button_triangle from ps4_event message
 *
 * @return  Triangle button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_triangle(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  55);
}

/**
 * @brief Get field button_left_shoulder from ps4_event message
 *
 * @return  Left shoulder button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_left_shoulder(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  56);
}

/**
 * @brief Get field button_right_shoulder from ps4_event message
 *
 * @return  Right shoulder button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_right_shoulder(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  57);
}

/**
 * @brief Get field button_left_trigger from ps4_event message
 *
 * @return  Left trigger pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_left_trigger(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  58);
}

/**
 * @brief Get field button_right_trigger from ps4_event message
 *
 * @return  Right trigger pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_right_trigger(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  59);
}

/**
 * @brief Get field button_left_joystick from ps4_event message
 *
 * @return  Left joystick button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_left_joystick(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  60);
}

/**
 * @brief Get field button_right_joystick from ps4_event message
 *
 * @return  Right joystick button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_right_joystick(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  61);
}

/**
 * @brief Get field button_share from ps4_event message
 *
 * @return  Share button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_share(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  62);
}

/**
 * @brief Get field button_options from ps4_event message
 *
 * @return  Options button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_options(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  63);
}

/**
 * @brief Get field button_touchpad from ps4_event message
 *
 * @return  Touchpad button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_touchpad(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  64);
}

/**
 * @brief Get field button_ps from ps4_event message
 *
 * @return  PS button pressed
 */
static inline uint8_t mavlink_msg_ps4_event_get_button_ps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  65);
}

/**
 * @brief Get field axis_lx from ps4_event message
 *
 * @return  Left stick X axis
 */
static inline int32_t mavlink_msg_ps4_event_get_axis_lx(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field axis_ly from ps4_event message
 *
 * @return  Left stick Y axis
 */
static inline int32_t mavlink_msg_ps4_event_get_axis_ly(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field axis_rx from ps4_event message
 *
 * @return  Right stick X axis
 */
static inline int32_t mavlink_msg_ps4_event_get_axis_rx(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field axis_ry from ps4_event message
 *
 * @return  Right stick Y axis
 */
static inline int32_t mavlink_msg_ps4_event_get_axis_ry(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Get field gyro_x from ps4_event message
 *
 * @return [deg_s] Gyroscope X axis
 */
static inline int32_t mavlink_msg_ps4_event_get_gyro_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  16);
}

/**
 * @brief Get field gyro_y from ps4_event message
 *
 * @return [deg_s] Gyroscope Y axis
 */
static inline int32_t mavlink_msg_ps4_event_get_gyro_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  20);
}

/**
 * @brief Get field gyro_z from ps4_event message
 *
 * @return [deg_s] Gyroscope Z axis
 */
static inline int32_t mavlink_msg_ps4_event_get_gyro_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  24);
}

/**
 * @brief Get field accel_x from ps4_event message
 *
 * @return [m_s2] Accelerometer X axis
 */
static inline int32_t mavlink_msg_ps4_event_get_accel_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  28);
}

/**
 * @brief Get field accel_y from ps4_event message
 *
 * @return [m_s2] Accelerometer Y axis
 */
static inline int32_t mavlink_msg_ps4_event_get_accel_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  32);
}

/**
 * @brief Get field accel_z from ps4_event message
 *
 * @return [m_s2] Accelerometer Z axis
 */
static inline int32_t mavlink_msg_ps4_event_get_accel_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  36);
}

/**
 * @brief Get field connected from ps4_event message
 *
 * @return  Controller connected
 */
static inline uint8_t mavlink_msg_ps4_event_get_connected(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  66);
}

/**
 * @brief Get field battery_level from ps4_event message
 *
 * @return [%] Battery level
 */
static inline int32_t mavlink_msg_ps4_event_get_battery_level(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  40);
}

/**
 * @brief Get field bluetooth from ps4_event message
 *
 * @return  Connected over Bluetooth
 */
static inline uint8_t mavlink_msg_ps4_event_get_bluetooth(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  67);
}

/**
 * @brief Get field debug from ps4_event message
 *
 * @return  Debug string
 */
static inline uint16_t mavlink_msg_ps4_event_get_debug(const mavlink_message_t* msg, char *debug)
{
    return _MAV_RETURN_char_array(msg, debug, 100,  68);
}

/**
 * @brief Get field temp from ps4_event message
 *
 * @return [degC] Temperature
 */
static inline int32_t mavlink_msg_ps4_event_get_temp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  44);
}

/**
 * @brief Decode a ps4_event message into a struct
 *
 * @param msg The message to decode
 * @param ps4_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_ps4_event_decode(const mavlink_message_t* msg, mavlink_ps4_event_t* ps4_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ps4_event->axis_lx = mavlink_msg_ps4_event_get_axis_lx(msg);
    ps4_event->axis_ly = mavlink_msg_ps4_event_get_axis_ly(msg);
    ps4_event->axis_rx = mavlink_msg_ps4_event_get_axis_rx(msg);
    ps4_event->axis_ry = mavlink_msg_ps4_event_get_axis_ry(msg);
    ps4_event->gyro_x = mavlink_msg_ps4_event_get_gyro_x(msg);
    ps4_event->gyro_y = mavlink_msg_ps4_event_get_gyro_y(msg);
    ps4_event->gyro_z = mavlink_msg_ps4_event_get_gyro_z(msg);
    ps4_event->accel_x = mavlink_msg_ps4_event_get_accel_x(msg);
    ps4_event->accel_y = mavlink_msg_ps4_event_get_accel_y(msg);
    ps4_event->accel_z = mavlink_msg_ps4_event_get_accel_z(msg);
    ps4_event->battery_level = mavlink_msg_ps4_event_get_battery_level(msg);
    ps4_event->temp = mavlink_msg_ps4_event_get_temp(msg);
    ps4_event->button_left = mavlink_msg_ps4_event_get_button_left(msg);
    ps4_event->button_right = mavlink_msg_ps4_event_get_button_right(msg);
    ps4_event->button_up = mavlink_msg_ps4_event_get_button_up(msg);
    ps4_event->button_down = mavlink_msg_ps4_event_get_button_down(msg);
    ps4_event->button_square = mavlink_msg_ps4_event_get_button_square(msg);
    ps4_event->button_cross = mavlink_msg_ps4_event_get_button_cross(msg);
    ps4_event->button_circle = mavlink_msg_ps4_event_get_button_circle(msg);
    ps4_event->button_triangle = mavlink_msg_ps4_event_get_button_triangle(msg);
    ps4_event->button_left_shoulder = mavlink_msg_ps4_event_get_button_left_shoulder(msg);
    ps4_event->button_right_shoulder = mavlink_msg_ps4_event_get_button_right_shoulder(msg);
    ps4_event->button_left_trigger = mavlink_msg_ps4_event_get_button_left_trigger(msg);
    ps4_event->button_right_trigger = mavlink_msg_ps4_event_get_button_right_trigger(msg);
    ps4_event->button_left_joystick = mavlink_msg_ps4_event_get_button_left_joystick(msg);
    ps4_event->button_right_joystick = mavlink_msg_ps4_event_get_button_right_joystick(msg);
    ps4_event->button_share = mavlink_msg_ps4_event_get_button_share(msg);
    ps4_event->button_options = mavlink_msg_ps4_event_get_button_options(msg);
    ps4_event->button_touchpad = mavlink_msg_ps4_event_get_button_touchpad(msg);
    ps4_event->button_ps = mavlink_msg_ps4_event_get_button_ps(msg);
    ps4_event->connected = mavlink_msg_ps4_event_get_connected(msg);
    ps4_event->bluetooth = mavlink_msg_ps4_event_get_bluetooth(msg);
    mavlink_msg_ps4_event_get_debug(msg, ps4_event->debug);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_PS4_EVENT_LEN? msg->len : MAVLINK_MSG_ID_PS4_EVENT_LEN;
        memset(ps4_event, 0, MAVLINK_MSG_ID_PS4_EVENT_LEN);
    memcpy(ps4_event, _MAV_PAYLOAD(msg), len);
#endif
}
