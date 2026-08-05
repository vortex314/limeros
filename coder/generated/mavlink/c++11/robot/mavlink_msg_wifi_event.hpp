// MESSAGE WIFI_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief WIFI_EVENT message
 *
 * Wifi event message
 */
struct WIFI_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40019;
    static constexpr size_t LENGTH = 308;
    static constexpr size_t MIN_LENGTH = 308;
    static constexpr uint8_t CRC_EXTRA = 179;
    static constexpr auto NAME = "WIFI_EVENT";


    std::array<char, 50> ip; /*<  IP address */
    std::array<char, 50> gateway; /*<  Gateway address */
    std::array<char, 50> netmask; /*<  Netmask */
    std::array<char, 50> ssid; /*<  SSID */
    std::array<char, 50> bssid; /*<  BSSID */
    int32_t channel; /*<  WiFi channel */
    int32_t rssi; /*< [dBm] Received signal strength */
    std::array<char, 50> mac; /*<  MAC address */


    inline std::string get_name(void) const override
    {
            return NAME;
    }

    inline Info get_message_info(void) const override
    {
            return { MSG_ID, LENGTH, MIN_LENGTH, CRC_EXTRA };
    }

    inline std::string to_yaml(void) const override
    {
        std::stringstream ss;

        ss << NAME << ":" << std::endl;
        ss << "  ip: \"" << to_string(ip) << "\"" << std::endl;
        ss << "  gateway: \"" << to_string(gateway) << "\"" << std::endl;
        ss << "  netmask: \"" << to_string(netmask) << "\"" << std::endl;
        ss << "  ssid: \"" << to_string(ssid) << "\"" << std::endl;
        ss << "  bssid: \"" << to_string(bssid) << "\"" << std::endl;
        ss << "  channel: " << channel << std::endl;
        ss << "  rssi: " << rssi << std::endl;
        ss << "  mac: \"" << to_string(mac) << "\"" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << channel;                       // offset: 0
        map << rssi;                          // offset: 4
        map << ip;                            // offset: 8
        map << gateway;                       // offset: 58
        map << netmask;                       // offset: 108
        map << ssid;                          // offset: 158
        map << bssid;                         // offset: 208
        map << mac;                           // offset: 258
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> channel;                       // offset: 0
        map >> rssi;                          // offset: 4
        map >> ip;                            // offset: 8
        map >> gateway;                       // offset: 58
        map >> netmask;                       // offset: 108
        map >> ssid;                          // offset: 158
        map >> bssid;                         // offset: 208
        map >> mac;                           // offset: 258
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
