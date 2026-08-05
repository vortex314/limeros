#pragma once
// MESSAGE MAX31855_EVENT PACKING

#define MAVLINK_MSG_ID_MAX31855_EVENT 40021


typedef struct __mavlink_max31855_event_t {
 float thermocouple_temp; /*< [degC] Thermocouple temperature in Celsius*/
 float internal_temp; /*< [degC] Internal temperature in Celsius*/
 uint8_t fault; /*<  Fault detected*/
 uint8_t fault_short_vcc; /*<  Short to VCC detected*/
 uint8_t fault_short_gnd; /*<  Short to GND detected*/
 uint8_t fault_open_tc; /*<  Open thermocouple detected*/
} mavlink_max31855_event_t;

#define MAVLINK_MSG_ID_MAX31855_EVENT_LEN 12
#define MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN 12
#define MAVLINK_MSG_ID_40021_LEN 12
#define MAVLINK_MSG_ID_40021_MIN_LEN 12

#define MAVLINK_MSG_ID_MAX31855_EVENT_CRC 226
#define MAVLINK_MSG_ID_40021_CRC 226



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_MAX31855_EVENT { \
    40021, \
    "MAX31855_EVENT", \
    6, \
    {  { "thermocouple_temp", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_max31855_event_t, thermocouple_temp) }, \
         { "internal_temp", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_max31855_event_t, internal_temp) }, \
         { "fault", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_max31855_event_t, fault) }, \
         { "fault_short_vcc", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_max31855_event_t, fault_short_vcc) }, \
         { "fault_short_gnd", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_max31855_event_t, fault_short_gnd) }, \
         { "fault_open_tc", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_max31855_event_t, fault_open_tc) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_MAX31855_EVENT { \
    "MAX31855_EVENT", \
    6, \
    {  { "thermocouple_temp", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_max31855_event_t, thermocouple_temp) }, \
         { "internal_temp", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_max31855_event_t, internal_temp) }, \
         { "fault", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_max31855_event_t, fault) }, \
         { "fault_short_vcc", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_max31855_event_t, fault_short_vcc) }, \
         { "fault_short_gnd", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_max31855_event_t, fault_short_gnd) }, \
         { "fault_open_tc", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_max31855_event_t, fault_open_tc) }, \
         } \
}
#endif

/**
 * @brief Pack a max31855_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param thermocouple_temp [degC] Thermocouple temperature in Celsius
 * @param internal_temp [degC] Internal temperature in Celsius
 * @param fault  Fault detected
 * @param fault_short_vcc  Short to VCC detected
 * @param fault_short_gnd  Short to GND detected
 * @param fault_open_tc  Open thermocouple detected
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_max31855_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float thermocouple_temp, float internal_temp, uint8_t fault, uint8_t fault_short_vcc, uint8_t fault_short_gnd, uint8_t fault_open_tc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAX31855_EVENT_LEN];
    _mav_put_float(buf, 0, thermocouple_temp);
    _mav_put_float(buf, 4, internal_temp);
    _mav_put_uint8_t(buf, 8, fault);
    _mav_put_uint8_t(buf, 9, fault_short_vcc);
    _mav_put_uint8_t(buf, 10, fault_short_gnd);
    _mav_put_uint8_t(buf, 11, fault_open_tc);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAX31855_EVENT_LEN);
#else
    mavlink_max31855_event_t packet;
    packet.thermocouple_temp = thermocouple_temp;
    packet.internal_temp = internal_temp;
    packet.fault = fault;
    packet.fault_short_vcc = fault_short_vcc;
    packet.fault_short_gnd = fault_short_gnd;
    packet.fault_open_tc = fault_open_tc;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAX31855_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAX31855_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_CRC);
}

/**
 * @brief Pack a max31855_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param thermocouple_temp [degC] Thermocouple temperature in Celsius
 * @param internal_temp [degC] Internal temperature in Celsius
 * @param fault  Fault detected
 * @param fault_short_vcc  Short to VCC detected
 * @param fault_short_gnd  Short to GND detected
 * @param fault_open_tc  Open thermocouple detected
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_max31855_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               float thermocouple_temp, float internal_temp, uint8_t fault, uint8_t fault_short_vcc, uint8_t fault_short_gnd, uint8_t fault_open_tc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAX31855_EVENT_LEN];
    _mav_put_float(buf, 0, thermocouple_temp);
    _mav_put_float(buf, 4, internal_temp);
    _mav_put_uint8_t(buf, 8, fault);
    _mav_put_uint8_t(buf, 9, fault_short_vcc);
    _mav_put_uint8_t(buf, 10, fault_short_gnd);
    _mav_put_uint8_t(buf, 11, fault_open_tc);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAX31855_EVENT_LEN);
#else
    mavlink_max31855_event_t packet;
    packet.thermocouple_temp = thermocouple_temp;
    packet.internal_temp = internal_temp;
    packet.fault = fault;
    packet.fault_short_vcc = fault_short_vcc;
    packet.fault_short_gnd = fault_short_gnd;
    packet.fault_open_tc = fault_open_tc;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAX31855_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAX31855_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_LEN);
#endif
}

/**
 * @brief Pack a max31855_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param thermocouple_temp [degC] Thermocouple temperature in Celsius
 * @param internal_temp [degC] Internal temperature in Celsius
 * @param fault  Fault detected
 * @param fault_short_vcc  Short to VCC detected
 * @param fault_short_gnd  Short to GND detected
 * @param fault_open_tc  Open thermocouple detected
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_max31855_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float thermocouple_temp,float internal_temp,uint8_t fault,uint8_t fault_short_vcc,uint8_t fault_short_gnd,uint8_t fault_open_tc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAX31855_EVENT_LEN];
    _mav_put_float(buf, 0, thermocouple_temp);
    _mav_put_float(buf, 4, internal_temp);
    _mav_put_uint8_t(buf, 8, fault);
    _mav_put_uint8_t(buf, 9, fault_short_vcc);
    _mav_put_uint8_t(buf, 10, fault_short_gnd);
    _mav_put_uint8_t(buf, 11, fault_open_tc);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAX31855_EVENT_LEN);
#else
    mavlink_max31855_event_t packet;
    packet.thermocouple_temp = thermocouple_temp;
    packet.internal_temp = internal_temp;
    packet.fault = fault;
    packet.fault_short_vcc = fault_short_vcc;
    packet.fault_short_gnd = fault_short_gnd;
    packet.fault_open_tc = fault_open_tc;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAX31855_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_MAX31855_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_CRC);
}

/**
 * @brief Encode a max31855_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param max31855_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_max31855_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_max31855_event_t* max31855_event)
{
    return mavlink_msg_max31855_event_pack(system_id, component_id, msg, max31855_event->thermocouple_temp, max31855_event->internal_temp, max31855_event->fault, max31855_event->fault_short_vcc, max31855_event->fault_short_gnd, max31855_event->fault_open_tc);
}

/**
 * @brief Encode a max31855_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param max31855_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_max31855_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_max31855_event_t* max31855_event)
{
    return mavlink_msg_max31855_event_pack_chan(system_id, component_id, chan, msg, max31855_event->thermocouple_temp, max31855_event->internal_temp, max31855_event->fault, max31855_event->fault_short_vcc, max31855_event->fault_short_gnd, max31855_event->fault_open_tc);
}

/**
 * @brief Encode a max31855_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param max31855_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_max31855_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_max31855_event_t* max31855_event)
{
    return mavlink_msg_max31855_event_pack_status(system_id, component_id, _status, msg,  max31855_event->thermocouple_temp, max31855_event->internal_temp, max31855_event->fault, max31855_event->fault_short_vcc, max31855_event->fault_short_gnd, max31855_event->fault_open_tc);
}

/**
 * @brief Send a max31855_event message
 * @param chan MAVLink channel to send the message
 *
 * @param thermocouple_temp [degC] Thermocouple temperature in Celsius
 * @param internal_temp [degC] Internal temperature in Celsius
 * @param fault  Fault detected
 * @param fault_short_vcc  Short to VCC detected
 * @param fault_short_gnd  Short to GND detected
 * @param fault_open_tc  Open thermocouple detected
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_max31855_event_send(mavlink_channel_t chan, float thermocouple_temp, float internal_temp, uint8_t fault, uint8_t fault_short_vcc, uint8_t fault_short_gnd, uint8_t fault_open_tc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_MAX31855_EVENT_LEN];
    _mav_put_float(buf, 0, thermocouple_temp);
    _mav_put_float(buf, 4, internal_temp);
    _mav_put_uint8_t(buf, 8, fault);
    _mav_put_uint8_t(buf, 9, fault_short_vcc);
    _mav_put_uint8_t(buf, 10, fault_short_gnd);
    _mav_put_uint8_t(buf, 11, fault_open_tc);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAX31855_EVENT, buf, MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_CRC);
#else
    mavlink_max31855_event_t packet;
    packet.thermocouple_temp = thermocouple_temp;
    packet.internal_temp = internal_temp;
    packet.fault = fault;
    packet.fault_short_vcc = fault_short_vcc;
    packet.fault_short_gnd = fault_short_gnd;
    packet.fault_open_tc = fault_open_tc;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAX31855_EVENT, (const char *)&packet, MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_CRC);
#endif
}

/**
 * @brief Send a max31855_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_max31855_event_send_struct(mavlink_channel_t chan, const mavlink_max31855_event_t* max31855_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_max31855_event_send(chan, max31855_event->thermocouple_temp, max31855_event->internal_temp, max31855_event->fault, max31855_event->fault_short_vcc, max31855_event->fault_short_gnd, max31855_event->fault_open_tc);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAX31855_EVENT, (const char *)max31855_event, MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_MAX31855_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_max31855_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float thermocouple_temp, float internal_temp, uint8_t fault, uint8_t fault_short_vcc, uint8_t fault_short_gnd, uint8_t fault_open_tc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, thermocouple_temp);
    _mav_put_float(buf, 4, internal_temp);
    _mav_put_uint8_t(buf, 8, fault);
    _mav_put_uint8_t(buf, 9, fault_short_vcc);
    _mav_put_uint8_t(buf, 10, fault_short_gnd);
    _mav_put_uint8_t(buf, 11, fault_open_tc);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAX31855_EVENT, buf, MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_CRC);
#else
    mavlink_max31855_event_t *packet = (mavlink_max31855_event_t *)msgbuf;
    packet->thermocouple_temp = thermocouple_temp;
    packet->internal_temp = internal_temp;
    packet->fault = fault;
    packet->fault_short_vcc = fault_short_vcc;
    packet->fault_short_gnd = fault_short_gnd;
    packet->fault_open_tc = fault_open_tc;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAX31855_EVENT, (const char *)packet, MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_LEN, MAVLINK_MSG_ID_MAX31855_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE MAX31855_EVENT UNPACKING


/**
 * @brief Get field thermocouple_temp from max31855_event message
 *
 * @return [degC] Thermocouple temperature in Celsius
 */
static inline float mavlink_msg_max31855_event_get_thermocouple_temp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field internal_temp from max31855_event message
 *
 * @return [degC] Internal temperature in Celsius
 */
static inline float mavlink_msg_max31855_event_get_internal_temp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field fault from max31855_event message
 *
 * @return  Fault detected
 */
static inline uint8_t mavlink_msg_max31855_event_get_fault(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field fault_short_vcc from max31855_event message
 *
 * @return  Short to VCC detected
 */
static inline uint8_t mavlink_msg_max31855_event_get_fault_short_vcc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field fault_short_gnd from max31855_event message
 *
 * @return  Short to GND detected
 */
static inline uint8_t mavlink_msg_max31855_event_get_fault_short_gnd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field fault_open_tc from max31855_event message
 *
 * @return  Open thermocouple detected
 */
static inline uint8_t mavlink_msg_max31855_event_get_fault_open_tc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  11);
}

/**
 * @brief Decode a max31855_event message into a struct
 *
 * @param msg The message to decode
 * @param max31855_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_max31855_event_decode(const mavlink_message_t* msg, mavlink_max31855_event_t* max31855_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    max31855_event->thermocouple_temp = mavlink_msg_max31855_event_get_thermocouple_temp(msg);
    max31855_event->internal_temp = mavlink_msg_max31855_event_get_internal_temp(msg);
    max31855_event->fault = mavlink_msg_max31855_event_get_fault(msg);
    max31855_event->fault_short_vcc = mavlink_msg_max31855_event_get_fault_short_vcc(msg);
    max31855_event->fault_short_gnd = mavlink_msg_max31855_event_get_fault_short_gnd(msg);
    max31855_event->fault_open_tc = mavlink_msg_max31855_event_get_fault_open_tc(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_MAX31855_EVENT_LEN? msg->len : MAVLINK_MSG_ID_MAX31855_EVENT_LEN;
        memset(max31855_event, 0, MAVLINK_MSG_ID_MAX31855_EVENT_LEN);
    memcpy(max31855_event, _MAV_PAYLOAD(msg), len);
#endif
}
