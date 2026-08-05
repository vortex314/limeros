#pragma once
// MESSAGE SYS_REQUEST PACKING

#define MAVLINK_MSG_ID_SYS_REQUEST 40013


typedef struct __mavlink_sys_request_t {
 uint64_t set_time; /*< [us] Set system time*/
 uint32_t req_id; /*<  For request/reply matching, 0 if not a request/reply*/
 uint8_t reboot; /*<  Reboot the device*/
 char console[100]; /*<  Console command string*/
} mavlink_sys_request_t;

#define MAVLINK_MSG_ID_SYS_REQUEST_LEN 113
#define MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN 113
#define MAVLINK_MSG_ID_40013_LEN 113
#define MAVLINK_MSG_ID_40013_MIN_LEN 113

#define MAVLINK_MSG_ID_SYS_REQUEST_CRC 15
#define MAVLINK_MSG_ID_40013_CRC 15

#define MAVLINK_MSG_SYS_REQUEST_FIELD_CONSOLE_LEN 100

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SYS_REQUEST { \
    40013, \
    "SYS_REQUEST", \
    4, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_sys_request_t, req_id) }, \
         { "set_time", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_sys_request_t, set_time) }, \
         { "reboot", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_sys_request_t, reboot) }, \
         { "console", NULL, MAVLINK_TYPE_CHAR, 100, 13, offsetof(mavlink_sys_request_t, console) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SYS_REQUEST { \
    "SYS_REQUEST", \
    4, \
    {  { "req_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_sys_request_t, req_id) }, \
         { "set_time", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_sys_request_t, set_time) }, \
         { "reboot", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_sys_request_t, reboot) }, \
         { "console", NULL, MAVLINK_TYPE_CHAR, 100, 13, offsetof(mavlink_sys_request_t, console) }, \
         } \
}
#endif

/**
 * @brief Pack a sys_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param set_time [us] Set system time
 * @param reboot  Reboot the device
 * @param console  Console command string
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sys_request_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t req_id, uint64_t set_time, uint8_t reboot, const char *console)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_REQUEST_LEN];
    _mav_put_uint64_t(buf, 0, set_time);
    _mav_put_uint32_t(buf, 8, req_id);
    _mav_put_uint8_t(buf, 12, reboot);
    _mav_put_char_array(buf, 13, console, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SYS_REQUEST_LEN);
#else
    mavlink_sys_request_t packet;
    packet.set_time = set_time;
    packet.req_id = req_id;
    packet.reboot = reboot;
    mav_array_assign_char(packet.console, console, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SYS_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SYS_REQUEST;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN, MAVLINK_MSG_ID_SYS_REQUEST_LEN, MAVLINK_MSG_ID_SYS_REQUEST_CRC);
}

/**
 * @brief Pack a sys_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param set_time [us] Set system time
 * @param reboot  Reboot the device
 * @param console  Console command string
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sys_request_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t req_id, uint64_t set_time, uint8_t reboot, const char *console)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_REQUEST_LEN];
    _mav_put_uint64_t(buf, 0, set_time);
    _mav_put_uint32_t(buf, 8, req_id);
    _mav_put_uint8_t(buf, 12, reboot);
    _mav_put_char_array(buf, 13, console, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SYS_REQUEST_LEN);
#else
    mavlink_sys_request_t packet;
    packet.set_time = set_time;
    packet.req_id = req_id;
    packet.reboot = reboot;
    mav_array_memcpy(packet.console, console, sizeof(char)*100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SYS_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SYS_REQUEST;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN, MAVLINK_MSG_ID_SYS_REQUEST_LEN, MAVLINK_MSG_ID_SYS_REQUEST_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN, MAVLINK_MSG_ID_SYS_REQUEST_LEN);
#endif
}

/**
 * @brief Pack a sys_request message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param set_time [us] Set system time
 * @param reboot  Reboot the device
 * @param console  Console command string
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sys_request_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t req_id,uint64_t set_time,uint8_t reboot,const char *console)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_REQUEST_LEN];
    _mav_put_uint64_t(buf, 0, set_time);
    _mav_put_uint32_t(buf, 8, req_id);
    _mav_put_uint8_t(buf, 12, reboot);
    _mav_put_char_array(buf, 13, console, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SYS_REQUEST_LEN);
#else
    mavlink_sys_request_t packet;
    packet.set_time = set_time;
    packet.req_id = req_id;
    packet.reboot = reboot;
    mav_array_assign_char(packet.console, console, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SYS_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SYS_REQUEST;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN, MAVLINK_MSG_ID_SYS_REQUEST_LEN, MAVLINK_MSG_ID_SYS_REQUEST_CRC);
}

/**
 * @brief Encode a sys_request struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param sys_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sys_request_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_sys_request_t* sys_request)
{
    return mavlink_msg_sys_request_pack(system_id, component_id, msg, sys_request->req_id, sys_request->set_time, sys_request->reboot, sys_request->console);
}

/**
 * @brief Encode a sys_request struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param sys_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sys_request_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_sys_request_t* sys_request)
{
    return mavlink_msg_sys_request_pack_chan(system_id, component_id, chan, msg, sys_request->req_id, sys_request->set_time, sys_request->reboot, sys_request->console);
}

/**
 * @brief Encode a sys_request struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param sys_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sys_request_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_sys_request_t* sys_request)
{
    return mavlink_msg_sys_request_pack_status(system_id, component_id, _status, msg,  sys_request->req_id, sys_request->set_time, sys_request->reboot, sys_request->console);
}

/**
 * @brief Send a sys_request message
 * @param chan MAVLink channel to send the message
 *
 * @param req_id  For request/reply matching, 0 if not a request/reply
 * @param set_time [us] Set system time
 * @param reboot  Reboot the device
 * @param console  Console command string
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_sys_request_send(mavlink_channel_t chan, uint32_t req_id, uint64_t set_time, uint8_t reboot, const char *console)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_REQUEST_LEN];
    _mav_put_uint64_t(buf, 0, set_time);
    _mav_put_uint32_t(buf, 8, req_id);
    _mav_put_uint8_t(buf, 12, reboot);
    _mav_put_char_array(buf, 13, console, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REQUEST, buf, MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN, MAVLINK_MSG_ID_SYS_REQUEST_LEN, MAVLINK_MSG_ID_SYS_REQUEST_CRC);
#else
    mavlink_sys_request_t packet;
    packet.set_time = set_time;
    packet.req_id = req_id;
    packet.reboot = reboot;
    mav_array_assign_char(packet.console, console, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REQUEST, (const char *)&packet, MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN, MAVLINK_MSG_ID_SYS_REQUEST_LEN, MAVLINK_MSG_ID_SYS_REQUEST_CRC);
#endif
}

/**
 * @brief Send a sys_request message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_sys_request_send_struct(mavlink_channel_t chan, const mavlink_sys_request_t* sys_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_sys_request_send(chan, sys_request->req_id, sys_request->set_time, sys_request->reboot, sys_request->console);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REQUEST, (const char *)sys_request, MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN, MAVLINK_MSG_ID_SYS_REQUEST_LEN, MAVLINK_MSG_ID_SYS_REQUEST_CRC);
#endif
}

#if MAVLINK_MSG_ID_SYS_REQUEST_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_sys_request_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t req_id, uint64_t set_time, uint8_t reboot, const char *console)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, set_time);
    _mav_put_uint32_t(buf, 8, req_id);
    _mav_put_uint8_t(buf, 12, reboot);
    _mav_put_char_array(buf, 13, console, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REQUEST, buf, MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN, MAVLINK_MSG_ID_SYS_REQUEST_LEN, MAVLINK_MSG_ID_SYS_REQUEST_CRC);
#else
    mavlink_sys_request_t *packet = (mavlink_sys_request_t *)msgbuf;
    packet->set_time = set_time;
    packet->req_id = req_id;
    packet->reboot = reboot;
    mav_array_assign_char(packet->console, console, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_REQUEST, (const char *)packet, MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN, MAVLINK_MSG_ID_SYS_REQUEST_LEN, MAVLINK_MSG_ID_SYS_REQUEST_CRC);
#endif
}
#endif

#endif

// MESSAGE SYS_REQUEST UNPACKING


/**
 * @brief Get field req_id from sys_request message
 *
 * @return  For request/reply matching, 0 if not a request/reply
 */
static inline uint32_t mavlink_msg_sys_request_get_req_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field set_time from sys_request message
 *
 * @return [us] Set system time
 */
static inline uint64_t mavlink_msg_sys_request_get_set_time(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field reboot from sys_request message
 *
 * @return  Reboot the device
 */
static inline uint8_t mavlink_msg_sys_request_get_reboot(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field console from sys_request message
 *
 * @return  Console command string
 */
static inline uint16_t mavlink_msg_sys_request_get_console(const mavlink_message_t* msg, char *console)
{
    return _MAV_RETURN_char_array(msg, console, 100,  13);
}

/**
 * @brief Decode a sys_request message into a struct
 *
 * @param msg The message to decode
 * @param sys_request C-struct to decode the message contents into
 */
static inline void mavlink_msg_sys_request_decode(const mavlink_message_t* msg, mavlink_sys_request_t* sys_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    sys_request->set_time = mavlink_msg_sys_request_get_set_time(msg);
    sys_request->req_id = mavlink_msg_sys_request_get_req_id(msg);
    sys_request->reboot = mavlink_msg_sys_request_get_reboot(msg);
    mavlink_msg_sys_request_get_console(msg, sys_request->console);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SYS_REQUEST_LEN? msg->len : MAVLINK_MSG_ID_SYS_REQUEST_LEN;
        memset(sys_request, 0, MAVLINK_MSG_ID_SYS_REQUEST_LEN);
    memcpy(sys_request, _MAV_PAYLOAD(msg), len);
#endif
}
