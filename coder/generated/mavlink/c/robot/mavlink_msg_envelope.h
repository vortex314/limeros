#pragma once
// MESSAGE ENVELOPE PACKING

#define MAVLINK_MSG_ID_ENVELOPE 40024


typedef struct __mavlink_envelope_t {
 uint32_t src; /*<  Source endpoint name*/
 uint32_t dst; /*<  Destination endpoint name*/
 uint32_t msg_type; /*<  Message type name*/
 uint32_t request_id; /*<  Request ID for matching request/reply*/
 uint32_t instance_id; /*<  Instance ID for matching request/reply*/
 uint8_t payload[255]; /*<  Serialized payload of the message*/
} mavlink_envelope_t;

#define MAVLINK_MSG_ID_ENVELOPE_LEN 275
#define MAVLINK_MSG_ID_ENVELOPE_MIN_LEN 275
#define MAVLINK_MSG_ID_40024_LEN 275
#define MAVLINK_MSG_ID_40024_MIN_LEN 275

#define MAVLINK_MSG_ID_ENVELOPE_CRC 76
#define MAVLINK_MSG_ID_40024_CRC 76

#define MAVLINK_MSG_ENVELOPE_FIELD_PAYLOAD_LEN 255

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ENVELOPE { \
    40024, \
    "ENVELOPE", \
    6, \
    {  { "src", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_envelope_t, src) }, \
         { "dst", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_envelope_t, dst) }, \
         { "msg_type", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_envelope_t, msg_type) }, \
         { "request_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_envelope_t, request_id) }, \
         { "instance_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 16, offsetof(mavlink_envelope_t, instance_id) }, \
         { "payload", NULL, MAVLINK_TYPE_UINT8_T, 255, 20, offsetof(mavlink_envelope_t, payload) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ENVELOPE { \
    "ENVELOPE", \
    6, \
    {  { "src", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_envelope_t, src) }, \
         { "dst", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_envelope_t, dst) }, \
         { "msg_type", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_envelope_t, msg_type) }, \
         { "request_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_envelope_t, request_id) }, \
         { "instance_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 16, offsetof(mavlink_envelope_t, instance_id) }, \
         { "payload", NULL, MAVLINK_TYPE_UINT8_T, 255, 20, offsetof(mavlink_envelope_t, payload) }, \
         } \
}
#endif

/**
 * @brief Pack a envelope message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param src  Source endpoint name
 * @param dst  Destination endpoint name
 * @param msg_type  Message type name
 * @param request_id  Request ID for matching request/reply
 * @param instance_id  Instance ID for matching request/reply
 * @param payload  Serialized payload of the message
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_envelope_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t src, uint32_t dst, uint32_t msg_type, uint32_t request_id, uint32_t instance_id, const uint8_t *payload)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENVELOPE_LEN];
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, dst);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_uint32_t(buf, 12, request_id);
    _mav_put_uint32_t(buf, 16, instance_id);
    _mav_put_uint8_t_array(buf, 20, payload, 255);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ENVELOPE_LEN);
#else
    mavlink_envelope_t packet;
    packet.src = src;
    packet.dst = dst;
    packet.msg_type = msg_type;
    packet.request_id = request_id;
    packet.instance_id = instance_id;
    mav_array_assign_uint8_t(packet.payload, payload, 255);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ENVELOPE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ENVELOPE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ENVELOPE_MIN_LEN, MAVLINK_MSG_ID_ENVELOPE_LEN, MAVLINK_MSG_ID_ENVELOPE_CRC);
}

/**
 * @brief Pack a envelope message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param src  Source endpoint name
 * @param dst  Destination endpoint name
 * @param msg_type  Message type name
 * @param request_id  Request ID for matching request/reply
 * @param instance_id  Instance ID for matching request/reply
 * @param payload  Serialized payload of the message
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_envelope_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t src, uint32_t dst, uint32_t msg_type, uint32_t request_id, uint32_t instance_id, const uint8_t *payload)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENVELOPE_LEN];
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, dst);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_uint32_t(buf, 12, request_id);
    _mav_put_uint32_t(buf, 16, instance_id);
    _mav_put_uint8_t_array(buf, 20, payload, 255);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ENVELOPE_LEN);
#else
    mavlink_envelope_t packet;
    packet.src = src;
    packet.dst = dst;
    packet.msg_type = msg_type;
    packet.request_id = request_id;
    packet.instance_id = instance_id;
    mav_array_memcpy(packet.payload, payload, sizeof(uint8_t)*255);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ENVELOPE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ENVELOPE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ENVELOPE_MIN_LEN, MAVLINK_MSG_ID_ENVELOPE_LEN, MAVLINK_MSG_ID_ENVELOPE_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ENVELOPE_MIN_LEN, MAVLINK_MSG_ID_ENVELOPE_LEN);
#endif
}

/**
 * @brief Pack a envelope message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param src  Source endpoint name
 * @param dst  Destination endpoint name
 * @param msg_type  Message type name
 * @param request_id  Request ID for matching request/reply
 * @param instance_id  Instance ID for matching request/reply
 * @param payload  Serialized payload of the message
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_envelope_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t src,uint32_t dst,uint32_t msg_type,uint32_t request_id,uint32_t instance_id,const uint8_t *payload)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENVELOPE_LEN];
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, dst);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_uint32_t(buf, 12, request_id);
    _mav_put_uint32_t(buf, 16, instance_id);
    _mav_put_uint8_t_array(buf, 20, payload, 255);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ENVELOPE_LEN);
#else
    mavlink_envelope_t packet;
    packet.src = src;
    packet.dst = dst;
    packet.msg_type = msg_type;
    packet.request_id = request_id;
    packet.instance_id = instance_id;
    mav_array_assign_uint8_t(packet.payload, payload, 255);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ENVELOPE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ENVELOPE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ENVELOPE_MIN_LEN, MAVLINK_MSG_ID_ENVELOPE_LEN, MAVLINK_MSG_ID_ENVELOPE_CRC);
}

/**
 * @brief Encode a envelope struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param envelope C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_envelope_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_envelope_t* envelope)
{
    return mavlink_msg_envelope_pack(system_id, component_id, msg, envelope->src, envelope->dst, envelope->msg_type, envelope->request_id, envelope->instance_id, envelope->payload);
}

/**
 * @brief Encode a envelope struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param envelope C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_envelope_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_envelope_t* envelope)
{
    return mavlink_msg_envelope_pack_chan(system_id, component_id, chan, msg, envelope->src, envelope->dst, envelope->msg_type, envelope->request_id, envelope->instance_id, envelope->payload);
}

/**
 * @brief Encode a envelope struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param envelope C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_envelope_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_envelope_t* envelope)
{
    return mavlink_msg_envelope_pack_status(system_id, component_id, _status, msg,  envelope->src, envelope->dst, envelope->msg_type, envelope->request_id, envelope->instance_id, envelope->payload);
}

/**
 * @brief Send a envelope message
 * @param chan MAVLink channel to send the message
 *
 * @param src  Source endpoint name
 * @param dst  Destination endpoint name
 * @param msg_type  Message type name
 * @param request_id  Request ID for matching request/reply
 * @param instance_id  Instance ID for matching request/reply
 * @param payload  Serialized payload of the message
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_envelope_send(mavlink_channel_t chan, uint32_t src, uint32_t dst, uint32_t msg_type, uint32_t request_id, uint32_t instance_id, const uint8_t *payload)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENVELOPE_LEN];
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, dst);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_uint32_t(buf, 12, request_id);
    _mav_put_uint32_t(buf, 16, instance_id);
    _mav_put_uint8_t_array(buf, 20, payload, 255);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENVELOPE, buf, MAVLINK_MSG_ID_ENVELOPE_MIN_LEN, MAVLINK_MSG_ID_ENVELOPE_LEN, MAVLINK_MSG_ID_ENVELOPE_CRC);
#else
    mavlink_envelope_t packet;
    packet.src = src;
    packet.dst = dst;
    packet.msg_type = msg_type;
    packet.request_id = request_id;
    packet.instance_id = instance_id;
    mav_array_assign_uint8_t(packet.payload, payload, 255);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENVELOPE, (const char *)&packet, MAVLINK_MSG_ID_ENVELOPE_MIN_LEN, MAVLINK_MSG_ID_ENVELOPE_LEN, MAVLINK_MSG_ID_ENVELOPE_CRC);
#endif
}

/**
 * @brief Send a envelope message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_envelope_send_struct(mavlink_channel_t chan, const mavlink_envelope_t* envelope)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_envelope_send(chan, envelope->src, envelope->dst, envelope->msg_type, envelope->request_id, envelope->instance_id, envelope->payload);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENVELOPE, (const char *)envelope, MAVLINK_MSG_ID_ENVELOPE_MIN_LEN, MAVLINK_MSG_ID_ENVELOPE_LEN, MAVLINK_MSG_ID_ENVELOPE_CRC);
#endif
}

#if MAVLINK_MSG_ID_ENVELOPE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_envelope_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t src, uint32_t dst, uint32_t msg_type, uint32_t request_id, uint32_t instance_id, const uint8_t *payload)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, src);
    _mav_put_uint32_t(buf, 4, dst);
    _mav_put_uint32_t(buf, 8, msg_type);
    _mav_put_uint32_t(buf, 12, request_id);
    _mav_put_uint32_t(buf, 16, instance_id);
    _mav_put_uint8_t_array(buf, 20, payload, 255);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENVELOPE, buf, MAVLINK_MSG_ID_ENVELOPE_MIN_LEN, MAVLINK_MSG_ID_ENVELOPE_LEN, MAVLINK_MSG_ID_ENVELOPE_CRC);
#else
    mavlink_envelope_t *packet = (mavlink_envelope_t *)msgbuf;
    packet->src = src;
    packet->dst = dst;
    packet->msg_type = msg_type;
    packet->request_id = request_id;
    packet->instance_id = instance_id;
    mav_array_assign_uint8_t(packet->payload, payload, 255);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENVELOPE, (const char *)packet, MAVLINK_MSG_ID_ENVELOPE_MIN_LEN, MAVLINK_MSG_ID_ENVELOPE_LEN, MAVLINK_MSG_ID_ENVELOPE_CRC);
#endif
}
#endif

#endif

// MESSAGE ENVELOPE UNPACKING


/**
 * @brief Get field src from envelope message
 *
 * @return  Source endpoint name
 */
static inline uint32_t mavlink_msg_envelope_get_src(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field dst from envelope message
 *
 * @return  Destination endpoint name
 */
static inline uint32_t mavlink_msg_envelope_get_dst(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field msg_type from envelope message
 *
 * @return  Message type name
 */
static inline uint32_t mavlink_msg_envelope_get_msg_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field request_id from envelope message
 *
 * @return  Request ID for matching request/reply
 */
static inline uint32_t mavlink_msg_envelope_get_request_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  12);
}

/**
 * @brief Get field instance_id from envelope message
 *
 * @return  Instance ID for matching request/reply
 */
static inline uint32_t mavlink_msg_envelope_get_instance_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  16);
}

/**
 * @brief Get field payload from envelope message
 *
 * @return  Serialized payload of the message
 */
static inline uint16_t mavlink_msg_envelope_get_payload(const mavlink_message_t* msg, uint8_t *payload)
{
    return _MAV_RETURN_uint8_t_array(msg, payload, 255,  20);
}

/**
 * @brief Decode a envelope message into a struct
 *
 * @param msg The message to decode
 * @param envelope C-struct to decode the message contents into
 */
static inline void mavlink_msg_envelope_decode(const mavlink_message_t* msg, mavlink_envelope_t* envelope)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    envelope->src = mavlink_msg_envelope_get_src(msg);
    envelope->dst = mavlink_msg_envelope_get_dst(msg);
    envelope->msg_type = mavlink_msg_envelope_get_msg_type(msg);
    envelope->request_id = mavlink_msg_envelope_get_request_id(msg);
    envelope->instance_id = mavlink_msg_envelope_get_instance_id(msg);
    mavlink_msg_envelope_get_payload(msg, envelope->payload);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ENVELOPE_LEN? msg->len : MAVLINK_MSG_ID_ENVELOPE_LEN;
        memset(envelope, 0, MAVLINK_MSG_ID_ENVELOPE_LEN);
    memcpy(envelope, _MAV_PAYLOAD(msg), len);
#endif
}
