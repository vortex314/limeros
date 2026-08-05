/** @file
 *	@brief MAVLink comm testsuite protocol generated from robot.xml
 *	@see http://mavlink.org
 */

#pragma once

#include <gtest/gtest.h>
#include "robot.hpp"

#ifdef TEST_INTEROP
using namespace mavlink;
#undef MAVLINK_HELPER
#include "mavlink.h"
#endif


TEST(robot, HOVERBOARD_REQUEST)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::HOVERBOARD_REQUEST packet_in{};
    packet_in.speed = 963497464;
    packet_in.steer = 963497672;

    mavlink::robot::msg::HOVERBOARD_REQUEST packet1{};
    mavlink::robot::msg::HOVERBOARD_REQUEST packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.speed, packet2.speed);
    EXPECT_EQ(packet1.steer, packet2.steer);
}

#ifdef TEST_INTEROP
TEST(robot_interop, HOVERBOARD_REQUEST)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_hoverboard_request_t packet_c {
         963497464, 963497672
    };

    mavlink::robot::msg::HOVERBOARD_REQUEST packet_in{};
    packet_in.speed = 963497464;
    packet_in.steer = 963497672;

    mavlink::robot::msg::HOVERBOARD_REQUEST packet2{};

    mavlink_msg_hoverboard_request_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.speed, packet2.speed);
    EXPECT_EQ(packet_in.steer, packet2.steer);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, HOVERBOARD_REPLY)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::HOVERBOARD_REPLY packet_in{};
    packet_in.speed = 963497464;
    packet_in.steer = 963497672;

    mavlink::robot::msg::HOVERBOARD_REPLY packet1{};
    mavlink::robot::msg::HOVERBOARD_REPLY packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.speed, packet2.speed);
    EXPECT_EQ(packet1.steer, packet2.steer);
}

#ifdef TEST_INTEROP
TEST(robot_interop, HOVERBOARD_REPLY)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_hoverboard_reply_t packet_c {
         963497464, 963497672
    };

    mavlink::robot::msg::HOVERBOARD_REPLY packet_in{};
    packet_in.speed = 963497464;
    packet_in.steer = 963497672;

    mavlink::robot::msg::HOVERBOARD_REPLY packet2{};

    mavlink_msg_hoverboard_reply_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.speed, packet2.speed);
    EXPECT_EQ(packet_in.steer, packet2.steer);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, GENERIC_REPLY)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::GENERIC_REPLY packet_in{};
    packet_in.req_id = 963497464;
    packet_in.error_code = 963497672;
    packet_in.message = to_char_array("MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFG");
    packet_in.msg_type = 963497880;

    mavlink::robot::msg::GENERIC_REPLY packet1{};
    mavlink::robot::msg::GENERIC_REPLY packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.req_id, packet2.req_id);
    EXPECT_EQ(packet1.error_code, packet2.error_code);
    EXPECT_EQ(packet1.message, packet2.message);
    EXPECT_EQ(packet1.msg_type, packet2.msg_type);
}

#ifdef TEST_INTEROP
TEST(robot_interop, GENERIC_REPLY)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_generic_reply_t packet_c {
         963497464, 963497672, 963497880, "MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFG"
    };

    mavlink::robot::msg::GENERIC_REPLY packet_in{};
    packet_in.req_id = 963497464;
    packet_in.error_code = 963497672;
    packet_in.message = to_char_array("MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFG");
    packet_in.msg_type = 963497880;

    mavlink::robot::msg::GENERIC_REPLY packet2{};

    mavlink_msg_generic_reply_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.req_id, packet2.req_id);
    EXPECT_EQ(packet_in.error_code, packet2.error_code);
    EXPECT_EQ(packet_in.message, packet2.message);
    EXPECT_EQ(packet_in.msg_type, packet2.msg_type);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, HOVERBOARD_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::HOVERBOARD_EVENT packet_in{};
    packet_in.ctrl_mod = 17235;
    packet_in.ctrl_typ = 17339;
    packet_in.cur_mot_max = 17443;
    packet_in.rpm_mot_max = 17547;
    packet_in.fi_weak_ena = 17651;
    packet_in.fi_weak_hi = 17755;
    packet_in.fi_weak_lo = 17859;
    packet_in.fi_weak_max = 17963;
    packet_in.phase_adv_max_deg = 18067;
    packet_in.input1_raw = 18171;
    packet_in.input1_typ = 18275;
    packet_in.input1_min = 18379;
    packet_in.input1_mid = 18483;
    packet_in.input1_max = 18587;
    packet_in.input1_cmd = 18691;
    packet_in.input2_raw = 18795;
    packet_in.input2_typ = 18899;
    packet_in.input2_min = 19003;
    packet_in.input2_mid = 19107;
    packet_in.input2_max = 19211;
    packet_in.input2_cmd = 19315;
    packet_in.aux_input1_raw = 19419;
    packet_in.aux_input1_typ = 19523;
    packet_in.aux_input1_min = 19627;
    packet_in.aux_input1_mid = 19731;
    packet_in.aux_input1_max = 19835;
    packet_in.aux_input1_cmd = 19939;
    packet_in.aux_input2_raw = 20043;
    packet_in.aux_input2_typ = 147;
    packet_in.aux_input2_min = 20147;
    packet_in.aux_input2_mid = 20251;
    packet_in.aux_input2_max = 20355;
    packet_in.aux_input2_cmd = 20459;
    packet_in.dc_curr = 20563;
    packet_in.rdc_curr = 20667;
    packet_in.ldc_curr = 20771;
    packet_in.cmdl = 20875;
    packet_in.cmdr = 20979;
    packet_in.spd_avg = 21083;
    packet_in.spdl = 21187;
    packet_in.spdr = 21291;
    packet_in.filter_rate = 21395;
    packet_in.spd_coef = 21499;
    packet_in.str_coef = 21603;
    packet_in.batv = 21707;
    packet_in.temp = 21811;

    mavlink::robot::msg::HOVERBOARD_EVENT packet1{};
    mavlink::robot::msg::HOVERBOARD_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.ctrl_mod, packet2.ctrl_mod);
    EXPECT_EQ(packet1.ctrl_typ, packet2.ctrl_typ);
    EXPECT_EQ(packet1.cur_mot_max, packet2.cur_mot_max);
    EXPECT_EQ(packet1.rpm_mot_max, packet2.rpm_mot_max);
    EXPECT_EQ(packet1.fi_weak_ena, packet2.fi_weak_ena);
    EXPECT_EQ(packet1.fi_weak_hi, packet2.fi_weak_hi);
    EXPECT_EQ(packet1.fi_weak_lo, packet2.fi_weak_lo);
    EXPECT_EQ(packet1.fi_weak_max, packet2.fi_weak_max);
    EXPECT_EQ(packet1.phase_adv_max_deg, packet2.phase_adv_max_deg);
    EXPECT_EQ(packet1.input1_raw, packet2.input1_raw);
    EXPECT_EQ(packet1.input1_typ, packet2.input1_typ);
    EXPECT_EQ(packet1.input1_min, packet2.input1_min);
    EXPECT_EQ(packet1.input1_mid, packet2.input1_mid);
    EXPECT_EQ(packet1.input1_max, packet2.input1_max);
    EXPECT_EQ(packet1.input1_cmd, packet2.input1_cmd);
    EXPECT_EQ(packet1.input2_raw, packet2.input2_raw);
    EXPECT_EQ(packet1.input2_typ, packet2.input2_typ);
    EXPECT_EQ(packet1.input2_min, packet2.input2_min);
    EXPECT_EQ(packet1.input2_mid, packet2.input2_mid);
    EXPECT_EQ(packet1.input2_max, packet2.input2_max);
    EXPECT_EQ(packet1.input2_cmd, packet2.input2_cmd);
    EXPECT_EQ(packet1.aux_input1_raw, packet2.aux_input1_raw);
    EXPECT_EQ(packet1.aux_input1_typ, packet2.aux_input1_typ);
    EXPECT_EQ(packet1.aux_input1_min, packet2.aux_input1_min);
    EXPECT_EQ(packet1.aux_input1_mid, packet2.aux_input1_mid);
    EXPECT_EQ(packet1.aux_input1_max, packet2.aux_input1_max);
    EXPECT_EQ(packet1.aux_input1_cmd, packet2.aux_input1_cmd);
    EXPECT_EQ(packet1.aux_input2_raw, packet2.aux_input2_raw);
    EXPECT_EQ(packet1.aux_input2_typ, packet2.aux_input2_typ);
    EXPECT_EQ(packet1.aux_input2_min, packet2.aux_input2_min);
    EXPECT_EQ(packet1.aux_input2_mid, packet2.aux_input2_mid);
    EXPECT_EQ(packet1.aux_input2_max, packet2.aux_input2_max);
    EXPECT_EQ(packet1.aux_input2_cmd, packet2.aux_input2_cmd);
    EXPECT_EQ(packet1.dc_curr, packet2.dc_curr);
    EXPECT_EQ(packet1.rdc_curr, packet2.rdc_curr);
    EXPECT_EQ(packet1.ldc_curr, packet2.ldc_curr);
    EXPECT_EQ(packet1.cmdl, packet2.cmdl);
    EXPECT_EQ(packet1.cmdr, packet2.cmdr);
    EXPECT_EQ(packet1.spd_avg, packet2.spd_avg);
    EXPECT_EQ(packet1.spdl, packet2.spdl);
    EXPECT_EQ(packet1.spdr, packet2.spdr);
    EXPECT_EQ(packet1.filter_rate, packet2.filter_rate);
    EXPECT_EQ(packet1.spd_coef, packet2.spd_coef);
    EXPECT_EQ(packet1.str_coef, packet2.str_coef);
    EXPECT_EQ(packet1.batv, packet2.batv);
    EXPECT_EQ(packet1.temp, packet2.temp);
}

#ifdef TEST_INTEROP
TEST(robot_interop, HOVERBOARD_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_hoverboard_event_t packet_c {
         17235, 17339, 17443, 17547, 17651, 17755, 17859, 17963, 18067, 18171, 18275, 18379, 18483, 18587, 18691, 18795, 18899, 19003, 19107, 19211, 19315, 19419, 19523, 19627, 19731, 19835, 19939, 20043, 20147, 20251, 20355, 20459, 20563, 20667, 20771, 20875, 20979, 21083, 21187, 21291, 21395, 21499, 21603, 21707, 21811, 147
    };

    mavlink::robot::msg::HOVERBOARD_EVENT packet_in{};
    packet_in.ctrl_mod = 17235;
    packet_in.ctrl_typ = 17339;
    packet_in.cur_mot_max = 17443;
    packet_in.rpm_mot_max = 17547;
    packet_in.fi_weak_ena = 17651;
    packet_in.fi_weak_hi = 17755;
    packet_in.fi_weak_lo = 17859;
    packet_in.fi_weak_max = 17963;
    packet_in.phase_adv_max_deg = 18067;
    packet_in.input1_raw = 18171;
    packet_in.input1_typ = 18275;
    packet_in.input1_min = 18379;
    packet_in.input1_mid = 18483;
    packet_in.input1_max = 18587;
    packet_in.input1_cmd = 18691;
    packet_in.input2_raw = 18795;
    packet_in.input2_typ = 18899;
    packet_in.input2_min = 19003;
    packet_in.input2_mid = 19107;
    packet_in.input2_max = 19211;
    packet_in.input2_cmd = 19315;
    packet_in.aux_input1_raw = 19419;
    packet_in.aux_input1_typ = 19523;
    packet_in.aux_input1_min = 19627;
    packet_in.aux_input1_mid = 19731;
    packet_in.aux_input1_max = 19835;
    packet_in.aux_input1_cmd = 19939;
    packet_in.aux_input2_raw = 20043;
    packet_in.aux_input2_typ = 147;
    packet_in.aux_input2_min = 20147;
    packet_in.aux_input2_mid = 20251;
    packet_in.aux_input2_max = 20355;
    packet_in.aux_input2_cmd = 20459;
    packet_in.dc_curr = 20563;
    packet_in.rdc_curr = 20667;
    packet_in.ldc_curr = 20771;
    packet_in.cmdl = 20875;
    packet_in.cmdr = 20979;
    packet_in.spd_avg = 21083;
    packet_in.spdl = 21187;
    packet_in.spdr = 21291;
    packet_in.filter_rate = 21395;
    packet_in.spd_coef = 21499;
    packet_in.str_coef = 21603;
    packet_in.batv = 21707;
    packet_in.temp = 21811;

    mavlink::robot::msg::HOVERBOARD_EVENT packet2{};

    mavlink_msg_hoverboard_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.ctrl_mod, packet2.ctrl_mod);
    EXPECT_EQ(packet_in.ctrl_typ, packet2.ctrl_typ);
    EXPECT_EQ(packet_in.cur_mot_max, packet2.cur_mot_max);
    EXPECT_EQ(packet_in.rpm_mot_max, packet2.rpm_mot_max);
    EXPECT_EQ(packet_in.fi_weak_ena, packet2.fi_weak_ena);
    EXPECT_EQ(packet_in.fi_weak_hi, packet2.fi_weak_hi);
    EXPECT_EQ(packet_in.fi_weak_lo, packet2.fi_weak_lo);
    EXPECT_EQ(packet_in.fi_weak_max, packet2.fi_weak_max);
    EXPECT_EQ(packet_in.phase_adv_max_deg, packet2.phase_adv_max_deg);
    EXPECT_EQ(packet_in.input1_raw, packet2.input1_raw);
    EXPECT_EQ(packet_in.input1_typ, packet2.input1_typ);
    EXPECT_EQ(packet_in.input1_min, packet2.input1_min);
    EXPECT_EQ(packet_in.input1_mid, packet2.input1_mid);
    EXPECT_EQ(packet_in.input1_max, packet2.input1_max);
    EXPECT_EQ(packet_in.input1_cmd, packet2.input1_cmd);
    EXPECT_EQ(packet_in.input2_raw, packet2.input2_raw);
    EXPECT_EQ(packet_in.input2_typ, packet2.input2_typ);
    EXPECT_EQ(packet_in.input2_min, packet2.input2_min);
    EXPECT_EQ(packet_in.input2_mid, packet2.input2_mid);
    EXPECT_EQ(packet_in.input2_max, packet2.input2_max);
    EXPECT_EQ(packet_in.input2_cmd, packet2.input2_cmd);
    EXPECT_EQ(packet_in.aux_input1_raw, packet2.aux_input1_raw);
    EXPECT_EQ(packet_in.aux_input1_typ, packet2.aux_input1_typ);
    EXPECT_EQ(packet_in.aux_input1_min, packet2.aux_input1_min);
    EXPECT_EQ(packet_in.aux_input1_mid, packet2.aux_input1_mid);
    EXPECT_EQ(packet_in.aux_input1_max, packet2.aux_input1_max);
    EXPECT_EQ(packet_in.aux_input1_cmd, packet2.aux_input1_cmd);
    EXPECT_EQ(packet_in.aux_input2_raw, packet2.aux_input2_raw);
    EXPECT_EQ(packet_in.aux_input2_typ, packet2.aux_input2_typ);
    EXPECT_EQ(packet_in.aux_input2_min, packet2.aux_input2_min);
    EXPECT_EQ(packet_in.aux_input2_mid, packet2.aux_input2_mid);
    EXPECT_EQ(packet_in.aux_input2_max, packet2.aux_input2_max);
    EXPECT_EQ(packet_in.aux_input2_cmd, packet2.aux_input2_cmd);
    EXPECT_EQ(packet_in.dc_curr, packet2.dc_curr);
    EXPECT_EQ(packet_in.rdc_curr, packet2.rdc_curr);
    EXPECT_EQ(packet_in.ldc_curr, packet2.ldc_curr);
    EXPECT_EQ(packet_in.cmdl, packet2.cmdl);
    EXPECT_EQ(packet_in.cmdr, packet2.cmdr);
    EXPECT_EQ(packet_in.spd_avg, packet2.spd_avg);
    EXPECT_EQ(packet_in.spdl, packet2.spdl);
    EXPECT_EQ(packet_in.spdr, packet2.spdr);
    EXPECT_EQ(packet_in.filter_rate, packet2.filter_rate);
    EXPECT_EQ(packet_in.spd_coef, packet2.spd_coef);
    EXPECT_EQ(packet_in.str_coef, packet2.str_coef);
    EXPECT_EQ(packet_in.batv, packet2.batv);
    EXPECT_EQ(packet_in.temp, packet2.temp);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, CUTTER_REQUEST)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::CUTTER_REQUEST packet_in{};
    packet_in.enabled = 17;
    packet_in.rpm = 963497464;

    mavlink::robot::msg::CUTTER_REQUEST packet1{};
    mavlink::robot::msg::CUTTER_REQUEST packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.enabled, packet2.enabled);
    EXPECT_EQ(packet1.rpm, packet2.rpm);
}

#ifdef TEST_INTEROP
TEST(robot_interop, CUTTER_REQUEST)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_cutter_request_t packet_c {
         963497464, 17
    };

    mavlink::robot::msg::CUTTER_REQUEST packet_in{};
    packet_in.enabled = 17;
    packet_in.rpm = 963497464;

    mavlink::robot::msg::CUTTER_REQUEST packet2{};

    mavlink_msg_cutter_request_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.enabled, packet2.enabled);
    EXPECT_EQ(packet_in.rpm, packet2.rpm);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, CUTTER_REPLY)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::CUTTER_REPLY packet_in{};
    packet_in.req_id = 963497464;
    packet_in.error_code = 963497672;
    packet_in.message = to_char_array("MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFG");
    packet_in.msg_type = 963497880;

    mavlink::robot::msg::CUTTER_REPLY packet1{};
    mavlink::robot::msg::CUTTER_REPLY packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.req_id, packet2.req_id);
    EXPECT_EQ(packet1.error_code, packet2.error_code);
    EXPECT_EQ(packet1.message, packet2.message);
    EXPECT_EQ(packet1.msg_type, packet2.msg_type);
}

#ifdef TEST_INTEROP
TEST(robot_interop, CUTTER_REPLY)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_cutter_reply_t packet_c {
         963497464, 963497672, 963497880, "MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFG"
    };

    mavlink::robot::msg::CUTTER_REPLY packet_in{};
    packet_in.req_id = 963497464;
    packet_in.error_code = 963497672;
    packet_in.message = to_char_array("MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFG");
    packet_in.msg_type = 963497880;

    mavlink::robot::msg::CUTTER_REPLY packet2{};

    mavlink_msg_cutter_reply_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.req_id, packet2.req_id);
    EXPECT_EQ(packet_in.error_code, packet2.error_code);
    EXPECT_EQ(packet_in.message, packet2.message);
    EXPECT_EQ(packet_in.msg_type, packet2.msg_type);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, CUTTER_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::CUTTER_EVENT packet_in{};
    packet_in.enabled = 53;
    packet_in.rpm = 963497464;
    packet_in.current = 45.0;
    packet_in.voltage = 73.0;
    packet_in.temperature = 101.0;

    mavlink::robot::msg::CUTTER_EVENT packet1{};
    mavlink::robot::msg::CUTTER_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.enabled, packet2.enabled);
    EXPECT_EQ(packet1.rpm, packet2.rpm);
    EXPECT_EQ(packet1.current, packet2.current);
    EXPECT_EQ(packet1.voltage, packet2.voltage);
    EXPECT_EQ(packet1.temperature, packet2.temperature);
}

#ifdef TEST_INTEROP
TEST(robot_interop, CUTTER_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_cutter_event_t packet_c {
         963497464, 45.0, 73.0, 101.0, 53
    };

    mavlink::robot::msg::CUTTER_EVENT packet_in{};
    packet_in.enabled = 53;
    packet_in.rpm = 963497464;
    packet_in.current = 45.0;
    packet_in.voltage = 73.0;
    packet_in.temperature = 101.0;

    mavlink::robot::msg::CUTTER_EVENT packet2{};

    mavlink_msg_cutter_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.enabled, packet2.enabled);
    EXPECT_EQ(packet_in.rpm, packet2.rpm);
    EXPECT_EQ(packet_in.current, packet2.current);
    EXPECT_EQ(packet_in.voltage, packet2.voltage);
    EXPECT_EQ(packet_in.temperature, packet2.temperature);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, COMPASS_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::COMPASS_EVENT packet_in{};
    packet_in.heading = 17.0;
    packet_in.pitch = 45.0;
    packet_in.roll = 73.0;
    packet_in.mag_x = 101.0;
    packet_in.mag_y = 129.0;
    packet_in.mag_z = 157.0;
    packet_in.accel_x = 185.0;
    packet_in.accel_y = 213.0;
    packet_in.accel_z = 241.0;

    mavlink::robot::msg::COMPASS_EVENT packet1{};
    mavlink::robot::msg::COMPASS_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.heading, packet2.heading);
    EXPECT_EQ(packet1.pitch, packet2.pitch);
    EXPECT_EQ(packet1.roll, packet2.roll);
    EXPECT_EQ(packet1.mag_x, packet2.mag_x);
    EXPECT_EQ(packet1.mag_y, packet2.mag_y);
    EXPECT_EQ(packet1.mag_z, packet2.mag_z);
    EXPECT_EQ(packet1.accel_x, packet2.accel_x);
    EXPECT_EQ(packet1.accel_y, packet2.accel_y);
    EXPECT_EQ(packet1.accel_z, packet2.accel_z);
}

#ifdef TEST_INTEROP
TEST(robot_interop, COMPASS_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_compass_event_t packet_c {
         17.0, 45.0, 73.0, 101.0, 129.0, 157.0, 185.0, 213.0, 241.0
    };

    mavlink::robot::msg::COMPASS_EVENT packet_in{};
    packet_in.heading = 17.0;
    packet_in.pitch = 45.0;
    packet_in.roll = 73.0;
    packet_in.mag_x = 101.0;
    packet_in.mag_y = 129.0;
    packet_in.mag_z = 157.0;
    packet_in.accel_x = 185.0;
    packet_in.accel_y = 213.0;
    packet_in.accel_z = 241.0;

    mavlink::robot::msg::COMPASS_EVENT packet2{};

    mavlink_msg_compass_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.heading, packet2.heading);
    EXPECT_EQ(packet_in.pitch, packet2.pitch);
    EXPECT_EQ(packet_in.roll, packet2.roll);
    EXPECT_EQ(packet_in.mag_x, packet2.mag_x);
    EXPECT_EQ(packet_in.mag_y, packet2.mag_y);
    EXPECT_EQ(packet_in.mag_z, packet2.mag_z);
    EXPECT_EQ(packet_in.accel_x, packet2.accel_x);
    EXPECT_EQ(packet_in.accel_y, packet2.accel_y);
    EXPECT_EQ(packet_in.accel_z, packet2.accel_z);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, IMU_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::IMU_EVENT packet_in{};
    packet_in.gyro_x = 17.0;
    packet_in.gyro_y = 45.0;
    packet_in.gyro_z = 73.0;
    packet_in.accel_x = 101.0;
    packet_in.accel_y = 129.0;
    packet_in.accel_z = 157.0;

    mavlink::robot::msg::IMU_EVENT packet1{};
    mavlink::robot::msg::IMU_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.gyro_x, packet2.gyro_x);
    EXPECT_EQ(packet1.gyro_y, packet2.gyro_y);
    EXPECT_EQ(packet1.gyro_z, packet2.gyro_z);
    EXPECT_EQ(packet1.accel_x, packet2.accel_x);
    EXPECT_EQ(packet1.accel_y, packet2.accel_y);
    EXPECT_EQ(packet1.accel_z, packet2.accel_z);
}

#ifdef TEST_INTEROP
TEST(robot_interop, IMU_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_imu_event_t packet_c {
         17.0, 45.0, 73.0, 101.0, 129.0, 157.0
    };

    mavlink::robot::msg::IMU_EVENT packet_in{};
    packet_in.gyro_x = 17.0;
    packet_in.gyro_y = 45.0;
    packet_in.gyro_z = 73.0;
    packet_in.accel_x = 101.0;
    packet_in.accel_y = 129.0;
    packet_in.accel_z = 157.0;

    mavlink::robot::msg::IMU_EVENT packet2{};

    mavlink_msg_imu_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.gyro_x, packet2.gyro_x);
    EXPECT_EQ(packet_in.gyro_y, packet2.gyro_y);
    EXPECT_EQ(packet_in.gyro_z, packet2.gyro_z);
    EXPECT_EQ(packet_in.accel_x, packet2.accel_x);
    EXPECT_EQ(packet_in.accel_y, packet2.accel_y);
    EXPECT_EQ(packet_in.accel_z, packet2.accel_z);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, US_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::US_EVENT packet_in{};
    packet_in.distance = 17.0;
    packet_in.temperature = 45.0;
    packet_in.status = 963497880;

    mavlink::robot::msg::US_EVENT packet1{};
    mavlink::robot::msg::US_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.distance, packet2.distance);
    EXPECT_EQ(packet1.temperature, packet2.temperature);
    EXPECT_EQ(packet1.status, packet2.status);
}

#ifdef TEST_INTEROP
TEST(robot_interop, US_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_us_event_t packet_c {
         17.0, 45.0, 963497880
    };

    mavlink::robot::msg::US_EVENT packet_in{};
    packet_in.distance = 17.0;
    packet_in.temperature = 45.0;
    packet_in.status = 963497880;

    mavlink::robot::msg::US_EVENT packet2{};

    mavlink_msg_us_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.distance, packet2.distance);
    EXPECT_EQ(packet_in.temperature, packet2.temperature);
    EXPECT_EQ(packet_in.status, packet2.status);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, PS4_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::PS4_EVENT packet_in{};
    packet_in.button_left = 149;
    packet_in.button_right = 216;
    packet_in.button_up = 27;
    packet_in.button_down = 94;
    packet_in.button_square = 161;
    packet_in.button_cross = 228;
    packet_in.button_circle = 39;
    packet_in.button_triangle = 106;
    packet_in.button_left_shoulder = 173;
    packet_in.button_right_shoulder = 240;
    packet_in.button_left_trigger = 51;
    packet_in.button_right_trigger = 118;
    packet_in.button_left_joystick = 185;
    packet_in.button_right_joystick = 252;
    packet_in.button_share = 63;
    packet_in.button_options = 130;
    packet_in.button_touchpad = 197;
    packet_in.button_ps = 8;
    packet_in.axis_lx = 963497464;
    packet_in.axis_ly = 963497672;
    packet_in.axis_rx = 963497880;
    packet_in.axis_ry = 963498088;
    packet_in.gyro_x = 963498296;
    packet_in.gyro_y = 963498504;
    packet_in.gyro_z = 963498712;
    packet_in.accel_x = 963498920;
    packet_in.accel_y = 963499128;
    packet_in.accel_z = 963499336;
    packet_in.connected = 75;
    packet_in.battery_level = 963499544;
    packet_in.bluetooth = 142;
    packet_in.debug = to_char_array("QRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJK");
    packet_in.temp = 963499752;

    mavlink::robot::msg::PS4_EVENT packet1{};
    mavlink::robot::msg::PS4_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.button_left, packet2.button_left);
    EXPECT_EQ(packet1.button_right, packet2.button_right);
    EXPECT_EQ(packet1.button_up, packet2.button_up);
    EXPECT_EQ(packet1.button_down, packet2.button_down);
    EXPECT_EQ(packet1.button_square, packet2.button_square);
    EXPECT_EQ(packet1.button_cross, packet2.button_cross);
    EXPECT_EQ(packet1.button_circle, packet2.button_circle);
    EXPECT_EQ(packet1.button_triangle, packet2.button_triangle);
    EXPECT_EQ(packet1.button_left_shoulder, packet2.button_left_shoulder);
    EXPECT_EQ(packet1.button_right_shoulder, packet2.button_right_shoulder);
    EXPECT_EQ(packet1.button_left_trigger, packet2.button_left_trigger);
    EXPECT_EQ(packet1.button_right_trigger, packet2.button_right_trigger);
    EXPECT_EQ(packet1.button_left_joystick, packet2.button_left_joystick);
    EXPECT_EQ(packet1.button_right_joystick, packet2.button_right_joystick);
    EXPECT_EQ(packet1.button_share, packet2.button_share);
    EXPECT_EQ(packet1.button_options, packet2.button_options);
    EXPECT_EQ(packet1.button_touchpad, packet2.button_touchpad);
    EXPECT_EQ(packet1.button_ps, packet2.button_ps);
    EXPECT_EQ(packet1.axis_lx, packet2.axis_lx);
    EXPECT_EQ(packet1.axis_ly, packet2.axis_ly);
    EXPECT_EQ(packet1.axis_rx, packet2.axis_rx);
    EXPECT_EQ(packet1.axis_ry, packet2.axis_ry);
    EXPECT_EQ(packet1.gyro_x, packet2.gyro_x);
    EXPECT_EQ(packet1.gyro_y, packet2.gyro_y);
    EXPECT_EQ(packet1.gyro_z, packet2.gyro_z);
    EXPECT_EQ(packet1.accel_x, packet2.accel_x);
    EXPECT_EQ(packet1.accel_y, packet2.accel_y);
    EXPECT_EQ(packet1.accel_z, packet2.accel_z);
    EXPECT_EQ(packet1.connected, packet2.connected);
    EXPECT_EQ(packet1.battery_level, packet2.battery_level);
    EXPECT_EQ(packet1.bluetooth, packet2.bluetooth);
    EXPECT_EQ(packet1.debug, packet2.debug);
    EXPECT_EQ(packet1.temp, packet2.temp);
}

#ifdef TEST_INTEROP
TEST(robot_interop, PS4_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_ps4_event_t packet_c {
         963497464, 963497672, 963497880, 963498088, 963498296, 963498504, 963498712, 963498920, 963499128, 963499336, 963499544, 963499752, 149, 216, 27, 94, 161, 228, 39, 106, 173, 240, 51, 118, 185, 252, 63, 130, 197, 8, 75, 142, "QRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJK"
    };

    mavlink::robot::msg::PS4_EVENT packet_in{};
    packet_in.button_left = 149;
    packet_in.button_right = 216;
    packet_in.button_up = 27;
    packet_in.button_down = 94;
    packet_in.button_square = 161;
    packet_in.button_cross = 228;
    packet_in.button_circle = 39;
    packet_in.button_triangle = 106;
    packet_in.button_left_shoulder = 173;
    packet_in.button_right_shoulder = 240;
    packet_in.button_left_trigger = 51;
    packet_in.button_right_trigger = 118;
    packet_in.button_left_joystick = 185;
    packet_in.button_right_joystick = 252;
    packet_in.button_share = 63;
    packet_in.button_options = 130;
    packet_in.button_touchpad = 197;
    packet_in.button_ps = 8;
    packet_in.axis_lx = 963497464;
    packet_in.axis_ly = 963497672;
    packet_in.axis_rx = 963497880;
    packet_in.axis_ry = 963498088;
    packet_in.gyro_x = 963498296;
    packet_in.gyro_y = 963498504;
    packet_in.gyro_z = 963498712;
    packet_in.accel_x = 963498920;
    packet_in.accel_y = 963499128;
    packet_in.accel_z = 963499336;
    packet_in.connected = 75;
    packet_in.battery_level = 963499544;
    packet_in.bluetooth = 142;
    packet_in.debug = to_char_array("QRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJK");
    packet_in.temp = 963499752;

    mavlink::robot::msg::PS4_EVENT packet2{};

    mavlink_msg_ps4_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.button_left, packet2.button_left);
    EXPECT_EQ(packet_in.button_right, packet2.button_right);
    EXPECT_EQ(packet_in.button_up, packet2.button_up);
    EXPECT_EQ(packet_in.button_down, packet2.button_down);
    EXPECT_EQ(packet_in.button_square, packet2.button_square);
    EXPECT_EQ(packet_in.button_cross, packet2.button_cross);
    EXPECT_EQ(packet_in.button_circle, packet2.button_circle);
    EXPECT_EQ(packet_in.button_triangle, packet2.button_triangle);
    EXPECT_EQ(packet_in.button_left_shoulder, packet2.button_left_shoulder);
    EXPECT_EQ(packet_in.button_right_shoulder, packet2.button_right_shoulder);
    EXPECT_EQ(packet_in.button_left_trigger, packet2.button_left_trigger);
    EXPECT_EQ(packet_in.button_right_trigger, packet2.button_right_trigger);
    EXPECT_EQ(packet_in.button_left_joystick, packet2.button_left_joystick);
    EXPECT_EQ(packet_in.button_right_joystick, packet2.button_right_joystick);
    EXPECT_EQ(packet_in.button_share, packet2.button_share);
    EXPECT_EQ(packet_in.button_options, packet2.button_options);
    EXPECT_EQ(packet_in.button_touchpad, packet2.button_touchpad);
    EXPECT_EQ(packet_in.button_ps, packet2.button_ps);
    EXPECT_EQ(packet_in.axis_lx, packet2.axis_lx);
    EXPECT_EQ(packet_in.axis_ly, packet2.axis_ly);
    EXPECT_EQ(packet_in.axis_rx, packet2.axis_rx);
    EXPECT_EQ(packet_in.axis_ry, packet2.axis_ry);
    EXPECT_EQ(packet_in.gyro_x, packet2.gyro_x);
    EXPECT_EQ(packet_in.gyro_y, packet2.gyro_y);
    EXPECT_EQ(packet_in.gyro_z, packet2.gyro_z);
    EXPECT_EQ(packet_in.accel_x, packet2.accel_x);
    EXPECT_EQ(packet_in.accel_y, packet2.accel_y);
    EXPECT_EQ(packet_in.accel_z, packet2.accel_z);
    EXPECT_EQ(packet_in.connected, packet2.connected);
    EXPECT_EQ(packet_in.battery_level, packet2.battery_level);
    EXPECT_EQ(packet_in.bluetooth, packet2.bluetooth);
    EXPECT_EQ(packet_in.debug, packet2.debug);
    EXPECT_EQ(packet_in.temp, packet2.temp);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, PS4_REQUEST)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::PS4_REQUEST packet_in{};
    packet_in.req_id = 963497464;
    packet_in.rumble_small = 963497672;
    packet_in.rumble_large = 963497880;
    packet_in.led_red = 963498088;
    packet_in.led_green = 963498296;
    packet_in.led_blue = 963498504;
    packet_in.led_flash_on = 963498712;
    packet_in.led_flash_off = 963498920;

    mavlink::robot::msg::PS4_REQUEST packet1{};
    mavlink::robot::msg::PS4_REQUEST packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.req_id, packet2.req_id);
    EXPECT_EQ(packet1.rumble_small, packet2.rumble_small);
    EXPECT_EQ(packet1.rumble_large, packet2.rumble_large);
    EXPECT_EQ(packet1.led_red, packet2.led_red);
    EXPECT_EQ(packet1.led_green, packet2.led_green);
    EXPECT_EQ(packet1.led_blue, packet2.led_blue);
    EXPECT_EQ(packet1.led_flash_on, packet2.led_flash_on);
    EXPECT_EQ(packet1.led_flash_off, packet2.led_flash_off);
}

#ifdef TEST_INTEROP
TEST(robot_interop, PS4_REQUEST)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_ps4_request_t packet_c {
         963497464, 963497672, 963497880, 963498088, 963498296, 963498504, 963498712, 963498920
    };

    mavlink::robot::msg::PS4_REQUEST packet_in{};
    packet_in.req_id = 963497464;
    packet_in.rumble_small = 963497672;
    packet_in.rumble_large = 963497880;
    packet_in.led_red = 963498088;
    packet_in.led_green = 963498296;
    packet_in.led_blue = 963498504;
    packet_in.led_flash_on = 963498712;
    packet_in.led_flash_off = 963498920;

    mavlink::robot::msg::PS4_REQUEST packet2{};

    mavlink_msg_ps4_request_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.req_id, packet2.req_id);
    EXPECT_EQ(packet_in.rumble_small, packet2.rumble_small);
    EXPECT_EQ(packet_in.rumble_large, packet2.rumble_large);
    EXPECT_EQ(packet_in.led_red, packet2.led_red);
    EXPECT_EQ(packet_in.led_green, packet2.led_green);
    EXPECT_EQ(packet_in.led_blue, packet2.led_blue);
    EXPECT_EQ(packet_in.led_flash_on, packet2.led_flash_on);
    EXPECT_EQ(packet_in.led_flash_off, packet2.led_flash_off);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, SYS_REQUEST)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::SYS_REQUEST packet_in{};
    packet_in.req_id = 963497880;
    packet_in.set_time = 93372036854775807ULL;
    packet_in.reboot = 41;
    packet_in.console = to_char_array("NOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGH");

    mavlink::robot::msg::SYS_REQUEST packet1{};
    mavlink::robot::msg::SYS_REQUEST packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.req_id, packet2.req_id);
    EXPECT_EQ(packet1.set_time, packet2.set_time);
    EXPECT_EQ(packet1.reboot, packet2.reboot);
    EXPECT_EQ(packet1.console, packet2.console);
}

#ifdef TEST_INTEROP
TEST(robot_interop, SYS_REQUEST)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_sys_request_t packet_c {
         93372036854775807ULL, 963497880, 41, "NOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGH"
    };

    mavlink::robot::msg::SYS_REQUEST packet_in{};
    packet_in.req_id = 963497880;
    packet_in.set_time = 93372036854775807ULL;
    packet_in.reboot = 41;
    packet_in.console = to_char_array("NOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGH");

    mavlink::robot::msg::SYS_REQUEST packet2{};

    mavlink_msg_sys_request_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.req_id, packet2.req_id);
    EXPECT_EQ(packet_in.set_time, packet2.set_time);
    EXPECT_EQ(packet_in.reboot, packet2.reboot);
    EXPECT_EQ(packet_in.console, packet2.console);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, SYS_REPLY)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::SYS_REPLY packet_in{};
    packet_in.req_id = 963497464;
    packet_in.rc = 963497672;
    packet_in.message = to_char_array("IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC");

    mavlink::robot::msg::SYS_REPLY packet1{};
    mavlink::robot::msg::SYS_REPLY packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.req_id, packet2.req_id);
    EXPECT_EQ(packet1.rc, packet2.rc);
    EXPECT_EQ(packet1.message, packet2.message);
}

#ifdef TEST_INTEROP
TEST(robot_interop, SYS_REPLY)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_sys_reply_t packet_c {
         963497464, 963497672, "IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC"
    };

    mavlink::robot::msg::SYS_REPLY packet_in{};
    packet_in.req_id = 963497464;
    packet_in.rc = 963497672;
    packet_in.message = to_char_array("IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC");

    mavlink::robot::msg::SYS_REPLY packet2{};

    mavlink_msg_sys_reply_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.req_id, packet2.req_id);
    EXPECT_EQ(packet_in.rc, packet2.rc);
    EXPECT_EQ(packet_in.message, packet2.message);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, SYS_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::SYS_EVENT packet_in{};
    packet_in.utc = 93372036854775807ULL;
    packet_in.uptime = 93372036854776311ULL;
    packet_in.free_heap = 93372036854776815ULL;
    packet_in.flash_size = 93372036854777319ULL;
    packet_in.cpu_board_type = to_char_array("GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC");
    packet_in.build_date_time = to_char_array("EFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZA");

    mavlink::robot::msg::SYS_EVENT packet1{};
    mavlink::robot::msg::SYS_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.utc, packet2.utc);
    EXPECT_EQ(packet1.uptime, packet2.uptime);
    EXPECT_EQ(packet1.free_heap, packet2.free_heap);
    EXPECT_EQ(packet1.flash_size, packet2.flash_size);
    EXPECT_EQ(packet1.cpu_board_type, packet2.cpu_board_type);
    EXPECT_EQ(packet1.build_date_time, packet2.build_date_time);
}

#ifdef TEST_INTEROP
TEST(robot_interop, SYS_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_sys_event_t packet_c {
         93372036854775807ULL, 93372036854776311ULL, 93372036854776815ULL, 93372036854777319ULL, "GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC", "EFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZA"
    };

    mavlink::robot::msg::SYS_EVENT packet_in{};
    packet_in.utc = 93372036854775807ULL;
    packet_in.uptime = 93372036854776311ULL;
    packet_in.free_heap = 93372036854776815ULL;
    packet_in.flash_size = 93372036854777319ULL;
    packet_in.cpu_board_type = to_char_array("GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC");
    packet_in.build_date_time = to_char_array("EFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZA");

    mavlink::robot::msg::SYS_EVENT packet2{};

    mavlink_msg_sys_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.utc, packet2.utc);
    EXPECT_EQ(packet_in.uptime, packet2.uptime);
    EXPECT_EQ(packet_in.free_heap, packet2.free_heap);
    EXPECT_EQ(packet_in.flash_size, packet2.flash_size);
    EXPECT_EQ(packet_in.cpu_board_type, packet2.cpu_board_type);
    EXPECT_EQ(packet_in.build_date_time, packet2.build_date_time);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, PING_REQUEST)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::PING_REQUEST packet_in{};
    packet_in.req_id = 963497880;
    packet_in.timestamp = 93372036854775807ULL;

    mavlink::robot::msg::PING_REQUEST packet1{};
    mavlink::robot::msg::PING_REQUEST packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.req_id, packet2.req_id);
    EXPECT_EQ(packet1.timestamp, packet2.timestamp);
}

#ifdef TEST_INTEROP
TEST(robot_interop, PING_REQUEST)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_ping_request_t packet_c {
         93372036854775807ULL, 963497880
    };

    mavlink::robot::msg::PING_REQUEST packet_in{};
    packet_in.req_id = 963497880;
    packet_in.timestamp = 93372036854775807ULL;

    mavlink::robot::msg::PING_REQUEST packet2{};

    mavlink_msg_ping_request_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.req_id, packet2.req_id);
    EXPECT_EQ(packet_in.timestamp, packet2.timestamp);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, PING_REPLY)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::PING_REPLY packet_in{};
    packet_in.req_id = 963497880;
    packet_in.timestamp = 93372036854775807ULL;

    mavlink::robot::msg::PING_REPLY packet1{};
    mavlink::robot::msg::PING_REPLY packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.req_id, packet2.req_id);
    EXPECT_EQ(packet1.timestamp, packet2.timestamp);
}

#ifdef TEST_INTEROP
TEST(robot_interop, PING_REPLY)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_ping_reply_t packet_c {
         93372036854775807ULL, 963497880
    };

    mavlink::robot::msg::PING_REPLY packet_in{};
    packet_in.req_id = 963497880;
    packet_in.timestamp = 93372036854775807ULL;

    mavlink::robot::msg::PING_REPLY packet2{};

    mavlink_msg_ping_reply_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.req_id, packet2.req_id);
    EXPECT_EQ(packet_in.timestamp, packet2.timestamp);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, BROKER_SUBSCRIBE_REQUEST)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::BROKER_SUBSCRIBE_REQUEST packet_in{};
    packet_in.src = 963497464;
    packet_in.msg_type = 963497672;

    mavlink::robot::msg::BROKER_SUBSCRIBE_REQUEST packet1{};
    mavlink::robot::msg::BROKER_SUBSCRIBE_REQUEST packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.src, packet2.src);
    EXPECT_EQ(packet1.msg_type, packet2.msg_type);
}

#ifdef TEST_INTEROP
TEST(robot_interop, BROKER_SUBSCRIBE_REQUEST)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_broker_subscribe_request_t packet_c {
         963497464, 963497672
    };

    mavlink::robot::msg::BROKER_SUBSCRIBE_REQUEST packet_in{};
    packet_in.src = 963497464;
    packet_in.msg_type = 963497672;

    mavlink::robot::msg::BROKER_SUBSCRIBE_REQUEST packet2{};

    mavlink_msg_broker_subscribe_request_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.src, packet2.src);
    EXPECT_EQ(packet_in.msg_type, packet2.msg_type);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, WIFI_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::WIFI_EVENT packet_in{};
    packet_in.ip = to_char_array("IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE");
    packet_in.gateway = to_char_array("GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC");
    packet_in.netmask = to_char_array("EFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZA");
    packet_in.ssid = to_char_array("CDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXY");
    packet_in.bssid = to_char_array("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW");
    packet_in.channel = 963497464;
    packet_in.rssi = 963497672;
    packet_in.mac = to_char_array("YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTU");

    mavlink::robot::msg::WIFI_EVENT packet1{};
    mavlink::robot::msg::WIFI_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.ip, packet2.ip);
    EXPECT_EQ(packet1.gateway, packet2.gateway);
    EXPECT_EQ(packet1.netmask, packet2.netmask);
    EXPECT_EQ(packet1.ssid, packet2.ssid);
    EXPECT_EQ(packet1.bssid, packet2.bssid);
    EXPECT_EQ(packet1.channel, packet2.channel);
    EXPECT_EQ(packet1.rssi, packet2.rssi);
    EXPECT_EQ(packet1.mac, packet2.mac);
}

#ifdef TEST_INTEROP
TEST(robot_interop, WIFI_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_wifi_event_t packet_c {
         963497464, 963497672, "IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE", "GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC", "EFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZA", "CDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXY", "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW", "YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTU"
    };

    mavlink::robot::msg::WIFI_EVENT packet_in{};
    packet_in.ip = to_char_array("IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE");
    packet_in.gateway = to_char_array("GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC");
    packet_in.netmask = to_char_array("EFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZA");
    packet_in.ssid = to_char_array("CDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXY");
    packet_in.bssid = to_char_array("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW");
    packet_in.channel = 963497464;
    packet_in.rssi = 963497672;
    packet_in.mac = to_char_array("YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTU");

    mavlink::robot::msg::WIFI_EVENT packet2{};

    mavlink_msg_wifi_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.ip, packet2.ip);
    EXPECT_EQ(packet_in.gateway, packet2.gateway);
    EXPECT_EQ(packet_in.netmask, packet2.netmask);
    EXPECT_EQ(packet_in.ssid, packet2.ssid);
    EXPECT_EQ(packet_in.bssid, packet2.bssid);
    EXPECT_EQ(packet_in.channel, packet2.channel);
    EXPECT_EQ(packet_in.rssi, packet2.rssi);
    EXPECT_EQ(packet_in.mac, packet2.mac);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, DEVICE_ALIVE_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::DEVICE_ALIVE_EVENT packet_in{};
    packet_in.device = to_char_array("IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE");
    packet_in.endpoint = to_char_array("GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC");
    packet_in.timestamp = 93372036854775807ULL;

    mavlink::robot::msg::DEVICE_ALIVE_EVENT packet1{};
    mavlink::robot::msg::DEVICE_ALIVE_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.device, packet2.device);
    EXPECT_EQ(packet1.endpoint, packet2.endpoint);
    EXPECT_EQ(packet1.timestamp, packet2.timestamp);
}

#ifdef TEST_INTEROP
TEST(robot_interop, DEVICE_ALIVE_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_device_alive_event_t packet_c {
         93372036854775807ULL, "IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE", "GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC"
    };

    mavlink::robot::msg::DEVICE_ALIVE_EVENT packet_in{};
    packet_in.device = to_char_array("IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE");
    packet_in.endpoint = to_char_array("GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC");
    packet_in.timestamp = 93372036854775807ULL;

    mavlink::robot::msg::DEVICE_ALIVE_EVENT packet2{};

    mavlink_msg_device_alive_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.device, packet2.device);
    EXPECT_EQ(packet_in.endpoint, packet2.endpoint);
    EXPECT_EQ(packet_in.timestamp, packet2.timestamp);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, MAX31855_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::MAX31855_EVENT packet_in{};
    packet_in.thermocouple_temp = 17.0;
    packet_in.internal_temp = 45.0;
    packet_in.fault = 29;
    packet_in.fault_short_vcc = 96;
    packet_in.fault_short_gnd = 163;
    packet_in.fault_open_tc = 230;

    mavlink::robot::msg::MAX31855_EVENT packet1{};
    mavlink::robot::msg::MAX31855_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.thermocouple_temp, packet2.thermocouple_temp);
    EXPECT_EQ(packet1.internal_temp, packet2.internal_temp);
    EXPECT_EQ(packet1.fault, packet2.fault);
    EXPECT_EQ(packet1.fault_short_vcc, packet2.fault_short_vcc);
    EXPECT_EQ(packet1.fault_short_gnd, packet2.fault_short_gnd);
    EXPECT_EQ(packet1.fault_open_tc, packet2.fault_open_tc);
}

#ifdef TEST_INTEROP
TEST(robot_interop, MAX31855_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_max31855_event_t packet_c {
         17.0, 45.0, 29, 96, 163, 230
    };

    mavlink::robot::msg::MAX31855_EVENT packet_in{};
    packet_in.thermocouple_temp = 17.0;
    packet_in.internal_temp = 45.0;
    packet_in.fault = 29;
    packet_in.fault_short_vcc = 96;
    packet_in.fault_short_gnd = 163;
    packet_in.fault_open_tc = 230;

    mavlink::robot::msg::MAX31855_EVENT packet2{};

    mavlink_msg_max31855_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.thermocouple_temp, packet2.thermocouple_temp);
    EXPECT_EQ(packet_in.internal_temp, packet2.internal_temp);
    EXPECT_EQ(packet_in.fault, packet2.fault);
    EXPECT_EQ(packet_in.fault_short_vcc, packet2.fault_short_vcc);
    EXPECT_EQ(packet_in.fault_short_gnd, packet2.fault_short_gnd);
    EXPECT_EQ(packet_in.fault_open_tc, packet2.fault_open_tc);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, HEATING_EVENT)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::HEATING_EVENT packet_in{};
    packet_in.temperature_c = 73.0;
    packet_in.setpoint_c = 101.0;
    packet_in.enabled = 65;
    packet_in.output_pct = 129.0;
    packet_in.heater_on = 132;
    packet_in.fault = 199;
    packet_in.timestamp_ms = 93372036854775807ULL;

    mavlink::robot::msg::HEATING_EVENT packet1{};
    mavlink::robot::msg::HEATING_EVENT packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.temperature_c, packet2.temperature_c);
    EXPECT_EQ(packet1.setpoint_c, packet2.setpoint_c);
    EXPECT_EQ(packet1.enabled, packet2.enabled);
    EXPECT_EQ(packet1.output_pct, packet2.output_pct);
    EXPECT_EQ(packet1.heater_on, packet2.heater_on);
    EXPECT_EQ(packet1.fault, packet2.fault);
    EXPECT_EQ(packet1.timestamp_ms, packet2.timestamp_ms);
}

#ifdef TEST_INTEROP
TEST(robot_interop, HEATING_EVENT)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_heating_event_t packet_c {
         93372036854775807ULL, 73.0, 101.0, 129.0, 65, 132, 199
    };

    mavlink::robot::msg::HEATING_EVENT packet_in{};
    packet_in.temperature_c = 73.0;
    packet_in.setpoint_c = 101.0;
    packet_in.enabled = 65;
    packet_in.output_pct = 129.0;
    packet_in.heater_on = 132;
    packet_in.fault = 199;
    packet_in.timestamp_ms = 93372036854775807ULL;

    mavlink::robot::msg::HEATING_EVENT packet2{};

    mavlink_msg_heating_event_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.temperature_c, packet2.temperature_c);
    EXPECT_EQ(packet_in.setpoint_c, packet2.setpoint_c);
    EXPECT_EQ(packet_in.enabled, packet2.enabled);
    EXPECT_EQ(packet_in.output_pct, packet2.output_pct);
    EXPECT_EQ(packet_in.heater_on, packet2.heater_on);
    EXPECT_EQ(packet_in.fault, packet2.fault);
    EXPECT_EQ(packet_in.timestamp_ms, packet2.timestamp_ms);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, HEATING_REQUEST)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::HEATING_REQUEST packet_in{};
    packet_in.setpoint_c = 17.0;
    packet_in.enabled = 53;
    packet_in.kp = 45.0;
    packet_in.ki = 73.0;
    packet_in.kd = 101.0;
    packet_in.reset_integral = 120;

    mavlink::robot::msg::HEATING_REQUEST packet1{};
    mavlink::robot::msg::HEATING_REQUEST packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.setpoint_c, packet2.setpoint_c);
    EXPECT_EQ(packet1.enabled, packet2.enabled);
    EXPECT_EQ(packet1.kp, packet2.kp);
    EXPECT_EQ(packet1.ki, packet2.ki);
    EXPECT_EQ(packet1.kd, packet2.kd);
    EXPECT_EQ(packet1.reset_integral, packet2.reset_integral);
}

#ifdef TEST_INTEROP
TEST(robot_interop, HEATING_REQUEST)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_heating_request_t packet_c {
         17.0, 45.0, 73.0, 101.0, 53, 120
    };

    mavlink::robot::msg::HEATING_REQUEST packet_in{};
    packet_in.setpoint_c = 17.0;
    packet_in.enabled = 53;
    packet_in.kp = 45.0;
    packet_in.ki = 73.0;
    packet_in.kd = 101.0;
    packet_in.reset_integral = 120;

    mavlink::robot::msg::HEATING_REQUEST packet2{};

    mavlink_msg_heating_request_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.setpoint_c, packet2.setpoint_c);
    EXPECT_EQ(packet_in.enabled, packet2.enabled);
    EXPECT_EQ(packet_in.kp, packet2.kp);
    EXPECT_EQ(packet_in.ki, packet2.ki);
    EXPECT_EQ(packet_in.kd, packet2.kd);
    EXPECT_EQ(packet_in.reset_integral, packet2.reset_integral);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, ENVELOPE)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::ENVELOPE packet_in{};
    packet_in.src = 963497464;
    packet_in.dst = 963497672;
    packet_in.msg_type = 963497880;
    packet_in.request_id = 963498088;
    packet_in.instance_id = 963498296;
    packet_in.payload = {{ 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 }};

    mavlink::robot::msg::ENVELOPE packet1{};
    mavlink::robot::msg::ENVELOPE packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.src, packet2.src);
    EXPECT_EQ(packet1.dst, packet2.dst);
    EXPECT_EQ(packet1.msg_type, packet2.msg_type);
    EXPECT_EQ(packet1.request_id, packet2.request_id);
    EXPECT_EQ(packet1.instance_id, packet2.instance_id);
    EXPECT_EQ(packet1.payload, packet2.payload);
}

#ifdef TEST_INTEROP
TEST(robot_interop, ENVELOPE)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_envelope_t packet_c {
         963497464, 963497672, 963497880, 963498088, 963498296, { 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 }
    };

    mavlink::robot::msg::ENVELOPE packet_in{};
    packet_in.src = 963497464;
    packet_in.dst = 963497672;
    packet_in.msg_type = 963497880;
    packet_in.request_id = 963498088;
    packet_in.instance_id = 963498296;
    packet_in.payload = {{ 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 }};

    mavlink::robot::msg::ENVELOPE packet2{};

    mavlink_msg_envelope_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.src, packet2.src);
    EXPECT_EQ(packet_in.dst, packet2.dst);
    EXPECT_EQ(packet_in.msg_type, packet2.msg_type);
    EXPECT_EQ(packet_in.request_id, packet2.request_id);
    EXPECT_EQ(packet_in.instance_id, packet2.instance_id);
    EXPECT_EQ(packet_in.payload, packet2.payload);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, ENDPOINT_ANNOUNCE)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::ENDPOINT_ANNOUNCE packet_in{};
    packet_in.id = 963497464;
    packet_in.name = to_char_array("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW");
    packet_in.description = to_char_array("YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRS");
    packet_in.services = {{ 963497672, 963497673, 963497674, 963497675, 963497676, 963497677, 963497678, 963497679, 963497680, 963497681, 963497682, 963497683, 963497684, 963497685, 963497686, 963497687 }};
    packet_in.events = {{ 963501000, 963501001, 963501002, 963501003, 963501004, 963501005, 963501006, 963501007, 963501008, 963501009, 963501010, 963501011, 963501012, 963501013, 963501014, 963501015 }};
    packet_in.replies = {{ 963504328, 963504329, 963504330, 963504331, 963504332, 963504333, 963504334, 963504335, 963504336, 963504337, 963504338, 963504339, 963504340, 963504341, 963504342, 963504343 }};
    packet_in.subscribes = {{ 963507656, 963507657, 963507658, 963507659, 963507660, 963507661, 963507662, 963507663, 963507664, 963507665, 963507666, 963507667, 963507668, 963507669, 963507670, 963507671 }};

    mavlink::robot::msg::ENDPOINT_ANNOUNCE packet1{};
    mavlink::robot::msg::ENDPOINT_ANNOUNCE packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.id, packet2.id);
    EXPECT_EQ(packet1.name, packet2.name);
    EXPECT_EQ(packet1.description, packet2.description);
    EXPECT_EQ(packet1.services, packet2.services);
    EXPECT_EQ(packet1.events, packet2.events);
    EXPECT_EQ(packet1.replies, packet2.replies);
    EXPECT_EQ(packet1.subscribes, packet2.subscribes);
}

#ifdef TEST_INTEROP
TEST(robot_interop, ENDPOINT_ANNOUNCE)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_endpoint_announce_t packet_c {
         963497464, { 963497672, 963497673, 963497674, 963497675, 963497676, 963497677, 963497678, 963497679, 963497680, 963497681, 963497682, 963497683, 963497684, 963497685, 963497686, 963497687 }, { 963501000, 963501001, 963501002, 963501003, 963501004, 963501005, 963501006, 963501007, 963501008, 963501009, 963501010, 963501011, 963501012, 963501013, 963501014, 963501015 }, { 963504328, 963504329, 963504330, 963504331, 963504332, 963504333, 963504334, 963504335, 963504336, 963504337, 963504338, 963504339, 963504340, 963504341, 963504342, 963504343 }, { 963507656, 963507657, 963507658, 963507659, 963507660, 963507661, 963507662, 963507663, 963507664, 963507665, 963507666, 963507667, 963507668, 963507669, 963507670, 963507671 }, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW", "YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRS"
    };

    mavlink::robot::msg::ENDPOINT_ANNOUNCE packet_in{};
    packet_in.id = 963497464;
    packet_in.name = to_char_array("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW");
    packet_in.description = to_char_array("YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRS");
    packet_in.services = {{ 963497672, 963497673, 963497674, 963497675, 963497676, 963497677, 963497678, 963497679, 963497680, 963497681, 963497682, 963497683, 963497684, 963497685, 963497686, 963497687 }};
    packet_in.events = {{ 963501000, 963501001, 963501002, 963501003, 963501004, 963501005, 963501006, 963501007, 963501008, 963501009, 963501010, 963501011, 963501012, 963501013, 963501014, 963501015 }};
    packet_in.replies = {{ 963504328, 963504329, 963504330, 963504331, 963504332, 963504333, 963504334, 963504335, 963504336, 963504337, 963504338, 963504339, 963504340, 963504341, 963504342, 963504343 }};
    packet_in.subscribes = {{ 963507656, 963507657, 963507658, 963507659, 963507660, 963507661, 963507662, 963507663, 963507664, 963507665, 963507666, 963507667, 963507668, 963507669, 963507670, 963507671 }};

    mavlink::robot::msg::ENDPOINT_ANNOUNCE packet2{};

    mavlink_msg_endpoint_announce_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.id, packet2.id);
    EXPECT_EQ(packet_in.name, packet2.name);
    EXPECT_EQ(packet_in.description, packet2.description);
    EXPECT_EQ(packet_in.services, packet2.services);
    EXPECT_EQ(packet_in.events, packet2.events);
    EXPECT_EQ(packet_in.replies, packet2.replies);
    EXPECT_EQ(packet_in.subscribes, packet2.subscribes);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(robot, ENDPOINT_ANNOUNCE_REPLY)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::robot::msg::ENDPOINT_ANNOUNCE_REPLY packet_in{};
    packet_in.utc = 93372036854775807ULL;

    mavlink::robot::msg::ENDPOINT_ANNOUNCE_REPLY packet1{};
    mavlink::robot::msg::ENDPOINT_ANNOUNCE_REPLY packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.utc, packet2.utc);
}

#ifdef TEST_INTEROP
TEST(robot_interop, ENDPOINT_ANNOUNCE_REPLY)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_endpoint_announce_reply_t packet_c {
         93372036854775807ULL
    };

    mavlink::robot::msg::ENDPOINT_ANNOUNCE_REPLY packet_in{};
    packet_in.utc = 93372036854775807ULL;

    mavlink::robot::msg::ENDPOINT_ANNOUNCE_REPLY packet2{};

    mavlink_msg_endpoint_announce_reply_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.utc, packet2.utc);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif
