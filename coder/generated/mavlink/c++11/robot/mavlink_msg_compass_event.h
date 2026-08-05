#pragma once
// MESSAGE COMPASS_EVENT PACKING

#define MAVLINK_MSG_ID_COMPASS_EVENT 40008


typedef struct __mavlink_compass_event_t {
 float heading; /*< [deg] Heading in degrees*/
 float pitch; /*< [deg] Pitch in degrees*/
 float roll; /*< [deg] Roll in degrees*/
 float mag_x; /*< [T] Magnetometer X axis in uT*/
 float mag_y; /*< [T] Magnetometer Y axis in uT*/
 float mag_z; /*< [T] Magnetometer Z axis in uT*/
 float accel_x; /*< [m_s2] Accelerometer X axis in m/s^2*/
 float accel_y; /*< [m_s2] Accelerometer Y axis in m/s^2*/
 float accel_z; /*< [m_s2] Accelerometer Z axis in m/s^2*/
} mavlink_compass_event_t;

#define MAVLINK_MSG_ID_COMPASS_EVENT_LEN 36
#define MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN 36
#define MAVLINK_MSG_ID_40008_LEN 36
#define MAVLINK_MSG_ID_40008_MIN_LEN 36

#define MAVLINK_MSG_ID_COMPASS_EVENT_CRC 154
#define MAVLINK_MSG_ID_40008_CRC 154



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_COMPASS_EVENT { \
    40008, \
    "COMPASS_EVENT", \
    9, \
    {  { "heading", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_compass_event_t, heading) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_compass_event_t, pitch) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_compass_event_t, roll) }, \
         { "mag_x", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_compass_event_t, mag_x) }, \
         { "mag_y", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_compass_event_t, mag_y) }, \
         { "mag_z", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_compass_event_t, mag_z) }, \
         { "accel_x", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_compass_event_t, accel_x) }, \
         { "accel_y", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_compass_event_t, accel_y) }, \
         { "accel_z", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_compass_event_t, accel_z) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_COMPASS_EVENT { \
    "COMPASS_EVENT", \
    9, \
    {  { "heading", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_compass_event_t, heading) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_compass_event_t, pitch) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_compass_event_t, roll) }, \
         { "mag_x", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_compass_event_t, mag_x) }, \
         { "mag_y", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_compass_event_t, mag_y) }, \
         { "mag_z", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_compass_event_t, mag_z) }, \
         { "accel_x", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_compass_event_t, accel_x) }, \
         { "accel_y", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_compass_event_t, accel_y) }, \
         { "accel_z", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_compass_event_t, accel_z) }, \
         } \
}
#endif

/**
 * @brief Pack a compass_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param heading [deg] Heading in degrees
 * @param pitch [deg] Pitch in degrees
 * @param roll [deg] Roll in degrees
 * @param mag_x [T] Magnetometer X axis in uT
 * @param mag_y [T] Magnetometer Y axis in uT
 * @param mag_z [T] Magnetometer Z axis in uT
 * @param accel_x [m_s2] Accelerometer X axis in m/s^2
 * @param accel_y [m_s2] Accelerometer Y axis in m/s^2
 * @param accel_z [m_s2] Accelerometer Z axis in m/s^2
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_compass_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float heading, float pitch, float roll, float mag_x, float mag_y, float mag_z, float accel_x, float accel_y, float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_COMPASS_EVENT_LEN];
    _mav_put_float(buf, 0, heading);
    _mav_put_float(buf, 4, pitch);
    _mav_put_float(buf, 8, roll);
    _mav_put_float(buf, 12, mag_x);
    _mav_put_float(buf, 16, mag_y);
    _mav_put_float(buf, 20, mag_z);
    _mav_put_float(buf, 24, accel_x);
    _mav_put_float(buf, 28, accel_y);
    _mav_put_float(buf, 32, accel_z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_COMPASS_EVENT_LEN);
#else
    mavlink_compass_event_t packet;
    packet.heading = heading;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.mag_x = mag_x;
    packet.mag_y = mag_y;
    packet.mag_z = mag_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_COMPASS_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_COMPASS_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_CRC);
}

/**
 * @brief Pack a compass_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param heading [deg] Heading in degrees
 * @param pitch [deg] Pitch in degrees
 * @param roll [deg] Roll in degrees
 * @param mag_x [T] Magnetometer X axis in uT
 * @param mag_y [T] Magnetometer Y axis in uT
 * @param mag_z [T] Magnetometer Z axis in uT
 * @param accel_x [m_s2] Accelerometer X axis in m/s^2
 * @param accel_y [m_s2] Accelerometer Y axis in m/s^2
 * @param accel_z [m_s2] Accelerometer Z axis in m/s^2
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_compass_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               float heading, float pitch, float roll, float mag_x, float mag_y, float mag_z, float accel_x, float accel_y, float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_COMPASS_EVENT_LEN];
    _mav_put_float(buf, 0, heading);
    _mav_put_float(buf, 4, pitch);
    _mav_put_float(buf, 8, roll);
    _mav_put_float(buf, 12, mag_x);
    _mav_put_float(buf, 16, mag_y);
    _mav_put_float(buf, 20, mag_z);
    _mav_put_float(buf, 24, accel_x);
    _mav_put_float(buf, 28, accel_y);
    _mav_put_float(buf, 32, accel_z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_COMPASS_EVENT_LEN);
#else
    mavlink_compass_event_t packet;
    packet.heading = heading;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.mag_x = mag_x;
    packet.mag_y = mag_y;
    packet.mag_z = mag_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_COMPASS_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_COMPASS_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_LEN);
#endif
}

/**
 * @brief Pack a compass_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param heading [deg] Heading in degrees
 * @param pitch [deg] Pitch in degrees
 * @param roll [deg] Roll in degrees
 * @param mag_x [T] Magnetometer X axis in uT
 * @param mag_y [T] Magnetometer Y axis in uT
 * @param mag_z [T] Magnetometer Z axis in uT
 * @param accel_x [m_s2] Accelerometer X axis in m/s^2
 * @param accel_y [m_s2] Accelerometer Y axis in m/s^2
 * @param accel_z [m_s2] Accelerometer Z axis in m/s^2
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_compass_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float heading,float pitch,float roll,float mag_x,float mag_y,float mag_z,float accel_x,float accel_y,float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_COMPASS_EVENT_LEN];
    _mav_put_float(buf, 0, heading);
    _mav_put_float(buf, 4, pitch);
    _mav_put_float(buf, 8, roll);
    _mav_put_float(buf, 12, mag_x);
    _mav_put_float(buf, 16, mag_y);
    _mav_put_float(buf, 20, mag_z);
    _mav_put_float(buf, 24, accel_x);
    _mav_put_float(buf, 28, accel_y);
    _mav_put_float(buf, 32, accel_z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_COMPASS_EVENT_LEN);
#else
    mavlink_compass_event_t packet;
    packet.heading = heading;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.mag_x = mag_x;
    packet.mag_y = mag_y;
    packet.mag_z = mag_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_COMPASS_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_COMPASS_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_CRC);
}

/**
 * @brief Encode a compass_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param compass_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_compass_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_compass_event_t* compass_event)
{
    return mavlink_msg_compass_event_pack(system_id, component_id, msg, compass_event->heading, compass_event->pitch, compass_event->roll, compass_event->mag_x, compass_event->mag_y, compass_event->mag_z, compass_event->accel_x, compass_event->accel_y, compass_event->accel_z);
}

/**
 * @brief Encode a compass_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param compass_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_compass_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_compass_event_t* compass_event)
{
    return mavlink_msg_compass_event_pack_chan(system_id, component_id, chan, msg, compass_event->heading, compass_event->pitch, compass_event->roll, compass_event->mag_x, compass_event->mag_y, compass_event->mag_z, compass_event->accel_x, compass_event->accel_y, compass_event->accel_z);
}

/**
 * @brief Encode a compass_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param compass_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_compass_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_compass_event_t* compass_event)
{
    return mavlink_msg_compass_event_pack_status(system_id, component_id, _status, msg,  compass_event->heading, compass_event->pitch, compass_event->roll, compass_event->mag_x, compass_event->mag_y, compass_event->mag_z, compass_event->accel_x, compass_event->accel_y, compass_event->accel_z);
}

/**
 * @brief Send a compass_event message
 * @param chan MAVLink channel to send the message
 *
 * @param heading [deg] Heading in degrees
 * @param pitch [deg] Pitch in degrees
 * @param roll [deg] Roll in degrees
 * @param mag_x [T] Magnetometer X axis in uT
 * @param mag_y [T] Magnetometer Y axis in uT
 * @param mag_z [T] Magnetometer Z axis in uT
 * @param accel_x [m_s2] Accelerometer X axis in m/s^2
 * @param accel_y [m_s2] Accelerometer Y axis in m/s^2
 * @param accel_z [m_s2] Accelerometer Z axis in m/s^2
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_compass_event_send(mavlink_channel_t chan, float heading, float pitch, float roll, float mag_x, float mag_y, float mag_z, float accel_x, float accel_y, float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_COMPASS_EVENT_LEN];
    _mav_put_float(buf, 0, heading);
    _mav_put_float(buf, 4, pitch);
    _mav_put_float(buf, 8, roll);
    _mav_put_float(buf, 12, mag_x);
    _mav_put_float(buf, 16, mag_y);
    _mav_put_float(buf, 20, mag_z);
    _mav_put_float(buf, 24, accel_x);
    _mav_put_float(buf, 28, accel_y);
    _mav_put_float(buf, 32, accel_z);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COMPASS_EVENT, buf, MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_CRC);
#else
    mavlink_compass_event_t packet;
    packet.heading = heading;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.mag_x = mag_x;
    packet.mag_y = mag_y;
    packet.mag_z = mag_z;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COMPASS_EVENT, (const char *)&packet, MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_CRC);
#endif
}

/**
 * @brief Send a compass_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_compass_event_send_struct(mavlink_channel_t chan, const mavlink_compass_event_t* compass_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_compass_event_send(chan, compass_event->heading, compass_event->pitch, compass_event->roll, compass_event->mag_x, compass_event->mag_y, compass_event->mag_z, compass_event->accel_x, compass_event->accel_y, compass_event->accel_z);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COMPASS_EVENT, (const char *)compass_event, MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_COMPASS_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_compass_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float heading, float pitch, float roll, float mag_x, float mag_y, float mag_z, float accel_x, float accel_y, float accel_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, heading);
    _mav_put_float(buf, 4, pitch);
    _mav_put_float(buf, 8, roll);
    _mav_put_float(buf, 12, mag_x);
    _mav_put_float(buf, 16, mag_y);
    _mav_put_float(buf, 20, mag_z);
    _mav_put_float(buf, 24, accel_x);
    _mav_put_float(buf, 28, accel_y);
    _mav_put_float(buf, 32, accel_z);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COMPASS_EVENT, buf, MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_CRC);
#else
    mavlink_compass_event_t *packet = (mavlink_compass_event_t *)msgbuf;
    packet->heading = heading;
    packet->pitch = pitch;
    packet->roll = roll;
    packet->mag_x = mag_x;
    packet->mag_y = mag_y;
    packet->mag_z = mag_z;
    packet->accel_x = accel_x;
    packet->accel_y = accel_y;
    packet->accel_z = accel_z;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COMPASS_EVENT, (const char *)packet, MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_LEN, MAVLINK_MSG_ID_COMPASS_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE COMPASS_EVENT UNPACKING


/**
 * @brief Get field heading from compass_event message
 *
 * @return [deg] Heading in degrees
 */
static inline float mavlink_msg_compass_event_get_heading(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field pitch from compass_event message
 *
 * @return [deg] Pitch in degrees
 */
static inline float mavlink_msg_compass_event_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field roll from compass_event message
 *
 * @return [deg] Roll in degrees
 */
static inline float mavlink_msg_compass_event_get_roll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field mag_x from compass_event message
 *
 * @return [T] Magnetometer X axis in uT
 */
static inline float mavlink_msg_compass_event_get_mag_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field mag_y from compass_event message
 *
 * @return [T] Magnetometer Y axis in uT
 */
static inline float mavlink_msg_compass_event_get_mag_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field mag_z from compass_event message
 *
 * @return [T] Magnetometer Z axis in uT
 */
static inline float mavlink_msg_compass_event_get_mag_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field accel_x from compass_event message
 *
 * @return [m_s2] Accelerometer X axis in m/s^2
 */
static inline float mavlink_msg_compass_event_get_accel_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field accel_y from compass_event message
 *
 * @return [m_s2] Accelerometer Y axis in m/s^2
 */
static inline float mavlink_msg_compass_event_get_accel_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field accel_z from compass_event message
 *
 * @return [m_s2] Accelerometer Z axis in m/s^2
 */
static inline float mavlink_msg_compass_event_get_accel_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Decode a compass_event message into a struct
 *
 * @param msg The message to decode
 * @param compass_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_compass_event_decode(const mavlink_message_t* msg, mavlink_compass_event_t* compass_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    compass_event->heading = mavlink_msg_compass_event_get_heading(msg);
    compass_event->pitch = mavlink_msg_compass_event_get_pitch(msg);
    compass_event->roll = mavlink_msg_compass_event_get_roll(msg);
    compass_event->mag_x = mavlink_msg_compass_event_get_mag_x(msg);
    compass_event->mag_y = mavlink_msg_compass_event_get_mag_y(msg);
    compass_event->mag_z = mavlink_msg_compass_event_get_mag_z(msg);
    compass_event->accel_x = mavlink_msg_compass_event_get_accel_x(msg);
    compass_event->accel_y = mavlink_msg_compass_event_get_accel_y(msg);
    compass_event->accel_z = mavlink_msg_compass_event_get_accel_z(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_COMPASS_EVENT_LEN? msg->len : MAVLINK_MSG_ID_COMPASS_EVENT_LEN;
        memset(compass_event, 0, MAVLINK_MSG_ID_COMPASS_EVENT_LEN);
    memcpy(compass_event, _MAV_PAYLOAD(msg), len);
#endif
}
