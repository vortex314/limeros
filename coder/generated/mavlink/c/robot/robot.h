/** @file
 *  @brief MAVLink comm protocol generated from robot.xml
 *  @see http://mavlink.org
 */
#pragma once
#ifndef MAVLINK_ROBOT_H
#define MAVLINK_ROBOT_H

#ifndef MAVLINK_H
    #error Wrong include order: MAVLINK_ROBOT.H MUST NOT BE DIRECTLY USED. Include mavlink.h from the same directory instead or set ALL AND EVERY defines from MAVLINK.H manually accordingly, including the #define MAVLINK_H call.
#endif

#define MAVLINK_ROBOT_XML_HASH 4696462752261820882

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#ifndef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {}
#endif

#ifndef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {{40001, 16, 8, 8, 0, 0, 0}, {40002, 223, 8, 8, 0, 0, 0}, {40003, 137, 112, 112, 0, 0, 0}, {40004, 194, 91, 91, 0, 0, 0}, {40005, 130, 5, 5, 0, 0, 0}, {40006, 138, 112, 112, 0, 0, 0}, {40007, 187, 17, 17, 0, 0, 0}, {40008, 154, 36, 36, 0, 0, 0}, {40009, 106, 24, 24, 0, 0, 0}, {40010, 169, 12, 12, 0, 0, 0}, {40011, 12, 168, 168, 0, 0, 0}, {40012, 140, 32, 32, 0, 0, 0}, {40013, 15, 113, 113, 0, 0, 0}, {40014, 129, 108, 108, 0, 0, 0}, {40015, 32, 132, 132, 0, 0, 0}, {40016, 61, 12, 12, 0, 0, 0}, {40017, 47, 12, 12, 0, 0, 0}, {40018, 33, 8, 8, 0, 0, 0}, {40019, 179, 308, 308, 0, 0, 0}, {40020, 131, 108, 108, 0, 0, 0}, {40021, 226, 12, 12, 0, 0, 0}, {40022, 82, 23, 23, 0, 0, 0}, {40023, 74, 18, 18, 0, 0, 0}, {40024, 76, 275, 275, 0, 0, 0}, {40025, 211, 410, 410, 0, 0, 0}, {40026, 236, 8, 8, 0, 0, 0}}
#endif

#include "../protocol.h"

#define MAVLINK_ENABLED_ROBOT

// ENUM DEFINITIONS


/** @brief  */
#ifndef HAVE_ENUM_CUSTOM_SENSOR_STATUS
#define HAVE_ENUM_CUSTOM_SENSOR_STATUS
typedef enum CUSTOM_SENSOR_STATUS
{
   CUSTOM_SENSOR_OK=0, /* Sensor operational | */
   CUSTOM_SENSOR_ERROR=1, /* Sensor fault detected | */
   CUSTOM_SENSOR_STATUS_ENUM_END=2, /*  | */
} CUSTOM_SENSOR_STATUS;
#endif

/** @brief  */
#ifndef HAVE_ENUM_AUX_INPUT2_TYPE
#define HAVE_ENUM_AUX_INPUT2_TYPE
typedef enum AUX_INPUT2_TYPE
{
   AUX_INPUT2_TYPE_DISABLED=0, /* Disabled | */
   AUX_INPUT2_TYPE_NORMAL_POT=1, /* Normal Pot | */
   AUX_INPUT2_TYPE_MIDDLE_RESTING_POT=2, /* Middle Resting Pot | */
   AUX_INPUT2_TYPE_AUTO_DETECT=3, /* Auto-detect | */
   AUX_INPUT2_TYPE_ENUM_END=4, /*  | */
} AUX_INPUT2_TYPE;
#endif

// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION 3
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION 3
#endif

// MESSAGE DEFINITIONS
#include "./mavlink_msg_hoverboard_request.h"
#include "./mavlink_msg_hoverboard_reply.h"
#include "./mavlink_msg_generic_reply.h"
#include "./mavlink_msg_hoverboard_event.h"
#include "./mavlink_msg_cutter_request.h"
#include "./mavlink_msg_cutter_reply.h"
#include "./mavlink_msg_cutter_event.h"
#include "./mavlink_msg_compass_event.h"
#include "./mavlink_msg_imu_event.h"
#include "./mavlink_msg_us_event.h"
#include "./mavlink_msg_ps4_event.h"
#include "./mavlink_msg_ps4_request.h"
#include "./mavlink_msg_sys_request.h"
#include "./mavlink_msg_sys_reply.h"
#include "./mavlink_msg_sys_event.h"
#include "./mavlink_msg_ping_request.h"
#include "./mavlink_msg_ping_reply.h"
#include "./mavlink_msg_broker_subscribe_request.h"
#include "./mavlink_msg_wifi_event.h"
#include "./mavlink_msg_device_alive_event.h"
#include "./mavlink_msg_max31855_event.h"
#include "./mavlink_msg_heating_event.h"
#include "./mavlink_msg_heating_request.h"
#include "./mavlink_msg_envelope.h"
#include "./mavlink_msg_endpoint_announce.h"
#include "./mavlink_msg_endpoint_announce_reply.h"

// base include



#if MAVLINK_ROBOT_XML_HASH == MAVLINK_PRIMARY_XML_HASH
# define MAVLINK_MESSAGE_INFO {MAVLINK_MESSAGE_INFO_HOVERBOARD_REQUEST, MAVLINK_MESSAGE_INFO_HOVERBOARD_REPLY, MAVLINK_MESSAGE_INFO_GENERIC_REPLY, MAVLINK_MESSAGE_INFO_HOVERBOARD_EVENT, MAVLINK_MESSAGE_INFO_CUTTER_REQUEST, MAVLINK_MESSAGE_INFO_CUTTER_REPLY, MAVLINK_MESSAGE_INFO_CUTTER_EVENT, MAVLINK_MESSAGE_INFO_COMPASS_EVENT, MAVLINK_MESSAGE_INFO_IMU_EVENT, MAVLINK_MESSAGE_INFO_US_EVENT, MAVLINK_MESSAGE_INFO_PS4_EVENT, MAVLINK_MESSAGE_INFO_PS4_REQUEST, MAVLINK_MESSAGE_INFO_SYS_REQUEST, MAVLINK_MESSAGE_INFO_SYS_REPLY, MAVLINK_MESSAGE_INFO_SYS_EVENT, MAVLINK_MESSAGE_INFO_PING_REQUEST, MAVLINK_MESSAGE_INFO_PING_REPLY, MAVLINK_MESSAGE_INFO_BROKER_SUBSCRIBE_REQUEST, MAVLINK_MESSAGE_INFO_WIFI_EVENT, MAVLINK_MESSAGE_INFO_DEVICE_ALIVE_EVENT, MAVLINK_MESSAGE_INFO_MAX31855_EVENT, MAVLINK_MESSAGE_INFO_HEATING_EVENT, MAVLINK_MESSAGE_INFO_HEATING_REQUEST, MAVLINK_MESSAGE_INFO_ENVELOPE, MAVLINK_MESSAGE_INFO_ENDPOINT_ANNOUNCE, MAVLINK_MESSAGE_INFO_ENDPOINT_ANNOUNCE_REPLY}
# define MAVLINK_MESSAGE_NAMES {{ "BROKER_SUBSCRIBE_REQUEST", 40018 }, { "COMPASS_EVENT", 40008 }, { "CUTTER_EVENT", 40007 }, { "CUTTER_REPLY", 40006 }, { "CUTTER_REQUEST", 40005 }, { "DEVICE_ALIVE_EVENT", 40020 }, { "ENDPOINT_ANNOUNCE", 40025 }, { "ENDPOINT_ANNOUNCE_REPLY", 40026 }, { "ENVELOPE", 40024 }, { "GENERIC_REPLY", 40003 }, { "HEATING_EVENT", 40022 }, { "HEATING_REQUEST", 40023 }, { "HOVERBOARD_EVENT", 40004 }, { "HOVERBOARD_REPLY", 40002 }, { "HOVERBOARD_REQUEST", 40001 }, { "IMU_EVENT", 40009 }, { "MAX31855_EVENT", 40021 }, { "PING_REPLY", 40017 }, { "PING_REQUEST", 40016 }, { "PS4_EVENT", 40011 }, { "PS4_REQUEST", 40012 }, { "SYS_EVENT", 40015 }, { "SYS_REPLY", 40014 }, { "SYS_REQUEST", 40013 }, { "US_EVENT", 40010 }, { "WIFI_EVENT", 40019 }}
# if MAVLINK_COMMAND_24BIT
#  include "../mavlink_get_info.h"
# endif
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAVLINK_ROBOT_H
