/** @file
 *	@brief MAVLink comm protocol generated from robot.xml
 *	@see http://mavlink.org
 */

#pragma once

#include <array>
#include <cstdint>
#include <sstream>

#ifndef MAVLINK_STX
#define MAVLINK_STX 253
#endif

#include "../message.hpp"

namespace mavlink {
namespace robot {

/**
 * Array of msg_entry needed for @p mavlink_parse_char() (through @p mavlink_get_msg_entry())
 */
constexpr std::array<mavlink_msg_entry_t, 26> MESSAGE_ENTRIES {{ {40001, 16, 8, 8, 0, 0, 0}, {40002, 223, 8, 8, 0, 0, 0}, {40003, 137, 112, 112, 0, 0, 0}, {40004, 194, 91, 91, 0, 0, 0}, {40005, 130, 5, 5, 0, 0, 0}, {40006, 138, 112, 112, 0, 0, 0}, {40007, 187, 17, 17, 0, 0, 0}, {40008, 154, 36, 36, 0, 0, 0}, {40009, 106, 24, 24, 0, 0, 0}, {40010, 169, 12, 12, 0, 0, 0}, {40011, 12, 168, 168, 0, 0, 0}, {40012, 140, 32, 32, 0, 0, 0}, {40013, 15, 113, 113, 0, 0, 0}, {40014, 129, 108, 108, 0, 0, 0}, {40015, 32, 132, 132, 0, 0, 0}, {40016, 61, 12, 12, 0, 0, 0}, {40017, 47, 12, 12, 0, 0, 0}, {40018, 33, 8, 8, 0, 0, 0}, {40019, 179, 308, 308, 0, 0, 0}, {40020, 131, 108, 108, 0, 0, 0}, {40021, 226, 12, 12, 0, 0, 0}, {40022, 82, 23, 23, 0, 0, 0}, {40023, 74, 18, 18, 0, 0, 0}, {40024, 76, 275, 275, 0, 0, 0}, {40025, 211, 410, 410, 0, 0, 0}, {40026, 236, 8, 8, 0, 0, 0} }};

//! MAVLINK VERSION
constexpr auto MAVLINK_VERSION = 3;


// ENUM DEFINITIONS


/** @brief  */
enum class CUSTOM_SENSOR_STATUS
{
    OK=0, /* Sensor operational | */
    ERROR=1, /* Sensor fault detected | */
};

//! CUSTOM_SENSOR_STATUS ENUM_END
constexpr auto CUSTOM_SENSOR_STATUS_ENUM_END = 2;

/** @brief  */
enum class AUX_INPUT2_TYPE : uint8_t
{
    DISABLED=0, /* Disabled | */
    NORMAL_POT=1, /* Normal Pot | */
    MIDDLE_RESTING_POT=2, /* Middle Resting Pot | */
    AUTO_DETECT=3, /* Auto-detect | */
};

//! AUX_INPUT2_TYPE ENUM_END
constexpr auto AUX_INPUT2_TYPE_ENUM_END = 4;


} // namespace robot
} // namespace mavlink

// MESSAGE DEFINITIONS
#include "./mavlink_msg_hoverboard_request.hpp"
#include "./mavlink_msg_hoverboard_reply.hpp"
#include "./mavlink_msg_generic_reply.hpp"
#include "./mavlink_msg_hoverboard_event.hpp"
#include "./mavlink_msg_cutter_request.hpp"
#include "./mavlink_msg_cutter_reply.hpp"
#include "./mavlink_msg_cutter_event.hpp"
#include "./mavlink_msg_compass_event.hpp"
#include "./mavlink_msg_imu_event.hpp"
#include "./mavlink_msg_us_event.hpp"
#include "./mavlink_msg_ps4_event.hpp"
#include "./mavlink_msg_ps4_request.hpp"
#include "./mavlink_msg_sys_request.hpp"
#include "./mavlink_msg_sys_reply.hpp"
#include "./mavlink_msg_sys_event.hpp"
#include "./mavlink_msg_ping_request.hpp"
#include "./mavlink_msg_ping_reply.hpp"
#include "./mavlink_msg_broker_subscribe_request.hpp"
#include "./mavlink_msg_wifi_event.hpp"
#include "./mavlink_msg_device_alive_event.hpp"
#include "./mavlink_msg_max31855_event.hpp"
#include "./mavlink_msg_heating_event.hpp"
#include "./mavlink_msg_heating_request.hpp"
#include "./mavlink_msg_envelope.hpp"
#include "./mavlink_msg_endpoint_announce.hpp"
#include "./mavlink_msg_endpoint_announce_reply.hpp"

// base include

