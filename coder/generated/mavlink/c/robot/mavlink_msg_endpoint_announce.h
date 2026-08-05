#pragma once
// MESSAGE ENDPOINT_ANNOUNCE PACKING

#define MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE 40025


typedef struct __mavlink_endpoint_announce_t {
 uint32_t id; /*<  Unique identifier for the announcing endpoint*/
 uint32_t services[16]; /*<  List of services provided by the endpoint*/
 uint32_t events[16]; /*<  List of events emitted by the endpoint*/
 uint32_t replies[16]; /*<  List of replies supported by the endpoint*/
 uint32_t subscribes[16]; /*<  List of subscriptions for the endpoint*/
 char name[50]; /*<  Name of the announcing endpoint*/
 char description[100]; /*<  Description of the announcing endpoint*/
} mavlink_endpoint_announce_t;

#define MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN 410
#define MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN 410
#define MAVLINK_MSG_ID_40025_LEN 410
#define MAVLINK_MSG_ID_40025_MIN_LEN 410

#define MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_CRC 211
#define MAVLINK_MSG_ID_40025_CRC 211

#define MAVLINK_MSG_ENDPOINT_ANNOUNCE_FIELD_SERVICES_LEN 16
#define MAVLINK_MSG_ENDPOINT_ANNOUNCE_FIELD_EVENTS_LEN 16
#define MAVLINK_MSG_ENDPOINT_ANNOUNCE_FIELD_REPLIES_LEN 16
#define MAVLINK_MSG_ENDPOINT_ANNOUNCE_FIELD_SUBSCRIBES_LEN 16
#define MAVLINK_MSG_ENDPOINT_ANNOUNCE_FIELD_NAME_LEN 50
#define MAVLINK_MSG_ENDPOINT_ANNOUNCE_FIELD_DESCRIPTION_LEN 100

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ENDPOINT_ANNOUNCE { \
    40025, \
    "ENDPOINT_ANNOUNCE", \
    7, \
    {  { "id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_endpoint_announce_t, id) }, \
         { "name", NULL, MAVLINK_TYPE_CHAR, 50, 260, offsetof(mavlink_endpoint_announce_t, name) }, \
         { "description", NULL, MAVLINK_TYPE_CHAR, 100, 310, offsetof(mavlink_endpoint_announce_t, description) }, \
         { "services", NULL, MAVLINK_TYPE_UINT32_T, 16, 4, offsetof(mavlink_endpoint_announce_t, services) }, \
         { "events", NULL, MAVLINK_TYPE_UINT32_T, 16, 68, offsetof(mavlink_endpoint_announce_t, events) }, \
         { "replies", NULL, MAVLINK_TYPE_UINT32_T, 16, 132, offsetof(mavlink_endpoint_announce_t, replies) }, \
         { "subscribes", NULL, MAVLINK_TYPE_UINT32_T, 16, 196, offsetof(mavlink_endpoint_announce_t, subscribes) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ENDPOINT_ANNOUNCE { \
    "ENDPOINT_ANNOUNCE", \
    7, \
    {  { "id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_endpoint_announce_t, id) }, \
         { "name", NULL, MAVLINK_TYPE_CHAR, 50, 260, offsetof(mavlink_endpoint_announce_t, name) }, \
         { "description", NULL, MAVLINK_TYPE_CHAR, 100, 310, offsetof(mavlink_endpoint_announce_t, description) }, \
         { "services", NULL, MAVLINK_TYPE_UINT32_T, 16, 4, offsetof(mavlink_endpoint_announce_t, services) }, \
         { "events", NULL, MAVLINK_TYPE_UINT32_T, 16, 68, offsetof(mavlink_endpoint_announce_t, events) }, \
         { "replies", NULL, MAVLINK_TYPE_UINT32_T, 16, 132, offsetof(mavlink_endpoint_announce_t, replies) }, \
         { "subscribes", NULL, MAVLINK_TYPE_UINT32_T, 16, 196, offsetof(mavlink_endpoint_announce_t, subscribes) }, \
         } \
}
#endif

/**
 * @brief Pack a endpoint_announce message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param id  Unique identifier for the announcing endpoint
 * @param name  Name of the announcing endpoint
 * @param description  Description of the announcing endpoint
 * @param services  List of services provided by the endpoint
 * @param events  List of events emitted by the endpoint
 * @param replies  List of replies supported by the endpoint
 * @param subscribes  List of subscriptions for the endpoint
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_endpoint_announce_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t id, const char *name, const char *description, const uint32_t *services, const uint32_t *events, const uint32_t *replies, const uint32_t *subscribes)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN];
    _mav_put_uint32_t(buf, 0, id);
    _mav_put_uint32_t_array(buf, 4, services, 16);
    _mav_put_uint32_t_array(buf, 68, events, 16);
    _mav_put_uint32_t_array(buf, 132, replies, 16);
    _mav_put_uint32_t_array(buf, 196, subscribes, 16);
    _mav_put_char_array(buf, 260, name, 50);
    _mav_put_char_array(buf, 310, description, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN);
#else
    mavlink_endpoint_announce_t packet;
    packet.id = id;
    mav_array_assign_uint32_t(packet.services, services, 16);
    mav_array_assign_uint32_t(packet.events, events, 16);
    mav_array_assign_uint32_t(packet.replies, replies, 16);
    mav_array_assign_uint32_t(packet.subscribes, subscribes, 16);
    mav_array_assign_char(packet.name, name, 50);
    mav_array_assign_char(packet.description, description, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_CRC);
}

/**
 * @brief Pack a endpoint_announce message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param id  Unique identifier for the announcing endpoint
 * @param name  Name of the announcing endpoint
 * @param description  Description of the announcing endpoint
 * @param services  List of services provided by the endpoint
 * @param events  List of events emitted by the endpoint
 * @param replies  List of replies supported by the endpoint
 * @param subscribes  List of subscriptions for the endpoint
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_endpoint_announce_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t id, const char *name, const char *description, const uint32_t *services, const uint32_t *events, const uint32_t *replies, const uint32_t *subscribes)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN];
    _mav_put_uint32_t(buf, 0, id);
    _mav_put_uint32_t_array(buf, 4, services, 16);
    _mav_put_uint32_t_array(buf, 68, events, 16);
    _mav_put_uint32_t_array(buf, 132, replies, 16);
    _mav_put_uint32_t_array(buf, 196, subscribes, 16);
    _mav_put_char_array(buf, 260, name, 50);
    _mav_put_char_array(buf, 310, description, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN);
#else
    mavlink_endpoint_announce_t packet;
    packet.id = id;
    mav_array_memcpy(packet.services, services, sizeof(uint32_t)*16);
    mav_array_memcpy(packet.events, events, sizeof(uint32_t)*16);
    mav_array_memcpy(packet.replies, replies, sizeof(uint32_t)*16);
    mav_array_memcpy(packet.subscribes, subscribes, sizeof(uint32_t)*16);
    mav_array_memcpy(packet.name, name, sizeof(char)*50);
    mav_array_memcpy(packet.description, description, sizeof(char)*100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN);
#endif
}

/**
 * @brief Pack a endpoint_announce message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param id  Unique identifier for the announcing endpoint
 * @param name  Name of the announcing endpoint
 * @param description  Description of the announcing endpoint
 * @param services  List of services provided by the endpoint
 * @param events  List of events emitted by the endpoint
 * @param replies  List of replies supported by the endpoint
 * @param subscribes  List of subscriptions for the endpoint
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_endpoint_announce_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t id,const char *name,const char *description,const uint32_t *services,const uint32_t *events,const uint32_t *replies,const uint32_t *subscribes)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN];
    _mav_put_uint32_t(buf, 0, id);
    _mav_put_uint32_t_array(buf, 4, services, 16);
    _mav_put_uint32_t_array(buf, 68, events, 16);
    _mav_put_uint32_t_array(buf, 132, replies, 16);
    _mav_put_uint32_t_array(buf, 196, subscribes, 16);
    _mav_put_char_array(buf, 260, name, 50);
    _mav_put_char_array(buf, 310, description, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN);
#else
    mavlink_endpoint_announce_t packet;
    packet.id = id;
    mav_array_assign_uint32_t(packet.services, services, 16);
    mav_array_assign_uint32_t(packet.events, events, 16);
    mav_array_assign_uint32_t(packet.replies, replies, 16);
    mav_array_assign_uint32_t(packet.subscribes, subscribes, 16);
    mav_array_assign_char(packet.name, name, 50);
    mav_array_assign_char(packet.description, description, 100);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_CRC);
}

/**
 * @brief Encode a endpoint_announce struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param endpoint_announce C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_endpoint_announce_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_endpoint_announce_t* endpoint_announce)
{
    return mavlink_msg_endpoint_announce_pack(system_id, component_id, msg, endpoint_announce->id, endpoint_announce->name, endpoint_announce->description, endpoint_announce->services, endpoint_announce->events, endpoint_announce->replies, endpoint_announce->subscribes);
}

/**
 * @brief Encode a endpoint_announce struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param endpoint_announce C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_endpoint_announce_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_endpoint_announce_t* endpoint_announce)
{
    return mavlink_msg_endpoint_announce_pack_chan(system_id, component_id, chan, msg, endpoint_announce->id, endpoint_announce->name, endpoint_announce->description, endpoint_announce->services, endpoint_announce->events, endpoint_announce->replies, endpoint_announce->subscribes);
}

/**
 * @brief Encode a endpoint_announce struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param endpoint_announce C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_endpoint_announce_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_endpoint_announce_t* endpoint_announce)
{
    return mavlink_msg_endpoint_announce_pack_status(system_id, component_id, _status, msg,  endpoint_announce->id, endpoint_announce->name, endpoint_announce->description, endpoint_announce->services, endpoint_announce->events, endpoint_announce->replies, endpoint_announce->subscribes);
}

/**
 * @brief Send a endpoint_announce message
 * @param chan MAVLink channel to send the message
 *
 * @param id  Unique identifier for the announcing endpoint
 * @param name  Name of the announcing endpoint
 * @param description  Description of the announcing endpoint
 * @param services  List of services provided by the endpoint
 * @param events  List of events emitted by the endpoint
 * @param replies  List of replies supported by the endpoint
 * @param subscribes  List of subscriptions for the endpoint
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_endpoint_announce_send(mavlink_channel_t chan, uint32_t id, const char *name, const char *description, const uint32_t *services, const uint32_t *events, const uint32_t *replies, const uint32_t *subscribes)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN];
    _mav_put_uint32_t(buf, 0, id);
    _mav_put_uint32_t_array(buf, 4, services, 16);
    _mav_put_uint32_t_array(buf, 68, events, 16);
    _mav_put_uint32_t_array(buf, 132, replies, 16);
    _mav_put_uint32_t_array(buf, 196, subscribes, 16);
    _mav_put_char_array(buf, 260, name, 50);
    _mav_put_char_array(buf, 310, description, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE, buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_CRC);
#else
    mavlink_endpoint_announce_t packet;
    packet.id = id;
    mav_array_assign_uint32_t(packet.services, services, 16);
    mav_array_assign_uint32_t(packet.events, events, 16);
    mav_array_assign_uint32_t(packet.replies, replies, 16);
    mav_array_assign_uint32_t(packet.subscribes, subscribes, 16);
    mav_array_assign_char(packet.name, name, 50);
    mav_array_assign_char(packet.description, description, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE, (const char *)&packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_CRC);
#endif
}

/**
 * @brief Send a endpoint_announce message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_endpoint_announce_send_struct(mavlink_channel_t chan, const mavlink_endpoint_announce_t* endpoint_announce)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_endpoint_announce_send(chan, endpoint_announce->id, endpoint_announce->name, endpoint_announce->description, endpoint_announce->services, endpoint_announce->events, endpoint_announce->replies, endpoint_announce->subscribes);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE, (const char *)endpoint_announce, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_CRC);
#endif
}

#if MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_endpoint_announce_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t id, const char *name, const char *description, const uint32_t *services, const uint32_t *events, const uint32_t *replies, const uint32_t *subscribes)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, id);
    _mav_put_uint32_t_array(buf, 4, services, 16);
    _mav_put_uint32_t_array(buf, 68, events, 16);
    _mav_put_uint32_t_array(buf, 132, replies, 16);
    _mav_put_uint32_t_array(buf, 196, subscribes, 16);
    _mav_put_char_array(buf, 260, name, 50);
    _mav_put_char_array(buf, 310, description, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE, buf, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_CRC);
#else
    mavlink_endpoint_announce_t *packet = (mavlink_endpoint_announce_t *)msgbuf;
    packet->id = id;
    mav_array_assign_uint32_t(packet->services, services, 16);
    mav_array_assign_uint32_t(packet->events, events, 16);
    mav_array_assign_uint32_t(packet->replies, replies, 16);
    mav_array_assign_uint32_t(packet->subscribes, subscribes, 16);
    mav_array_assign_char(packet->name, name, 50);
    mav_array_assign_char(packet->description, description, 100);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE, (const char *)packet, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_CRC);
#endif
}
#endif

#endif

// MESSAGE ENDPOINT_ANNOUNCE UNPACKING


/**
 * @brief Get field id from endpoint_announce message
 *
 * @return  Unique identifier for the announcing endpoint
 */
static inline uint32_t mavlink_msg_endpoint_announce_get_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field name from endpoint_announce message
 *
 * @return  Name of the announcing endpoint
 */
static inline uint16_t mavlink_msg_endpoint_announce_get_name(const mavlink_message_t* msg, char *name)
{
    return _MAV_RETURN_char_array(msg, name, 50,  260);
}

/**
 * @brief Get field description from endpoint_announce message
 *
 * @return  Description of the announcing endpoint
 */
static inline uint16_t mavlink_msg_endpoint_announce_get_description(const mavlink_message_t* msg, char *description)
{
    return _MAV_RETURN_char_array(msg, description, 100,  310);
}

/**
 * @brief Get field services from endpoint_announce message
 *
 * @return  List of services provided by the endpoint
 */
static inline uint16_t mavlink_msg_endpoint_announce_get_services(const mavlink_message_t* msg, uint32_t *services)
{
    return _MAV_RETURN_uint32_t_array(msg, services, 16,  4);
}

/**
 * @brief Get field events from endpoint_announce message
 *
 * @return  List of events emitted by the endpoint
 */
static inline uint16_t mavlink_msg_endpoint_announce_get_events(const mavlink_message_t* msg, uint32_t *events)
{
    return _MAV_RETURN_uint32_t_array(msg, events, 16,  68);
}

/**
 * @brief Get field replies from endpoint_announce message
 *
 * @return  List of replies supported by the endpoint
 */
static inline uint16_t mavlink_msg_endpoint_announce_get_replies(const mavlink_message_t* msg, uint32_t *replies)
{
    return _MAV_RETURN_uint32_t_array(msg, replies, 16,  132);
}

/**
 * @brief Get field subscribes from endpoint_announce message
 *
 * @return  List of subscriptions for the endpoint
 */
static inline uint16_t mavlink_msg_endpoint_announce_get_subscribes(const mavlink_message_t* msg, uint32_t *subscribes)
{
    return _MAV_RETURN_uint32_t_array(msg, subscribes, 16,  196);
}

/**
 * @brief Decode a endpoint_announce message into a struct
 *
 * @param msg The message to decode
 * @param endpoint_announce C-struct to decode the message contents into
 */
static inline void mavlink_msg_endpoint_announce_decode(const mavlink_message_t* msg, mavlink_endpoint_announce_t* endpoint_announce)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    endpoint_announce->id = mavlink_msg_endpoint_announce_get_id(msg);
    mavlink_msg_endpoint_announce_get_services(msg, endpoint_announce->services);
    mavlink_msg_endpoint_announce_get_events(msg, endpoint_announce->events);
    mavlink_msg_endpoint_announce_get_replies(msg, endpoint_announce->replies);
    mavlink_msg_endpoint_announce_get_subscribes(msg, endpoint_announce->subscribes);
    mavlink_msg_endpoint_announce_get_name(msg, endpoint_announce->name);
    mavlink_msg_endpoint_announce_get_description(msg, endpoint_announce->description);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN? msg->len : MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN;
        memset(endpoint_announce, 0, MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_LEN);
    memcpy(endpoint_announce, _MAV_PAYLOAD(msg), len);
#endif
}
