/** @file
 *    @brief MAVLink comm protocol testsuite generated from robot.xml
 *    @see https://mavlink.io/en/
 */
#pragma once
#ifndef ROBOT_TESTSUITE_H
#define ROBOT_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL

static void mavlink_test_robot(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{

    mavlink_test_robot(system_id, component_id, last_msg);
}
#endif




static void mavlink_test_hoverboard_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HOVERBOARD_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_hoverboard_request_t packet_in = {
        963497464,963497672
    };
    mavlink_hoverboard_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.speed = packet_in.speed;
        packet1.steer = packet_in.steer;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HOVERBOARD_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HOVERBOARD_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_hoverboard_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_request_pack(system_id, component_id, &msg , packet1.speed , packet1.steer );
    mavlink_msg_hoverboard_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.speed , packet1.steer );
    mavlink_msg_hoverboard_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_hoverboard_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_request_send(MAVLINK_COMM_1 , packet1.speed , packet1.steer );
    mavlink_msg_hoverboard_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("HOVERBOARD_REQUEST") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_HOVERBOARD_REQUEST) != NULL);
#endif
}

static void mavlink_test_hoverboard_reply(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HOVERBOARD_REPLY >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_hoverboard_reply_t packet_in = {
        963497464,963497672
    };
    mavlink_hoverboard_reply_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.speed = packet_in.speed;
        packet1.steer = packet_in.steer;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HOVERBOARD_REPLY_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_reply_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_hoverboard_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_reply_pack(system_id, component_id, &msg , packet1.speed , packet1.steer );
    mavlink_msg_hoverboard_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_reply_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.speed , packet1.steer );
    mavlink_msg_hoverboard_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_hoverboard_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_reply_send(MAVLINK_COMM_1 , packet1.speed , packet1.steer );
    mavlink_msg_hoverboard_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("HOVERBOARD_REPLY") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_HOVERBOARD_REPLY) != NULL);
#endif
}

static void mavlink_test_generic_reply(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_GENERIC_REPLY >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_generic_reply_t packet_in = {
        963497464,963497672,963497880,"MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFG"
    };
    mavlink_generic_reply_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.req_id = packet_in.req_id;
        packet1.error_code = packet_in.error_code;
        packet1.msg_type = packet_in.msg_type;
        
        mav_array_memcpy(packet1.message, packet_in.message, sizeof(char)*100);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_GENERIC_REPLY_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_GENERIC_REPLY_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_generic_reply_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_generic_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_generic_reply_pack(system_id, component_id, &msg , packet1.req_id , packet1.error_code , packet1.message , packet1.msg_type );
    mavlink_msg_generic_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_generic_reply_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.req_id , packet1.error_code , packet1.message , packet1.msg_type );
    mavlink_msg_generic_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_generic_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_generic_reply_send(MAVLINK_COMM_1 , packet1.req_id , packet1.error_code , packet1.message , packet1.msg_type );
    mavlink_msg_generic_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("GENERIC_REPLY") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_GENERIC_REPLY) != NULL);
#endif
}

static void mavlink_test_hoverboard_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HOVERBOARD_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_hoverboard_event_t packet_in = {
        17235,17339,17443,17547,17651,17755,17859,17963,18067,18171,18275,18379,18483,18587,18691,18795,18899,19003,19107,19211,19315,19419,19523,19627,19731,19835,19939,20043,20147,20251,20355,20459,20563,20667,20771,20875,20979,21083,21187,21291,21395,21499,21603,21707,21811,147
    };
    mavlink_hoverboard_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.ctrl_mod = packet_in.ctrl_mod;
        packet1.ctrl_typ = packet_in.ctrl_typ;
        packet1.cur_mot_max = packet_in.cur_mot_max;
        packet1.rpm_mot_max = packet_in.rpm_mot_max;
        packet1.fi_weak_ena = packet_in.fi_weak_ena;
        packet1.fi_weak_hi = packet_in.fi_weak_hi;
        packet1.fi_weak_lo = packet_in.fi_weak_lo;
        packet1.fi_weak_max = packet_in.fi_weak_max;
        packet1.phase_adv_max_deg = packet_in.phase_adv_max_deg;
        packet1.input1_raw = packet_in.input1_raw;
        packet1.input1_typ = packet_in.input1_typ;
        packet1.input1_min = packet_in.input1_min;
        packet1.input1_mid = packet_in.input1_mid;
        packet1.input1_max = packet_in.input1_max;
        packet1.input1_cmd = packet_in.input1_cmd;
        packet1.input2_raw = packet_in.input2_raw;
        packet1.input2_typ = packet_in.input2_typ;
        packet1.input2_min = packet_in.input2_min;
        packet1.input2_mid = packet_in.input2_mid;
        packet1.input2_max = packet_in.input2_max;
        packet1.input2_cmd = packet_in.input2_cmd;
        packet1.aux_input1_raw = packet_in.aux_input1_raw;
        packet1.aux_input1_typ = packet_in.aux_input1_typ;
        packet1.aux_input1_min = packet_in.aux_input1_min;
        packet1.aux_input1_mid = packet_in.aux_input1_mid;
        packet1.aux_input1_max = packet_in.aux_input1_max;
        packet1.aux_input1_cmd = packet_in.aux_input1_cmd;
        packet1.aux_input2_raw = packet_in.aux_input2_raw;
        packet1.aux_input2_min = packet_in.aux_input2_min;
        packet1.aux_input2_mid = packet_in.aux_input2_mid;
        packet1.aux_input2_max = packet_in.aux_input2_max;
        packet1.aux_input2_cmd = packet_in.aux_input2_cmd;
        packet1.dc_curr = packet_in.dc_curr;
        packet1.rdc_curr = packet_in.rdc_curr;
        packet1.ldc_curr = packet_in.ldc_curr;
        packet1.cmdl = packet_in.cmdl;
        packet1.cmdr = packet_in.cmdr;
        packet1.spd_avg = packet_in.spd_avg;
        packet1.spdl = packet_in.spdl;
        packet1.spdr = packet_in.spdr;
        packet1.filter_rate = packet_in.filter_rate;
        packet1.spd_coef = packet_in.spd_coef;
        packet1.str_coef = packet_in.str_coef;
        packet1.batv = packet_in.batv;
        packet1.temp = packet_in.temp;
        packet1.aux_input2_typ = packet_in.aux_input2_typ;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_hoverboard_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_event_pack(system_id, component_id, &msg , packet1.ctrl_mod , packet1.ctrl_typ , packet1.cur_mot_max , packet1.rpm_mot_max , packet1.fi_weak_ena , packet1.fi_weak_hi , packet1.fi_weak_lo , packet1.fi_weak_max , packet1.phase_adv_max_deg , packet1.input1_raw , packet1.input1_typ , packet1.input1_min , packet1.input1_mid , packet1.input1_max , packet1.input1_cmd , packet1.input2_raw , packet1.input2_typ , packet1.input2_min , packet1.input2_mid , packet1.input2_max , packet1.input2_cmd , packet1.aux_input1_raw , packet1.aux_input1_typ , packet1.aux_input1_min , packet1.aux_input1_mid , packet1.aux_input1_max , packet1.aux_input1_cmd , packet1.aux_input2_raw , packet1.aux_input2_typ , packet1.aux_input2_min , packet1.aux_input2_mid , packet1.aux_input2_max , packet1.aux_input2_cmd , packet1.dc_curr , packet1.rdc_curr , packet1.ldc_curr , packet1.cmdl , packet1.cmdr , packet1.spd_avg , packet1.spdl , packet1.spdr , packet1.filter_rate , packet1.spd_coef , packet1.str_coef , packet1.batv , packet1.temp );
    mavlink_msg_hoverboard_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.ctrl_mod , packet1.ctrl_typ , packet1.cur_mot_max , packet1.rpm_mot_max , packet1.fi_weak_ena , packet1.fi_weak_hi , packet1.fi_weak_lo , packet1.fi_weak_max , packet1.phase_adv_max_deg , packet1.input1_raw , packet1.input1_typ , packet1.input1_min , packet1.input1_mid , packet1.input1_max , packet1.input1_cmd , packet1.input2_raw , packet1.input2_typ , packet1.input2_min , packet1.input2_mid , packet1.input2_max , packet1.input2_cmd , packet1.aux_input1_raw , packet1.aux_input1_typ , packet1.aux_input1_min , packet1.aux_input1_mid , packet1.aux_input1_max , packet1.aux_input1_cmd , packet1.aux_input2_raw , packet1.aux_input2_typ , packet1.aux_input2_min , packet1.aux_input2_mid , packet1.aux_input2_max , packet1.aux_input2_cmd , packet1.dc_curr , packet1.rdc_curr , packet1.ldc_curr , packet1.cmdl , packet1.cmdr , packet1.spd_avg , packet1.spdl , packet1.spdr , packet1.filter_rate , packet1.spd_coef , packet1.str_coef , packet1.batv , packet1.temp );
    mavlink_msg_hoverboard_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_hoverboard_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_hoverboard_event_send(MAVLINK_COMM_1 , packet1.ctrl_mod , packet1.ctrl_typ , packet1.cur_mot_max , packet1.rpm_mot_max , packet1.fi_weak_ena , packet1.fi_weak_hi , packet1.fi_weak_lo , packet1.fi_weak_max , packet1.phase_adv_max_deg , packet1.input1_raw , packet1.input1_typ , packet1.input1_min , packet1.input1_mid , packet1.input1_max , packet1.input1_cmd , packet1.input2_raw , packet1.input2_typ , packet1.input2_min , packet1.input2_mid , packet1.input2_max , packet1.input2_cmd , packet1.aux_input1_raw , packet1.aux_input1_typ , packet1.aux_input1_min , packet1.aux_input1_mid , packet1.aux_input1_max , packet1.aux_input1_cmd , packet1.aux_input2_raw , packet1.aux_input2_typ , packet1.aux_input2_min , packet1.aux_input2_mid , packet1.aux_input2_max , packet1.aux_input2_cmd , packet1.dc_curr , packet1.rdc_curr , packet1.ldc_curr , packet1.cmdl , packet1.cmdr , packet1.spd_avg , packet1.spdl , packet1.spdr , packet1.filter_rate , packet1.spd_coef , packet1.str_coef , packet1.batv , packet1.temp );
    mavlink_msg_hoverboard_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("HOVERBOARD_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_HOVERBOARD_EVENT) != NULL);
#endif
}

static void mavlink_test_cutter_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CUTTER_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_cutter_request_t packet_in = {
        963497464,17
    };
    mavlink_cutter_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.rpm = packet_in.rpm;
        packet1.enabled = packet_in.enabled;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CUTTER_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_cutter_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_request_pack(system_id, component_id, &msg , packet1.enabled , packet1.rpm );
    mavlink_msg_cutter_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.enabled , packet1.rpm );
    mavlink_msg_cutter_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_cutter_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_request_send(MAVLINK_COMM_1 , packet1.enabled , packet1.rpm );
    mavlink_msg_cutter_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("CUTTER_REQUEST") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_CUTTER_REQUEST) != NULL);
#endif
}

static void mavlink_test_cutter_reply(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CUTTER_REPLY >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_cutter_reply_t packet_in = {
        963497464,963497672,963497880,"MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFG"
    };
    mavlink_cutter_reply_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.req_id = packet_in.req_id;
        packet1.error_code = packet_in.error_code;
        packet1.msg_type = packet_in.msg_type;
        
        mav_array_memcpy(packet1.message, packet_in.message, sizeof(char)*100);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CUTTER_REPLY_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_reply_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_cutter_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_reply_pack(system_id, component_id, &msg , packet1.req_id , packet1.error_code , packet1.message , packet1.msg_type );
    mavlink_msg_cutter_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_reply_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.req_id , packet1.error_code , packet1.message , packet1.msg_type );
    mavlink_msg_cutter_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_cutter_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_reply_send(MAVLINK_COMM_1 , packet1.req_id , packet1.error_code , packet1.message , packet1.msg_type );
    mavlink_msg_cutter_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("CUTTER_REPLY") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_CUTTER_REPLY) != NULL);
#endif
}

static void mavlink_test_cutter_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CUTTER_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_cutter_event_t packet_in = {
        963497464,45.0,73.0,101.0,53
    };
    mavlink_cutter_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.rpm = packet_in.rpm;
        packet1.current = packet_in.current;
        packet1.voltage = packet_in.voltage;
        packet1.temperature = packet_in.temperature;
        packet1.enabled = packet_in.enabled;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CUTTER_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_cutter_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_event_pack(system_id, component_id, &msg , packet1.enabled , packet1.rpm , packet1.current , packet1.voltage , packet1.temperature );
    mavlink_msg_cutter_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.enabled , packet1.rpm , packet1.current , packet1.voltage , packet1.temperature );
    mavlink_msg_cutter_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_cutter_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cutter_event_send(MAVLINK_COMM_1 , packet1.enabled , packet1.rpm , packet1.current , packet1.voltage , packet1.temperature );
    mavlink_msg_cutter_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("CUTTER_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_CUTTER_EVENT) != NULL);
#endif
}

static void mavlink_test_compass_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_COMPASS_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_compass_event_t packet_in = {
        17.0,45.0,73.0,101.0,129.0,157.0,185.0,213.0,241.0
    };
    mavlink_compass_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.heading = packet_in.heading;
        packet1.pitch = packet_in.pitch;
        packet1.roll = packet_in.roll;
        packet1.mag_x = packet_in.mag_x;
        packet1.mag_y = packet_in.mag_y;
        packet1.mag_z = packet_in.mag_z;
        packet1.accel_x = packet_in.accel_x;
        packet1.accel_y = packet_in.accel_y;
        packet1.accel_z = packet_in.accel_z;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_COMPASS_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_compass_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_compass_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_compass_event_pack(system_id, component_id, &msg , packet1.heading , packet1.pitch , packet1.roll , packet1.mag_x , packet1.mag_y , packet1.mag_z , packet1.accel_x , packet1.accel_y , packet1.accel_z );
    mavlink_msg_compass_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_compass_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.heading , packet1.pitch , packet1.roll , packet1.mag_x , packet1.mag_y , packet1.mag_z , packet1.accel_x , packet1.accel_y , packet1.accel_z );
    mavlink_msg_compass_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_compass_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_compass_event_send(MAVLINK_COMM_1 , packet1.heading , packet1.pitch , packet1.roll , packet1.mag_x , packet1.mag_y , packet1.mag_z , packet1.accel_x , packet1.accel_y , packet1.accel_z );
    mavlink_msg_compass_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("COMPASS_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_COMPASS_EVENT) != NULL);
#endif
}

static void mavlink_test_imu_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_IMU_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_imu_event_t packet_in = {
        17.0,45.0,73.0,101.0,129.0,157.0
    };
    mavlink_imu_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.gyro_x = packet_in.gyro_x;
        packet1.gyro_y = packet_in.gyro_y;
        packet1.gyro_z = packet_in.gyro_z;
        packet1.accel_x = packet_in.accel_x;
        packet1.accel_y = packet_in.accel_y;
        packet1.accel_z = packet_in.accel_z;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_IMU_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_imu_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_event_pack(system_id, component_id, &msg , packet1.gyro_x , packet1.gyro_y , packet1.gyro_z , packet1.accel_x , packet1.accel_y , packet1.accel_z );
    mavlink_msg_imu_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.gyro_x , packet1.gyro_y , packet1.gyro_z , packet1.accel_x , packet1.accel_y , packet1.accel_z );
    mavlink_msg_imu_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_imu_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_event_send(MAVLINK_COMM_1 , packet1.gyro_x , packet1.gyro_y , packet1.gyro_z , packet1.accel_x , packet1.accel_y , packet1.accel_z );
    mavlink_msg_imu_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("IMU_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_IMU_EVENT) != NULL);
#endif
}

static void mavlink_test_us_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_US_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_us_event_t packet_in = {
        17.0,45.0,963497880
    };
    mavlink_us_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.distance = packet_in.distance;
        packet1.temperature = packet_in.temperature;
        packet1.status = packet_in.status;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_US_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_US_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_us_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_us_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_us_event_pack(system_id, component_id, &msg , packet1.distance , packet1.temperature , packet1.status );
    mavlink_msg_us_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_us_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.distance , packet1.temperature , packet1.status );
    mavlink_msg_us_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_us_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_us_event_send(MAVLINK_COMM_1 , packet1.distance , packet1.temperature , packet1.status );
    mavlink_msg_us_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("US_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_US_EVENT) != NULL);
#endif
}

static void mavlink_test_ps4_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_PS4_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ps4_event_t packet_in = {
        963497464,963497672,963497880,963498088,963498296,963498504,963498712,963498920,963499128,963499336,963499544,963499752,149,216,27,94,161,228,39,106,173,240,51,118,185,252,63,130,197,8,75,142,"QRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJK"
    };
    mavlink_ps4_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.axis_lx = packet_in.axis_lx;
        packet1.axis_ly = packet_in.axis_ly;
        packet1.axis_rx = packet_in.axis_rx;
        packet1.axis_ry = packet_in.axis_ry;
        packet1.gyro_x = packet_in.gyro_x;
        packet1.gyro_y = packet_in.gyro_y;
        packet1.gyro_z = packet_in.gyro_z;
        packet1.accel_x = packet_in.accel_x;
        packet1.accel_y = packet_in.accel_y;
        packet1.accel_z = packet_in.accel_z;
        packet1.battery_level = packet_in.battery_level;
        packet1.temp = packet_in.temp;
        packet1.button_left = packet_in.button_left;
        packet1.button_right = packet_in.button_right;
        packet1.button_up = packet_in.button_up;
        packet1.button_down = packet_in.button_down;
        packet1.button_square = packet_in.button_square;
        packet1.button_cross = packet_in.button_cross;
        packet1.button_circle = packet_in.button_circle;
        packet1.button_triangle = packet_in.button_triangle;
        packet1.button_left_shoulder = packet_in.button_left_shoulder;
        packet1.button_right_shoulder = packet_in.button_right_shoulder;
        packet1.button_left_trigger = packet_in.button_left_trigger;
        packet1.button_right_trigger = packet_in.button_right_trigger;
        packet1.button_left_joystick = packet_in.button_left_joystick;
        packet1.button_right_joystick = packet_in.button_right_joystick;
        packet1.button_share = packet_in.button_share;
        packet1.button_options = packet_in.button_options;
        packet1.button_touchpad = packet_in.button_touchpad;
        packet1.button_ps = packet_in.button_ps;
        packet1.connected = packet_in.connected;
        packet1.bluetooth = packet_in.bluetooth;
        
        mav_array_memcpy(packet1.debug, packet_in.debug, sizeof(char)*100);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_PS4_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ps4_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ps4_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ps4_event_pack(system_id, component_id, &msg , packet1.button_left , packet1.button_right , packet1.button_up , packet1.button_down , packet1.button_square , packet1.button_cross , packet1.button_circle , packet1.button_triangle , packet1.button_left_shoulder , packet1.button_right_shoulder , packet1.button_left_trigger , packet1.button_right_trigger , packet1.button_left_joystick , packet1.button_right_joystick , packet1.button_share , packet1.button_options , packet1.button_touchpad , packet1.button_ps , packet1.axis_lx , packet1.axis_ly , packet1.axis_rx , packet1.axis_ry , packet1.gyro_x , packet1.gyro_y , packet1.gyro_z , packet1.accel_x , packet1.accel_y , packet1.accel_z , packet1.connected , packet1.battery_level , packet1.bluetooth , packet1.debug , packet1.temp );
    mavlink_msg_ps4_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ps4_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.button_left , packet1.button_right , packet1.button_up , packet1.button_down , packet1.button_square , packet1.button_cross , packet1.button_circle , packet1.button_triangle , packet1.button_left_shoulder , packet1.button_right_shoulder , packet1.button_left_trigger , packet1.button_right_trigger , packet1.button_left_joystick , packet1.button_right_joystick , packet1.button_share , packet1.button_options , packet1.button_touchpad , packet1.button_ps , packet1.axis_lx , packet1.axis_ly , packet1.axis_rx , packet1.axis_ry , packet1.gyro_x , packet1.gyro_y , packet1.gyro_z , packet1.accel_x , packet1.accel_y , packet1.accel_z , packet1.connected , packet1.battery_level , packet1.bluetooth , packet1.debug , packet1.temp );
    mavlink_msg_ps4_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ps4_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ps4_event_send(MAVLINK_COMM_1 , packet1.button_left , packet1.button_right , packet1.button_up , packet1.button_down , packet1.button_square , packet1.button_cross , packet1.button_circle , packet1.button_triangle , packet1.button_left_shoulder , packet1.button_right_shoulder , packet1.button_left_trigger , packet1.button_right_trigger , packet1.button_left_joystick , packet1.button_right_joystick , packet1.button_share , packet1.button_options , packet1.button_touchpad , packet1.button_ps , packet1.axis_lx , packet1.axis_ly , packet1.axis_rx , packet1.axis_ry , packet1.gyro_x , packet1.gyro_y , packet1.gyro_z , packet1.accel_x , packet1.accel_y , packet1.accel_z , packet1.connected , packet1.battery_level , packet1.bluetooth , packet1.debug , packet1.temp );
    mavlink_msg_ps4_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("PS4_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_PS4_EVENT) != NULL);
#endif
}

static void mavlink_test_ps4_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_PS4_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ps4_request_t packet_in = {
        963497464,963497672,963497880,963498088,963498296,963498504,963498712,963498920
    };
    mavlink_ps4_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.req_id = packet_in.req_id;
        packet1.rumble_small = packet_in.rumble_small;
        packet1.rumble_large = packet_in.rumble_large;
        packet1.led_red = packet_in.led_red;
        packet1.led_green = packet_in.led_green;
        packet1.led_blue = packet_in.led_blue;
        packet1.led_flash_on = packet_in.led_flash_on;
        packet1.led_flash_off = packet_in.led_flash_off;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_PS4_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ps4_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ps4_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ps4_request_pack(system_id, component_id, &msg , packet1.req_id , packet1.rumble_small , packet1.rumble_large , packet1.led_red , packet1.led_green , packet1.led_blue , packet1.led_flash_on , packet1.led_flash_off );
    mavlink_msg_ps4_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ps4_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.req_id , packet1.rumble_small , packet1.rumble_large , packet1.led_red , packet1.led_green , packet1.led_blue , packet1.led_flash_on , packet1.led_flash_off );
    mavlink_msg_ps4_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ps4_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ps4_request_send(MAVLINK_COMM_1 , packet1.req_id , packet1.rumble_small , packet1.rumble_large , packet1.led_red , packet1.led_green , packet1.led_blue , packet1.led_flash_on , packet1.led_flash_off );
    mavlink_msg_ps4_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("PS4_REQUEST") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_PS4_REQUEST) != NULL);
#endif
}

static void mavlink_test_sys_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SYS_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_sys_request_t packet_in = {
        93372036854775807ULL,963497880,41,"NOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGH"
    };
    mavlink_sys_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.set_time = packet_in.set_time;
        packet1.req_id = packet_in.req_id;
        packet1.reboot = packet_in.reboot;
        
        mav_array_memcpy(packet1.console, packet_in.console, sizeof(char)*100);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SYS_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_sys_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_request_pack(system_id, component_id, &msg , packet1.req_id , packet1.set_time , packet1.reboot , packet1.console );
    mavlink_msg_sys_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.req_id , packet1.set_time , packet1.reboot , packet1.console );
    mavlink_msg_sys_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_sys_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_request_send(MAVLINK_COMM_1 , packet1.req_id , packet1.set_time , packet1.reboot , packet1.console );
    mavlink_msg_sys_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SYS_REQUEST") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SYS_REQUEST) != NULL);
#endif
}

static void mavlink_test_sys_reply(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SYS_REPLY >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_sys_reply_t packet_in = {
        963497464,963497672,"IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC"
    };
    mavlink_sys_reply_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.req_id = packet_in.req_id;
        packet1.rc = packet_in.rc;
        
        mav_array_memcpy(packet1.message, packet_in.message, sizeof(char)*100);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SYS_REPLY_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_reply_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_sys_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_reply_pack(system_id, component_id, &msg , packet1.req_id , packet1.rc , packet1.message );
    mavlink_msg_sys_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_reply_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.req_id , packet1.rc , packet1.message );
    mavlink_msg_sys_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_sys_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_reply_send(MAVLINK_COMM_1 , packet1.req_id , packet1.rc , packet1.message );
    mavlink_msg_sys_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SYS_REPLY") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SYS_REPLY) != NULL);
#endif
}

static void mavlink_test_sys_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_SYS_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_sys_event_t packet_in = {
        93372036854775807ULL,93372036854776311ULL,93372036854776815ULL,93372036854777319ULL,"GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC","EFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZA"
    };
    mavlink_sys_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.utc = packet_in.utc;
        packet1.uptime = packet_in.uptime;
        packet1.free_heap = packet_in.free_heap;
        packet1.flash_size = packet_in.flash_size;
        
        mav_array_memcpy(packet1.cpu_board_type, packet_in.cpu_board_type, sizeof(char)*50);
        mav_array_memcpy(packet1.build_date_time, packet_in.build_date_time, sizeof(char)*50);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_SYS_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_sys_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_event_pack(system_id, component_id, &msg , packet1.utc , packet1.uptime , packet1.free_heap , packet1.flash_size , packet1.cpu_board_type , packet1.build_date_time );
    mavlink_msg_sys_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.utc , packet1.uptime , packet1.free_heap , packet1.flash_size , packet1.cpu_board_type , packet1.build_date_time );
    mavlink_msg_sys_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_sys_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sys_event_send(MAVLINK_COMM_1 , packet1.utc , packet1.uptime , packet1.free_heap , packet1.flash_size , packet1.cpu_board_type , packet1.build_date_time );
    mavlink_msg_sys_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("SYS_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_SYS_EVENT) != NULL);
#endif
}

static void mavlink_test_ping_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_PING_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ping_request_t packet_in = {
        93372036854775807ULL,963497880
    };
    mavlink_ping_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.timestamp = packet_in.timestamp;
        packet1.req_id = packet_in.req_id;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_PING_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_PING_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ping_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ping_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ping_request_pack(system_id, component_id, &msg , packet1.req_id , packet1.timestamp );
    mavlink_msg_ping_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ping_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.req_id , packet1.timestamp );
    mavlink_msg_ping_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ping_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ping_request_send(MAVLINK_COMM_1 , packet1.req_id , packet1.timestamp );
    mavlink_msg_ping_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("PING_REQUEST") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_PING_REQUEST) != NULL);
#endif
}

static void mavlink_test_ping_reply(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_PING_REPLY >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ping_reply_t packet_in = {
        93372036854775807ULL,963497880
    };
    mavlink_ping_reply_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.timestamp = packet_in.timestamp;
        packet1.req_id = packet_in.req_id;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_PING_REPLY_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_PING_REPLY_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ping_reply_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ping_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ping_reply_pack(system_id, component_id, &msg , packet1.req_id , packet1.timestamp );
    mavlink_msg_ping_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ping_reply_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.req_id , packet1.timestamp );
    mavlink_msg_ping_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ping_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ping_reply_send(MAVLINK_COMM_1 , packet1.req_id , packet1.timestamp );
    mavlink_msg_ping_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("PING_REPLY") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_PING_REPLY) != NULL);
#endif
}

static void mavlink_test_broker_subscribe_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_broker_subscribe_request_t packet_in = {
        963497464,963497672
    };
    mavlink_broker_subscribe_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.src = packet_in.src;
        packet1.msg_type = packet_in.msg_type;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_broker_subscribe_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_broker_subscribe_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_broker_subscribe_request_pack(system_id, component_id, &msg , packet1.src , packet1.msg_type );
    mavlink_msg_broker_subscribe_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_broker_subscribe_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.src , packet1.msg_type );
    mavlink_msg_broker_subscribe_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_broker_subscribe_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_broker_subscribe_request_send(MAVLINK_COMM_1 , packet1.src , packet1.msg_type );
    mavlink_msg_broker_subscribe_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("BROKER_SUBSCRIBE_REQUEST") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_BROKER_SUBSCRIBE_REQUEST) != NULL);
#endif
}

static void mavlink_test_wifi_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_WIFI_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_wifi_event_t packet_in = {
        963497464,963497672,"IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE","GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC","EFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZA","CDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXY","ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW","YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTU"
    };
    mavlink_wifi_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.channel = packet_in.channel;
        packet1.rssi = packet_in.rssi;
        
        mav_array_memcpy(packet1.ip, packet_in.ip, sizeof(char)*50);
        mav_array_memcpy(packet1.gateway, packet_in.gateway, sizeof(char)*50);
        mav_array_memcpy(packet1.netmask, packet_in.netmask, sizeof(char)*50);
        mav_array_memcpy(packet1.ssid, packet_in.ssid, sizeof(char)*50);
        mav_array_memcpy(packet1.bssid, packet_in.bssid, sizeof(char)*50);
        mav_array_memcpy(packet1.mac, packet_in.mac, sizeof(char)*50);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_WIFI_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_wifi_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_wifi_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_wifi_event_pack(system_id, component_id, &msg , packet1.ip , packet1.gateway , packet1.netmask , packet1.ssid , packet1.bssid , packet1.channel , packet1.rssi , packet1.mac );
    mavlink_msg_wifi_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_wifi_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.ip , packet1.gateway , packet1.netmask , packet1.ssid , packet1.bssid , packet1.channel , packet1.rssi , packet1.mac );
    mavlink_msg_wifi_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_wifi_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_wifi_event_send(MAVLINK_COMM_1 , packet1.ip , packet1.gateway , packet1.netmask , packet1.ssid , packet1.bssid , packet1.channel , packet1.rssi , packet1.mac );
    mavlink_msg_wifi_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("WIFI_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_WIFI_EVENT) != NULL);
#endif
}

static void mavlink_test_device_alive_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_device_alive_event_t packet_in = {
        93372036854775807ULL,"IJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE","GHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABC"
    };
    mavlink_device_alive_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.timestamp = packet_in.timestamp;
        
        mav_array_memcpy(packet1.device, packet_in.device, sizeof(char)*50);
        mav_array_memcpy(packet1.endpoint, packet_in.endpoint, sizeof(char)*50);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_device_alive_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_device_alive_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_device_alive_event_pack(system_id, component_id, &msg , packet1.device , packet1.endpoint , packet1.timestamp );
    mavlink_msg_device_alive_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_device_alive_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.device , packet1.endpoint , packet1.timestamp );
    mavlink_msg_device_alive_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_device_alive_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_device_alive_event_send(MAVLINK_COMM_1 , packet1.device , packet1.endpoint , packet1.timestamp );
    mavlink_msg_device_alive_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("DEVICE_ALIVE_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_DEVICE_ALIVE_EVENT) != NULL);
#endif
}

static void mavlink_test_max31855_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_MAX31855_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_max31855_event_t packet_in = {
        17.0,45.0,29,96,163,230
    };
    mavlink_max31855_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.thermocouple_temp = packet_in.thermocouple_temp;
        packet1.internal_temp = packet_in.internal_temp;
        packet1.fault = packet_in.fault;
        packet1.fault_short_vcc = packet_in.fault_short_vcc;
        packet1.fault_short_gnd = packet_in.fault_short_gnd;
        packet1.fault_open_tc = packet_in.fault_open_tc;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_MAX31855_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_max31855_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_max31855_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_max31855_event_pack(system_id, component_id, &msg , packet1.thermocouple_temp , packet1.internal_temp , packet1.fault , packet1.fault_short_vcc , packet1.fault_short_gnd , packet1.fault_open_tc );
    mavlink_msg_max31855_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_max31855_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.thermocouple_temp , packet1.internal_temp , packet1.fault , packet1.fault_short_vcc , packet1.fault_short_gnd , packet1.fault_open_tc );
    mavlink_msg_max31855_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_max31855_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_max31855_event_send(MAVLINK_COMM_1 , packet1.thermocouple_temp , packet1.internal_temp , packet1.fault , packet1.fault_short_vcc , packet1.fault_short_gnd , packet1.fault_open_tc );
    mavlink_msg_max31855_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("MAX31855_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_MAX31855_EVENT) != NULL);
#endif
}

static void mavlink_test_heating_event(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HEATING_EVENT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_heating_event_t packet_in = {
        93372036854775807ULL,73.0,101.0,129.0,65,132,199
    };
    mavlink_heating_event_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.timestamp_ms = packet_in.timestamp_ms;
        packet1.temperature_c = packet_in.temperature_c;
        packet1.setpoint_c = packet_in.setpoint_c;
        packet1.output_pct = packet_in.output_pct;
        packet1.enabled = packet_in.enabled;
        packet1.heater_on = packet_in.heater_on;
        packet1.fault = packet_in.fault;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HEATING_EVENT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_heating_event_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_heating_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_heating_event_pack(system_id, component_id, &msg , packet1.temperature_c , packet1.setpoint_c , packet1.enabled , packet1.output_pct , packet1.heater_on , packet1.fault , packet1.timestamp_ms );
    mavlink_msg_heating_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_heating_event_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.temperature_c , packet1.setpoint_c , packet1.enabled , packet1.output_pct , packet1.heater_on , packet1.fault , packet1.timestamp_ms );
    mavlink_msg_heating_event_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_heating_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_heating_event_send(MAVLINK_COMM_1 , packet1.temperature_c , packet1.setpoint_c , packet1.enabled , packet1.output_pct , packet1.heater_on , packet1.fault , packet1.timestamp_ms );
    mavlink_msg_heating_event_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("HEATING_EVENT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_HEATING_EVENT) != NULL);
#endif
}

static void mavlink_test_heating_request(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HEATING_REQUEST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_heating_request_t packet_in = {
        17.0,45.0,73.0,101.0,53,120
    };
    mavlink_heating_request_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.setpoint_c = packet_in.setpoint_c;
        packet1.kp = packet_in.kp;
        packet1.ki = packet_in.ki;
        packet1.kd = packet_in.kd;
        packet1.enabled = packet_in.enabled;
        packet1.reset_integral = packet_in.reset_integral;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_HEATING_REQUEST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_heating_request_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_heating_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_heating_request_pack(system_id, component_id, &msg , packet1.setpoint_c , packet1.enabled , packet1.kp , packet1.ki , packet1.kd , packet1.reset_integral );
    mavlink_msg_heating_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_heating_request_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.setpoint_c , packet1.enabled , packet1.kp , packet1.ki , packet1.kd , packet1.reset_integral );
    mavlink_msg_heating_request_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_heating_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_heating_request_send(MAVLINK_COMM_1 , packet1.setpoint_c , packet1.enabled , packet1.kp , packet1.ki , packet1.kd , packet1.reset_integral );
    mavlink_msg_heating_request_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("HEATING_REQUEST") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_HEATING_REQUEST) != NULL);
#endif
}

static void mavlink_test_envelope(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ENVELOPE >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_envelope_t packet_in = {
        963497464,963497672,963497880,963498088,963498296,{ 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 }
    };
    mavlink_envelope_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.src = packet_in.src;
        packet1.dst = packet_in.dst;
        packet1.msg_type = packet_in.msg_type;
        packet1.request_id = packet_in.request_id;
        packet1.instance_id = packet_in.instance_id;
        
        mav_array_memcpy(packet1.payload, packet_in.payload, sizeof(uint8_t)*255);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ENVELOPE_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ENVELOPE_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_envelope_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_envelope_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_envelope_pack(system_id, component_id, &msg , packet1.src , packet1.dst , packet1.msg_type , packet1.request_id , packet1.instance_id , packet1.payload );
    mavlink_msg_envelope_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_envelope_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.src , packet1.dst , packet1.msg_type , packet1.request_id , packet1.instance_id , packet1.payload );
    mavlink_msg_envelope_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_envelope_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_envelope_send(MAVLINK_COMM_1 , packet1.src , packet1.dst , packet1.msg_type , packet1.request_id , packet1.instance_id , packet1.payload );
    mavlink_msg_envelope_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("ENVELOPE") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_ENVELOPE) != NULL);
#endif
}

static void mavlink_test_endpoint_announce(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_endpoint_announce_t packet_in = {
        963497464,{ 963497672, 963497673, 963497674, 963497675, 963497676, 963497677, 963497678, 963497679, 963497680, 963497681, 963497682, 963497683, 963497684, 963497685, 963497686, 963497687 },{ 963501000, 963501001, 963501002, 963501003, 963501004, 963501005, 963501006, 963501007, 963501008, 963501009, 963501010, 963501011, 963501012, 963501013, 963501014, 963501015 },{ 963504328, 963504329, 963504330, 963504331, 963504332, 963504333, 963504334, 963504335, 963504336, 963504337, 963504338, 963504339, 963504340, 963504341, 963504342, 963504343 },{ 963507656, 963507657, 963507658, 963507659, 963507660, 963507661, 963507662, 963507663, 963507664, 963507665, 963507666, 963507667, 963507668, 963507669, 963507670, 963507671 },"ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW","YZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRS"
    };
    mavlink_endpoint_announce_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.id = packet_in.id;
        
        mav_array_memcpy(packet1.services, packet_in.services, sizeof(uint32_t)*16);
        mav_array_memcpy(packet1.events, packet_in.events, sizeof(uint32_t)*16);
        mav_array_memcpy(packet1.replies, packet_in.replies, sizeof(uint32_t)*16);
        mav_array_memcpy(packet1.subscribes, packet_in.subscribes, sizeof(uint32_t)*16);
        mav_array_memcpy(packet1.name, packet_in.name, sizeof(char)*50);
        mav_array_memcpy(packet1.description, packet_in.description, sizeof(char)*100);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_endpoint_announce_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_endpoint_announce_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_endpoint_announce_pack(system_id, component_id, &msg , packet1.id , packet1.name , packet1.description , packet1.services , packet1.events , packet1.replies , packet1.subscribes );
    mavlink_msg_endpoint_announce_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_endpoint_announce_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.id , packet1.name , packet1.description , packet1.services , packet1.events , packet1.replies , packet1.subscribes );
    mavlink_msg_endpoint_announce_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_endpoint_announce_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_endpoint_announce_send(MAVLINK_COMM_1 , packet1.id , packet1.name , packet1.description , packet1.services , packet1.events , packet1.replies , packet1.subscribes );
    mavlink_msg_endpoint_announce_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("ENDPOINT_ANNOUNCE") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE) != NULL);
#endif
}

static void mavlink_test_endpoint_announce_reply(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_endpoint_announce_reply_t packet_in = {
        93372036854775807ULL
    };
    mavlink_endpoint_announce_reply_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.utc = packet_in.utc;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_endpoint_announce_reply_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_endpoint_announce_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_endpoint_announce_reply_pack(system_id, component_id, &msg , packet1.utc );
    mavlink_msg_endpoint_announce_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_endpoint_announce_reply_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.utc );
    mavlink_msg_endpoint_announce_reply_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_endpoint_announce_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_endpoint_announce_reply_send(MAVLINK_COMM_1 , packet1.utc );
    mavlink_msg_endpoint_announce_reply_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("ENDPOINT_ANNOUNCE_REPLY") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_ENDPOINT_ANNOUNCE_REPLY) != NULL);
#endif
}

static void mavlink_test_robot(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_hoverboard_request(system_id, component_id, last_msg);
    mavlink_test_hoverboard_reply(system_id, component_id, last_msg);
    mavlink_test_generic_reply(system_id, component_id, last_msg);
    mavlink_test_hoverboard_event(system_id, component_id, last_msg);
    mavlink_test_cutter_request(system_id, component_id, last_msg);
    mavlink_test_cutter_reply(system_id, component_id, last_msg);
    mavlink_test_cutter_event(system_id, component_id, last_msg);
    mavlink_test_compass_event(system_id, component_id, last_msg);
    mavlink_test_imu_event(system_id, component_id, last_msg);
    mavlink_test_us_event(system_id, component_id, last_msg);
    mavlink_test_ps4_event(system_id, component_id, last_msg);
    mavlink_test_ps4_request(system_id, component_id, last_msg);
    mavlink_test_sys_request(system_id, component_id, last_msg);
    mavlink_test_sys_reply(system_id, component_id, last_msg);
    mavlink_test_sys_event(system_id, component_id, last_msg);
    mavlink_test_ping_request(system_id, component_id, last_msg);
    mavlink_test_ping_reply(system_id, component_id, last_msg);
    mavlink_test_broker_subscribe_request(system_id, component_id, last_msg);
    mavlink_test_wifi_event(system_id, component_id, last_msg);
    mavlink_test_device_alive_event(system_id, component_id, last_msg);
    mavlink_test_max31855_event(system_id, component_id, last_msg);
    mavlink_test_heating_event(system_id, component_id, last_msg);
    mavlink_test_heating_request(system_id, component_id, last_msg);
    mavlink_test_envelope(system_id, component_id, last_msg);
    mavlink_test_endpoint_announce(system_id, component_id, last_msg);
    mavlink_test_endpoint_announce_reply(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // ROBOT_TESTSUITE_H
