#pragma once
// MESSAGE IMU_EVENT PACKING

#define MAVLINK_MSG_ID_IMU_EVENT 40009


typedef struct __mavlink_imu_event_t {
 float gyro_x; /*< [deg_s] Gyroscope X axis in deg/s*/
 float gyro_y; /*< [deg_s] Gyroscope Y axis in deg/s*/
 float gyro_z; /*< [deg_s] Gyroscope Z axis in deg/s*/
 float accel_x; /*< [m_s2] Accelerometer X axis in m/s^2*/
 float accel_y; /*< [m_s2] Accelerometer Y axis in m/s^2*/
 float accel_z; /*< [m_s2] Accelerometer Z axis in m/s^2*/
} mavlink_imu_event_t;

#define MAVLINK_MSG_ID_IMU_EVENT_LEN 24
#define MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN 24
#define MAVLINK_MSG_ID_40009_LEN 24
#define MAVLINK_MSG_ID_40009_MIN_LEN 24

#define MAVLINK_MSG_ID_IMU_EVENT_CRC 106
#define MAVLINK_MSG_ID_40009_CRC 106



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IMU_EVENT { \
    40009, \
    "IMU_EVENT", \
    6, \
    {  { "gyro_x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_imu_event_t, gyro_x) }, \
         { "gyro_y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_imu_event_t, gyro_y) }, \
         { "gyro_z", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_imu_event_t, gyro_z) }, \
         { "accel_x", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_imu_event_t, accel_x) }, \
         { "accel_y", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_imu_event_t, accel_y) }, \
         { "accel_z", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_imu_event_t, accel_z) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IMU_EVENT { \
    "IMU_EVENT", \
    6, \
    {  { "gyro_x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_imu_event_t, gyro_x) }, \
         { "gyro_y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_imu_event_t, gyro_y) }, \
         { "gyro_z", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_imu_event_t, gyro_z) }, \
         { "accel_x", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_imu_event_t, accel_x) }, \
         { "accel_y", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_imu_event_t, accel_y) }, \
         { "accel_z", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_imu_event_t, accel_z) }, \
         } \
}
#endif

/**
 * @brief Pack a imu_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param gyro_x [deg_s] Gyroscope X axis in deg/s
 * @param gyro_y [deg_s] Gyroscope Y axis in deg/s
 * @param gyro_z [deg_s] Gyroscope Z axis in deg/s
 * @param accel_x [m_s2] Accelerometer X axis in m/s^2
 * @param accel_y [m_s2] Accelerometer Y axis in m/s^2
 * @param accel_z [m_s2] Accelerometer Z axis in m/s^2
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_imu_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float gyro_x, float gyro_y, float gyro_z, float accel_x, float accel_y, float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_EVENT_LEN];
    _mav_put_float(buf, 0, gyro_x);
    _mav_put_float(buf, 4, gyro_y);
    _mav_put_float(buf, 8, gyro_z);
    _mav_put_float(buf, 12, accel_x);
    _mav_put_float(buf, 16, accel_y);
    _mav_put_float(buf, 20, accel_z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IMU_EVENT_LEN);
#else
    mavlink_imu_event_t packet;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IMU_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IMU_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN, MAVLINK_MSG_ID_IMU_EVENT_LEN, MAVLINK_MSG_ID_IMU_EVENT_CRC);
}

/**
 * @brief Pack a imu_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param gyro_x [deg_s] Gyroscope X axis in deg/s
 * @param gyro_y [deg_s] Gyroscope Y axis in deg/s
 * @param gyro_z [deg_s] Gyroscope Z axis in deg/s
 * @param accel_x [m_s2] Accelerometer X axis in m/s^2
 * @param accel_y [m_s2] Accelerometer Y axis in m/s^2
 * @param accel_z [m_s2] Accelerometer Z axis in m/s^2
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_imu_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               float gyro_x, float gyro_y, float gyro_z, float accel_x, float accel_y, float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_EVENT_LEN];
    _mav_put_float(buf, 0, gyro_x);
    _mav_put_float(buf, 4, gyro_y);
    _mav_put_float(buf, 8, gyro_z);
    _mav_put_float(buf, 12, accel_x);
    _mav_put_float(buf, 16, accel_y);
    _mav_put_float(buf, 20, accel_z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IMU_EVENT_LEN);
#else
    mavlink_imu_event_t packet;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IMU_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IMU_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN, MAVLINK_MSG_ID_IMU_EVENT_LEN, MAVLINK_MSG_ID_IMU_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN, MAVLINK_MSG_ID_IMU_EVENT_LEN);
#endif
}

/**
 * @brief Pack a imu_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param gyro_x [deg_s] Gyroscope X axis in deg/s
 * @param gyro_y [deg_s] Gyroscope Y axis in deg/s
 * @param gyro_z [deg_s] Gyroscope Z axis in deg/s
 * @param accel_x [m_s2] Accelerometer X axis in m/s^2
 * @param accel_y [m_s2] Accelerometer Y axis in m/s^2
 * @param accel_z [m_s2] Accelerometer Z axis in m/s^2
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_imu_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float gyro_x,float gyro_y,float gyro_z,float accel_x,float accel_y,float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_EVENT_LEN];
    _mav_put_float(buf, 0, gyro_x);
    _mav_put_float(buf, 4, gyro_y);
    _mav_put_float(buf, 8, gyro_z);
    _mav_put_float(buf, 12, accel_x);
    _mav_put_float(buf, 16, accel_y);
    _mav_put_float(buf, 20, accel_z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IMU_EVENT_LEN);
#else
    mavlink_imu_event_t packet;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IMU_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IMU_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN, MAVLINK_MSG_ID_IMU_EVENT_LEN, MAVLINK_MSG_ID_IMU_EVENT_CRC);
}

/**
 * @brief Encode a imu_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param imu_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_imu_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_imu_event_t* imu_event)
{
    return mavlink_msg_imu_event_pack(system_id, component_id, msg, imu_event->gyro_x, imu_event->gyro_y, imu_event->gyro_z, imu_event->accel_x, imu_event->accel_y, imu_event->accel_z);
}

/**
 * @brief Encode a imu_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param imu_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_imu_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_imu_event_t* imu_event)
{
    return mavlink_msg_imu_event_pack_chan(system_id, component_id, chan, msg, imu_event->gyro_x, imu_event->gyro_y, imu_event->gyro_z, imu_event->accel_x, imu_event->accel_y, imu_event->accel_z);
}

/**
 * @brief Encode a imu_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param imu_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_imu_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_imu_event_t* imu_event)
{
    return mavlink_msg_imu_event_pack_status(system_id, component_id, _status, msg,  imu_event->gyro_x, imu_event->gyro_y, imu_event->gyro_z, imu_event->accel_x, imu_event->accel_y, imu_event->accel_z);
}

/**
 * @brief Send a imu_event message
 * @param chan MAVLink channel to send the message
 *
 * @param gyro_x [deg_s] Gyroscope X axis in deg/s
 * @param gyro_y [deg_s] Gyroscope Y axis in deg/s
 * @param gyro_z [deg_s] Gyroscope Z axis in deg/s
 * @param accel_x [m_s2] Accelerometer X axis in m/s^2
 * @param accel_y [m_s2] Accelerometer Y axis in m/s^2
 * @param accel_z [m_s2] Accelerometer Z axis in m/s^2
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_imu_event_send(mavlink_channel_t chan, float gyro_x, float gyro_y, float gyro_z, float accel_x, float accel_y, float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_EVENT_LEN];
    _mav_put_float(buf, 0, gyro_x);
    _mav_put_float(buf, 4, gyro_y);
    _mav_put_float(buf, 8, gyro_z);
    _mav_put_float(buf, 12, accel_x);
    _mav_put_float(buf, 16, accel_y);
    _mav_put_float(buf, 20, accel_z);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_EVENT, buf, MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN, MAVLINK_MSG_ID_IMU_EVENT_LEN, MAVLINK_MSG_ID_IMU_EVENT_CRC);
#else
    mavlink_imu_event_t packet;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_EVENT, (const char *)&packet, MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN, MAVLINK_MSG_ID_IMU_EVENT_LEN, MAVLINK_MSG_ID_IMU_EVENT_CRC);
#endif
}

/**
 * @brief Send a imu_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_imu_event_send_struct(mavlink_channel_t chan, const mavlink_imu_event_t* imu_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_imu_event_send(chan, imu_event->gyro_x, imu_event->gyro_y, imu_event->gyro_z, imu_event->accel_x, imu_event->accel_y, imu_event->accel_z);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_EVENT, (const char *)imu_event, MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN, MAVLINK_MSG_ID_IMU_EVENT_LEN, MAVLINK_MSG_ID_IMU_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_IMU_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_imu_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float gyro_x, float gyro_y, float gyro_z, float accel_x, float accel_y, float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, gyro_x);
    _mav_put_float(buf, 4, gyro_y);
    _mav_put_float(buf, 8, gyro_z);
    _mav_put_float(buf, 12, accel_x);
    _mav_put_float(buf, 16, accel_y);
    _mav_put_float(buf, 20, accel_z);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_EVENT, buf, MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN, MAVLINK_MSG_ID_IMU_EVENT_LEN, MAVLINK_MSG_ID_IMU_EVENT_CRC);
#else
    mavlink_imu_event_t *packet = (mavlink_imu_event_t *)msgbuf;
    packet->gyro_x = gyro_x;
    packet->gyro_y = gyro_y;
    packet->gyro_z = gyro_z;
    packet->accel_x = accel_x;
    packet->accel_y = accel_y;
    packet->accel_z = accel_z;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_EVENT, (const char *)packet, MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN, MAVLINK_MSG_ID_IMU_EVENT_LEN, MAVLINK_MSG_ID_IMU_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE IMU_EVENT UNPACKING


/**
 * @brief Get field gyro_x from imu_event message
 *
 * @return [deg_s] Gyroscope X axis in deg/s
 */
static inline float mavlink_msg_imu_event_get_gyro_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field gyro_y from imu_event message
 *
 * @return [deg_s] Gyroscope Y axis in deg/s
 */
static inline float mavlink_msg_imu_event_get_gyro_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field gyro_z from imu_event message
 *
 * @return [deg_s] Gyroscope Z axis in deg/s
 */
static inline float mavlink_msg_imu_event_get_gyro_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field accel_x from imu_event message
 *
 * @return [m_s2] Accelerometer X axis in m/s^2
 */
static inline float mavlink_msg_imu_event_get_accel_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field accel_y from imu_event message
 *
 * @return [m_s2] Accelerometer Y axis in m/s^2
 */
static inline float mavlink_msg_imu_event_get_accel_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field accel_z from imu_event message
 *
 * @return [m_s2] Accelerometer Z axis in m/s^2
 */
static inline float mavlink_msg_imu_event_get_accel_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Decode a imu_event message into a struct
 *
 * @param msg The message to decode
 * @param imu_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_imu_event_decode(const mavlink_message_t* msg, mavlink_imu_event_t* imu_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    imu_event->gyro_x = mavlink_msg_imu_event_get_gyro_x(msg);
    imu_event->gyro_y = mavlink_msg_imu_event_get_gyro_y(msg);
    imu_event->gyro_z = mavlink_msg_imu_event_get_gyro_z(msg);
    imu_event->accel_x = mavlink_msg_imu_event_get_accel_x(msg);
    imu_event->accel_y = mavlink_msg_imu_event_get_accel_y(msg);
    imu_event->accel_z = mavlink_msg_imu_event_get_accel_z(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IMU_EVENT_LEN? msg->len : MAVLINK_MSG_ID_IMU_EVENT_LEN;
        memset(imu_event, 0, MAVLINK_MSG_ID_IMU_EVENT_LEN);
    memcpy(imu_event, _MAV_PAYLOAD(msg), len);
#endif
}
