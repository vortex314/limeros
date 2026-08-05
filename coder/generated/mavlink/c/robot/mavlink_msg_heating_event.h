#pragma once
// MESSAGE HEATING_EVENT PACKING

#define MAVLINK_MSG_ID_HEATING_EVENT 40022


typedef struct __mavlink_heating_event_t {
 uint64_t timestamp_ms; /*< [ms] Timestamp in milliseconds since epoch*/
 float temperature_c; /*< [degC] Current temperature in Celsius*/
 float setpoint_c; /*< [degC] Setpoint temperature in Celsius*/
 float output_pct; /*< [%] Output percentage of the heating element*/
 uint8_t enabled; /*<  Heating enabled or disabled*/
 uint8_t heater_on; /*<  Heater is currently on or off*/
 uint8_t fault; /*<  Fault detected in the heating system*/
} mavlink_heating_event_t;

#define MAVLINK_MSG_ID_HEATING_EVENT_LEN 23
#define MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN 23
#define MAVLINK_MSG_ID_40022_LEN 23
#define MAVLINK_MSG_ID_40022_MIN_LEN 23

#define MAVLINK_MSG_ID_HEATING_EVENT_CRC 82
#define MAVLINK_MSG_ID_40022_CRC 82



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_HEATING_EVENT { \
    40022, \
    "HEATING_EVENT", \
    7, \
    {  { "temperature_c", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_heating_event_t, temperature_c) }, \
         { "setpoint_c", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_heating_event_t, setpoint_c) }, \
         { "enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_heating_event_t, enabled) }, \
         { "output_pct", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_heating_event_t, output_pct) }, \
         { "heater_on", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_heating_event_t, heater_on) }, \
         { "fault", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_heating_event_t, fault) }, \
         { "timestamp_ms", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_heating_event_t, timestamp_ms) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_HEATING_EVENT { \
    "HEATING_EVENT", \
    7, \
    {  { "temperature_c", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_heating_event_t, temperature_c) }, \
         { "setpoint_c", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_heating_event_t, setpoint_c) }, \
         { "enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_heating_event_t, enabled) }, \
         { "output_pct", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_heating_event_t, output_pct) }, \
         { "heater_on", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_heating_event_t, heater_on) }, \
         { "fault", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_heating_event_t, fault) }, \
         { "timestamp_ms", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_heating_event_t, timestamp_ms) }, \
         } \
}
#endif

/**
 * @brief Pack a heating_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param temperature_c [degC] Current temperature in Celsius
 * @param setpoint_c [degC] Setpoint temperature in Celsius
 * @param enabled  Heating enabled or disabled
 * @param output_pct [%] Output percentage of the heating element
 * @param heater_on  Heater is currently on or off
 * @param fault  Fault detected in the heating system
 * @param timestamp_ms [ms] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_heating_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float temperature_c, float setpoint_c, uint8_t enabled, float output_pct, uint8_t heater_on, uint8_t fault, uint64_t timestamp_ms)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HEATING_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, timestamp_ms);
    _mav_put_float(buf, 8, temperature_c);
    _mav_put_float(buf, 12, setpoint_c);
    _mav_put_float(buf, 16, output_pct);
    _mav_put_uint8_t(buf, 20, enabled);
    _mav_put_uint8_t(buf, 21, heater_on);
    _mav_put_uint8_t(buf, 22, fault);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HEATING_EVENT_LEN);
#else
    mavlink_heating_event_t packet;
    packet.timestamp_ms = timestamp_ms;
    packet.temperature_c = temperature_c;
    packet.setpoint_c = setpoint_c;
    packet.output_pct = output_pct;
    packet.enabled = enabled;
    packet.heater_on = heater_on;
    packet.fault = fault;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HEATING_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HEATING_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN, MAVLINK_MSG_ID_HEATING_EVENT_LEN, MAVLINK_MSG_ID_HEATING_EVENT_CRC);
}

/**
 * @brief Pack a heating_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param temperature_c [degC] Current temperature in Celsius
 * @param setpoint_c [degC] Setpoint temperature in Celsius
 * @param enabled  Heating enabled or disabled
 * @param output_pct [%] Output percentage of the heating element
 * @param heater_on  Heater is currently on or off
 * @param fault  Fault detected in the heating system
 * @param timestamp_ms [ms] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_heating_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               float temperature_c, float setpoint_c, uint8_t enabled, float output_pct, uint8_t heater_on, uint8_t fault, uint64_t timestamp_ms)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HEATING_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, timestamp_ms);
    _mav_put_float(buf, 8, temperature_c);
    _mav_put_float(buf, 12, setpoint_c);
    _mav_put_float(buf, 16, output_pct);
    _mav_put_uint8_t(buf, 20, enabled);
    _mav_put_uint8_t(buf, 21, heater_on);
    _mav_put_uint8_t(buf, 22, fault);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HEATING_EVENT_LEN);
#else
    mavlink_heating_event_t packet;
    packet.timestamp_ms = timestamp_ms;
    packet.temperature_c = temperature_c;
    packet.setpoint_c = setpoint_c;
    packet.output_pct = output_pct;
    packet.enabled = enabled;
    packet.heater_on = heater_on;
    packet.fault = fault;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HEATING_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HEATING_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN, MAVLINK_MSG_ID_HEATING_EVENT_LEN, MAVLINK_MSG_ID_HEATING_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN, MAVLINK_MSG_ID_HEATING_EVENT_LEN);
#endif
}

/**
 * @brief Pack a heating_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param temperature_c [degC] Current temperature in Celsius
 * @param setpoint_c [degC] Setpoint temperature in Celsius
 * @param enabled  Heating enabled or disabled
 * @param output_pct [%] Output percentage of the heating element
 * @param heater_on  Heater is currently on or off
 * @param fault  Fault detected in the heating system
 * @param timestamp_ms [ms] Timestamp in milliseconds since epoch
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_heating_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float temperature_c,float setpoint_c,uint8_t enabled,float output_pct,uint8_t heater_on,uint8_t fault,uint64_t timestamp_ms)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HEATING_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, timestamp_ms);
    _mav_put_float(buf, 8, temperature_c);
    _mav_put_float(buf, 12, setpoint_c);
    _mav_put_float(buf, 16, output_pct);
    _mav_put_uint8_t(buf, 20, enabled);
    _mav_put_uint8_t(buf, 21, heater_on);
    _mav_put_uint8_t(buf, 22, fault);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HEATING_EVENT_LEN);
#else
    mavlink_heating_event_t packet;
    packet.timestamp_ms = timestamp_ms;
    packet.temperature_c = temperature_c;
    packet.setpoint_c = setpoint_c;
    packet.output_pct = output_pct;
    packet.enabled = enabled;
    packet.heater_on = heater_on;
    packet.fault = fault;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HEATING_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HEATING_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN, MAVLINK_MSG_ID_HEATING_EVENT_LEN, MAVLINK_MSG_ID_HEATING_EVENT_CRC);
}

/**
 * @brief Encode a heating_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param heating_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_heating_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_heating_event_t* heating_event)
{
    return mavlink_msg_heating_event_pack(system_id, component_id, msg, heating_event->temperature_c, heating_event->setpoint_c, heating_event->enabled, heating_event->output_pct, heating_event->heater_on, heating_event->fault, heating_event->timestamp_ms);
}

/**
 * @brief Encode a heating_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param heating_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_heating_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_heating_event_t* heating_event)
{
    return mavlink_msg_heating_event_pack_chan(system_id, component_id, chan, msg, heating_event->temperature_c, heating_event->setpoint_c, heating_event->enabled, heating_event->output_pct, heating_event->heater_on, heating_event->fault, heating_event->timestamp_ms);
}

/**
 * @brief Encode a heating_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param heating_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_heating_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_heating_event_t* heating_event)
{
    return mavlink_msg_heating_event_pack_status(system_id, component_id, _status, msg,  heating_event->temperature_c, heating_event->setpoint_c, heating_event->enabled, heating_event->output_pct, heating_event->heater_on, heating_event->fault, heating_event->timestamp_ms);
}

/**
 * @brief Send a heating_event message
 * @param chan MAVLink channel to send the message
 *
 * @param temperature_c [degC] Current temperature in Celsius
 * @param setpoint_c [degC] Setpoint temperature in Celsius
 * @param enabled  Heating enabled or disabled
 * @param output_pct [%] Output percentage of the heating element
 * @param heater_on  Heater is currently on or off
 * @param fault  Fault detected in the heating system
 * @param timestamp_ms [ms] Timestamp in milliseconds since epoch
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_heating_event_send(mavlink_channel_t chan, float temperature_c, float setpoint_c, uint8_t enabled, float output_pct, uint8_t heater_on, uint8_t fault, uint64_t timestamp_ms)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HEATING_EVENT_LEN];
    _mav_put_uint64_t(buf, 0, timestamp_ms);
    _mav_put_float(buf, 8, temperature_c);
    _mav_put_float(buf, 12, setpoint_c);
    _mav_put_float(buf, 16, output_pct);
    _mav_put_uint8_t(buf, 20, enabled);
    _mav_put_uint8_t(buf, 21, heater_on);
    _mav_put_uint8_t(buf, 22, fault);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_EVENT, buf, MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN, MAVLINK_MSG_ID_HEATING_EVENT_LEN, MAVLINK_MSG_ID_HEATING_EVENT_CRC);
#else
    mavlink_heating_event_t packet;
    packet.timestamp_ms = timestamp_ms;
    packet.temperature_c = temperature_c;
    packet.setpoint_c = setpoint_c;
    packet.output_pct = output_pct;
    packet.enabled = enabled;
    packet.heater_on = heater_on;
    packet.fault = fault;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_EVENT, (const char *)&packet, MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN, MAVLINK_MSG_ID_HEATING_EVENT_LEN, MAVLINK_MSG_ID_HEATING_EVENT_CRC);
#endif
}

/**
 * @brief Send a heating_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_heating_event_send_struct(mavlink_channel_t chan, const mavlink_heating_event_t* heating_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_heating_event_send(chan, heating_event->temperature_c, heating_event->setpoint_c, heating_event->enabled, heating_event->output_pct, heating_event->heater_on, heating_event->fault, heating_event->timestamp_ms);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_EVENT, (const char *)heating_event, MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN, MAVLINK_MSG_ID_HEATING_EVENT_LEN, MAVLINK_MSG_ID_HEATING_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_HEATING_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_heating_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float temperature_c, float setpoint_c, uint8_t enabled, float output_pct, uint8_t heater_on, uint8_t fault, uint64_t timestamp_ms)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, timestamp_ms);
    _mav_put_float(buf, 8, temperature_c);
    _mav_put_float(buf, 12, setpoint_c);
    _mav_put_float(buf, 16, output_pct);
    _mav_put_uint8_t(buf, 20, enabled);
    _mav_put_uint8_t(buf, 21, heater_on);
    _mav_put_uint8_t(buf, 22, fault);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_EVENT, buf, MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN, MAVLINK_MSG_ID_HEATING_EVENT_LEN, MAVLINK_MSG_ID_HEATING_EVENT_CRC);
#else
    mavlink_heating_event_t *packet = (mavlink_heating_event_t *)msgbuf;
    packet->timestamp_ms = timestamp_ms;
    packet->temperature_c = temperature_c;
    packet->setpoint_c = setpoint_c;
    packet->output_pct = output_pct;
    packet->enabled = enabled;
    packet->heater_on = heater_on;
    packet->fault = fault;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_EVENT, (const char *)packet, MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN, MAVLINK_MSG_ID_HEATING_EVENT_LEN, MAVLINK_MSG_ID_HEATING_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE HEATING_EVENT UNPACKING


/**
 * @brief Get field temperature_c from heating_event message
 *
 * @return [degC] Current temperature in Celsius
 */
static inline float mavlink_msg_heating_event_get_temperature_c(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field setpoint_c from heating_event message
 *
 * @return [degC] Setpoint temperature in Celsius
 */
static inline float mavlink_msg_heating_event_get_setpoint_c(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field enabled from heating_event message
 *
 * @return  Heating enabled or disabled
 */
static inline uint8_t mavlink_msg_heating_event_get_enabled(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field output_pct from heating_event message
 *
 * @return [%] Output percentage of the heating element
 */
static inline float mavlink_msg_heating_event_get_output_pct(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field heater_on from heating_event message
 *
 * @return  Heater is currently on or off
 */
static inline uint8_t mavlink_msg_heating_event_get_heater_on(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  21);
}

/**
 * @brief Get field fault from heating_event message
 *
 * @return  Fault detected in the heating system
 */
static inline uint8_t mavlink_msg_heating_event_get_fault(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  22);
}

/**
 * @brief Get field timestamp_ms from heating_event message
 *
 * @return [ms] Timestamp in milliseconds since epoch
 */
static inline uint64_t mavlink_msg_heating_event_get_timestamp_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Decode a heating_event message into a struct
 *
 * @param msg The message to decode
 * @param heating_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_heating_event_decode(const mavlink_message_t* msg, mavlink_heating_event_t* heating_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    heating_event->timestamp_ms = mavlink_msg_heating_event_get_timestamp_ms(msg);
    heating_event->temperature_c = mavlink_msg_heating_event_get_temperature_c(msg);
    heating_event->setpoint_c = mavlink_msg_heating_event_get_setpoint_c(msg);
    heating_event->output_pct = mavlink_msg_heating_event_get_output_pct(msg);
    heating_event->enabled = mavlink_msg_heating_event_get_enabled(msg);
    heating_event->heater_on = mavlink_msg_heating_event_get_heater_on(msg);
    heating_event->fault = mavlink_msg_heating_event_get_fault(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_HEATING_EVENT_LEN? msg->len : MAVLINK_MSG_ID_HEATING_EVENT_LEN;
        memset(heating_event, 0, MAVLINK_MSG_ID_HEATING_EVENT_LEN);
    memcpy(heating_event, _MAV_PAYLOAD(msg), len);
#endif
}
