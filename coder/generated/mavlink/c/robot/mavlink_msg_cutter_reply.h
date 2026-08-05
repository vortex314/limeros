#pragma once
// MESSAGE CUTTER_REPLY PACKING

#define MAVLINK_MSG_ID_CUTTER_REPLY 40006


typedef struct __mavlink_cutter_reply_t {
 uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply*/
 uint32_t error_code; /*<  Error code, 0 if no error*/
 uint32_t msg_type; /*<  Message type identifier, the original request*/
 char message[100]; /*<  Error message or additional information*/
} mavlink_cutter_reply_t;

#define MAVLINK_MSG_ID_CUTTER_REPLY_LEN 112
#define MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN 112
#define MAVLINK_MSG_ID_40006_LEN 112
#define MAVLINK_MSG_ID_40006_MIN_LEN 112

#define MAVLINK_MSG_ID_CUTTER_REPLY_CRC 138
#define MAVLINK_MSG_ID_40006_CRC 138

#define MAVLINK_MSG_CUTTER_REPLY_FIELD_MESSAGE_LEN 100

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CUTTER_REPLY { \
    40006, \
    "CUTTER_REPLY", \
    4, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_cutter_reply_t, req_id) }, \
         { "error_code", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_cutter_reply_t, error_code) }, \
         { "message", NULL, MAVLINK_TYPE_CHAR, 100, 12, offsetof(mavlink_cutter_reply_t, message) }, \
         { "msg_type", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_cutter_reply_t, msg_type) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CUTTER_REPLY { \
    "CUTTER_REPLY", \
    4, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_cutter_reply_t, req_id) }, \
         { "error_code", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_cutter_reply_t, error_code) }, \
         { "message", NULL, MAVLINK_TYPE_CHAR, 100, 12, offsetof(mavlink_cutter_reply_t, message) }, \
         { "msg_type", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_cutter_reply_t, msg_type) }, \
         } \
}
#endif

/**
 * @brief Pack a cutter_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param error_code  Error code, 0 if no error
 * @param message  Error message or additional information
 * @param msg_type  Message type identifier, the original request
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cutter_reply_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t req_id, uint32_t error_code, const char *message, uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_REPLY_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_uint32_t(buf, 4, error_code);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_char_array(buf, 12, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUTTER_REPLY_LEN);
#else
    mavlink_cutter_reply_t packet;
    packet.req_id = req_id;
    packet.error_code = error_code;
    packet.msg_type = msg_type;
    mav_array_assign_char(packet.message, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUTTER_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUTTER_REPLY;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_CRC);
}

/**
 * @brief Pack a cutter_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param error_code  Error code, 0 if no error
 * @param message  Error message or additional information
 * @param msg_type  Message type identifier, the original request
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cutter_reply_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t req_id, uint32_t error_code, const char *message, uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_REPLY_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_uint32_t(buf, 4, error_code);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_char_array(buf, 12, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUTTER_REPLY_LEN);
#else
    mavlink_cutter_reply_t packet;
    packet.req_id = req_id;
    packet.error_code = error_code;
    packet.msg_type = msg_type;
    mav_array_memcpy(packet.message, message, sizeof(char)*100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUTTER_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUTTER_REPLY;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_LEN);
#endif
}

/**
 * @brief Pack a cutter_reply message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param error_code  Error code, 0 if no error
 * @param message  Error message or additional information
 * @param msg_type  Message type identifier, the original request
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cutter_reply_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t req_id,uint32_t error_code,const char *message,uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_REPLY_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_uint32_t(buf, 4, error_code);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_char_array(buf, 12, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUTTER_REPLY_LEN);
#else
    mavlink_cutter_reply_t packet;
    packet.req_id = req_id;
    packet.error_code = error_code;
    packet.msg_type = msg_type;
    mav_array_assign_char(packet.message, message, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUTTER_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUTTER_REPLY;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_CRC);
}

/**
 * @brief Encode a cutter_reply struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param cutter_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cutter_reply_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_cutter_reply_t* cutter_reply)
{
    return mavlink_msg_cutter_reply_pack(system_id, component_id, msg, cutter_reply->req_id, cutter_reply->error_code, cutter_reply->message, cutter_reply->msg_type);
}

/**
 * @brief Encode a cutter_reply struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cutter_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cutter_reply_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_cutter_reply_t* cutter_reply)
{
    return mavlink_msg_cutter_reply_pack_chan(system_id, component_id, chan, msg, cutter_reply->req_id, cutter_reply->error_code, cutter_reply->message, cutter_reply->msg_type);
}

/**
 * @brief Encode a cutter_reply struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param cutter_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cutter_reply_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_cutter_reply_t* cutter_reply)
{
    return mavlink_msg_cutter_reply_pack_status(system_id, component_id, _status, msg,  cutter_reply->req_id, cutter_reply->error_code, cutter_reply->message, cutter_reply->msg_type);
}

/**
 * @brief Send a cutter_reply message
 * @param chan MAVLink channel to send the message
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param error_code  Error code, 0 if no error
 * @param message  Error message or additional information
 * @param msg_type  Message type identifier, the original request
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_cutter_reply_send(mavlink_channel_t chan, uint32_t req_id, uint32_t error_code, const char *message, uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_REPLY_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_uint32_t(buf, 4, error_code);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_char_array(buf, 12, message, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REPLY, buf, MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_CRC);
#else
    mavlink_cutter_reply_t packet;
    packet.req_id = req_id;
    packet.error_code = error_code;
    packet.msg_type = msg_type;
    mav_array_assign_char(packet.message, message, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REPLY, (const char *)&packet, MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_CRC);
#endif
}

/**
 * @brief Send a cutter_reply message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_cutter_reply_send_struct(mavlink_channel_t chan, const mavlink_cutter_reply_t* cutter_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_cutter_reply_send(chan, cutter_reply->req_id, cutter_reply->error_code, cutter_reply->message, cutter_reply->msg_type);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REPLY, (const char *)cutter_reply, MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_CRC);
#endif
}

#if MAVLINK_MSG_ID_CUTTER_REPLY_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_cutter_reply_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t req_id, uint32_t error_code, const char *message, uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_uint32_t(buf, 4, error_code);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_char_array(buf, 12, message, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REPLY, buf, MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_CRC);
#else
    mavlink_cutter_reply_t *packet = (mavlink_cutter_reply_t *)msgbuf;
    packet->req_id = req_id;
    packet->error_code = error_code;
    packet->msg_type = msg_type;
    mav_array_assign_char(packet->message, message, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_REPLY, (const char *)packet, MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_LEN, MAVLINK_MSG_ID_CUTTER_REPLY_CRC);
#endif
}
#endif

#endif

// MESSAGE CUTTER_REPLY UNPACKING


/**
 * @brief Get field req_id from cutter_reply message
 *
 * @return  For request/reply matching, 0 if not a request/reply
 */
static inline uint32_t mavlink_msg_cutter_reply_get_req_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field error_code from cutter_reply message
 *
 * @return  Error code, 0 if no error
 */
static inline uint32_t mavlink_msg_cutter_reply_get_error_code(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field message from cutter_reply message
 *
 * @return  Error message or additional information
 */
static inline uint16_t mavlink_msg_cutter_reply_get_message(const mavlink_message_t* msg, char *message)
{
    return _MAV_RETURN_char_array(msg, message, 100,  12);
}

/**
 * @brief Get field msg_type from cutter_reply message
 *
 * @return  Message type identifier, the original request
 */
static inline uint32_t mavlink_msg_cutter_reply_get_msg_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Decode a cutter_reply message into a struct
 *
 * @param msg The message to decode
 * @param cutter_reply C-struct to decode the message contents into
 */
static inline void mavlink_msg_cutter_reply_decode(const mavlink_message_t* msg, mavlink_cutter_reply_t* cutter_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    cutter_reply->req_id = mavlink_msg_cutter_reply_get_req_id(msg);
    cutter_reply->error_code = mavlink_msg_cutter_reply_get_error_code(msg);
    cutter_reply->msg_type = mavlink_msg_cutter_reply_get_msg_type(msg);
    mavlink_msg_cutter_reply_get_message(msg, cutter_reply->message);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CUTTER_REPLY_LEN? msg->len : MAVLINK_MSG_ID_CUTTER_REPLY_LEN;
        memset(cutter_reply, 0, MAVLINK_MSG_ID_CUTTER_REPLY_LEN);
    memcpy(cutter_reply, _MAV_PAYLOAD(msg), len);
#endif
}
