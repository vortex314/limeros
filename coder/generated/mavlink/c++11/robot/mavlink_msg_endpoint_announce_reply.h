#pragma once
// MESSAGE ENDPOINT_ANNOUNCE_REPLY PACKING

#define MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY 40026


typedef struct __mavlink_endpoint_announce_reply_t {
 uint64_t utc; /*< [us] Timestamp in milliseconds since epoch*/
} mavlink_endpoint_announce_reply_t;

#define MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN 8
#define MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN 8
#define MAVLINK_MSG_ID_40026_LEN 8
#define MAVLINK_MSG_ID_40026_MIN_LEN 8

#define MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_CRC 236
#define MAVLINK_MSG_ID_40026_CRC 236



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ENDPOINT_ANNOUNCE_REPLY { \
    40026, \
    "ENDPOINT_ANNOUNCE_REPLY", \
    1, \
    {  { "utc", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_endpoint_announce_reply_t, utc) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ENDPOINT_ANNOUNCE_REPLY { \
    "ENDPOINT_ANNOUNCE_REPLY", \
    1, \
    {  { "utc", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_endpoint_announce_reply_t, utc) }, \
         } \
}
#endif

/**
 * @brief Pack a endpoint_announce_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param utc [us] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_endpoint_announce_reply_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint64_t utc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN];
    _mav_put_uint64_t(buf, 0, utc);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN);
#else
    mavlink_endpoint_announce_reply_t packet;
    packet.utc = utc;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_CRC);
}

/**
 * @brief Pack a endpoint_announce_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param utc [us] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_endpoint_announce_reply_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint64_t utc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN];
    _mav_put_uint64_t(buf, 0, utc);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN);
#else
    mavlink_endpoint_announce_reply_t packet;
    packet.utc = utc;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN);
#endif
}

/**
 * @brief Pack a endpoint_announce_reply message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param utc [us] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_endpoint_announce_reply_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint64_t utc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN];
    _mav_put_uint64_t(buf, 0, utc);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN);
#else
    mavlink_endpoint_announce_reply_t packet;
    packet.utc = utc;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_CRC);
}

/**
 * @brief Encode a endpoint_announce_reply struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param endpoint_announce_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_endpoint_announce_reply_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_endpoint_announce_reply_t* endpoint_announce_reply)
{
    return mavlink_msg_endpoint_announce_reply_pack(system_id, component_id, msg, endpoint_announce_reply->utc);
}

/**
 * @brief Encode a endpoint_announce_reply struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param endpoint_announce_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_endpoint_announce_reply_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_endpoint_announce_reply_t* endpoint_announce_reply)
{
    return mavlink_msg_endpoint_announce_reply_pack_chan(system_id, component_id, chan, msg, endpoint_announce_reply->utc);
}

/**
 * @brief Encode a endpoint_announce_reply struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param endpoint_announce_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_endpoint_announce_reply_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_endpoint_announce_reply_t* endpoint_announce_reply)
{
    return mavlink_msg_endpoint_announce_reply_pack_status(system_id, component_id, _status, msg,  endpoint_announce_reply->utc);
}

/**
 * @brief Send a endpoint_announce_reply message
 * @param chan MAVLink channel to send the message
 *
 * @param utc [us] Timestamp in milliseconds since epoch
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_endpoint_announce_reply_send(mavlink_channel_t chan, uint64_t utc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN];
    _mav_put_uint64_t(buf, 0, utc);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY, buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_CRC);
#else
    mavlink_endpoint_announce_reply_t packet;
    packet.utc = utc;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY, (const char *)&packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_CRC);
#endif
}

/**
 * @brief Send a endpoint_announce_reply message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_endpoint_announce_reply_send_struct(mavlink_channel_t chan, const mavlink_endpoint_announce_reply_t* endpoint_announce_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_endpoint_announce_reply_send(chan, endpoint_announce_reply->utc);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY, (const char *)endpoint_announce_reply, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_CRC);
#endif
}

#if MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_endpoint_announce_reply_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint64_t utc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, utc);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY, buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_CRC);
#else
    mavlink_endpoint_announce_reply_t *packet = (mavlink_endpoint_announce_reply_t *)msgbuf;
    packet->utc = utc;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY, (const char *)packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_CRC);
#endif
}
#endif

#endif

// MESSAGE ENDPOINT_ANNOUNCE_REPLY UNPACKING


/**
 * @brief Get field utc from endpoint_announce_reply message
 *
 * @return [us] Timestamp in milliseconds since epoch
 */
static inline uint64_t mavlink_msg_endpoint_announce_reply_get_utc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Decode a endpoint_announce_reply message into a struct
 *
 * @param msg The message to decode
 * @param endpoint_announce_reply C-struct to decode the message contents into
 */
static inline void mavlink_msg_endpoint_announce_reply_decode(const mavlink_message_t* msg, mavlink_endpoint_announce_reply_t* endpoint_announce_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    endpoint_announce_reply->utc = mavlink_msg_endpoint_announce_reply_get_utc(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN? msg->len : MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN;
        memset(endpoint_announce_reply, 0, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_LEN);
    memcpy(endpoint_announce_reply, _MAV_PAYLOAD(msg), len);
#endif
}
