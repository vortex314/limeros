#pragma once
// MESSAGE HOVERBOARD_REPLY PACKING

#define MAVLINK_MSG_ID_HOVERBOARD_REPLY 40002


typedef struct __mavlink_hoverboard_reply_t {
 int32_t speed; /*< [m/s] Speed command for the hoverboard*/
 int32_t steer; /*< [deg] Steering command for the hoverboard*/
} mavlink_hoverboard_reply_t;

#define MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN 8
#define MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN 8
#define MAVLINK_MSG_ID_40002_LEN 8
#define MAVLINK_MSG_ID_40002_MIN_LEN 8

#define MAVLINK_MSG_ID_HOVERBOARD_REPLY_CRC 223
#define MAVLINK_MSG_ID_40002_CRC 223



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_HOVERBOARD_REPLY { \
    40002, \
    "HOVERBOARD_REPLY", \
    2, \
    {  { "speed", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_hoverboard_reply_t, speed) }, \
         { "steer", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_hoverboard_reply_t, steer) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_HOVERBOARD_REPLY { \
    "HOVERBOARD_REPLY", \
    2, \
    {  { "speed", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_hoverboard_reply_t, speed) }, \
         { "steer", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_hoverboard_reply_t, steer) }, \
         } \
}
#endif

/**
 * @brief Pack a hoverboard_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param speed [m/s] Speed command for the hoverboard
 * @param steer [deg] Steering command for the hoverboard
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hoverboard_reply_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               int32_t speed, int32_t steer)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN];
    _mav_put_int32_t(buf, 0, speed);
    _mav_put_int32_t(buf, 4, steer);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN);
#else
    mavlink_hoverboard_reply_t packet;
    packet.speed = speed;
    packet.steer = steer;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HOVERBOARD_REPLY;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_CRC);
}

/**
 * @brief Pack a hoverboard_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param speed [m/s] Speed command for the hoverboard
 * @param steer [deg] Steering command for the hoverboard
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hoverboard_reply_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               int32_t speed, int32_t steer)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN];
    _mav_put_int32_t(buf, 0, speed);
    _mav_put_int32_t(buf, 4, steer);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN);
#else
    mavlink_hoverboard_reply_t packet;
    packet.speed = speed;
    packet.steer = steer;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HOVERBOARD_REPLY;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN);
#endif
}

/**
 * @brief Pack a hoverboard_reply message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param speed [m/s] Speed command for the hoverboard
 * @param steer [deg] Steering command for the hoverboard
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hoverboard_reply_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   int32_t speed,int32_t steer)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN];
    _mav_put_int32_t(buf, 0, speed);
    _mav_put_int32_t(buf, 4, steer);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN);
#else
    mavlink_hoverboard_reply_t packet;
    packet.speed = speed;
    packet.steer = steer;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HOVERBOARD_REPLY;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_CRC);
}

/**
 * @brief Encode a hoverboard_reply struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param hoverboard_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hoverboard_reply_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_hoverboard_reply_t* hoverboard_reply)
{
    return mavlink_msg_hoverboard_reply_pack(system_id, component_id, msg, hoverboard_reply->speed, hoverboard_reply->steer);
}

/**
 * @brief Encode a hoverboard_reply struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param hoverboard_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hoverboard_reply_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_hoverboard_reply_t* hoverboard_reply)
{
    return mavlink_msg_hoverboard_reply_pack_chan(system_id, component_id, chan, msg, hoverboard_reply->speed, hoverboard_reply->steer);
}

/**
 * @brief Encode a hoverboard_reply struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param hoverboard_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hoverboard_reply_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_hoverboard_reply_t* hoverboard_reply)
{
    return mavlink_msg_hoverboard_reply_pack_status(system_id, component_id, _status, msg,  hoverboard_reply->speed, hoverboard_reply->steer);
}

/**
 * @brief Send a hoverboard_reply message
 * @param chan MAVLink channel to send the message
 *
 * @param speed [m/s] Speed command for the hoverboard
 * @param steer [deg] Steering command for the hoverboard
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_hoverboard_reply_send(mavlink_channel_t chan, int32_t speed, int32_t steer)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN];
    _mav_put_int32_t(buf, 0, speed);
    _mav_put_int32_t(buf, 4, steer);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_REPLY, buf, MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_CRC);
#else
    mavlink_hoverboard_reply_t packet;
    packet.speed = speed;
    packet.steer = steer;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_REPLY, (const char *)&packet, MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_CRC);
#endif
}

/**
 * @brief Send a hoverboard_reply message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_hoverboard_reply_send_struct(mavlink_channel_t chan, const mavlink_hoverboard_reply_t* hoverboard_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_hoverboard_reply_send(chan, hoverboard_reply->speed, hoverboard_reply->steer);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_REPLY, (const char *)hoverboard_reply, MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_CRC);
#endif
}

#if MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_hoverboard_reply_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  int32_t speed, int32_t steer)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, speed);
    _mav_put_int32_t(buf, 4, steer);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_REPLY, buf, MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_CRC);
#else
    mavlink_hoverboard_reply_t *packet = (mavlink_hoverboard_reply_t *)msgbuf;
    packet->speed = speed;
    packet->steer = steer;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_REPLY, (const char *)packet, MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN, MAVLINK_MSG_ID_HOVERBOARD_REPLY_CRC);
#endif
}
#endif

#endif

// MESSAGE HOVERBOARD_REPLY UNPACKING


/**
 * @brief Get field speed from hoverboard_reply message
 *
 * @return [m/s] Speed command for the hoverboard
 */
static inline int32_t mavlink_msg_hoverboard_reply_get_speed(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field steer from hoverboard_reply message
 *
 * @return [deg] Steering command for the hoverboard
 */
static inline int32_t mavlink_msg_hoverboard_reply_get_steer(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Decode a hoverboard_reply message into a struct
 *
 * @param msg The message to decode
 * @param hoverboard_reply C-struct to decode the message contents into
 */
static inline void mavlink_msg_hoverboard_reply_decode(const mavlink_message_t* msg, mavlink_hoverboard_reply_t* hoverboard_reply)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    hoverboard_reply->speed = mavlink_msg_hoverboard_reply_get_speed(msg);
    hoverboard_reply->steer = mavlink_msg_hoverboard_reply_get_steer(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN? msg->len : MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN;
        memset(hoverboard_reply, 0, MAVLINK_MSG_ID_HOVERBOARD_REPLY_LEN);
    memcpy(hoverboard_reply, _MAV_PAYLOAD(msg), len);
#endif
}
