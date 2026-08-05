#pragma once
// MESSAGE US_EVENT PACKING

#define MAVLINK_MSG_ID_US_EVENT 40010


typedef struct __mavlink_us_event_t {
 float distance; /*< [m] Distance in meters*/
 float temperature; /*< [degC] Temperature in Celsius*/
 int32_t status; /*<  Status code, 0 if no error*/
} mavlink_us_event_t;

#define MAVLINK_MSG_ID_US_EVENT_LEN 12
#define MAVLINK_MSG_ID_US_EVENT_MIN_LEN 12
#define MAVLINK_MSG_ID_40010_LEN 12
#define MAVLINK_MSG_ID_40010_MIN_LEN 12

#define MAVLINK_MSG_ID_US_EVENT_CRC 169
#define MAVLINK_MSG_ID_40010_CRC 169



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_US_EVENT { \
    40010, \
    "US_EVENT", \
    3, \
    {  { "distance", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_us_event_t, distance) }, \
         { "temperature", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_us_event_t, temperature) }, \
         { "status", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_us_event_t, status) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_US_EVENT { \
    "US_EVENT", \
    3, \
    {  { "distance", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_us_event_t, distance) }, \
         { "temperature", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_us_event_t, temperature) }, \
         { "status", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_us_event_t, status) }, \
         } \
}
#endif

/**
 * @brief Pack a us_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param distance [m] Distance in meters
 * @param temperature [degC] Temperature in Celsius
 * @param status  Status code, 0 if no error
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_us_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float distance, float temperature, int32_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_US_EVENT_LEN];
    _mav_put_float(buf, 0, distance);
    _mav_put_float(buf, 4, temperature);
    _mav_put_int32_t(buf, 8, status);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_US_EVENT_LEN);
#else
    mavlink_us_event_t packet;
    packet.distance = distance;
    packet.temperature = temperature;
    packet.status = status;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_US_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_US_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_US_EVENT_MIN_LEN, MAVLINK_MSG_ID_US_EVENT_LEN, MAVLINK_MSG_ID_US_EVENT_CRC);
}

/**
 * @brief Pack a us_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param distance [m] Distance in meters
 * @param temperature [degC] Temperature in Celsius
 * @param status  Status code, 0 if no error
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_us_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               float distance, float temperature, int32_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_US_EVENT_LEN];
    _mav_put_float(buf, 0, distance);
    _mav_put_float(buf, 4, temperature);
    _mav_put_int32_t(buf, 8, status);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_US_EVENT_LEN);
#else
    mavlink_us_event_t packet;
    packet.distance = distance;
    packet.temperature = temperature;
    packet.status = status;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_US_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_US_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_US_EVENT_MIN_LEN, MAVLINK_MSG_ID_US_EVENT_LEN, MAVLINK_MSG_ID_US_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_US_EVENT_MIN_LEN, MAVLINK_MSG_ID_US_EVENT_LEN);
#endif
}

/**
 * @brief Pack a us_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param distance [m] Distance in meters
 * @param temperature [degC] Temperature in Celsius
 * @param status  Status code, 0 if no error
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_us_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float distance,float temperature,int32_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_US_EVENT_LEN];
    _mav_put_float(buf, 0, distance);
    _mav_put_float(buf, 4, temperature);
    _mav_put_int32_t(buf, 8, status);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_US_EVENT_LEN);
#else
    mavlink_us_event_t packet;
    packet.distance = distance;
    packet.temperature = temperature;
    packet.status = status;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_US_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_US_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_US_EVENT_MIN_LEN, MAVLINK_MSG_ID_US_EVENT_LEN, MAVLINK_MSG_ID_US_EVENT_CRC);
}

/**
 * @brief Encode a us_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param us_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_us_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_us_event_t* us_event)
{
    return mavlink_msg_us_event_pack(system_id, component_id, msg, us_event->distance, us_event->temperature, us_event->status);
}

/**
 * @brief Encode a us_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param us_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_us_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_us_event_t* us_event)
{
    return mavlink_msg_us_event_pack_chan(system_id, component_id, chan, msg, us_event->distance, us_event->temperature, us_event->status);
}

/**
 * @brief Encode a us_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param us_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_us_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_us_event_t* us_event)
{
    return mavlink_msg_us_event_pack_status(system_id, component_id, _status, msg,  us_event->distance, us_event->temperature, us_event->status);
}

/**
 * @brief Send a us_event message
 * @param chan MAVLink channel to send the message
 *
 * @param distance [m] Distance in meters
 * @param temperature [degC] Temperature in Celsius
 * @param status  Status code, 0 if no error
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_us_event_send(mavlink_channel_t chan, float distance, float temperature, int32_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_US_EVENT_LEN];
    _mav_put_float(buf, 0, distance);
    _mav_put_float(buf, 4, temperature);
    _mav_put_int32_t(buf, 8, status);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_US_EVENT, buf, MAVLINK_MSG_ID_US_EVENT_MIN_LEN, MAVLINK_MSG_ID_US_EVENT_LEN, MAVLINK_MSG_ID_US_EVENT_CRC);
#else
    mavlink_us_event_t packet;
    packet.distance = distance;
    packet.temperature = temperature;
    packet.status = status;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_US_EVENT, (const char *)&packet, MAVLINK_MSG_ID_US_EVENT_MIN_LEN, MAVLINK_MSG_ID_US_EVENT_LEN, MAVLINK_MSG_ID_US_EVENT_CRC);
#endif
}

/**
 * @brief Send a us_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_us_event_send_struct(mavlink_channel_t chan, const mavlink_us_event_t* us_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_us_event_send(chan, us_event->distance, us_event->temperature, us_event->status);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_US_EVENT, (const char *)us_event, MAVLINK_MSG_ID_US_EVENT_MIN_LEN, MAVLINK_MSG_ID_US_EVENT_LEN, MAVLINK_MSG_ID_US_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_US_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_us_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float distance, float temperature, int32_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, distance);
    _mav_put_float(buf, 4, temperature);
    _mav_put_int32_t(buf, 8, status);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_US_EVENT, buf, MAVLINK_MSG_ID_US_EVENT_MIN_LEN, MAVLINK_MSG_ID_US_EVENT_LEN, MAVLINK_MSG_ID_US_EVENT_CRC);
#else
    mavlink_us_event_t *packet = (mavlink_us_event_t *)msgbuf;
    packet->distance = distance;
    packet->temperature = temperature;
    packet->status = status;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_US_EVENT, (const char *)packet, MAVLINK_MSG_ID_US_EVENT_MIN_LEN, MAVLINK_MSG_ID_US_EVENT_LEN, MAVLINK_MSG_ID_US_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE US_EVENT UNPACKING


/**
 * @brief Get field distance from us_event message
 *
 * @return [m] Distance in meters
 */
static inline float mavlink_msg_us_event_get_distance(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field temperature from us_event message
 *
 * @return [degC] Temperature in Celsius
 */
static inline float mavlink_msg_us_event_get_temperature(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field status from us_event message
 *
 * @return  Status code, 0 if no error
 */
static inline int32_t mavlink_msg_us_event_get_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Decode a us_event message into a struct
 *
 * @param msg The message to decode
 * @param us_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_us_event_decode(const mavlink_message_t* msg, mavlink_us_event_t* us_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    us_event->distance = mavlink_msg_us_event_get_distance(msg);
    us_event->temperature = mavlink_msg_us_event_get_temperature(msg);
    us_event->status = mavlink_msg_us_event_get_status(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_US_EVENT_LEN? msg->len : MAVLINK_MSG_ID_US_EVENT_LEN;
        memset(us_event, 0, MAVLINK_MSG_ID_US_EVENT_LEN);
    memcpy(us_event, _MAV_PAYLOAD(msg), len);
#endif
}
