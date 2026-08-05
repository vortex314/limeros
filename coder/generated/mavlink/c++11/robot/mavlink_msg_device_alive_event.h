#pragma once
// MESSAGE DEVICE_ALIVE_EVENT PACKING

#define MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT 40020


typedef struct __mavlink_device_alive_event_t {
 uint64_t timestamp; /*< [ms] Timestamp in milliseconds since epoch*/
 char device[50]; /*<  Device name*/
 char endpoint[50]; /*<  Endpoint name*/
} mavlink_device_alive_event_t;

#define MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN 108
#define MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN 108
#define MAVLINK_MSG_ID_40020_LEN 108
#define MAVLINK_MSG_ID_40020_MIN_LEN 108

#define MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_CRC 131
#define MAVLINK_MSG_ID_40020_CRC 131

#define MAVLINK_MSG_DEVICE_ALIVE_EVENT_FIELD_DEVICE_LEN 50
#define MAVLINK_MSG_DEVICE_ALIVE_EVENT_FIELD_ENDPOINT_LEN 50

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DEVICE_ALIVE_EVENT { \
    40020, \
    "DEVICE_ALIVE_EVENT", \
    3, \
    {  { "device", NULL, MAVLINK_TYPE_CHAR, 50, 8, offsetof(mavlink_device_alive_event_t, device) }, \
         { "endpoint", NULL, MAVLINK_TYPE_CHAR, 50, 58, offsetof(mavlink_device_alive_event_t, endpoint) }, \
         { "timestamp", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_device_alive_event_t, timestamp) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DEVICE_ALIVE_EVENT { \
    "DEVICE_ALIVE_EVENT", \
    3, \
    {  { "device", NULL, MAVLINK_TYPE_CHAR, 50, 8, offsetof(mavlink_device_alive_event_t, device) }, \
         { "endpoint", NULL, MAVLINK_TYPE_CHAR, 50, 58, offsetof(mavlink_device_alive_event_t, endpoint) }, \
         { "timestamp", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_device_alive_event_t, timestamp) }, \
         } \
}
#endif

/**
 * @brief Pack a device_alive_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param device  Device name
 * @param endpoint  Endpoint name
 * @param timestamp [ms] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_device_alive_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *device, const char *endpoint, uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_char_array(buf, 8, device, 50);
    _mav_put_char_array(buf, 58, endpoint, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN);
#else
    mavlink_device_alive_event_t packet;
    packet.timestamp = timestamp;
    mav_array_assign_char(packet.device, device, 50);
    mav_array_assign_char(packet.endpoint, endpoint, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_CRC);
}

/**
 * @brief Pack a device_alive_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param device  Device name
 * @param endpoint  Endpoint name
 * @param timestamp [ms] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_device_alive_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               const char *device, const char *endpoint, uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_char_array(buf, 8, device, 50);
    _mav_put_char_array(buf, 58, endpoint, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN);
#else
    mavlink_device_alive_event_t packet;
    packet.timestamp = timestamp;
    mav_array_memcpy(packet.device, device, sizeof(char)*50);
    mav_array_memcpy(packet.endpoint, endpoint, sizeof(char)*50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN);
#endif
}

/**
 * @brief Pack a device_alive_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param device  Device name
 * @param endpoint  Endpoint name
 * @param timestamp [ms] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_device_alive_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *device,const char *endpoint,uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_char_array(buf, 8, device, 50);
    _mav_put_char_array(buf, 58, endpoint, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN);
#else
    mavlink_device_alive_event_t packet;
    packet.timestamp = timestamp;
    mav_array_assign_char(packet.device, device, 50);
    mav_array_assign_char(packet.endpoint, endpoint, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_CRC);
}

/**
 * @brief Encode a device_alive_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param device_alive_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_device_alive_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_device_alive_event_t* device_alive_event)
{
    return mavlink_msg_device_alive_event_pack(system_id, component_id, msg, device_alive_event->device, device_alive_event->endpoint, device_alive_event->timestamp);
}

/**
 * @brief Encode a device_alive_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param device_alive_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_device_alive_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_device_alive_event_t* device_alive_event)
{
    return mavlink_msg_device_alive_event_pack_chan(system_id, component_id, chan, msg, device_alive_event->device, device_alive_event->endpoint, device_alive_event->timestamp);
}

/**
 * @brief Encode a device_alive_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param device_alive_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_device_alive_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_device_alive_event_t* device_alive_event)
{
    return mavlink_msg_device_alive_event_pack_status(system_id, component_id, _status, msg,  device_alive_event->device, device_alive_event->endpoint, device_alive_event->timestamp);
}

/**
 * @brief Send a device_alive_event message
 * @param chan MAVLink channel to send the message
 *
 * @param device  Device name
 * @param endpoint  Endpoint name
 * @param timestamp [ms] Timestamp in milliseconds since epoch
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_device_alive_event_send(mavlink_channel_t chan, const char *device, const char *endpoint, uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_char_array(buf, 8, device, 50);
    _mav_put_char_array(buf, 58, endpoint, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT, buf, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_CRC);
#else
    mavlink_device_alive_event_t packet;
    packet.timestamp = timestamp;
    mav_array_assign_char(packet.device, device, 50);
    mav_array_assign_char(packet.endpoint, endpoint, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT, (const char *)&packet, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_CRC);
#endif
}

/**
 * @brief Send a device_alive_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_device_alive_event_send_struct(mavlink_channel_t chan, const mavlink_device_alive_event_t* device_alive_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_device_alive_event_send(chan, device_alive_event->device, device_alive_event->endpoint, device_alive_event->timestamp);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT, (const char *)device_alive_event, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_device_alive_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *device, const char *endpoint, uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_char_array(buf, 8, device, 50);
    _mav_put_char_array(buf, 58, endpoint, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT, buf, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_CRC);
#else
    mavlink_device_alive_event_t *packet = (mavlink_device_alive_event_t *)msgbuf;
    packet->timestamp = timestamp;
    mav_array_assign_char(packet->device, device, 50);
    mav_array_assign_char(packet->endpoint, endpoint, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT, (const char *)packet, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE DEVICE_ALIVE_EVENT UNPACKING


/**
 * @brief Get field device from device_alive_event message
 *
 * @return  Device name
 */
static inline uint16_t mavlink_msg_device_alive_event_get_device(const mavlink_message_t* msg, char *device)
{
    return _MAV_RETURN_char_array(msg, device, 50,  8);
}

/**
 * @brief Get field endpoint from device_alive_event message
 *
 * @return  Endpoint name
 */
static inline uint16_t mavlink_msg_device_alive_event_get_endpoint(const mavlink_message_t* msg, char *endpoint)
{
    return _MAV_RETURN_char_array(msg, endpoint, 50,  58);
}

/**
 * @brief Get field timestamp from device_alive_event message
 *
 * @return [ms] Timestamp in milliseconds since epoch
 */
static inline uint64_t mavlink_msg_device_alive_event_get_timestamp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Decode a device_alive_event message into a struct
 *
 * @param msg The message to decode
 * @param device_alive_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_device_alive_event_decode(const mavlink_message_t* msg, mavlink_device_alive_event_t* device_alive_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    device_alive_event->timestamp = mavlink_msg_device_alive_event_get_timestamp(msg);
    mavlink_msg_device_alive_event_get_device(msg, device_alive_event->device);
    mavlink_msg_device_alive_event_get_endpoint(msg, device_alive_event->endpoint);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN? msg->len : MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN;
        memset(device_alive_event, 0, MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_LEN);
    memcpy(device_alive_event, _MAV_PAYLOAD(msg), len);
#endif
}
