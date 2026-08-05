#pragma once
// MESSAGE PS4_REQUEST PACKING

#define MAVLINK_MSG_ID_PS4_REQUEST 40012


typedef struct __mavlink_ps4_request_t {
 uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply*/
 int32_t rumble_small; /*<  Small rumble intensity*/
 int32_t rumble_large; /*<  Large rumble intensity*/
 int32_t led_red; /*<  LED red channel*/
 int32_t led_green; /*<  LED green channel*/
 int32_t led_blue; /*<  LED blue channel*/
 int32_t led_flash_on; /*<  LED flash on duration*/
 int32_t led_flash_off; /*<  LED flash off duration*/
} mavlink_ps4_request_t;

#define MAVLINK_MSG_ID_PS4_REQUEST_LEN 32
#define MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN 32
#define MAVLINK_MSG_ID_40012_LEN 32
#define MAVLINK_MSG_ID_40012_MIN_LEN 32

#define MAVLINK_MSG_ID_PS4_REQUEST_CRC 140
#define MAVLINK_MSG_ID_40012_CRC 140



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_PS4_REQUEST { \
    40012, \
    "PS4_REQUEST", \
    8, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ps4_request_t, req_id) }, \
         { "rumble_small", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_ps4_request_t, rumble_small) }, \
         { "rumble_large", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_ps4_request_t, rumble_large) }, \
         { "led_red", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_ps4_request_t, led_red) }, \
         { "led_green", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_ps4_request_t, led_green) }, \
         { "led_blue", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_ps4_request_t, led_blue) }, \
         { "led_flash_on", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_ps4_request_t, led_flash_on) }, \
         { "led_flash_off", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_ps4_request_t, led_flash_off) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_PS4_REQUEST { \
    "PS4_REQUEST", \
    8, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ps4_request_t, req_id) }, \
         { "rumble_small", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_ps4_request_t, rumble_small) }, \
         { "rumble_large", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_ps4_request_t, rumble_large) }, \
         { "led_red", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_ps4_request_t, led_red) }, \
         { "led_green", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_ps4_request_t, led_green) }, \
         { "led_blue", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_ps4_request_t, led_blue) }, \
         { "led_flash_on", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_ps4_request_t, led_flash_on) }, \
         { "led_flash_off", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_ps4_request_t, led_flash_off) }, \
         } \
}
#endif

/**
 * @brief Pack a ps4_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param rumble_small  Small rumble intensity
 * @param rumble_large  Large rumble intensity
 * @param led_red  LED red channel
 * @param led_green  LED green channel
 * @param led_blue  LED blue channel
 * @param led_flash_on  LED flash on duration
 * @param led_flash_off  LED flash off duration
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ps4_request_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t req_id, int32_t rumble_small, int32_t rumble_large, int32_t led_red, int32_t led_green, int32_t led_blue, int32_t led_flash_on, int32_t led_flash_off)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PS4_REQUEST_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rumble_small);
    _mav_put_int32_t(buf, 8, rumble_large);
    _mav_put_int32_t(buf, 12, led_red);
    _mav_put_int32_t(buf, 16, led_green);
    _mav_put_int32_t(buf, 20, led_blue);
    _mav_put_int32_t(buf, 24, led_flash_on);
    _mav_put_int32_t(buf, 28, led_flash_off);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PS4_REQUEST_LEN);
#else
    mavlink_ps4_request_t packet;
    packet.req_id = req_id;
    packet.rumble_small = rumble_small;
    packet.rumble_large = rumble_large;
    packet.led_red = led_red;
    packet.led_green = led_green;
    packet.led_blue = led_blue;
    packet.led_flash_on = led_flash_on;
    packet.led_flash_off = led_flash_off;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PS4_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PS4_REQUEST;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN, MAVLINK_MSG_ID_PS4_REQUEST_LEN, MAVLINK_MSG_ID_PS4_REQUEST_CRC);
}

/**
 * @brief Pack a ps4_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param rumble_small  Small rumble intensity
 * @param rumble_large  Large rumble intensity
 * @param led_red  LED red channel
 * @param led_green  LED green channel
 * @param led_blue  LED blue channel
 * @param led_flash_on  LED flash on duration
 * @param led_flash_off  LED flash off duration
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ps4_request_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t req_id, int32_t rumble_small, int32_t rumble_large, int32_t led_red, int32_t led_green, int32_t led_blue, int32_t led_flash_on, int32_t led_flash_off)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PS4_REQUEST_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rumble_small);
    _mav_put_int32_t(buf, 8, rumble_large);
    _mav_put_int32_t(buf, 12, led_red);
    _mav_put_int32_t(buf, 16, led_green);
    _mav_put_int32_t(buf, 20, led_blue);
    _mav_put_int32_t(buf, 24, led_flash_on);
    _mav_put_int32_t(buf, 28, led_flash_off);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PS4_REQUEST_LEN);
#else
    mavlink_ps4_request_t packet;
    packet.req_id = req_id;
    packet.rumble_small = rumble_small;
    packet.rumble_large = rumble_large;
    packet.led_red = led_red;
    packet.led_green = led_green;
    packet.led_blue = led_blue;
    packet.led_flash_on = led_flash_on;
    packet.led_flash_off = led_flash_off;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PS4_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PS4_REQUEST;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN, MAVLINK_MSG_ID_PS4_REQUEST_LEN, MAVLINK_MSG_ID_PS4_REQUEST_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN, MAVLINK_MSG_ID_PS4_REQUEST_LEN);
#endif
}

/**
 * @brief Pack a ps4_request message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param rumble_small  Small rumble intensity
 * @param rumble_large  Large rumble intensity
 * @param led_red  LED red channel
 * @param led_green  LED green channel
 * @param led_blue  LED blue channel
 * @param led_flash_on  LED flash on duration
 * @param led_flash_off  LED flash off duration
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ps4_request_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t req_id,int32_t rumble_small,int32_t rumble_large,int32_t led_red,int32_t led_green,int32_t led_blue,int32_t led_flash_on,int32_t led_flash_off)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PS4_REQUEST_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rumble_small);
    _mav_put_int32_t(buf, 8, rumble_large);
    _mav_put_int32_t(buf, 12, led_red);
    _mav_put_int32_t(buf, 16, led_green);
    _mav_put_int32_t(buf, 20, led_blue);
    _mav_put_int32_t(buf, 24, led_flash_on);
    _mav_put_int32_t(buf, 28, led_flash_off);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_PS4_REQUEST_LEN);
#else
    mavlink_ps4_request_t packet;
    packet.req_id = req_id;
    packet.rumble_small = rumble_small;
    packet.rumble_large = rumble_large;
    packet.led_red = led_red;
    packet.led_green = led_green;
    packet.led_blue = led_blue;
    packet.led_flash_on = led_flash_on;
    packet.led_flash_off = led_flash_off;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_PS4_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_PS4_REQUEST;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN, MAVLINK_MSG_ID_PS4_REQUEST_LEN, MAVLINK_MSG_ID_PS4_REQUEST_CRC);
}

/**
 * @brief Encode a ps4_request struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ps4_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ps4_request_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ps4_request_t* ps4_request)
{
    return mavlink_msg_ps4_request_pack(system_id, component_id, msg, ps4_request->req_id, ps4_request->rumble_small, ps4_request->rumble_large, ps4_request->led_red, ps4_request->led_green, ps4_request->led_blue, ps4_request->led_flash_on, ps4_request->led_flash_off);
}

/**
 * @brief Encode a ps4_request struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ps4_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ps4_request_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ps4_request_t* ps4_request)
{
    return mavlink_msg_ps4_request_pack_chan(system_id, component_id, chan, msg, ps4_request->req_id, ps4_request->rumble_small, ps4_request->rumble_large, ps4_request->led_red, ps4_request->led_green, ps4_request->led_blue, ps4_request->led_flash_on, ps4_request->led_flash_off);
}

/**
 * @brief Encode a ps4_request struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ps4_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ps4_request_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ps4_request_t* ps4_request)
{
    return mavlink_msg_ps4_request_pack_status(system_id, component_id, _status, msg,  ps4_request->req_id, ps4_request->rumble_small, ps4_request->rumble_large, ps4_request->led_red, ps4_request->led_green, ps4_request->led_blue, ps4_request->led_flash_on, ps4_request->led_flash_off);
}

/**
 * @brief Send a ps4_request message
 * @param chan MAVLink channel to send the message
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param rumble_small  Small rumble intensity
 * @param rumble_large  Large rumble intensity
 * @param led_red  LED red channel
 * @param led_green  LED green channel
 * @param led_blue  LED blue channel
 * @param led_flash_on  LED flash on duration
 * @param led_flash_off  LED flash off duration
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ps4_request_send(mavlink_channel_t chan, uint32_t req_id, int32_t rumble_small, int32_t rumble_large, int32_t led_red, int32_t led_green, int32_t led_blue, int32_t led_flash_on, int32_t led_flash_off)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_PS4_REQUEST_LEN];
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rumble_small);
    _mav_put_int32_t(buf, 8, rumble_large);
    _mav_put_int32_t(buf, 12, led_red);
    _mav_put_int32_t(buf, 16, led_green);
    _mav_put_int32_t(buf, 20, led_blue);
    _mav_put_int32_t(buf, 24, led_flash_on);
    _mav_put_int32_t(buf, 28, led_flash_off);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_REQUEST, buf, MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN, MAVLINK_MSG_ID_PS4_REQUEST_LEN, MAVLINK_MSG_ID_PS4_REQUEST_CRC);
#else
    mavlink_ps4_request_t packet;
    packet.req_id = req_id;
    packet.rumble_small = rumble_small;
    packet.rumble_large = rumble_large;
    packet.led_red = led_red;
    packet.led_green = led_green;
    packet.led_blue = led_blue;
    packet.led_flash_on = led_flash_on;
    packet.led_flash_off = led_flash_off;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_REQUEST, (const char *)&packet, MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN, MAVLINK_MSG_ID_PS4_REQUEST_LEN, MAVLINK_MSG_ID_PS4_REQUEST_CRC);
#endif
}

/**
 * @brief Send a ps4_request message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ps4_request_send_struct(mavlink_channel_t chan, const mavlink_ps4_request_t* ps4_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ps4_request_send(chan, ps4_request->req_id, ps4_request->rumble_small, ps4_request->rumble_large, ps4_request->led_red, ps4_request->led_green, ps4_request->led_blue, ps4_request->led_flash_on, ps4_request->led_flash_off);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_REQUEST, (const char *)ps4_request, MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN, MAVLINK_MSG_ID_PS4_REQUEST_LEN, MAVLINK_MSG_ID_PS4_REQUEST_CRC);
#endif
}

#if MAVLINK_MSG_ID_PS4_REQUEST_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ps4_request_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t req_id, int32_t rumble_small, int32_t rumble_large, int32_t led_red, int32_t led_green, int32_t led_blue, int32_t led_flash_on, int32_t led_flash_off)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, req_id);
    _mav_put_int32_t(buf, 4, rumble_small);
    _mav_put_int32_t(buf, 8, rumble_large);
    _mav_put_int32_t(buf, 12, led_red);
    _mav_put_int32_t(buf, 16, led_green);
    _mav_put_int32_t(buf, 20, led_blue);
    _mav_put_int32_t(buf, 24, led_flash_on);
    _mav_put_int32_t(buf, 28, led_flash_off);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_REQUEST, buf, MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN, MAVLINK_MSG_ID_PS4_REQUEST_LEN, MAVLINK_MSG_ID_PS4_REQUEST_CRC);
#else
    mavlink_ps4_request_t *packet = (mavlink_ps4_request_t *)msgbuf;
    packet->req_id = req_id;
    packet->rumble_small = rumble_small;
    packet->rumble_large = rumble_large;
    packet->led_red = led_red;
    packet->led_green = led_green;
    packet->led_blue = led_blue;
    packet->led_flash_on = led_flash_on;
    packet->led_flash_off = led_flash_off;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_PS4_REQUEST, (const char *)packet, MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN, MAVLINK_MSG_ID_PS4_REQUEST_LEN, MAVLINK_MSG_ID_PS4_REQUEST_CRC);
#endif
}
#endif

#endif

// MESSAGE PS4_REQUEST UNPACKING


/**
 * @brief Get field req_id from ps4_request message
 *
 * @return  For request/reply matching, 0 if not a request/reply
 */
static inline uint32_t mavlink_msg_ps4_request_get_req_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field rumble_small from ps4_request message
 *
 * @return  Small rumble intensity
 */
static inline int32_t mavlink_msg_ps4_request_get_rumble_small(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field rumble_large from ps4_request message
 *
 * @return  Large rumble intensity
 */
static inline int32_t mavlink_msg_ps4_request_get_rumble_large(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field led_red from ps4_request message
 *
 * @return  LED red channel
 */
static inline int32_t mavlink_msg_ps4_request_get_led_red(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Get field led_green from ps4_request message
 *
 * @return  LED green channel
 */
static inline int32_t mavlink_msg_ps4_request_get_led_green(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  16);
}

/**
 * @brief Get field led_blue from ps4_request message
 *
 * @return  LED blue channel
 */
static inline int32_t mavlink_msg_ps4_request_get_led_blue(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  20);
}

/**
 * @brief Get field led_flash_on from ps4_request message
 *
 * @return  LED flash on duration
 */
static inline int32_t mavlink_msg_ps4_request_get_led_flash_on(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  24);
}

/**
 * @brief Get field led_flash_off from ps4_request message
 *
 * @return  LED flash off duration
 */
static inline int32_t mavlink_msg_ps4_request_get_led_flash_off(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  28);
}

/**
 * @brief Decode a ps4_request message into a struct
 *
 * @param msg The message to decode
 * @param ps4_request C-struct to decode the message contents into
 */
static inline void mavlink_msg_ps4_request_decode(const mavlink_message_t* msg, mavlink_ps4_request_t* ps4_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ps4_request->req_id = mavlink_msg_ps4_request_get_req_id(msg);
    ps4_request->rumble_small = mavlink_msg_ps4_request_get_rumble_small(msg);
    ps4_request->rumble_large = mavlink_msg_ps4_request_get_rumble_large(msg);
    ps4_request->led_red = mavlink_msg_ps4_request_get_led_red(msg);
    ps4_request->led_green = mavlink_msg_ps4_request_get_led_green(msg);
    ps4_request->led_blue = mavlink_msg_ps4_request_get_led_blue(msg);
    ps4_request->led_flash_on = mavlink_msg_ps4_request_get_led_flash_on(msg);
    ps4_request->led_flash_off = mavlink_msg_ps4_request_get_led_flash_off(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_PS4_REQUEST_LEN? msg->len : MAVLINK_MSG_ID_PS4_REQUEST_LEN;
        memset(ps4_request, 0, MAVLINK_MSG_ID_PS4_REQUEST_LEN);
    memcpy(ps4_request, _MAV_PAYLOAD(msg), len);
#endif
}
