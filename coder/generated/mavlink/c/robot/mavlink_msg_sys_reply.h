#pragma once
// MESSAGE SYS_REPLY PACKING

#define MAVLINK_MSG_ID_SYS_REPLY 40014


typedef struct __mavlink_sys_reply_t {
 uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply*/
 int32_t rc; /*<  Return code*/
 char message[100]; /*<  Result message*/
} mavlink_sys_reply_t;

#define MAVLINK_MSG_ID_SYS_REPLY_LEN 108
#define MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN 108
#define MAVLINK_MSG_ID_40014_LEN 108
#define MAVLINK_MSG_ID_40014_MIN_LEN 108

#define MAVLINK_MSG_ID_SYS_REPLY_CRC 129
#define MAVLINK_MSG_ID_40014_CRC 129

#define MAVLINK_MSG_SYS_REPLY_FIELD_MESSAGE_LEN 100

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SYS_REPLY { \
    40014, \
    "SYS_REPLY", \
    3, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_sys_reply_t, req_id) }, \
         { "rc", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_sys_reply_t, rc) }, \
         { "message", NULL, MAVLINK_TYPE_CHAR, 100, 8, offsetof(mavlink_sys_reply_t, message) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SYS_REPLY { \
    "SYS_REPLY", \
    3, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_sys_reply_t, req_id) }, \
         { "rc", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_sys_reply_t, rc) }, \
         { "message", NULL, MAVLINK_TYPE_CHAR, 100, 8, offsetof(mavlink_sys_reply_t, message) }, \
         } \
}
#endif

/**
 * @brief Pack a sys_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param rc  Return code
 * @param message  Result message
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sys_reply_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t req_id, int32_t rc, const char *message)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_REPLY_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rc);
    _mav_put_char_array(buf, 8, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SYS_REPLY_LEN);
#else
    mavlink_sys_reply_t packet;
    packet.req_id = req_id;
    packet.rc = rc;
    mav_array_assign_char(packet.message, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SYS_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SYS_REPLY;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN, MAVLINK_MSG_ID_SYS_REPLY_LEN, MAVLINK_MSG_ID_SYS_REPLY_CRC);
}

/**
 * @brief Pack a sys_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param rc  Return code
 * @param message  Result message
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sys_reply_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t req_id, int32_t rc, const char *message)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_REPLY_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rc);
    _mav_put_char_array(buf, 8, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SYS_REPLY_LEN);
#else
    mavlink_sys_reply_t packet;
    packet.req_id = req_id;
    packet.rc = rc;
    mav_array_memcpy(packet.message, message, sizeof(char)*100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SYS_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SYS_REPLY;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN, MAVLINK_MSG_ID_SYS_REPLY_LEN, MAVLINK_MSG_ID_SYS_REPLY_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN, MAVLINK_MSG_ID_SYS_REPLY_LEN);
#endif
}

/**
 * @brief Pack a sys_reply message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param rc  Return code
 * @param message  Result message
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sys_reply_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t req_id,int32_t rc,const char *message)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_REPLY_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rc);
    _mav_put_char_array(buf, 8, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SYS_REPLY_LEN);
#else
    mavlink_sys_reply_t packet;
    packet.req_id = req_id;
    packet.rc = rc;
    mav_array_assign_char(packet.message, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SYS_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SYS_REPLY;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN, MAVLINK_MSG_ID_SYS_REPLY_LEN, MAVLINK_MSG_ID_SYS_REPLY_CRC);
}

/**
 * @brief Encode a sys_reply struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param sys_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sys_reply_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_sys_reply_t* sys_reply)
{
    return mavlink_msg_sys_reply_pack(system_id, component_id, msg, sys_reply->req_id, sys_reply->rc, sys_reply->message);
}

/**
 * @brief Encode a sys_reply struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param sys_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sys_reply_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_sys_reply_t* sys_reply)
{
    return mavlink_msg_sys_reply_pack_chan(system_id, component_id, chan, msg, sys_reply->req_id, sys_reply->rc, sys_reply->message);
}

/**
 * @brief Encode a sys_reply struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param sys_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sys_reply_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_sys_reply_t* sys_reply)
{
    return mavlink_msg_sys_reply_pack_status(system_id, component_id, _status, msg,  sys_reply->req_id, sys_reply->rc, sys_reply->message);
}

/**
 * @brief Send a sys_reply message
 * @param chan MAVLink channel to send the message
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param rc  Return code
 * @param message  Result message
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_sys_reply_send(mavlink_channel_t chan, uint32_t req_id, int32_t rc, const char *message)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_REPLY_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rc);
    _mav_put_char_array(buf, 8, message, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REPLY, buf, MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN, MAVLINK_MSG_ID_SYS_REPLY_LEN, MAVLINK_MSG_ID_SYS_REPLY_CRC);
#else
    mavlink_sys_reply_t packet;
    packet.req_id = req_id;
    packet.rc = rc;
    mav_array_assign_char(packet.message, message, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REPLY, (const char *)&packet, MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN, MAVLINK_MSG_ID_SYS_REPLY_LEN, MAVLINK_MSG_ID_SYS_REPLY_CRC);
#endif
}

/**
 * @brief Send a sys_reply message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_sys_reply_send_struct(mavlink_channel_t chan, const mavlink_sys_reply_t* sys_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_sys_reply_send(chan, sys_reply->req_id, sys_reply->rc, sys_reply->message);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REPLY, (const char *)sys_reply, MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN, MAVLINK_MSG_ID_SYS_REPLY_LEN, MAVLINK_MSG_ID_SYS_REPLY_CRC);
#endif
}

#if MAVLINK_MSG_ID_SYS_REPLY_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_sys_reply_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t req_id, int32_t rc, const char *message)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rc);
    _mav_put_char_array(buf, 8, message, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REPLY, buf, MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN, MAVLINK_MSG_ID_SYS_REPLY_LEN, MAVLINK_MSG_ID_SYS_REPLY_CRC);
#else
    mavlink_sys_reply_t *packet = (mavlink_sys_reply_t *)msgbuf;
    packet->req_id = req_id;
    packet->rc = rc;
    mav_array_assign_char(packet->message, message, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REPLY, (const char *)packet, MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN, MAVLINK_MSG_ID_SYS_REPLY_LEN, MAVLINK_MSG_ID_SYS_REPLY_CRC);
#endif
}
#endif

#endif

// MESSAGE SYS_REPLY UNPACKING


/**
 * @brief Get field req_id from sys_reply message
 *
 * @return  For request/reply matching, 0 if not a request/reply
 */
static inline uint32_t mavlink_msg_sys_reply_get_req_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field rc from sys_reply message
 *
 * @return  Return code
 */
static inline int32_t mavlink_msg_sys_reply_get_rc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field message from sys_reply message
 *
 * @return  Result message
 */
static inline uint16_t mavlink_msg_sys_reply_get_message(const mavlink_message_t* msg, char *message)
{
    return _MAV_RETURN_char_array(msg, message, 100,  8);
}

/**
 * @brief Decode a sys_reply message into a struct
 *
 * @param msg The message to decode
 * @param sys_reply C-struct to decode the message contents into
 */
static inline void mavlink_msg_sys_reply_decode(const mavlink_message_t* msg, mavlink_sys_reply_t* sys_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    sys_reply->req_id = mavlink_msg_sys_reply_get_req_id(msg);
    sys_reply->rc = mavlink_msg_sys_reply_get_rc(msg);
    mavlink_msg_sys_reply_get_message(msg, sys_reply->message);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SYS_REPLY_LEN? msg->len : MAVLINK_MSG_ID_SYS_REPLY_LEN;
        memset(sys_reply, 0, MAVLINK_MSG_ID_SYS_REPLY_LEN);
    memcpy(sys_reply, _MAV_PAYLOAD(msg), len);
#endif
}
