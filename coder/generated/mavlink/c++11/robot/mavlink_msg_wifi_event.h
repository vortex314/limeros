#pragma once
// MESSAGE WIFI_EVENT PACKING

#define MAVLINK_MSG_ID_WIFI_EVENT 40019


typedef struct __mavlink_wifi_event_t {
 int32_t channel; /*<  WiFi channel*/
 int32_t rssi; /*< [dBm] Received signal strength*/
 char ip[50]; /*<  IP address*/
 char gateway[50]; /*<  Gateway address*/
 char netmask[50]; /*<  Netmask*/
 char ssid[50]; /*<  SSID*/
 char bssid[50]; /*<  BSSID*/
 char mac[50]; /*<  MAC address*/
} mavlink_wifi_event_t;

#define MAVLINK_MSG_ID_WIFI_EVENT_LEN 308
#define MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN 308
#define MAVLINK_MSG_ID_40019_LEN 308
#define MAVLINK_MSG_ID_40019_MIN_LEN 308

#define MAVLINK_MSG_ID_WIFI_EVENT_CRC 179
#define MAVLINK_MSG_ID_40019_CRC 179

#define MAVLINK_MSG_WIFI_EVENT_FIELD_IP_LEN 50
#define MAVLINK_MSG_WIFI_EVENT_FIELD_GATEWAY_LEN 50
#define MAVLINK_MSG_WIFI_EVENT_FIELD_NETMASK_LEN 50
#define MAVLINK_MSG_WIFI_EVENT_FIELD_SSID_LEN 50
#define MAVLINK_MSG_WIFI_EVENT_FIELD_BSSID_LEN 50
#define MAVLINK_MSG_WIFI_EVENT_FIELD_MAC_LEN 50

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_WIFI_EVENT { \
    40019, \
    "WIFI_EVENT", \
    8, \
    {  { "ip", NULL, MAVLINK_TYPE_CHAR, 50, 8, offsetof(mavlink_wifi_event_t, ip) }, \
         { "gateway", NULL, MAVLINK_TYPE_CHAR, 50, 58, offsetof(mavlink_wifi_event_t, gateway) }, \
         { "netmask", NULL, MAVLINK_TYPE_CHAR, 50, 108, offsetof(mavlink_wifi_event_t, netmask) }, \
         { "ssid", NULL, MAVLINK_TYPE_CHAR, 50, 158, offsetof(mavlink_wifi_event_t, ssid) }, \
         { "bssid", NULL, MAVLINK_TYPE_CHAR, 50, 208, offsetof(mavlink_wifi_event_t, bssid) }, \
         { "channel", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_wifi_event_t, channel) }, \
         { "rssi", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_wifi_event_t, rssi) }, \
         { "mac", NULL, MAVLINK_TYPE_CHAR, 50, 258, offsetof(mavlink_wifi_event_t, mac) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_WIFI_EVENT { \
    "WIFI_EVENT", \
    8, \
    {  { "ip", NULL, MAVLINK_TYPE_CHAR, 50, 8, offsetof(mavlink_wifi_event_t, ip) }, \
         { "gateway", NULL, MAVLINK_TYPE_CHAR, 50, 58, offsetof(mavlink_wifi_event_t, gateway) }, \
         { "netmask", NULL, MAVLINK_TYPE_CHAR, 50, 108, offsetof(mavlink_wifi_event_t, netmask) }, \
         { "ssid", NULL, MAVLINK_TYPE_CHAR, 50, 158, offsetof(mavlink_wifi_event_t, ssid) }, \
         { "bssid", NULL, MAVLINK_TYPE_CHAR, 50, 208, offsetof(mavlink_wifi_event_t, bssid) }, \
         { "channel", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_wifi_event_t, channel) }, \
         { "rssi", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_wifi_event_t, rssi) }, \
         { "mac", NULL, MAVLINK_TYPE_CHAR, 50, 258, offsetof(mavlink_wifi_event_t, mac) }, \
         } \
}
#endif

/**
 * @brief Pack a wifi_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param ip  IP address
 * @param gateway  Gateway address
 * @param netmask  Netmask
 * @param ssid  SSID
 * @param bssid  BSSID
 * @param channel  WiFi channel
 * @param rssi [dBm] Received signal strength
 * @param mac  MAC address
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_wifi_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *ip, const char *gateway, const char *netmask, const char *ssid, const char *bssid, int32_t channel, int32_t rssi, const char *mac)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WIFI_EVENT_LEN];
    _mav_put_int32_t(buf, 0, channel);
    _mav_put_int32_t(buf, 4, rssi);
    _mav_put_char_array(buf, 8, ip, 50);
    _mav_put_char_array(buf, 58, gateway, 50);
    _mav_put_char_array(buf, 108, netmask, 50);
    _mav_put_char_array(buf, 158, ssid, 50);
    _mav_put_char_array(buf, 208, bssid, 50);
    _mav_put_char_array(buf, 258, mac, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WIFI_EVENT_LEN);
#else
    mavlink_wifi_event_t packet;
    packet.channel = channel;
    packet.rssi = rssi;
    mav_array_assign_char(packet.ip, ip, 50);
    mav_array_assign_char(packet.gateway, gateway, 50);
    mav_array_assign_char(packet.netmask, netmask, 50);
    mav_array_assign_char(packet.ssid, ssid, 50);
    mav_array_assign_char(packet.bssid, bssid, 50);
    mav_array_assign_char(packet.mac, mac, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WIFI_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WIFI_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN, MAVLINK_MSG_ID_WIFI_EVENT_LEN, MAVLINK_MSG_ID_WIFI_EVENT_CRC);
}

/**
 * @brief Pack a wifi_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param ip  IP address
 * @param gateway  Gateway address
 * @param netmask  Netmask
 * @param ssid  SSID
 * @param bssid  BSSID
 * @param channel  WiFi channel
 * @param rssi [dBm] Received signal strength
 * @param mac  MAC address
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_wifi_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               const char *ip, const char *gateway, const char *netmask, const char *ssid, const char *bssid, int32_t channel, int32_t rssi, const char *mac)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WIFI_EVENT_LEN];
    _mav_put_int32_t(buf, 0, channel);
    _mav_put_int32_t(buf, 4, rssi);
    _mav_put_char_array(buf, 8, ip, 50);
    _mav_put_char_array(buf, 58, gateway, 50);
    _mav_put_char_array(buf, 108, netmask, 50);
    _mav_put_char_array(buf, 158, ssid, 50);
    _mav_put_char_array(buf, 208, bssid, 50);
    _mav_put_char_array(buf, 258, mac, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WIFI_EVENT_LEN);
#else
    mavlink_wifi_event_t packet;
    packet.channel = channel;
    packet.rssi = rssi;
    mav_array_memcpy(packet.ip, ip, sizeof(char)*50);
    mav_array_memcpy(packet.gateway, gateway, sizeof(char)*50);
    mav_array_memcpy(packet.netmask, netmask, sizeof(char)*50);
    mav_array_memcpy(packet.ssid, ssid, sizeof(char)*50);
    mav_array_memcpy(packet.bssid, bssid, sizeof(char)*50);
    mav_array_memcpy(packet.mac, mac, sizeof(char)*50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WIFI_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WIFI_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN, MAVLINK_MSG_ID_WIFI_EVENT_LEN, MAVLINK_MSG_ID_WIFI_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN, MAVLINK_MSG_ID_WIFI_EVENT_LEN);
#endif
}

/**
 * @brief Pack a wifi_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ip  IP address
 * @param gateway  Gateway address
 * @param netmask  Netmask
 * @param ssid  SSID
 * @param bssid  BSSID
 * @param channel  WiFi channel
 * @param rssi [dBm] Received signal strength
 * @param mac  MAC address
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_wifi_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *ip,const char *gateway,const char *netmask,const char *ssid,const char *bssid,int32_t channel,int32_t rssi,const char *mac)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WIFI_EVENT_LEN];
    _mav_put_int32_t(buf, 0, channel);
    _mav_put_int32_t(buf, 4, rssi);
    _mav_put_char_array(buf, 8, ip, 50);
    _mav_put_char_array(buf, 58, gateway, 50);
    _mav_put_char_array(buf, 108, netmask, 50);
    _mav_put_char_array(buf, 158, ssid, 50);
    _mav_put_char_array(buf, 208, bssid, 50);
    _mav_put_char_array(buf, 258, mac, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WIFI_EVENT_LEN);
#else
    mavlink_wifi_event_t packet;
    packet.channel = channel;
    packet.rssi = rssi;
    mav_array_assign_char(packet.ip, ip, 50);
    mav_array_assign_char(packet.gateway, gateway, 50);
    mav_array_assign_char(packet.netmask, netmask, 50);
    mav_array_assign_char(packet.ssid, ssid, 50);
    mav_array_assign_char(packet.bssid, bssid, 50);
    mav_array_assign_char(packet.mac, mac, 50);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WIFI_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WIFI_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN, MAVLINK_MSG_ID_WIFI_EVENT_LEN, MAVLINK_MSG_ID_WIFI_EVENT_CRC);
}

/**
 * @brief Encode a wifi_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param wifi_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_wifi_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_wifi_event_t* wifi_event)
{
    return mavlink_msg_wifi_event_pack(system_id, component_id, msg, wifi_event->ip, wifi_event->gateway, wifi_event->netmask, wifi_event->ssid, wifi_event->bssid, wifi_event->channel, wifi_event->rssi, wifi_event->mac);
}

/**
 * @brief Encode a wifi_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param wifi_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_wifi_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_wifi_event_t* wifi_event)
{
    return mavlink_msg_wifi_event_pack_chan(system_id, component_id, chan, msg, wifi_event->ip, wifi_event->gateway, wifi_event->netmask, wifi_event->ssid, wifi_event->bssid, wifi_event->channel, wifi_event->rssi, wifi_event->mac);
}

/**
 * @brief Encode a wifi_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param wifi_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_wifi_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_wifi_event_t* wifi_event)
{
    return mavlink_msg_wifi_event_pack_status(system_id, component_id, _status, msg,  wifi_event->ip, wifi_event->gateway, wifi_event->netmask, wifi_event->ssid, wifi_event->bssid, wifi_event->channel, wifi_event->rssi, wifi_event->mac);
}

/**
 * @brief Send a wifi_event message
 * @param chan MAVLink channel to send the message
 *
 * @param ip  IP address
 * @param gateway  Gateway address
 * @param netmask  Netmask
 * @param ssid  SSID
 * @param bssid  BSSID
 * @param channel  WiFi channel
 * @param rssi [dBm] Received signal strength
 * @param mac  MAC address
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_wifi_event_send(mavlink_channel_t chan, const char *ip, const char *gateway, const char *netmask, const char *ssid, const char *bssid, int32_t channel, int32_t rssi, const char *mac)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WIFI_EVENT_LEN];
    _mav_put_int32_t(buf, 0, channel);
    _mav_put_int32_t(buf, 4, rssi);
    _mav_put_char_array(buf, 8, ip, 50);
    _mav_put_char_array(buf, 58, gateway, 50);
    _mav_put_char_array(buf, 108, netmask, 50);
    _mav_put_char_array(buf, 158, ssid, 50);
    _mav_put_char_array(buf, 208, bssid, 50);
    _mav_put_char_array(buf, 258, mac, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WIFI_EVENT, buf, MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN, MAVLINK_MSG_ID_WIFI_EVENT_LEN, MAVLINK_MSG_ID_WIFI_EVENT_CRC);
#else
    mavlink_wifi_event_t packet;
    packet.channel = channel;
    packet.rssi = rssi;
    mav_array_assign_char(packet.ip, ip, 50);
    mav_array_assign_char(packet.gateway, gateway, 50);
    mav_array_assign_char(packet.netmask, netmask, 50);
    mav_array_assign_char(packet.ssid, ssid, 50);
    mav_array_assign_char(packet.bssid, bssid, 50);
    mav_array_assign_char(packet.mac, mac, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WIFI_EVENT, (const char *)&packet, MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN, MAVLINK_MSG_ID_WIFI_EVENT_LEN, MAVLINK_MSG_ID_WIFI_EVENT_CRC);
#endif
}

/**
 * @brief Send a wifi_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_wifi_event_send_struct(mavlink_channel_t chan, const mavlink_wifi_event_t* wifi_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_wifi_event_send(chan, wifi_event->ip, wifi_event->gateway, wifi_event->netmask, wifi_event->ssid, wifi_event->bssid, wifi_event->channel, wifi_event->rssi, wifi_event->mac);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WIFI_EVENT, (const char *)wifi_event, MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN, MAVLINK_MSG_ID_WIFI_EVENT_LEN, MAVLINK_MSG_ID_WIFI_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_WIFI_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_wifi_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *ip, const char *gateway, const char *netmask, const char *ssid, const char *bssid, int32_t channel, int32_t rssi, const char *mac)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, channel);
    _mav_put_int32_t(buf, 4, rssi);
    _mav_put_char_array(buf, 8, ip, 50);
    _mav_put_char_array(buf, 58, gateway, 50);
    _mav_put_char_array(buf, 108, netmask, 50);
    _mav_put_char_array(buf, 158, ssid, 50);
    _mav_put_char_array(buf, 208, bssid, 50);
    _mav_put_char_array(buf, 258, mac, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WIFI_EVENT, buf, MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN, MAVLINK_MSG_ID_WIFI_EVENT_LEN, MAVLINK_MSG_ID_WIFI_EVENT_CRC);
#else
    mavlink_wifi_event_t *packet = (mavlink_wifi_event_t *)msgbuf;
    packet->channel = channel;
    packet->rssi = rssi;
    mav_array_assign_char(packet->ip, ip, 50);
    mav_array_assign_char(packet->gateway, gateway, 50);
    mav_array_assign_char(packet->netmask, netmask, 50);
    mav_array_assign_char(packet->ssid, ssid, 50);
    mav_array_assign_char(packet->bssid, bssid, 50);
    mav_array_assign_char(packet->mac, mac, 50);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WIFI_EVENT, (const char *)packet, MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN, MAVLINK_MSG_ID_WIFI_EVENT_LEN, MAVLINK_MSG_ID_WIFI_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE WIFI_EVENT UNPACKING


/**
 * @brief Get field ip from wifi_event message
 *
 * @return  IP address
 */
static inline uint16_t mavlink_msg_wifi_event_get_ip(const mavlink_message_t* msg, char *ip)
{
    return _MAV_RETURN_char_array(msg, ip, 50,  8);
}

/**
 * @brief Get field gateway from wifi_event message
 *
 * @return  Gateway address
 */
static inline uint16_t mavlink_msg_wifi_event_get_gateway(const mavlink_message_t* msg, char *gateway)
{
    return _MAV_RETURN_char_array(msg, gateway, 50,  58);
}

/**
 * @brief Get field netmask from wifi_event message
 *
 * @return  Netmask
 */
static inline uint16_t mavlink_msg_wifi_event_get_netmask(const mavlink_message_t* msg, char *netmask)
{
    return _MAV_RETURN_char_array(msg, netmask, 50,  108);
}

/**
 * @brief Get field ssid from wifi_event message
 *
 * @return  SSID
 */
static inline uint16_t mavlink_msg_wifi_event_get_ssid(const mavlink_message_t* msg, char *ssid)
{
    return _MAV_RETURN_char_array(msg, ssid, 50,  158);
}

/**
 * @brief Get field bssid from wifi_event message
 *
 * @return  BSSID
 */
static inline uint16_t mavlink_msg_wifi_event_get_bssid(const mavlink_message_t* msg, char *bssid)
{
    return _MAV_RETURN_char_array(msg, bssid, 50,  208);
}

/**
 * @brief Get field channel from wifi_event message
 *
 * @return  WiFi channel
 */
static inline int32_t mavlink_msg_wifi_event_get_channel(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field rssi from wifi_event message
 *
 * @return [dBm] Received signal strength
 */
static inline int32_t mavlink_msg_wifi_event_get_rssi(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field mac from wifi_event message
 *
 * @return  MAC address
 */
static inline uint16_t mavlink_msg_wifi_event_get_mac(const mavlink_message_t* msg, char *mac)
{
    return _MAV_RETURN_char_array(msg, mac, 50,  258);
}

/**
 * @brief Decode a wifi_event message into a struct
 *
 * @param msg The message to decode
 * @param wifi_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_wifi_event_decode(const mavlink_message_t* msg, mavlink_wifi_event_t* wifi_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    wifi_event->channel = mavlink_msg_wifi_event_get_channel(msg);
    wifi_event->rssi = mavlink_msg_wifi_event_get_rssi(msg);
    mavlink_msg_wifi_event_get_ip(msg, wifi_event->ip);
    mavlink_msg_wifi_event_get_gateway(msg, wifi_event->gateway);
    mavlink_msg_wifi_event_get_netmask(msg, wifi_event->netmask);
    mavlink_msg_wifi_event_get_ssid(msg, wifi_event->ssid);
    mavlink_msg_wifi_event_get_bssid(msg, wifi_event->bssid);
    mavlink_msg_wifi_event_get_mac(msg, wifi_event->mac);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_WIFI_EVENT_LEN? msg->len : MAVLINK_MSG_ID_WIFI_EVENT_LEN;
        memset(wifi_event, 0, MAVLINK_MSG_ID_WIFI_EVENT_LEN);
    memcpy(wifi_event, _MAV_PAYLOAD(msg), len);
#endif
}
