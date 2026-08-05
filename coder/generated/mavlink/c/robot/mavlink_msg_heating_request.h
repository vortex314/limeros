#pragma once
// MESSAGE HEATING_REQUEST PACKING

#define MAVLINK_MSG_ID_HEATING_REQUEST 40023


typedef struct __mavlink_heating_request_t {
 float setpoint_c; /*< [degC] Setpoint temperature in Celsius*/
 float kp; /*<  Proportional gain for PID controller*/
 float ki; /*<  Integral gain for PID controller*/
 float kd; /*<  Derivative gain for PID controller*/
 uint8_t enabled; /*<  Enable or disable heating*/
 uint8_t reset_integral; /*<  Reset the integral term of the PID controller*/
} mavlink_heating_request_t;

#define MAVLINK_MSG_ID_HEATING_REQUEST_LEN 18
#define MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN 18
#define MAVLINK_MSG_ID_40023_LEN 18
#define MAVLINK_MSG_ID_40023_MIN_LEN 18

#define MAVLINK_MSG_ID_HEATING_REQUEST_CRC 74
#define MAVLINK_MSG_ID_40023_CRC 74



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_HEATING_REQUEST { \
    40023, \
    "HEATING_REQUEST", \
    6, \
    {  { "setpoint_c", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_heating_request_t, setpoint_c) }, \
         { "enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_heating_request_t, enabled) }, \
         { "kp", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_heating_request_t, kp) }, \
         { "ki", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_heating_request_t, ki) }, \
         { "kd", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_heating_request_t, kd) }, \
         { "reset_integral", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_heating_request_t, reset_integral) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_HEATING_REQUEST { \
    "HEATING_REQUEST", \
    6, \
    {  { "setpoint_c", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_heating_request_t, setpoint_c) }, \
         { "enabled", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_heating_request_t, enabled) }, \
         { "kp", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_heating_request_t, kp) }, \
         { "ki", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_heating_request_t, ki) }, \
         { "kd", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_heating_request_t, kd) }, \
         { "reset_integral", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_heating_request_t, reset_integral) }, \
         } \
}
#endif

/**
 * @brief Pack a heating_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param setpoint_c [degC] Setpoint temperature in Celsius
 * @param enabled  Enable or disable heating
 * @param kp  Proportional gain for PID controller
 * @param ki  Integral gain for PID controller
 * @param kd  Derivative gain for PID controller
 * @param reset_integral  Reset the integral term of the PID controller
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_heating_request_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float setpoint_c, uint8_t enabled, float kp, float ki, float kd, uint8_t reset_integral)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HEATING_REQUEST_LEN];
    _mav_put_float(buf, 0, setpoint_c);
    _mav_put_float(buf, 4, kp);
    _mav_put_float(buf, 8, ki);
    _mav_put_float(buf, 12, kd);
    _mav_put_uint8_t(buf, 16, enabled);
    _mav_put_uint8_t(buf, 17, reset_integral);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HEATING_REQUEST_LEN);
#else
    mavlink_heating_request_t packet;
    packet.setpoint_c = setpoint_c;
    packet.kp = kp;
    packet.ki = ki;
    packet.kd = kd;
    packet.enabled = enabled;
    packet.reset_integral = reset_integral;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HEATING_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HEATING_REQUEST;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_CRC);
}

/**
 * @brief Pack a heating_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param setpoint_c [degC] Setpoint temperature in Celsius
 * @param enabled  Enable or disable heating
 * @param kp  Proportional gain for PID controller
 * @param ki  Integral gain for PID controller
 * @param kd  Derivative gain for PID controller
 * @param reset_integral  Reset the integral term of the PID controller
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_heating_request_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               float setpoint_c, uint8_t enabled, float kp, float ki, float kd, uint8_t reset_integral)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HEATING_REQUEST_LEN];
    _mav_put_float(buf, 0, setpoint_c);
    _mav_put_float(buf, 4, kp);
    _mav_put_float(buf, 8, ki);
    _mav_put_float(buf, 12, kd);
    _mav_put_uint8_t(buf, 16, enabled);
    _mav_put_uint8_t(buf, 17, reset_integral);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HEATING_REQUEST_LEN);
#else
    mavlink_heating_request_t packet;
    packet.setpoint_c = setpoint_c;
    packet.kp = kp;
    packet.ki = ki;
    packet.kd = kd;
    packet.enabled = enabled;
    packet.reset_integral = reset_integral;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HEATING_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HEATING_REQUEST;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_LEN);
#endif
}

/**
 * @brief Pack a heating_request message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param setpoint_c [degC] Setpoint temperature in Celsius
 * @param enabled  Enable or disable heating
 * @param kp  Proportional gain for PID controller
 * @param ki  Integral gain for PID controller
 * @param kd  Derivative gain for PID controller
 * @param reset_integral  Reset the integral term of the PID controller
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_heating_request_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float setpoint_c,uint8_t enabled,float kp,float ki,float kd,uint8_t reset_integral)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HEATING_REQUEST_LEN];
    _mav_put_float(buf, 0, setpoint_c);
    _mav_put_float(buf, 4, kp);
    _mav_put_float(buf, 8, ki);
    _mav_put_float(buf, 12, kd);
    _mav_put_uint8_t(buf, 16, enabled);
    _mav_put_uint8_t(buf, 17, reset_integral);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HEATING_REQUEST_LEN);
#else
    mavlink_heating_request_t packet;
    packet.setpoint_c = setpoint_c;
    packet.kp = kp;
    packet.ki = ki;
    packet.kd = kd;
    packet.enabled = enabled;
    packet.reset_integral = reset_integral;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HEATING_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HEATING_REQUEST;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_CRC);
}

/**
 * @brief Encode a heating_request struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param heating_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_heating_request_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_heating_request_t* heating_request)
{
    return mavlink_msg_heating_request_pack(system_id, component_id, msg, heating_request->setpoint_c, heating_request->enabled, heating_request->kp, heating_request->ki, heating_request->kd, heating_request->reset_integral);
}

/**
 * @brief Encode a heating_request struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param heating_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_heating_request_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_heating_request_t* heating_request)
{
    return mavlink_msg_heating_request_pack_chan(system_id, component_id, chan, msg, heating_request->setpoint_c, heating_request->enabled, heating_request->kp, heating_request->ki, heating_request->kd, heating_request->reset_integral);
}

/**
 * @brief Encode a heating_request struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param heating_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_heating_request_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_heating_request_t* heating_request)
{
    return mavlink_msg_heating_request_pack_status(system_id, component_id, _status, msg,  heating_request->setpoint_c, heating_request->enabled, heating_request->kp, heating_request->ki, heating_request->kd, heating_request->reset_integral);
}

/**
 * @brief Send a heating_request message
 * @param chan MAVLink channel to send the message
 *
 * @param setpoint_c [degC] Setpoint temperature in Celsius
 * @param enabled  Enable or disable heating
 * @param kp  Proportional gain for PID controller
 * @param ki  Integral gain for PID controller
 * @param kd  Derivative gain for PID controller
 * @param reset_integral  Reset the integral term of the PID controller
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_heating_request_send(mavlink_channel_t chan, float setpoint_c, uint8_t enabled, float kp, float ki, float kd, uint8_t reset_integral)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HEATING_REQUEST_LEN];
    _mav_put_float(buf, 0, setpoint_c);
    _mav_put_float(buf, 4, kp);
    _mav_put_float(buf, 8, ki);
    _mav_put_float(buf, 12, kd);
    _mav_put_uint8_t(buf, 16, enabled);
    _mav_put_uint8_t(buf, 17, reset_integral);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_REQUEST, buf, MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_CRC);
#else
    mavlink_heating_request_t packet;
    packet.setpoint_c = setpoint_c;
    packet.kp = kp;
    packet.ki = ki;
    packet.kd = kd;
    packet.enabled = enabled;
    packet.reset_integral = reset_integral;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_REQUEST, (const char *)&packet, MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_CRC);
#endif
}

/**
 * @brief Send a heating_request message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_heating_request_send_struct(mavlink_channel_t chan, const mavlink_heating_request_t* heating_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_heating_request_send(chan, heating_request->setpoint_c, heating_request->enabled, heating_request->kp, heating_request->ki, heating_request->kd, heating_request->reset_integral);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_REQUEST, (const char *)heating_request, MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_CRC);
#endif
}

#if MAVLINK_MSG_ID_HEATING_REQUEST_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_heating_request_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float setpoint_c, uint8_t enabled, float kp, float ki, float kd, uint8_t reset_integral)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, setpoint_c);
    _mav_put_float(buf, 4, kp);
    _mav_put_float(buf, 8, ki);
    _mav_put_float(buf, 12, kd);
    _mav_put_uint8_t(buf, 16, enabled);
    _mav_put_uint8_t(buf, 17, reset_integral);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_REQUEST, buf, MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_CRC);
#else
    mavlink_heating_request_t *packet = (mavlink_heating_request_t *)msgbuf;
    packet->setpoint_c = setpoint_c;
    packet->kp = kp;
    packet->ki = ki;
    packet->kd = kd;
    packet->enabled = enabled;
    packet->reset_integral = reset_integral;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HEATING_REQUEST, (const char *)packet, MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_LEN, MAVLINK_MSG_ID_HEATING_REQUEST_CRC);
#endif
}
#endif

#endif

// MESSAGE HEATING_REQUEST UNPACKING


/**
 * @brief Get field setpoint_c from heating_request message
 *
 * @return [degC] Setpoint temperature in Celsius
 */
static inline float mavlink_msg_heating_request_get_setpoint_c(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field enabled from heating_request message
 *
 * @return  Enable or disable heating
 */
static inline uint8_t mavlink_msg_heating_request_get_enabled(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Get field kp from heating_request message
 *
 * @return  Proportional gain for PID controller
 */
static inline float mavlink_msg_heating_request_get_kp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field ki from heating_request message
 *
 * @return  Integral gain for PID controller
 */
static inline float mavlink_msg_heating_request_get_ki(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field kd from heating_request message
 *
 * @return  Derivative gain for PID controller
 */
static inline float mavlink_msg_heating_request_get_kd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field reset_integral from heating_request message
 *
 * @return  Reset the integral term of the PID controller
 */
static inline uint8_t mavlink_msg_heating_request_get_reset_integral(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  17);
}

/**
 * @brief Decode a heating_request message into a struct
 *
 * @param msg The message to decode
 * @param heating_request C-struct to decode the message contents into
 */
static inline void mavlink_msg_heating_request_decode(const mavlink_message_t* msg, mavlink_heating_request_t* heating_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    heating_request->setpoint_c = mavlink_msg_heating_request_get_setpoint_c(msg);
    heating_request->kp = mavlink_msg_heating_request_get_kp(msg);
    heating_request->ki = mavlink_msg_heating_request_get_ki(msg);
    heating_request->kd = mavlink_msg_heating_request_get_kd(msg);
    heating_request->enabled = mavlink_msg_heating_request_get_enabled(msg);
    heating_request->reset_integral = mavlink_msg_heating_request_get_reset_integral(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_HEATING_REQUEST_LEN? msg->len : MAVLINK_MSG_ID_HEATING_REQUEST_LEN;
        memset(heating_request, 0, MAVLINK_MSG_ID_HEATING_REQUEST_LEN);
    memcpy(heating_request, _MAV_PAYLOAD(msg), len);
#endif
}
