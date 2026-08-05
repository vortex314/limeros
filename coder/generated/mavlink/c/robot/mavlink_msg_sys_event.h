#pragma once
// MESSAGE SYS_EVENT PACKING

#define MAVLINK_MSG_ID_SYS_EVENT 40015


typedef struct __mavlink_sys_event_t {
 uint64_t utc; /*< [us] UTC timestamp*/
 uint64_t uptime; /*< [ms] Uptime*/
 uint64_t free_heap; /*< [bytes] Free heap*/
 uint64_t flash_size; /*< [bytes] Flash size*/
 char cpu_board_type[50]; /*<  CPU board type*/
 char build_date_time[50]; /*<  Build date and time*/
} mavlink_sys_event_t;

#define MAVLINK_MSG_ID_SYS_EVENT_LEN 132
#define MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN 132
#define MAVLINK_MSG_ID_40015_LEN 132
#define MAVLINK_MSG_ID_40015_MIN_LEN 132

#define MAVLINK_MSG_ID_SYS_EVENT_CRC 32
#define MAVLINK_MSG_ID_40015_CRC 32

#define MAVLINK_MSG_SYS_EVENT_FIELD_CPU_BOARD_TYPE_LEN 50
#define MAVLINK_MSG_SYS_EVENT_FIELD_BUILD_DATE_TIME_LEN 50

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SYS_EVENT { \
    40015, \
    "SYS_EVENT", \
    6, \
    {  { "utc", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_sys_event_t, utc) }, \
         { "uptime", NULL, MAVLINK_TYPE_UINT64_T, 0, 8, offsetof(mavlink_sys_event_t, uptime) }, \
         { "free_heap", NULL, MAVLINK_TYPE_UINT64_T, 0, 16, offsetof(mavlink_sys_event_t, free_heap) }, \
         { "flash_size", NULL, MAVLINK_TYPE_UINT64_T, 0, 24, offsetof(mavlink_sys_event_t, flash_size) }, \
         { "cpu_board_type", NULL, MAVLINK_TYPE_CHAR, 50, 32, offsetof(mavlink_sys_event_t, cpu_board_type) }, \
         { "build_date_time", NULL, MAVLINK_TYPE_CHAR, 50, 82, offsetof(mavlink_sys_event_t, build_date_time) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SYS_EVENT { \
    "SYS_EVENT", \
    6, \
    {  { "utc", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_sys_event_t, utc) }, \
         { "uptime", NULL, MAVLINK_TYPE_UINT64_T, 0, 8, offsetof(mavlink_sys_event_t, uptime) }, \
         { "free_heap", NULL, MAVLINK_TYPE_UINT64_T, 0, 16, offsetof(mavlink_sys_event_t, free_heap) }, \
         { "flash_size", NULL, MAVLINK_TYPE_UINT64_T, 0, 24, offsetof(mavlink_sys_event_t, flash_size) }, \
         { "cpu_board_type", NULL, MAVLINK_TYPE_CHAR, 50, 32, offsetof(mavlink_sys_event_t, cpu_board_type) }, \
         { "build_date_time", NULL, MAVLINK_TYPE_CHAR, 50, 82, offsetof(mavlink_sys_event_t, build_date_time) }, \
         } \
}
#endif

/**
 * @brief Pack a sys_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param utc [us] UTC timestamp
 * @param uptime [ms] Uptime
 * @param free_heap [bytes] Free heap
 * @param flash_size [bytes] Flash size
 * @param cpu_board_type  CPU board type
 * @param build_date_time  Build date and time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sys_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint64_t utc, uint64_t uptime, uint64_t free_heap, uint64_t flash_size, const char *cpu_board_type, const char *build_date_time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, utc);
    _mav_put_uint64_t(buf, 8, uptime);
    _mav_put_uint64_t(buf, 16, free_heap);
    _mav_put_uint64_t(buf, 24, flash_size);
    _mav_put_char_array(buf, 32, cpu_board_type, 50);
    _mav_put_char_array(buf, 82, build_date_time, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SYS_EVENT_LEN);
#else
    mavlink_sys_event_t packet;
    packet.utc = utc;
    packet.uptime = uptime;
    packet.free_heap = free_heap;
    packet.flash_size = flash_size;
    mav_array_assign_char(packet.cpu_board_type, cpu_board_type, 50);
    mav_array_assign_char(packet.build_date_time, build_date_time, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SYS_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SYS_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN, MAVLINK_MSG_ID_SYS_EVENT_LEN, MAVLINK_MSG_ID_SYS_EVENT_CRC);
}

/**
 * @brief Pack a sys_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param utc [us] UTC timestamp
 * @param uptime [ms] Uptime
 * @param free_heap [bytes] Free heap
 * @param flash_size [bytes] Flash size
 * @param cpu_board_type  CPU board type
 * @param build_date_time  Build date and time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sys_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint64_t utc, uint64_t uptime, uint64_t free_heap, uint64_t flash_size, const char *cpu_board_type, const char *build_date_time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, utc);
    _mav_put_uint64_t(buf, 8, uptime);
    _mav_put_uint64_t(buf, 16, free_heap);
    _mav_put_uint64_t(buf, 24, flash_size);
    _mav_put_char_array(buf, 32, cpu_board_type, 50);
    _mav_put_char_array(buf, 82, build_date_time, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SYS_EVENT_LEN);
#else
    mavlink_sys_event_t packet;
    packet.utc = utc;
    packet.uptime = uptime;
    packet.free_heap = free_heap;
    packet.flash_size = flash_size;
    mav_array_memcpy(packet.cpu_board_type, cpu_board_type, sizeof(char)*50);
    mav_array_memcpy(packet.build_date_time, build_date_time, sizeof(char)*50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SYS_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SYS_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN, MAVLINK_MSG_ID_SYS_EVENT_LEN, MAVLINK_MSG_ID_SYS_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN, MAVLINK_MSG_ID_SYS_EVENT_LEN);
#endif
}

/**
 * @brief Pack a sys_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param utc [us] UTC timestamp
 * @param uptime [ms] Uptime
 * @param free_heap [bytes] Free heap
 * @param flash_size [bytes] Flash size
 * @param cpu_board_type  CPU board type
 * @param build_date_time  Build date and time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sys_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint64_t utc,uint64_t uptime,uint64_t free_heap,uint64_t flash_size,const char *cpu_board_type,const char *build_date_time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, utc);
    _mav_put_uint64_t(buf, 8, uptime);
    _mav_put_uint64_t(buf, 16, free_heap);
    _mav_put_uint64_t(buf, 24, flash_size);
    _mav_put_char_array(buf, 32, cpu_board_type, 50);
    _mav_put_char_array(buf, 82, build_date_time, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SYS_EVENT_LEN);
#else
    mavlink_sys_event_t packet;
    packet.utc = utc;
    packet.uptime = uptime;
    packet.free_heap = free_heap;
    packet.flash_size = flash_size;
    mav_array_assign_char(packet.cpu_board_type, cpu_board_type, 50);
    mav_array_assign_char(packet.build_date_time, build_date_time, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SYS_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SYS_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN, MAVLINK_MSG_ID_SYS_EVENT_LEN, MAVLINK_MSG_ID_SYS_EVENT_CRC);
}

/**
 * @brief Encode a sys_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param sys_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sys_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_sys_event_t* sys_event)
{
    return mavlink_msg_sys_event_pack(system_id, component_id, msg, sys_event->utc, sys_event->uptime, sys_event->free_heap, sys_event->flash_size, sys_event->cpu_board_type, sys_event->build_date_time);
}

/**
 * @brief Encode a sys_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param sys_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sys_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_sys_event_t* sys_event)
{
    return mavlink_msg_sys_event_pack_chan(system_id, component_id, chan, msg, sys_event->utc, sys_event->uptime, sys_event->free_heap, sys_event->flash_size, sys_event->cpu_board_type, sys_event->build_date_time);
}

/**
 * @brief Encode a sys_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param sys_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sys_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_sys_event_t* sys_event)
{
    return mavlink_msg_sys_event_pack_status(system_id, component_id, _status, msg,  sys_event->utc, sys_event->uptime, sys_event->free_heap, sys_event->flash_size, sys_event->cpu_board_type, sys_event->build_date_time);
}

/**
 * @brief Send a sys_event message
 * @param chan MAVLink channel to send the message
 *
 * @param utc [us] UTC timestamp
 * @param uptime [ms] Uptime
 * @param free_heap [bytes] Free heap
 * @param flash_size [bytes] Flash size
 * @param cpu_board_type  CPU board type
 * @param build_date_time  Build date and time
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_sys_event_send(mavlink_channel_t chan, uint64_t utc, uint64_t uptime, uint64_t free_heap, uint64_t flash_size, const char *cpu_board_type, const char *build_date_time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SYS_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, utc);
    _mav_put_uint64_t(buf, 8, uptime);
    _mav_put_uint64_t(buf, 16, free_heap);
    _mav_put_uint64_t(buf, 24, flash_size);
    _mav_put_char_array(buf, 32, cpu_board_type, 50);
    _mav_put_char_array(buf, 82, build_date_time, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_EVENT, buf, MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN, MAVLINK_MSG_ID_SYS_EVENT_LEN, MAVLINK_MSG_ID_SYS_EVENT_CRC);
#else
    mavlink_sys_event_t packet;
    packet.utc = utc;
    packet.uptime = uptime;
    packet.free_heap = free_heap;
    packet.flash_size = flash_size;
    mav_array_assign_char(packet.cpu_board_type, cpu_board_type, 50);
    mav_array_assign_char(packet.build_date_time, build_date_time, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_EVENT, (const char *)&packet, MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN, MAVLINK_MSG_ID_SYS_EVENT_LEN, MAVLINK_MSG_ID_SYS_EVENT_CRC);
#endif
}

/**
 * @brief Send a sys_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_sys_event_send_struct(mavlink_channel_t chan, const mavlink_sys_event_t* sys_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_sys_event_send(chan, sys_event->utc, sys_event->uptime, sys_event->free_heap, sys_event->flash_size, sys_event->cpu_board_type, sys_event->build_date_time);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_EVENT, (const char *)sys_event, MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN, MAVLINK_MSG_ID_SYS_EVENT_LEN, MAVLINK_MSG_ID_SYS_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_SYS_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_sys_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint64_t utc, uint64_t uptime, uint64_t free_heap, uint64_t flash_size, const char *cpu_board_type, const char *build_date_time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, utc);
    _mav_put_uint64_t(buf, 8, uptime);
    _mav_put_uint64_t(buf, 16, free_heap);
    _mav_put_uint64_t(buf, 24, flash_size);
    _mav_put_char_array(buf, 32, cpu_board_type, 50);
    _mav_put_char_array(buf, 82, build_date_time, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_EVENT, buf, MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN, MAVLINK_MSG_ID_SYS_EVENT_LEN, MAVLINK_MSG_ID_SYS_EVENT_CRC);
#else
    mavlink_sys_event_t *packet = (mavlink_sys_event_t *)msgbuf;
    packet->utc = utc;
    packet->uptime = uptime;
    packet->free_heap = free_heap;
    packet->flash_size = flash_size;
    mav_array_assign_char(packet->cpu_board_type, cpu_board_type, 50);
    mav_array_assign_char(packet->build_date_time, build_date_time, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SYS_EVENT, (const char *)packet, MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN, MAVLINK_MSG_ID_SYS_EVENT_LEN, MAVLINK_MSG_ID_SYS_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE SYS_EVENT UNPACKING


/**
 * @brief Get field utc from sys_event message
 *
 * @return [us] UTC timestamp
 */
static inline uint64_t mavlink_msg_sys_event_get_utc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field uptime from sys_event message
 *
 * @return [ms] Uptime
 */
static inline uint64_t mavlink_msg_sys_event_get_uptime(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  8);
}

/**
 * @brief Get field free_heap from sys_event message
 *
 * @return [bytes] Free heap
 */
static inline uint64_t mavlink_msg_sys_event_get_free_heap(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  16);
}

/**
 * @brief Get field flash_size from sys_event message
 *
 * @return [bytes] Flash size
 */
static inline uint64_t mavlink_msg_sys_event_get_flash_size(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  24);
}

/**
 * @brief Get field cpu_board_type from sys_event message
 *
 * @return  CPU board type
 */
static inline uint16_t mavlink_msg_sys_event_get_cpu_board_type(const mavlink_message_t* msg, char *cpu_board_type)
{
    return _MAV_RETURN_char_array(msg, cpu_board_type, 50,  32);
}

/**
 * @brief Get field build_date_time from sys_event message
 *
 * @return  Build date and time
 */
static inline uint16_t mavlink_msg_sys_event_get_build_date_time(const mavlink_message_t* msg, char *build_date_time)
{
    return _MAV_RETURN_char_array(msg, build_date_time, 50,  82);
}

/**
 * @brief Decode a sys_event message into a struct
 *
 * @param msg The message to decode
 * @param sys_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_sys_event_decode(const mavlink_message_t* msg, mavlink_sys_event_t* sys_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    sys_event->utc = mavlink_msg_sys_event_get_utc(msg);
    sys_event->uptime = mavlink_msg_sys_event_get_uptime(msg);
    sys_event->free_heap = mavlink_msg_sys_event_get_free_heap(msg);
    sys_event->flash_size = mavlink_msg_sys_event_get_flash_size(msg);
    mavlink_msg_sys_event_get_cpu_board_type(msg, sys_event->cpu_board_type);
    mavlink_msg_sys_event_get_build_date_time(msg, sys_event->build_date_time);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SYS_EVENT_LEN? msg->len : MAVLINK_MSG_ID_SYS_EVENT_LEN;
        memset(sys_event, 0, MAVLINK_MSG_ID_SYS_EVENT_LEN);
    memcpy(sys_event, _MAV_PAYLOAD(msg), len);
#endif
}
