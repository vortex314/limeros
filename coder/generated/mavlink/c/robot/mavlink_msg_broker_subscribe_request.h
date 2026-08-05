#pragma once
// MESSAGE BROKER_SUBSCRIBE_REQUEST PACKING

#define MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST 40018


typedef struct __mavlink_broker_subscribe_request_t {
 uint32_t src; /*<  Source endpoint ID; 0 to subscribe to all sources*/
 uint32_t msg_type; /*<  Message type ID; 0 to subscribe to all message types*/
} mavlink_broker_subscribe_request_t;

#define MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN 8
#define MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN 8
#define MAVLINK_MSG_ID_40018_LEN 8
#define MAVLINK_MSG_ID_40018_MIN_LEN 8

#define MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_CRC 33
#define MAVLINK_MSG_ID_40018_CRC 33



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_BROKER_SUBSCRIBE_REQUEST { \
    40018, \
    "BROKER_SUBSCRIBE_REQUEST", \
    2, \
    {  { "src", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_broker_subscribe_request_t, src) }, \
         { "msg_type", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_broker_subscribe_request_t, msg_type) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_BROKER_SUBSCRIBE_REQUEST { \
    "BROKER_SUBSCRIBE_REQUEST", \
    2, \
    {  { "src", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_broker_subscribe_request_t, src) }, \
         { "msg_type", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_broker_subscribe_request_t, msg_type) }, \
         } \
}
#endif

/**
 * @brief Pack a broker_subscribe_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param src  Source endpoint ID; 0 to subscribe to all sources
 * @param msg_type  Message type ID; 0 to subscribe to all message types
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_broker_subscribe_request_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t src, uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN];
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, msg_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN);
#else
    mavlink_broker_subscribe_request_t packet;
    packet.src = src;
    packet.msg_type = msg_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_CRC);
}

/**
 * @brief Pack a broker_subscribe_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param src  Source endpoint ID; 0 to subscribe to all sources
 * @param msg_type  Message type ID; 0 to subscribe to all message types
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_broker_subscribe_request_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t src, uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN];
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, msg_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN);
#else
    mavlink_broker_subscribe_request_t packet;
    packet.src = src;
    packet.msg_type = msg_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN);
#endif
}

/**
 * @brief Pack a broker_subscribe_request message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param src  Source endpoint ID; 0 to subscribe to all sources
 * @param msg_type  Message type ID; 0 to subscribe to all message types
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_broker_subscribe_request_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t src,uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN];
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, msg_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN);
#else
    mavlink_broker_subscribe_request_t packet;
    packet.src = src;
    packet.msg_type = msg_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_CRC);
}

/**
 * @brief Encode a broker_subscribe_request struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param broker_subscribe_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_broker_subscribe_request_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_broker_subscribe_request_t* broker_subscribe_request)
{
    return mavlink_msg_broker_subscribe_request_pack(system_id, component_id, msg, broker_subscribe_request->src, broker_subscribe_request->msg_type);
}

/**
 * @brief Encode a broker_subscribe_request struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param broker_subscribe_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_broker_subscribe_request_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_broker_subscribe_request_t* broker_subscribe_request)
{
    return mavlink_msg_broker_subscribe_request_pack_chan(system_id, component_id, chan, msg, broker_subscribe_request->src, broker_subscribe_request->msg_type);
}

/**
 * @brief Encode a broker_subscribe_request struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param broker_subscribe_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_broker_subscribe_request_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_broker_subscribe_request_t* broker_subscribe_request)
{
    return mavlink_msg_broker_subscribe_request_pack_status(system_id, component_id, _status, msg,  broker_subscribe_request->src, broker_subscribe_request->msg_type);
}

/**
 * @brief Send a broker_subscribe_request message
 * @param chan MAVLink channel to send the message
 *
 * @param src  Source endpoint ID; 0 to subscribe to all sources
 * @param msg_type  Message type ID; 0 to subscribe to all message types
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_broker_subscribe_request_send(mavlink_channel_t chan, uint32_t src, uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN];
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, msg_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST, buf, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_CRC);
#else
    mavlink_broker_subscribe_request_t packet;
    packet.src = src;
    packet.msg_type = msg_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST, (const char *)&packet, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_CRC);
#endif
}

/**
 * @brief Send a broker_subscribe_request message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_broker_subscribe_request_send_struct(mavlink_channel_t chan, const mavlink_broker_subscribe_request_t* broker_subscribe_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_broker_subscribe_request_send(chan, broker_subscribe_request->src, broker_subscribe_request->msg_type);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST, (const char *)broker_subscribe_request, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_CRC);
#endif
}

#if MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_broker_subscribe_request_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t src, uint32_t msg_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, msg_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST, buf, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_CRC);
#else
    mavlink_broker_subscribe_request_t *packet = (mavlink_broker_subscribe_request_t *)msgbuf;
    packet->src = src;
    packet->msg_type = msg_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST, (const char *)packet, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_CRC);
#endif
}
#endif

#endif

// MESSAGE BROKER_SUBSCRIBE_REQUEST UNPACKING


/**
 * @brief Get field src from broker_subscribe_request message
 *
 * @return  Source endpoint ID; 0 to subscribe to all sources
 */
static inline uint32_t mavlink_msg_broker_subscribe_request_get_src(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field msg_type from broker_subscribe_request message
 *
 * @return  Message type ID; 0 to subscribe to all message types
 */
static inline uint32_t mavlink_msg_broker_subscribe_request_get_msg_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Decode a broker_subscribe_request message into a struct
 *
 * @param msg The message to decode
 * @param broker_subscribe_request C-struct to decode the message contents into
 */
static inline void mavlink_msg_broker_subscribe_request_decode(const mavlink_message_t* msg, mavlink_broker_subscribe_request_t* broker_subscribe_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    broker_subscribe_request->src = mavlink_msg_broker_subscribe_request_get_src(msg);
    broker_subscribe_request->msg_type = mavlink_msg_broker_subscribe_request_get_msg_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN? msg->len : MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN;
        memset(broker_subscribe_request, 0, MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_LEN);
    memcpy(broker_subscribe_request, _MAV_PAYLOAD(msg), len);
#endif
}
