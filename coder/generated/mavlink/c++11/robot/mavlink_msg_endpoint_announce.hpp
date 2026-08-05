// MESSAGE ENDPOINT_ANNOUNCE support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief ENDPOINT_ANNOUNCE message
 *
 * Endpoint announce message for service discovery
 */
struct ENDPOINT_ANNOUNCE : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40025;
    static constexpr size_t LENGTH = 410;
    static constexpr size_t MIN_LENGTH = 410;
    static constexpr uint8_t CRC_EXTRA = 211;
    static constexpr auto NAME = "ENDPOINT_ANNOUNCE";


    uint32_t id; /*<  Unique identifier for the announcing endpoint */
    std::array<char, 50> name; /*<  Name of the announcing endpoint */
    std::array<char, 100> description; /*<  Description of the announcing endpoint */
    std::array<uint32_t, 16> services; /*<  List of services provided by the endpoint */
    std::array<uint32_t, 16> events; /*<  List of events emitted by the endpoint */
    std::array<uint32_t, 16> replies; /*<  List of replies supported by the endpoint */
    std::array<uint32_t, 16> subscribes; /*<  List of subscriptions for the endpoint */


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
        ss << "  id: " << id << std::endl;
        ss << "  name: \"" << to_string(name) << "\"" << std::endl;
        ss << "  description: \"" << to_string(description) << "\"" << std::endl;
        ss << "  services: [" << to_string(services) << "]" << std::endl;
        ss << "  events: [" << to_string(events) << "]" << std::endl;
        ss << "  replies: [" << to_string(replies) << "]" << std::endl;
        ss << "  subscribes: [" << to_string(subscribes) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << id;                            // offset: 0
        map << services;                      // offset: 4
        map << events;                        // offset: 68
        map << replies;                       // offset: 132
        map << subscribes;                    // offset: 196
        map << name;                          // offset: 260
        map << description;                   // offset: 310
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> id;                            // offset: 0
        map >> services;                      // offset: 4
        map >> events;                        // offset: 68
        map >> replies;                       // offset: 132
        map >> subscribes;                    // offset: 196
        map >> name;                          // offset: 260
        map >> description;                   // offset: 310
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
