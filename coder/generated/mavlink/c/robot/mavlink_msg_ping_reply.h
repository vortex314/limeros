#pragma once
// MESSAGE PING_REPLY PACKING

#define MAVLINK_MSG_ID_PING_REPLY 40017


typedef struct __mavlink_ping_reply_t {
 uint64_t timestamp; /*< [ms] Timestamp in milliseconds since epoch*/
 uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply*/
} mavlink_ping_reply_t;

#define MAVLINK_MSG_ID_PING_REPLY_LEN 12
#define MAVLINK_MSG_ID_PING_REPLY_MIN_LEN 12
#define MAVLINK_MSG_ID_40017_LEN 12
#define MAVLINK_MSG_ID_40017_MIN_LEN 12

#define MAVLINK_MSG_ID_PING_REPLY_CRC 47
#define MAVLINK_MSG_ID_40017_CRC 47



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_PING_REPLY { \
    40017, \
    "PING_REPLY", \
    2, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_ping_reply_t, req_id) }, \
         { "timestamp", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_ping_reply_t, timestamp) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_PING_REPLY { \
    "PING_REPLY", \
    2, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_ping_reply_t, req_id) }, \
         { "timestamp", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_ping_reply_t, timestamp) }, \
         } \
}
#endif

/**
 * @brief Pack a ping_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param timestamp [ms] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ping_reply_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t req_id, uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PING_REPLY_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_uint32_t(buf, 8, req_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PING_REPLY_LEN);
#else
    mavlink_ping_reply_t packet;
    packet.timestamp = timestamp;
    packet.req_id = req_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PING_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PING_REPLY;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_PING_REPLY_MIN_LEN, MAVLINK_MSG_ID_PING_REPLY_LEN, MAVLINK_MSG_ID_PING_REPLY_CRC);
}

/**
 * @brief Pack a ping_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param timestamp [ms] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ping_reply_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t req_id, uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PING_REPLY_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_uint32_t(buf, 8, req_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PING_REPLY_LEN);
#else
    mavlink_ping_reply_t packet;
    packet.timestamp = timestamp;
    packet.req_id = req_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PING_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PING_REPLY;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_PING_REPLY_MIN_LEN, MAVLINK_MSG_ID_PING_REPLY_LEN, MAVLINK_MSG_ID_PING_REPLY_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_PING_REPLY_MIN_LEN, MAVLINK_MSG_ID_PING_REPLY_LEN);
#endif
}

/**
 * @brief Pack a ping_reply message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param timestamp [ms] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ping_reply_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t req_id,uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PING_REPLY_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_uint32_t(buf, 8, req_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PING_REPLY_LEN);
#else
    mavlink_ping_reply_t packet;
    packet.timestamp = timestamp;
    packet.req_id = req_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PING_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PING_REPLY;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_PING_REPLY_MIN_LEN, MAVLINK_MSG_ID_PING_REPLY_LEN, MAVLINK_MSG_ID_PING_REPLY_CRC);
}

/**
 * @brief Encode a ping_reply struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ping_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ping_reply_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ping_reply_t* ping_reply)
{
    return mavlink_msg_ping_reply_pack(system_id, component_id, msg, ping_reply->req_id, ping_reply->timestamp);
}

/**
 * @brief Encode a ping_reply struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ping_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ping_reply_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ping_reply_t* ping_reply)
{
    return mavlink_msg_ping_reply_pack_chan(system_id, component_id, chan, msg, ping_reply->req_id, ping_reply->timestamp);
}

/**
 * @brief Encode a ping_reply struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ping_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ping_reply_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ping_reply_t* ping_reply)
{
    return mavlink_msg_ping_reply_pack_status(system_id, component_id, _status, msg,  ping_reply->req_id, ping_reply->timestamp);
}

/**
 * @brief Send a ping_reply message
 * @param chan MAVLink channel to send the message
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param timestamp [ms] Timestamp in milliseconds since epoch
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ping_reply_send(mavlink_channel_t chan, uint32_t req_id, uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PING_REPLY_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_uint32_t(buf, 8, req_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PING_REPLY, buf, MAVLINK_MSG_ID_PING_REPLY_MIN_LEN, MAVLINK_MSG_ID_PING_REPLY_LEN, MAVLINK_MSG_ID_PING_REPLY_CRC);
#else
    mavlink_ping_reply_t packet;
    packet.timestamp = timestamp;
    packet.req_id = req_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PING_REPLY, (const char *)&packet, MAVLINK_MSG_ID_PING_REPLY_MIN_LEN, MAVLINK_MSG_ID_PING_REPLY_LEN, MAVLINK_MSG_ID_PING_REPLY_CRC);
#endif
}

/**
 * @brief Send a ping_reply message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ping_reply_send_struct(mavlink_channel_t chan, const mavlink_ping_reply_t* ping_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ping_reply_send(chan, ping_reply->req_id, ping_reply->timestamp);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PING_REPLY, (const char *)ping_reply, MAVLINK_MSG_ID_PING_REPLY_MIN_LEN, MAVLINK_MSG_ID_PING_REPLY_LEN, MAVLINK_MSG_ID_PING_REPLY_CRC);
#endif
}

#if MAVLINK_MSG_ID_PING_REPLY_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ping_reply_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t req_id, uint64_t timestamp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_uint32_t(buf, 8, req_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PING_REPLY, buf, MAVLINK_MSG_ID_PING_REPLY_MIN_LEN, MAVLINK_MSG_ID_PING_REPLY_LEN, MAVLINK_MSG_ID_PING_REPLY_CRC);
#else
    mavlink_ping_reply_t *packet = (mavlink_ping_reply_t *)msgbuf;
    packet->timestamp = timestamp;
    packet->req_id = req_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PING_REPLY, (const char *)packet, MAVLINK_MSG_ID_PING_REPLY_MIN_LEN, MAVLINK_MSG_ID_PING_REPLY_LEN, MAVLINK_MSG_ID_PING_REPLY_CRC);
#endif
}
#endif

#endif

// MESSAGE PING_REPLY UNPACKING


/**
 * @brief Get field req_id from ping_reply message
 *
 * @return  For request/reply matching, 0 if not a request/reply
 */
static inline uint32_t mavlink_msg_ping_reply_get_req_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field timestamp from ping_reply message
 *
 * @return [ms] Timestamp in milliseconds since epoch
 */
static inline uint64_t mavlink_msg_ping_reply_get_timestamp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Decode a ping_reply message into a struct
 *
 * @param msg The message to decode
 * @param ping_reply C-struct to decode the message contents into
 */
static inline void mavlink_msg_ping_reply_decode(const mavlink_message_t* msg, mavlink_ping_reply_t* ping_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ping_reply->timestamp = mavlink_msg_ping_reply_get_timestamp(msg);
    ping_reply->req_id = mavlink_msg_ping_reply_get_req_id(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_PING_REPLY_LEN? msg->len : MAVLINK_MSG_ID_PING_REPLY_LEN;
        memset(ping_reply, 0, MAVLINK_MSG_ID_PING_REPLY_LEN);
    memcpy(ping_reply, _MAV_PAYLOAD(msg), len);
#endif
}
