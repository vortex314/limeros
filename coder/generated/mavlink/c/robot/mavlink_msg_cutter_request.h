#pragma once
// MESSAGE CUTTER_REQUEST PACKING

#define MAVLINK_MSG_ID_CUTTER_REQUEST 40005


typedef struct __mavlink_cutter_request_t {
 int32_t rpm; /*< [rpm] Speed command for the cutter in RPM*/
 uint8_t enabled; /*<  Enable or disable the cutter*/
} mavlink_cutter_request_t;

#define MAVLINK_MSG_ID_CUTTER_REQUEST_LEN 5
#define MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN 5
#define MAVLINK_MSG_ID_40005_LEN 5
#define MAVLINK_MSG_ID_40005_MIN_LEN 5

#define MAVLINK_MSG_ID_CUTTER_REQUEST_CRC 130
#define MAVLINK_MSG_ID_40005_CRC 130



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CUTTER_REQUEST { \
    40005, \
    "CUTTER_REQUEST", \
    2, \
    {  { "enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_cutter_request_t, enabled) }, \
         { "rpm", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_cutter_request_t, rpm) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CUTTER_REQUEST { \
    "CUTTER_REQUEST", \
    2, \
    {  { "enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_cutter_request_t, enabled) }, \
         { "rpm", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_cutter_request_t, rpm) }, \
         } \
}
#endif

/**
 * @brief Pack a cutter_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param enabled  Enable or disable the cutter
 * @param rpm [rpm] Speed command for the cutter in RPM
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cutter_request_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t enabled, int32_t rpm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_REQUEST_LEN];
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_uint8_t(buf, 4, enabled);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN);
#else
    mavlink_cutter_request_t packet;
    packet.rpm = rpm;
    packet.enabled = enabled;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUTTER_REQUEST;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_CRC);
}

/**
 * @brief Pack a cutter_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param enabled  Enable or disable the cutter
 * @param rpm [rpm] Speed command for the cutter in RPM
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cutter_request_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t enabled, int32_t rpm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_REQUEST_LEN];
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_uint8_t(buf, 4, enabled);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN);
#else
    mavlink_cutter_request_t packet;
    packet.rpm = rpm;
    packet.enabled = enabled;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUTTER_REQUEST;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN);
#endif
}

/**
 * @brief Pack a cutter_request message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param enabled  Enable or disable the cutter
 * @param rpm [rpm] Speed command for the cutter in RPM
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cutter_request_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t enabled,int32_t rpm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_REQUEST_LEN];
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_uint8_t(buf, 4, enabled);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN);
#else
    mavlink_cutter_request_t packet;
    packet.rpm = rpm;
    packet.enabled = enabled;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUTTER_REQUEST;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_CRC);
}

/**
 * @brief Encode a cutter_request struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param cutter_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cutter_request_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_cutter_request_t* cutter_request)
{
    return mavlink_msg_cutter_request_pack(system_id, component_id, msg, cutter_request->enabled, cutter_request->rpm);
}

/**
 * @brief Encode a cutter_request struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cutter_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cutter_request_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_cutter_request_t* cutter_request)
{
    return mavlink_msg_cutter_request_pack_chan(system_id, component_id, chan, msg, cutter_request->enabled, cutter_request->rpm);
}

/**
 * @brief Encode a cutter_request struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param cutter_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cutter_request_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_cutter_request_t* cutter_request)
{
    return mavlink_msg_cutter_request_pack_status(system_id, component_id, _status, msg,  cutter_request->enabled, cutter_request->rpm);
}

/**
 * @brief Send a cutter_request message
 * @param chan MAVLink channel to send the message
 *
 * @param enabled  Enable or disable the cutter
 * @param rpm [rpm] Speed command for the cutter in RPM
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_cutter_request_send(mavlink_channel_t chan, uint8_t enabled, int32_t rpm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_REQUEST_LEN];
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_uint8_t(buf, 4, enabled);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REQUEST, buf, MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_CRC);
#else
    mavlink_cutter_request_t packet;
    packet.rpm = rpm;
    packet.enabled = enabled;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REQUEST, (const char *)&packet, MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_CRC);
#endif
}

/**
 * @brief Send a cutter_request message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_cutter_request_send_struct(mavlink_channel_t chan, const mavlink_cutter_request_t* cutter_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_cutter_request_send(chan, cutter_request->enabled, cutter_request->rpm);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REQUEST, (const char *)cutter_request, MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_CRC);
#endif
}

#if MAVLINK_MSG_ID_CUTTER_REQUEST_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_cutter_request_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t enabled, int32_t rpm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_uint8_t(buf, 4, enabled);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REQUEST, buf, MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_CRC);
#else
    mavlink_cutter_request_t *packet = (mavlink_cutter_request_t *)msgbuf;
    packet->rpm = rpm;
    packet->enabled = enabled;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REQUEST, (const char *)packet, MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN, MAVLINK_MSG_ID_CUTTER_REQUEST_CRC);
#endif
}
#endif

#endif

// MESSAGE CUTTER_REQUEST UNPACKING


/**
 * @brief Get field enabled from cutter_request message
 *
 * @return  Enable or disable the cutter
 */
static inline uint8_t mavlink_msg_cutter_request_get_enabled(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field rpm from cutter_request message
 *
 * @return [rpm] Speed command for the cutter in RPM
 */
static inline int32_t mavlink_msg_cutter_request_get_rpm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Decode a cutter_request message into a struct
 *
 * @param msg The message to decode
 * @param cutter_request C-struct to decode the message contents into
 */
static inline void mavlink_msg_cutter_request_decode(const mavlink_message_t* msg, mavlink_cutter_request_t* cutter_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    cutter_request->rpm = mavlink_msg_cutter_request_get_rpm(msg);
    cutter_request->enabled = mavlink_msg_cutter_request_get_enabled(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CUTTER_REQUEST_LEN? msg->len : MAVLINK_MSG_ID_CUTTER_REQUEST_LEN;
        memset(cutter_request, 0, MAVLINK_MSG_ID_CUTTER_REQUEST_LEN);
    memcpy(cutter_request, _MAV_PAYLOAD(msg), len);
#endif
}
