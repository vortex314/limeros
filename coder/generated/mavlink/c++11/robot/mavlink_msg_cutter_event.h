#pragma once
// MESSAGE CUTTER_EVENT PACKING

#define MAVLINK_MSG_ID_CUTTER_EVENT 40007


typedef struct __mavlink_cutter_event_t {
 int32_t rpm; /*< [rpm] Current speed of the cutter in RPM*/
 float current; /*< [A] Current drawn by the cutter in Amperes*/
 float voltage; /*< [V] Voltage supplied to the cutter in Volts*/
 float temperature; /*< [degC] Temperature of the cutter in Celsius*/
 uint8_t enabled; /*<  Cutter enabled or disabled*/
} mavlink_cutter_event_t;

#define MAVLINK_MSG_ID_CUTTER_EVENT_LEN 17
#define MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN 17
#define MAVLINK_MSG_ID_40007_LEN 17
#define MAVLINK_MSG_ID_40007_MIN_LEN 17

#define MAVLINK_MSG_ID_CUTTER_EVENT_CRC 187
#define MAVLINK_MSG_ID_40007_CRC 187



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CUTTER_EVENT { \
    40007, \
    "CUTTER_EVENT", \
    5, \
    {  { "enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_cutter_event_t, enabled) }, \
         { "rpm", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_cutter_event_t, rpm) }, \
         { "current", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_cutter_event_t, current) }, \
         { "voltage", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_cutter_event_t, voltage) }, \
         { "temperature", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_cutter_event_t, temperature) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CUTTER_EVENT { \
    "CUTTER_EVENT", \
    5, \
    {  { "enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_cutter_event_t, enabled) }, \
         { "rpm", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_cutter_event_t, rpm) }, \
         { "current", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_cutter_event_t, current) }, \
         { "voltage", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_cutter_event_t, voltage) }, \
         { "temperature", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_cutter_event_t, temperature) }, \
         } \
}
#endif

/**
 * @brief Pack a cutter_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param enabled  Cutter enabled or disabled
 * @param rpm [rpm] Current speed of the cutter in RPM
 * @param current [A] Current drawn by the cutter in Amperes
 * @param voltage [V] Voltage supplied to the cutter in Volts
 * @param temperature [degC] Temperature of the cutter in Celsius
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cutter_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t enabled, int32_t rpm, float current, float voltage, float temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_EVENT_LEN];
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_float(buf, 4, current);
    _mav_put_float(buf, 8, voltage);
    _mav_put_float(buf, 12, temperature);
    _mav_put_uint8_t(buf, 16, enabled);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUTTER_EVENT_LEN);
#else
    mavlink_cutter_event_t packet;
    packet.rpm = rpm;
    packet.current = current;
    packet.voltage = voltage;
    packet.temperature = temperature;
    packet.enabled = enabled;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUTTER_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUTTER_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_CRC);
}

/**
 * @brief Pack a cutter_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param enabled  Cutter enabled or disabled
 * @param rpm [rpm] Current speed of the cutter in RPM
 * @param current [A] Current drawn by the cutter in Amperes
 * @param voltage [V] Voltage supplied to the cutter in Volts
 * @param temperature [degC] Temperature of the cutter in Celsius
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cutter_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t enabled, int32_t rpm, float current, float voltage, float temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_EVENT_LEN];
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_float(buf, 4, current);
    _mav_put_float(buf, 8, voltage);
    _mav_put_float(buf, 12, temperature);
    _mav_put_uint8_t(buf, 16, enabled);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUTTER_EVENT_LEN);
#else
    mavlink_cutter_event_t packet;
    packet.rpm = rpm;
    packet.current = current;
    packet.voltage = voltage;
    packet.temperature = temperature;
    packet.enabled = enabled;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUTTER_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUTTER_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_LEN);
#endif
}

/**
 * @brief Pack a cutter_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param enabled  Cutter enabled or disabled
 * @param rpm [rpm] Current speed of the cutter in RPM
 * @param current [A] Current drawn by the cutter in Amperes
 * @param voltage [V] Voltage supplied to the cutter in Volts
 * @param temperature [degC] Temperature of the cutter in Celsius
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cutter_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t enabled,int32_t rpm,float current,float voltage,float temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_EVENT_LEN];
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_float(buf, 4, current);
    _mav_put_float(buf, 8, voltage);
    _mav_put_float(buf, 12, temperature);
    _mav_put_uint8_t(buf, 16, enabled);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUTTER_EVENT_LEN);
#else
    mavlink_cutter_event_t packet;
    packet.rpm = rpm;
    packet.current = current;
    packet.voltage = voltage;
    packet.temperature = temperature;
    packet.enabled = enabled;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUTTER_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUTTER_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_CRC);
}

/**
 * @brief Encode a cutter_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param cutter_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cutter_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_cutter_event_t* cutter_event)
{
    return mavlink_msg_cutter_event_pack(system_id, component_id, msg, cutter_event->enabled, cutter_event->rpm, cutter_event->current, cutter_event->voltage, cutter_event->temperature);
}

/**
 * @brief Encode a cutter_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cutter_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cutter_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_cutter_event_t* cutter_event)
{
    return mavlink_msg_cutter_event_pack_chan(system_id, component_id, chan, msg, cutter_event->enabled, cutter_event->rpm, cutter_event->current, cutter_event->voltage, cutter_event->temperature);
}

/**
 * @brief Encode a cutter_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param cutter_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cutter_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_cutter_event_t* cutter_event)
{
    return mavlink_msg_cutter_event_pack_status(system_id, component_id, _status, msg,  cutter_event->enabled, cutter_event->rpm, cutter_event->current, cutter_event->voltage, cutter_event->temperature);
}

/**
 * @brief Send a cutter_event message
 * @param chan MAVLink channel to send the message
 *
 * @param enabled  Cutter enabled or disabled
 * @param rpm [rpm] Current speed of the cutter in RPM
 * @param current [A] Current drawn by the cutter in Amperes
 * @param voltage [V] Voltage supplied to the cutter in Volts
 * @param temperature [degC] Temperature of the cutter in Celsius
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_cutter_event_send(mavlink_channel_t chan, uint8_t enabled, int32_t rpm, float current, float voltage, float temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUTTER_EVENT_LEN];
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_float(buf, 4, current);
    _mav_put_float(buf, 8, voltage);
    _mav_put_float(buf, 12, temperature);
    _mav_put_uint8_t(buf, 16, enabled);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_EVENT, buf, MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_CRC);
#else
    mavlink_cutter_event_t packet;
    packet.rpm = rpm;
    packet.current = current;
    packet.voltage = voltage;
    packet.temperature = temperature;
    packet.enabled = enabled;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_EVENT, (const char *)&packet, MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_CRC);
#endif
}

/**
 * @brief Send a cutter_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_cutter_event_send_struct(mavlink_channel_t chan, const mavlink_cutter_event_t* cutter_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_cutter_event_send(chan, cutter_event->enabled, cutter_event->rpm, cutter_event->current, cutter_event->voltage, cutter_event->temperature);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_EVENT, (const char *)cutter_event, MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_CUTTER_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_cutter_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t enabled, int32_t rpm, float current, float voltage, float temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, rpm);
    _mav_put_float(buf, 4, current);
    _mav_put_float(buf, 8, voltage);
    _mav_put_float(buf, 12, temperature);
    _mav_put_uint8_t(buf, 16, enabled);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_EVENT, buf, MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_CRC);
#else
    mavlink_cutter_event_t *packet = (mavlink_cutter_event_t *)msgbuf;
    packet->rpm = rpm;
    packet->current = current;
    packet->voltage = voltage;
    packet->temperature = temperature;
    packet->enabled = enabled;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUTTER_EVENT, (const char *)packet, MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_LEN, MAVLINK_MSG_ID_CUTTER_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE CUTTER_EVENT UNPACKING


/**
 * @brief Get field enabled from cutter_event message
 *
 * @return  Cutter enabled or disabled
 */
static inline uint8_t mavlink_msg_cutter_event_get_enabled(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Get field rpm from cutter_event message
 *
 * @return [rpm] Current speed of the cutter in RPM
 */
static inline int32_t mavlink_msg_cutter_event_get_rpm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field current from cutter_event message
 *
 * @return [A] Current drawn by the cutter in Amperes
 */
static inline float mavlink_msg_cutter_event_get_current(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field voltage from cutter_event message
 *
 * @return [V] Voltage supplied to the cutter in Volts
 */
static inline float mavlink_msg_cutter_event_get_voltage(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field temperature from cutter_event message
 *
 * @return [degC] Temperature of the cutter in Celsius
 */
static inline float mavlink_msg_cutter_event_get_temperature(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Decode a cutter_event message into a struct
 *
 * @param msg The message to decode
 * @param cutter_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_cutter_event_decode(const mavlink_message_t* msg, mavlink_cutter_event_t* cutter_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    cutter_event->rpm = mavlink_msg_cutter_event_get_rpm(msg);
    cutter_event->current = mavlink_msg_cutter_event_get_current(msg);
    cutter_event->voltage = mavlink_msg_cutter_event_get_voltage(msg);
    cutter_event->temperature = mavlink_msg_cutter_event_get_temperature(msg);
    cutter_event->enabled = mavlink_msg_cutter_event_get_enabled(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CUTTER_EVENT_LEN? msg->len : MAVLINK_MSG_ID_CUTTER_EVENT_LEN;
        memset(cutter_event, 0, MAVLINK_MSG_ID_CUTTER_EVENT_LEN);
    memcpy(cutter_event, _MAV_PAYLOAD(msg), len);
#endif
}
