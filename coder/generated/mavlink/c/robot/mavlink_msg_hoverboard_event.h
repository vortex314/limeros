#pragma once
// MESSAGE HOVERBOARD_EVENT PACKING

#define MAVLINK_MSG_ID_HOVERBOARD_EVENT 40004


typedef struct __mavlink_hoverboard_event_t {
 int16_t ctrl_mod; /*<  Control mode 1:Voltage 2:Speed 3:Torque*/
 int16_t ctrl_typ; /*<  Control type 0:Commutation 1:Sinusoidal 2:FOC*/
 int16_t cur_mot_max; /*< [A] Max phase current*/
 int16_t rpm_mot_max; /*< [rpm] Max motor RPM*/
 int16_t fi_weak_ena; /*<  Enable field weakening 0:OFF 1:ON*/
 int16_t fi_weak_hi; /*< [rpm] Field weakening high RPM*/
 int16_t fi_weak_lo; /*< [rpm] Field weakening low RPM*/
 int16_t fi_weak_max; /*< [A] Field weakening max current (FOC only)*/
 int16_t phase_adv_max_deg; /*< [deg] Max phase advance angle (SIN only)*/
 int16_t input1_raw; /*<  Input1 raw value*/
 int16_t input1_typ; /*<  Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect*/
 int16_t input1_min; /*<  Input1 minimum value*/
 int16_t input1_mid; /*<  Input1 middle value*/
 int16_t input1_max; /*<  Input1 maximum value*/
 int16_t input1_cmd; /*<  Input1 command value*/
 int16_t input2_raw; /*<  Input2 raw value*/
 int16_t input2_typ; /*<  Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect*/
 int16_t input2_min; /*<  Input2 minimum value*/
 int16_t input2_mid; /*<  Input2 middle value*/
 int16_t input2_max; /*<  Input2 maximum value*/
 int16_t input2_cmd; /*<  Input2 command value*/
 int16_t aux_input1_raw; /*<  Aux input1 raw value*/
 int16_t aux_input1_typ; /*<  Aux input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect*/
 int16_t aux_input1_min; /*<  Aux input1 minimum value*/
 int16_t aux_input1_mid; /*<  Aux input1 middle value*/
 int16_t aux_input1_max; /*<  Aux input1 maximum value*/
 int16_t aux_input1_cmd; /*<  Aux input1 command value*/
 int16_t aux_input2_raw; /*<  Aux input2 raw value*/
 int16_t aux_input2_min; /*<  Aux input2 minimum value*/
 int16_t aux_input2_mid; /*<  Aux input2 middle value*/
 int16_t aux_input2_max; /*<  Aux input2 maximum value*/
 int16_t aux_input2_cmd; /*<  Aux input2 command value*/
 int16_t dc_curr; /*< [cA] Total DC link current A *100*/
 int16_t rdc_curr; /*< [cA] Right DC link current A *100*/
 int16_t ldc_curr; /*< [cA] Left DC link current A *100*/
 int16_t cmdl; /*< [rpm] Left motor command RPM*/
 int16_t cmdr; /*< [rpm] Right motor command RPM*/
 int16_t spd_avg; /*< [rpm] Motor measured average RPM*/
 int16_t spdl; /*< [rpm] Left motor measured RPM*/
 int16_t spdr; /*< [rpm] Right motor measured RPM*/
 int16_t filter_rate; /*< [cHz] Rate *10*/
 int16_t spd_coef; /*<  Speed coefficient *10*/
 int16_t str_coef; /*<  Steer coefficient *10*/
 int16_t batv; /*< [cV] Calibrated battery voltage *100*/
 int16_t temp; /*< [cdegC] Calibrated temperature C *10*/
 uint8_t aux_input2_typ; /*<  Aux input2 type*/
} mavlink_hoverboard_event_t;

#define MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN 91
#define MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN 91
#define MAVLINK_MSG_ID_40004_LEN 91
#define MAVLINK_MSG_ID_40004_MIN_LEN 91

#define MAVLINK_MSG_ID_HOVERBOARD_EVENT_CRC 194
#define MAVLINK_MSG_ID_40004_CRC 194



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_HOVERBOARD_EVENT { \
    40004, \
    "HOVERBOARD_EVENT", \
    46, \
    {  { "ctrl_mod", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_hoverboard_event_t, ctrl_mod) }, \
         { "ctrl_typ", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_hoverboard_event_t, ctrl_typ) }, \
         { "cur_mot_max", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_hoverboard_event_t, cur_mot_max) }, \
         { "rpm_mot_max", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_hoverboard_event_t, rpm_mot_max) }, \
         { "fi_weak_ena", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_hoverboard_event_t, fi_weak_ena) }, \
         { "fi_weak_hi", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_hoverboard_event_t, fi_weak_hi) }, \
         { "fi_weak_lo", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_hoverboard_event_t, fi_weak_lo) }, \
         { "fi_weak_max", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_hoverboard_event_t, fi_weak_max) }, \
         { "phase_adv_max_deg", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_hoverboard_event_t, phase_adv_max_deg) }, \
         { "input1_raw", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_hoverboard_event_t, input1_raw) }, \
         { "input1_typ", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_hoverboard_event_t, input1_typ) }, \
         { "input1_min", NULL, MAVLINK_TYPE_INT16_T, 0, 22, offsetof(mavlink_hoverboard_event_t, input1_min) }, \
         { "input1_mid", NULL, MAVLINK_TYPE_INT16_T, 0, 24, offsetof(mavlink_hoverboard_event_t, input1_mid) }, \
         { "input1_max", NULL, MAVLINK_TYPE_INT16_T, 0, 26, offsetof(mavlink_hoverboard_event_t, input1_max) }, \
         { "input1_cmd", NULL, MAVLINK_TYPE_INT16_T, 0, 28, offsetof(mavlink_hoverboard_event_t, input1_cmd) }, \
         { "input2_raw", NULL, MAVLINK_TYPE_INT16_T, 0, 30, offsetof(mavlink_hoverboard_event_t, input2_raw) }, \
         { "input2_typ", NULL, MAVLINK_TYPE_INT16_T, 0, 32, offsetof(mavlink_hoverboard_event_t, input2_typ) }, \
         { "input2_min", NULL, MAVLINK_TYPE_INT16_T, 0, 34, offsetof(mavlink_hoverboard_event_t, input2_min) }, \
         { "input2_mid", NULL, MAVLINK_TYPE_INT16_T, 0, 36, offsetof(mavlink_hoverboard_event_t, input2_mid) }, \
         { "input2_max", NULL, MAVLINK_TYPE_INT16_T, 0, 38, offsetof(mavlink_hoverboard_event_t, input2_max) }, \
         { "input2_cmd", NULL, MAVLINK_TYPE_INT16_T, 0, 40, offsetof(mavlink_hoverboard_event_t, input2_cmd) }, \
         { "aux_input1_raw", NULL, MAVLINK_TYPE_INT16_T, 0, 42, offsetof(mavlink_hoverboard_event_t, aux_input1_raw) }, \
         { "aux_input1_typ", NULL, MAVLINK_TYPE_INT16_T, 0, 44, offsetof(mavlink_hoverboard_event_t, aux_input1_typ) }, \
         { "aux_input1_min", NULL, MAVLINK_TYPE_INT16_T, 0, 46, offsetof(mavlink_hoverboard_event_t, aux_input1_min) }, \
         { "aux_input1_mid", NULL, MAVLINK_TYPE_INT16_T, 0, 48, offsetof(mavlink_hoverboard_event_t, aux_input1_mid) }, \
         { "aux_input1_max", NULL, MAVLINK_TYPE_INT16_T, 0, 50, offsetof(mavlink_hoverboard_event_t, aux_input1_max) }, \
         { "aux_input1_cmd", NULL, MAVLINK_TYPE_INT16_T, 0, 52, offsetof(mavlink_hoverboard_event_t, aux_input1_cmd) }, \
         { "aux_input2_raw", NULL, MAVLINK_TYPE_INT16_T, 0, 54, offsetof(mavlink_hoverboard_event_t, aux_input2_raw) }, \
         { "aux_input2_typ", NULL, MAVLINK_TYPE_UINT8_T, 0, 90, offsetof(mavlink_hoverboard_event_t, aux_input2_typ) }, \
         { "aux_input2_min", NULL, MAVLINK_TYPE_INT16_T, 0, 56, offsetof(mavlink_hoverboard_event_t, aux_input2_min) }, \
         { "aux_input2_mid", NULL, MAVLINK_TYPE_INT16_T, 0, 58, offsetof(mavlink_hoverboard_event_t, aux_input2_mid) }, \
         { "aux_input2_max", NULL, MAVLINK_TYPE_INT16_T, 0, 60, offsetof(mavlink_hoverboard_event_t, aux_input2_max) }, \
         { "aux_input2_cmd", NULL, MAVLINK_TYPE_INT16_T, 0, 62, offsetof(mavlink_hoverboard_event_t, aux_input2_cmd) }, \
         { "dc_curr", NULL, MAVLINK_TYPE_INT16_T, 0, 64, offsetof(mavlink_hoverboard_event_t, dc_curr) }, \
         { "rdc_curr", NULL, MAVLINK_TYPE_INT16_T, 0, 66, offsetof(mavlink_hoverboard_event_t, rdc_curr) }, \
         { "ldc_curr", NULL, MAVLINK_TYPE_INT16_T, 0, 68, offsetof(mavlink_hoverboard_event_t, ldc_curr) }, \
         { "cmdl", NULL, MAVLINK_TYPE_INT16_T, 0, 70, offsetof(mavlink_hoverboard_event_t, cmdl) }, \
         { "cmdr", NULL, MAVLINK_TYPE_INT16_T, 0, 72, offsetof(mavlink_hoverboard_event_t, cmdr) }, \
         { "spd_avg", NULL, MAVLINK_TYPE_INT16_T, 0, 74, offsetof(mavlink_hoverboard_event_t, spd_avg) }, \
         { "spdl", NULL, MAVLINK_TYPE_INT16_T, 0, 76, offsetof(mavlink_hoverboard_event_t, spdl) }, \
         { "spdr", NULL, MAVLINK_TYPE_INT16_T, 0, 78, offsetof(mavlink_hoverboard_event_t, spdr) }, \
         { "filter_rate", NULL, MAVLINK_TYPE_INT16_T, 0, 80, offsetof(mavlink_hoverboard_event_t, filter_rate) }, \
         { "spd_coef", NULL, MAVLINK_TYPE_INT16_T, 0, 82, offsetof(mavlink_hoverboard_event_t, spd_coef) }, \
         { "str_coef", NULL, MAVLINK_TYPE_INT16_T, 0, 84, offsetof(mavlink_hoverboard_event_t, str_coef) }, \
         { "batv", NULL, MAVLINK_TYPE_INT16_T, 0, 86, offsetof(mavlink_hoverboard_event_t, batv) }, \
         { "temp", NULL, MAVLINK_TYPE_INT16_T, 0, 88, offsetof(mavlink_hoverboard_event_t, temp) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_HOVERBOARD_EVENT { \
    "HOVERBOARD_EVENT", \
    46, \
    {  { "ctrl_mod", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_hoverboard_event_t, ctrl_mod) }, \
         { "ctrl_typ", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_hoverboard_event_t, ctrl_typ) }, \
         { "cur_mot_max", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_hoverboard_event_t, cur_mot_max) }, \
         { "rpm_mot_max", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_hoverboard_event_t, rpm_mot_max) }, \
         { "fi_weak_ena", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_hoverboard_event_t, fi_weak_ena) }, \
         { "fi_weak_hi", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_hoverboard_event_t, fi_weak_hi) }, \
         { "fi_weak_lo", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_hoverboard_event_t, fi_weak_lo) }, \
         { "fi_weak_max", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_hoverboard_event_t, fi_weak_max) }, \
         { "phase_adv_max_deg", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_hoverboard_event_t, phase_adv_max_deg) }, \
         { "input1_raw", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_hoverboard_event_t, input1_raw) }, \
         { "input1_typ", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_hoverboard_event_t, input1_typ) }, \
         { "input1_min", NULL, MAVLINK_TYPE_INT16_T, 0, 22, offsetof(mavlink_hoverboard_event_t, input1_min) }, \
         { "input1_mid", NULL, MAVLINK_TYPE_INT16_T, 0, 24, offsetof(mavlink_hoverboard_event_t, input1_mid) }, \
         { "input1_max", NULL, MAVLINK_TYPE_INT16_T, 0, 26, offsetof(mavlink_hoverboard_event_t, input1_max) }, \
         { "input1_cmd", NULL, MAVLINK_TYPE_INT16_T, 0, 28, offsetof(mavlink_hoverboard_event_t, input1_cmd) }, \
         { "input2_raw", NULL, MAVLINK_TYPE_INT16_T, 0, 30, offsetof(mavlink_hoverboard_event_t, input2_raw) }, \
         { "input2_typ", NULL, MAVLINK_TYPE_INT16_T, 0, 32, offsetof(mavlink_hoverboard_event_t, input2_typ) }, \
         { "input2_min", NULL, MAVLINK_TYPE_INT16_T, 0, 34, offsetof(mavlink_hoverboard_event_t, input2_min) }, \
         { "input2_mid", NULL, MAVLINK_TYPE_INT16_T, 0, 36, offsetof(mavlink_hoverboard_event_t, input2_mid) }, \
         { "input2_max", NULL, MAVLINK_TYPE_INT16_T, 0, 38, offsetof(mavlink_hoverboard_event_t, input2_max) }, \
         { "input2_cmd", NULL, MAVLINK_TYPE_INT16_T, 0, 40, offsetof(mavlink_hoverboard_event_t, input2_cmd) }, \
         { "aux_input1_raw", NULL, MAVLINK_TYPE_INT16_T, 0, 42, offsetof(mavlink_hoverboard_event_t, aux_input1_raw) }, \
         { "aux_input1_typ", NULL, MAVLINK_TYPE_INT16_T, 0, 44, offsetof(mavlink_hoverboard_event_t, aux_input1_typ) }, \
         { "aux_input1_min", NULL, MAVLINK_TYPE_INT16_T, 0, 46, offsetof(mavlink_hoverboard_event_t, aux_input1_min) }, \
         { "aux_input1_mid", NULL, MAVLINK_TYPE_INT16_T, 0, 48, offsetof(mavlink_hoverboard_event_t, aux_input1_mid) }, \
         { "aux_input1_max", NULL, MAVLINK_TYPE_INT16_T, 0, 50, offsetof(mavlink_hoverboard_event_t, aux_input1_max) }, \
         { "aux_input1_cmd", NULL, MAVLINK_TYPE_INT16_T, 0, 52, offsetof(mavlink_hoverboard_event_t, aux_input1_cmd) }, \
         { "aux_input2_raw", NULL, MAVLINK_TYPE_INT16_T, 0, 54, offsetof(mavlink_hoverboard_event_t, aux_input2_raw) }, \
         { "aux_input2_typ", NULL, MAVLINK_TYPE_UINT8_T, 0, 90, offsetof(mavlink_hoverboard_event_t, aux_input2_typ) }, \
         { "aux_input2_min", NULL, MAVLINK_TYPE_INT16_T, 0, 56, offsetof(mavlink_hoverboard_event_t, aux_input2_min) }, \
         { "aux_input2_mid", NULL, MAVLINK_TYPE_INT16_T, 0, 58, offsetof(mavlink_hoverboard_event_t, aux_input2_mid) }, \
         { "aux_input2_max", NULL, MAVLINK_TYPE_INT16_T, 0, 60, offsetof(mavlink_hoverboard_event_t, aux_input2_max) }, \
         { "aux_input2_cmd", NULL, MAVLINK_TYPE_INT16_T, 0, 62, offsetof(mavlink_hoverboard_event_t, aux_input2_cmd) }, \
         { "dc_curr", NULL, MAVLINK_TYPE_INT16_T, 0, 64, offsetof(mavlink_hoverboard_event_t, dc_curr) }, \
         { "rdc_curr", NULL, MAVLINK_TYPE_INT16_T, 0, 66, offsetof(mavlink_hoverboard_event_t, rdc_curr) }, \
         { "ldc_curr", NULL, MAVLINK_TYPE_INT16_T, 0, 68, offsetof(mavlink_hoverboard_event_t, ldc_curr) }, \
         { "cmdl", NULL, MAVLINK_TYPE_INT16_T, 0, 70, offsetof(mavlink_hoverboard_event_t, cmdl) }, \
         { "cmdr", NULL, MAVLINK_TYPE_INT16_T, 0, 72, offsetof(mavlink_hoverboard_event_t, cmdr) }, \
         { "spd_avg", NULL, MAVLINK_TYPE_INT16_T, 0, 74, offsetof(mavlink_hoverboard_event_t, spd_avg) }, \
         { "spdl", NULL, MAVLINK_TYPE_INT16_T, 0, 76, offsetof(mavlink_hoverboard_event_t, spdl) }, \
         { "spdr", NULL, MAVLINK_TYPE_INT16_T, 0, 78, offsetof(mavlink_hoverboard_event_t, spdr) }, \
         { "filter_rate", NULL, MAVLINK_TYPE_INT16_T, 0, 80, offsetof(mavlink_hoverboard_event_t, filter_rate) }, \
         { "spd_coef", NULL, MAVLINK_TYPE_INT16_T, 0, 82, offsetof(mavlink_hoverboard_event_t, spd_coef) }, \
         { "str_coef", NULL, MAVLINK_TYPE_INT16_T, 0, 84, offsetof(mavlink_hoverboard_event_t, str_coef) }, \
         { "batv", NULL, MAVLINK_TYPE_INT16_T, 0, 86, offsetof(mavlink_hoverboard_event_t, batv) }, \
         { "temp", NULL, MAVLINK_TYPE_INT16_T, 0, 88, offsetof(mavlink_hoverboard_event_t, temp) }, \
         } \
}
#endif

/**
 * @brief Pack a hoverboard_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param ctrl_mod  Control mode 1:Voltage 2:Speed 3:Torque
 * @param ctrl_typ  Control type 0:Commutation 1:Sinusoidal 2:FOC
 * @param cur_mot_max [A] Max phase current
 * @param rpm_mot_max [rpm] Max motor RPM
 * @param fi_weak_ena  Enable field weakening 0:OFF 1:ON
 * @param fi_weak_hi [rpm] Field weakening high RPM
 * @param fi_weak_lo [rpm] Field weakening low RPM
 * @param fi_weak_max [A] Field weakening max current (FOC only)
 * @param phase_adv_max_deg [deg] Max phase advance angle (SIN only)
 * @param input1_raw  Input1 raw value
 * @param input1_typ  Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param input1_min  Input1 minimum value
 * @param input1_mid  Input1 middle value
 * @param input1_max  Input1 maximum value
 * @param input1_cmd  Input1 command value
 * @param input2_raw  Input2 raw value
 * @param input2_typ  Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param input2_min  Input2 minimum value
 * @param input2_mid  Input2 middle value
 * @param input2_max  Input2 maximum value
 * @param input2_cmd  Input2 command value
 * @param aux_input1_raw  Aux input1 raw value
 * @param aux_input1_typ  Aux input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param aux_input1_min  Aux input1 minimum value
 * @param aux_input1_mid  Aux input1 middle value
 * @param aux_input1_max  Aux input1 maximum value
 * @param aux_input1_cmd  Aux input1 command value
 * @param aux_input2_raw  Aux input2 raw value
 * @param aux_input2_typ  Aux input2 type
 * @param aux_input2_min  Aux input2 minimum value
 * @param aux_input2_mid  Aux input2 middle value
 * @param aux_input2_max  Aux input2 maximum value
 * @param aux_input2_cmd  Aux input2 command value
 * @param dc_curr [cA] Total DC link current A *100
 * @param rdc_curr [cA] Right DC link current A *100
 * @param ldc_curr [cA] Left DC link current A *100
 * @param cmdl [rpm] Left motor command RPM
 * @param cmdr [rpm] Right motor command RPM
 * @param spd_avg [rpm] Motor measured average RPM
 * @param spdl [rpm] Left motor measured RPM
 * @param spdr [rpm] Right motor measured RPM
 * @param filter_rate [cHz] Rate *10
 * @param spd_coef  Speed coefficient *10
 * @param str_coef  Steer coefficient *10
 * @param batv [cV] Calibrated battery voltage *100
 * @param temp [cdegC] Calibrated temperature C *10
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hoverboard_event_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               int16_t ctrl_mod, int16_t ctrl_typ, int16_t cur_mot_max, int16_t rpm_mot_max, int16_t fi_weak_ena, int16_t fi_weak_hi, int16_t fi_weak_lo, int16_t fi_weak_max, int16_t phase_adv_max_deg, int16_t input1_raw, int16_t input1_typ, int16_t input1_min, int16_t input1_mid, int16_t input1_max, int16_t input1_cmd, int16_t input2_raw, int16_t input2_typ, int16_t input2_min, int16_t input2_mid, int16_t input2_max, int16_t input2_cmd, int16_t aux_input1_raw, int16_t aux_input1_typ, int16_t aux_input1_min, int16_t aux_input1_mid, int16_t aux_input1_max, int16_t aux_input1_cmd, int16_t aux_input2_raw, uint8_t aux_input2_typ, int16_t aux_input2_min, int16_t aux_input2_mid, int16_t aux_input2_max, int16_t aux_input2_cmd, int16_t dc_curr, int16_t rdc_curr, int16_t ldc_curr, int16_t cmdl, int16_t cmdr, int16_t spd_avg, int16_t spdl, int16_t spdr, int16_t filter_rate, int16_t spd_coef, int16_t str_coef, int16_t batv, int16_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN];
    _mav_put_int16_t(buf, 0, ctrl_mod);
    _mav_put_int16_t(buf, 2, ctrl_typ);
    _mav_put_int16_t(buf, 4, cur_mot_max);
    _mav_put_int16_t(buf, 6, rpm_mot_max);
    _mav_put_int16_t(buf, 8, fi_weak_ena);
    _mav_put_int16_t(buf, 10, fi_weak_hi);
    _mav_put_int16_t(buf, 12, fi_weak_lo);
    _mav_put_int16_t(buf, 14, fi_weak_max);
    _mav_put_int16_t(buf, 16, phase_adv_max_deg);
    _mav_put_int16_t(buf, 18, input1_raw);
    _mav_put_int16_t(buf, 20, input1_typ);
    _mav_put_int16_t(buf, 22, input1_min);
    _mav_put_int16_t(buf, 24, input1_mid);
    _mav_put_int16_t(buf, 26, input1_max);
    _mav_put_int16_t(buf, 28, input1_cmd);
    _mav_put_int16_t(buf, 30, input2_raw);
    _mav_put_int16_t(buf, 32, input2_typ);
    _mav_put_int16_t(buf, 34, input2_min);
    _mav_put_int16_t(buf, 36, input2_mid);
    _mav_put_int16_t(buf, 38, input2_max);
    _mav_put_int16_t(buf, 40, input2_cmd);
    _mav_put_int16_t(buf, 42, aux_input1_raw);
    _mav_put_int16_t(buf, 44, aux_input1_typ);
    _mav_put_int16_t(buf, 46, aux_input1_min);
    _mav_put_int16_t(buf, 48, aux_input1_mid);
    _mav_put_int16_t(buf, 50, aux_input1_max);
    _mav_put_int16_t(buf, 52, aux_input1_cmd);
    _mav_put_int16_t(buf, 54, aux_input2_raw);
    _mav_put_int16_t(buf, 56, aux_input2_min);
    _mav_put_int16_t(buf, 58, aux_input2_mid);
    _mav_put_int16_t(buf, 60, aux_input2_max);
    _mav_put_int16_t(buf, 62, aux_input2_cmd);
    _mav_put_int16_t(buf, 64, dc_curr);
    _mav_put_int16_t(buf, 66, rdc_curr);
    _mav_put_int16_t(buf, 68, ldc_curr);
    _mav_put_int16_t(buf, 70, cmdl);
    _mav_put_int16_t(buf, 72, cmdr);
    _mav_put_int16_t(buf, 74, spd_avg);
    _mav_put_int16_t(buf, 76, spdl);
    _mav_put_int16_t(buf, 78, spdr);
    _mav_put_int16_t(buf, 80, filter_rate);
    _mav_put_int16_t(buf, 82, spd_coef);
    _mav_put_int16_t(buf, 84, str_coef);
    _mav_put_int16_t(buf, 86, batv);
    _mav_put_int16_t(buf, 88, temp);
    _mav_put_uint8_t(buf, 90, aux_input2_typ);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN);
#else
    mavlink_hoverboard_event_t packet;
    packet.ctrl_mod = ctrl_mod;
    packet.ctrl_typ = ctrl_typ;
    packet.cur_mot_max = cur_mot_max;
    packet.rpm_mot_max = rpm_mot_max;
    packet.fi_weak_ena = fi_weak_ena;
    packet.fi_weak_hi = fi_weak_hi;
    packet.fi_weak_lo = fi_weak_lo;
    packet.fi_weak_max = fi_weak_max;
    packet.phase_adv_max_deg = phase_adv_max_deg;
    packet.input1_raw = input1_raw;
    packet.input1_typ = input1_typ;
    packet.input1_min = input1_min;
    packet.input1_mid = input1_mid;
    packet.input1_max = input1_max;
    packet.input1_cmd = input1_cmd;
    packet.input2_raw = input2_raw;
    packet.input2_typ = input2_typ;
    packet.input2_min = input2_min;
    packet.input2_mid = input2_mid;
    packet.input2_max = input2_max;
    packet.input2_cmd = input2_cmd;
    packet.aux_input1_raw = aux_input1_raw;
    packet.aux_input1_typ = aux_input1_typ;
    packet.aux_input1_min = aux_input1_min;
    packet.aux_input1_mid = aux_input1_mid;
    packet.aux_input1_max = aux_input1_max;
    packet.aux_input1_cmd = aux_input1_cmd;
    packet.aux_input2_raw = aux_input2_raw;
    packet.aux_input2_min = aux_input2_min;
    packet.aux_input2_mid = aux_input2_mid;
    packet.aux_input2_max = aux_input2_max;
    packet.aux_input2_cmd = aux_input2_cmd;
    packet.dc_curr = dc_curr;
    packet.rdc_curr = rdc_curr;
    packet.ldc_curr = ldc_curr;
    packet.cmdl = cmdl;
    packet.cmdr = cmdr;
    packet.spd_avg = spd_avg;
    packet.spdl = spdl;
    packet.spdr = spdr;
    packet.filter_rate = filter_rate;
    packet.spd_coef = spd_coef;
    packet.str_coef = str_coef;
    packet.batv = batv;
    packet.temp = temp;
    packet.aux_input2_typ = aux_input2_typ;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HOVERBOARD_EVENT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_CRC);
}

/**
 * @brief Pack a hoverboard_event message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param ctrl_mod  Control mode 1:Voltage 2:Speed 3:Torque
 * @param ctrl_typ  Control type 0:Commutation 1:Sinusoidal 2:FOC
 * @param cur_mot_max [A] Max phase current
 * @param rpm_mot_max [rpm] Max motor RPM
 * @param fi_weak_ena  Enable field weakening 0:OFF 1:ON
 * @param fi_weak_hi [rpm] Field weakening high RPM
 * @param fi_weak_lo [rpm] Field weakening low RPM
 * @param fi_weak_max [A] Field weakening max current (FOC only)
 * @param phase_adv_max_deg [deg] Max phase advance angle (SIN only)
 * @param input1_raw  Input1 raw value
 * @param input1_typ  Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param input1_min  Input1 minimum value
 * @param input1_mid  Input1 middle value
 * @param input1_max  Input1 maximum value
 * @param input1_cmd  Input1 command value
 * @param input2_raw  Input2 raw value
 * @param input2_typ  Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param input2_min  Input2 minimum value
 * @param input2_mid  Input2 middle value
 * @param input2_max  Input2 maximum value
 * @param input2_cmd  Input2 command value
 * @param aux_input1_raw  Aux input1 raw value
 * @param aux_input1_typ  Aux input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param aux_input1_min  Aux input1 minimum value
 * @param aux_input1_mid  Aux input1 middle value
 * @param aux_input1_max  Aux input1 maximum value
 * @param aux_input1_cmd  Aux input1 command value
 * @param aux_input2_raw  Aux input2 raw value
 * @param aux_input2_typ  Aux input2 type
 * @param aux_input2_min  Aux input2 minimum value
 * @param aux_input2_mid  Aux input2 middle value
 * @param aux_input2_max  Aux input2 maximum value
 * @param aux_input2_cmd  Aux input2 command value
 * @param dc_curr [cA] Total DC link current A *100
 * @param rdc_curr [cA] Right DC link current A *100
 * @param ldc_curr [cA] Left DC link current A *100
 * @param cmdl [rpm] Left motor command RPM
 * @param cmdr [rpm] Right motor command RPM
 * @param spd_avg [rpm] Motor measured average RPM
 * @param spdl [rpm] Left motor measured RPM
 * @param spdr [rpm] Right motor measured RPM
 * @param filter_rate [cHz] Rate *10
 * @param spd_coef  Speed coefficient *10
 * @param str_coef  Steer coefficient *10
 * @param batv [cV] Calibrated battery voltage *100
 * @param temp [cdegC] Calibrated temperature C *10
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hoverboard_event_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               int16_t ctrl_mod, int16_t ctrl_typ, int16_t cur_mot_max, int16_t rpm_mot_max, int16_t fi_weak_ena, int16_t fi_weak_hi, int16_t fi_weak_lo, int16_t fi_weak_max, int16_t phase_adv_max_deg, int16_t input1_raw, int16_t input1_typ, int16_t input1_min, int16_t input1_mid, int16_t input1_max, int16_t input1_cmd, int16_t input2_raw, int16_t input2_typ, int16_t input2_min, int16_t input2_mid, int16_t input2_max, int16_t input2_cmd, int16_t aux_input1_raw, int16_t aux_input1_typ, int16_t aux_input1_min, int16_t aux_input1_mid, int16_t aux_input1_max, int16_t aux_input1_cmd, int16_t aux_input2_raw, uint8_t aux_input2_typ, int16_t aux_input2_min, int16_t aux_input2_mid, int16_t aux_input2_max, int16_t aux_input2_cmd, int16_t dc_curr, int16_t rdc_curr, int16_t ldc_curr, int16_t cmdl, int16_t cmdr, int16_t spd_avg, int16_t spdl, int16_t spdr, int16_t filter_rate, int16_t spd_coef, int16_t str_coef, int16_t batv, int16_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN];
    _mav_put_int16_t(buf, 0, ctrl_mod);
    _mav_put_int16_t(buf, 2, ctrl_typ);
    _mav_put_int16_t(buf, 4, cur_mot_max);
    _mav_put_int16_t(buf, 6, rpm_mot_max);
    _mav_put_int16_t(buf, 8, fi_weak_ena);
    _mav_put_int16_t(buf, 10, fi_weak_hi);
    _mav_put_int16_t(buf, 12, fi_weak_lo);
    _mav_put_int16_t(buf, 14, fi_weak_max);
    _mav_put_int16_t(buf, 16, phase_adv_max_deg);
    _mav_put_int16_t(buf, 18, input1_raw);
    _mav_put_int16_t(buf, 20, input1_typ);
    _mav_put_int16_t(buf, 22, input1_min);
    _mav_put_int16_t(buf, 24, input1_mid);
    _mav_put_int16_t(buf, 26, input1_max);
    _mav_put_int16_t(buf, 28, input1_cmd);
    _mav_put_int16_t(buf, 30, input2_raw);
    _mav_put_int16_t(buf, 32, input2_typ);
    _mav_put_int16_t(buf, 34, input2_min);
    _mav_put_int16_t(buf, 36, input2_mid);
    _mav_put_int16_t(buf, 38, input2_max);
    _mav_put_int16_t(buf, 40, input2_cmd);
    _mav_put_int16_t(buf, 42, aux_input1_raw);
    _mav_put_int16_t(buf, 44, aux_input1_typ);
    _mav_put_int16_t(buf, 46, aux_input1_min);
    _mav_put_int16_t(buf, 48, aux_input1_mid);
    _mav_put_int16_t(buf, 50, aux_input1_max);
    _mav_put_int16_t(buf, 52, aux_input1_cmd);
    _mav_put_int16_t(buf, 54, aux_input2_raw);
    _mav_put_int16_t(buf, 56, aux_input2_min);
    _mav_put_int16_t(buf, 58, aux_input2_mid);
    _mav_put_int16_t(buf, 60, aux_input2_max);
    _mav_put_int16_t(buf, 62, aux_input2_cmd);
    _mav_put_int16_t(buf, 64, dc_curr);
    _mav_put_int16_t(buf, 66, rdc_curr);
    _mav_put_int16_t(buf, 68, ldc_curr);
    _mav_put_int16_t(buf, 70, cmdl);
    _mav_put_int16_t(buf, 72, cmdr);
    _mav_put_int16_t(buf, 74, spd_avg);
    _mav_put_int16_t(buf, 76, spdl);
    _mav_put_int16_t(buf, 78, spdr);
    _mav_put_int16_t(buf, 80, filter_rate);
    _mav_put_int16_t(buf, 82, spd_coef);
    _mav_put_int16_t(buf, 84, str_coef);
    _mav_put_int16_t(buf, 86, batv);
    _mav_put_int16_t(buf, 88, temp);
    _mav_put_uint8_t(buf, 90, aux_input2_typ);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN);
#else
    mavlink_hoverboard_event_t packet;
    packet.ctrl_mod = ctrl_mod;
    packet.ctrl_typ = ctrl_typ;
    packet.cur_mot_max = cur_mot_max;
    packet.rpm_mot_max = rpm_mot_max;
    packet.fi_weak_ena = fi_weak_ena;
    packet.fi_weak_hi = fi_weak_hi;
    packet.fi_weak_lo = fi_weak_lo;
    packet.fi_weak_max = fi_weak_max;
    packet.phase_adv_max_deg = phase_adv_max_deg;
    packet.input1_raw = input1_raw;
    packet.input1_typ = input1_typ;
    packet.input1_min = input1_min;
    packet.input1_mid = input1_mid;
    packet.input1_max = input1_max;
    packet.input1_cmd = input1_cmd;
    packet.input2_raw = input2_raw;
    packet.input2_typ = input2_typ;
    packet.input2_min = input2_min;
    packet.input2_mid = input2_mid;
    packet.input2_max = input2_max;
    packet.input2_cmd = input2_cmd;
    packet.aux_input1_raw = aux_input1_raw;
    packet.aux_input1_typ = aux_input1_typ;
    packet.aux_input1_min = aux_input1_min;
    packet.aux_input1_mid = aux_input1_mid;
    packet.aux_input1_max = aux_input1_max;
    packet.aux_input1_cmd = aux_input1_cmd;
    packet.aux_input2_raw = aux_input2_raw;
    packet.aux_input2_min = aux_input2_min;
    packet.aux_input2_mid = aux_input2_mid;
    packet.aux_input2_max = aux_input2_max;
    packet.aux_input2_cmd = aux_input2_cmd;
    packet.dc_curr = dc_curr;
    packet.rdc_curr = rdc_curr;
    packet.ldc_curr = ldc_curr;
    packet.cmdl = cmdl;
    packet.cmdr = cmdr;
    packet.spd_avg = spd_avg;
    packet.spdl = spdl;
    packet.spdr = spdr;
    packet.filter_rate = filter_rate;
    packet.spd_coef = spd_coef;
    packet.str_coef = str_coef;
    packet.batv = batv;
    packet.temp = temp;
    packet.aux_input2_typ = aux_input2_typ;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HOVERBOARD_EVENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN);
#endif
}

/**
 * @brief Pack a hoverboard_event message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ctrl_mod  Control mode 1:Voltage 2:Speed 3:Torque
 * @param ctrl_typ  Control type 0:Commutation 1:Sinusoidal 2:FOC
 * @param cur_mot_max [A] Max phase current
 * @param rpm_mot_max [rpm] Max motor RPM
 * @param fi_weak_ena  Enable field weakening 0:OFF 1:ON
 * @param fi_weak_hi [rpm] Field weakening high RPM
 * @param fi_weak_lo [rpm] Field weakening low RPM
 * @param fi_weak_max [A] Field weakening max current (FOC only)
 * @param phase_adv_max_deg [deg] Max phase advance angle (SIN only)
 * @param input1_raw  Input1 raw value
 * @param input1_typ  Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param input1_min  Input1 minimum value
 * @param input1_mid  Input1 middle value
 * @param input1_max  Input1 maximum value
 * @param input1_cmd  Input1 command value
 * @param input2_raw  Input2 raw value
 * @param input2_typ  Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param input2_min  Input2 minimum value
 * @param input2_mid  Input2 middle value
 * @param input2_max  Input2 maximum value
 * @param input2_cmd  Input2 command value
 * @param aux_input1_raw  Aux input1 raw value
 * @param aux_input1_typ  Aux input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param aux_input1_min  Aux input1 minimum value
 * @param aux_input1_mid  Aux input1 middle value
 * @param aux_input1_max  Aux input1 maximum value
 * @param aux_input1_cmd  Aux input1 command value
 * @param aux_input2_raw  Aux input2 raw value
 * @param aux_input2_typ  Aux input2 type
 * @param aux_input2_min  Aux input2 minimum value
 * @param aux_input2_mid  Aux input2 middle value
 * @param aux_input2_max  Aux input2 maximum value
 * @param aux_input2_cmd  Aux input2 command value
 * @param dc_curr [cA] Total DC link current A *100
 * @param rdc_curr [cA] Right DC link current A *100
 * @param ldc_curr [cA] Left DC link current A *100
 * @param cmdl [rpm] Left motor command RPM
 * @param cmdr [rpm] Right motor command RPM
 * @param spd_avg [rpm] Motor measured average RPM
 * @param spdl [rpm] Left motor measured RPM
 * @param spdr [rpm] Right motor measured RPM
 * @param filter_rate [cHz] Rate *10
 * @param spd_coef  Speed coefficient *10
 * @param str_coef  Steer coefficient *10
 * @param batv [cV] Calibrated battery voltage *100
 * @param temp [cdegC] Calibrated temperature C *10
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hoverboard_event_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   int16_t ctrl_mod,int16_t ctrl_typ,int16_t cur_mot_max,int16_t rpm_mot_max,int16_t fi_weak_ena,int16_t fi_weak_hi,int16_t fi_weak_lo,int16_t fi_weak_max,int16_t phase_adv_max_deg,int16_t input1_raw,int16_t input1_typ,int16_t input1_min,int16_t input1_mid,int16_t input1_max,int16_t input1_cmd,int16_t input2_raw,int16_t input2_typ,int16_t input2_min,int16_t input2_mid,int16_t input2_max,int16_t input2_cmd,int16_t aux_input1_raw,int16_t aux_input1_typ,int16_t aux_input1_min,int16_t aux_input1_mid,int16_t aux_input1_max,int16_t aux_input1_cmd,int16_t aux_input2_raw,uint8_t aux_input2_typ,int16_t aux_input2_min,int16_t aux_input2_mid,int16_t aux_input2_max,int16_t aux_input2_cmd,int16_t dc_curr,int16_t rdc_curr,int16_t ldc_curr,int16_t cmdl,int16_t cmdr,int16_t spd_avg,int16_t spdl,int16_t spdr,int16_t filter_rate,int16_t spd_coef,int16_t str_coef,int16_t batv,int16_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN];
    _mav_put_int16_t(buf, 0, ctrl_mod);
    _mav_put_int16_t(buf, 2, ctrl_typ);
    _mav_put_int16_t(buf, 4, cur_mot_max);
    _mav_put_int16_t(buf, 6, rpm_mot_max);
    _mav_put_int16_t(buf, 8, fi_weak_ena);
    _mav_put_int16_t(buf, 10, fi_weak_hi);
    _mav_put_int16_t(buf, 12, fi_weak_lo);
    _mav_put_int16_t(buf, 14, fi_weak_max);
    _mav_put_int16_t(buf, 16, phase_adv_max_deg);
    _mav_put_int16_t(buf, 18, input1_raw);
    _mav_put_int16_t(buf, 20, input1_typ);
    _mav_put_int16_t(buf, 22, input1_min);
    _mav_put_int16_t(buf, 24, input1_mid);
    _mav_put_int16_t(buf, 26, input1_max);
    _mav_put_int16_t(buf, 28, input1_cmd);
    _mav_put_int16_t(buf, 30, input2_raw);
    _mav_put_int16_t(buf, 32, input2_typ);
    _mav_put_int16_t(buf, 34, input2_min);
    _mav_put_int16_t(buf, 36, input2_mid);
    _mav_put_int16_t(buf, 38, input2_max);
    _mav_put_int16_t(buf, 40, input2_cmd);
    _mav_put_int16_t(buf, 42, aux_input1_raw);
    _mav_put_int16_t(buf, 44, aux_input1_typ);
    _mav_put_int16_t(buf, 46, aux_input1_min);
    _mav_put_int16_t(buf, 48, aux_input1_mid);
    _mav_put_int16_t(buf, 50, aux_input1_max);
    _mav_put_int16_t(buf, 52, aux_input1_cmd);
    _mav_put_int16_t(buf, 54, aux_input2_raw);
    _mav_put_int16_t(buf, 56, aux_input2_min);
    _mav_put_int16_t(buf, 58, aux_input2_mid);
    _mav_put_int16_t(buf, 60, aux_input2_max);
    _mav_put_int16_t(buf, 62, aux_input2_cmd);
    _mav_put_int16_t(buf, 64, dc_curr);
    _mav_put_int16_t(buf, 66, rdc_curr);
    _mav_put_int16_t(buf, 68, ldc_curr);
    _mav_put_int16_t(buf, 70, cmdl);
    _mav_put_int16_t(buf, 72, cmdr);
    _mav_put_int16_t(buf, 74, spd_avg);
    _mav_put_int16_t(buf, 76, spdl);
    _mav_put_int16_t(buf, 78, spdr);
    _mav_put_int16_t(buf, 80, filter_rate);
    _mav_put_int16_t(buf, 82, spd_coef);
    _mav_put_int16_t(buf, 84, str_coef);
    _mav_put_int16_t(buf, 86, batv);
    _mav_put_int16_t(buf, 88, temp);
    _mav_put_uint8_t(buf, 90, aux_input2_typ);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN);
#else
    mavlink_hoverboard_event_t packet;
    packet.ctrl_mod = ctrl_mod;
    packet.ctrl_typ = ctrl_typ;
    packet.cur_mot_max = cur_mot_max;
    packet.rpm_mot_max = rpm_mot_max;
    packet.fi_weak_ena = fi_weak_ena;
    packet.fi_weak_hi = fi_weak_hi;
    packet.fi_weak_lo = fi_weak_lo;
    packet.fi_weak_max = fi_weak_max;
    packet.phase_adv_max_deg = phase_adv_max_deg;
    packet.input1_raw = input1_raw;
    packet.input1_typ = input1_typ;
    packet.input1_min = input1_min;
    packet.input1_mid = input1_mid;
    packet.input1_max = input1_max;
    packet.input1_cmd = input1_cmd;
    packet.input2_raw = input2_raw;
    packet.input2_typ = input2_typ;
    packet.input2_min = input2_min;
    packet.input2_mid = input2_mid;
    packet.input2_max = input2_max;
    packet.input2_cmd = input2_cmd;
    packet.aux_input1_raw = aux_input1_raw;
    packet.aux_input1_typ = aux_input1_typ;
    packet.aux_input1_min = aux_input1_min;
    packet.aux_input1_mid = aux_input1_mid;
    packet.aux_input1_max = aux_input1_max;
    packet.aux_input1_cmd = aux_input1_cmd;
    packet.aux_input2_raw = aux_input2_raw;
    packet.aux_input2_min = aux_input2_min;
    packet.aux_input2_mid = aux_input2_mid;
    packet.aux_input2_max = aux_input2_max;
    packet.aux_input2_cmd = aux_input2_cmd;
    packet.dc_curr = dc_curr;
    packet.rdc_curr = rdc_curr;
    packet.ldc_curr = ldc_curr;
    packet.cmdl = cmdl;
    packet.cmdr = cmdr;
    packet.spd_avg = spd_avg;
    packet.spdl = spdl;
    packet.spdr = spdr;
    packet.filter_rate = filter_rate;
    packet.spd_coef = spd_coef;
    packet.str_coef = str_coef;
    packet.batv = batv;
    packet.temp = temp;
    packet.aux_input2_typ = aux_input2_typ;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HOVERBOARD_EVENT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_CRC);
}

/**
 * @brief Encode a hoverboard_event struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param hoverboard_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hoverboard_event_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_hoverboard_event_t* hoverboard_event)
{
    return mavlink_msg_hoverboard_event_pack(system_id, component_id, msg, hoverboard_event->ctrl_mod, hoverboard_event->ctrl_typ, hoverboard_event->cur_mot_max, hoverboard_event->rpm_mot_max, hoverboard_event->fi_weak_ena, hoverboard_event->fi_weak_hi, hoverboard_event->fi_weak_lo, hoverboard_event->fi_weak_max, hoverboard_event->phase_adv_max_deg, hoverboard_event->input1_raw, hoverboard_event->input1_typ, hoverboard_event->input1_min, hoverboard_event->input1_mid, hoverboard_event->input1_max, hoverboard_event->input1_cmd, hoverboard_event->input2_raw, hoverboard_event->input2_typ, hoverboard_event->input2_min, hoverboard_event->input2_mid, hoverboard_event->input2_max, hoverboard_event->input2_cmd, hoverboard_event->aux_input1_raw, hoverboard_event->aux_input1_typ, hoverboard_event->aux_input1_min, hoverboard_event->aux_input1_mid, hoverboard_event->aux_input1_max, hoverboard_event->aux_input1_cmd, hoverboard_event->aux_input2_raw, hoverboard_event->aux_input2_typ, hoverboard_event->aux_input2_min, hoverboard_event->aux_input2_mid, hoverboard_event->aux_input2_max, hoverboard_event->aux_input2_cmd, hoverboard_event->dc_curr, hoverboard_event->rdc_curr, hoverboard_event->ldc_curr, hoverboard_event->cmdl, hoverboard_event->cmdr, hoverboard_event->spd_avg, hoverboard_event->spdl, hoverboard_event->spdr, hoverboard_event->filter_rate, hoverboard_event->spd_coef, hoverboard_event->str_coef, hoverboard_event->batv, hoverboard_event->temp);
}

/**
 * @brief Encode a hoverboard_event struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param hoverboard_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hoverboard_event_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_hoverboard_event_t* hoverboard_event)
{
    return mavlink_msg_hoverboard_event_pack_chan(system_id, component_id, chan, msg, hoverboard_event->ctrl_mod, hoverboard_event->ctrl_typ, hoverboard_event->cur_mot_max, hoverboard_event->rpm_mot_max, hoverboard_event->fi_weak_ena, hoverboard_event->fi_weak_hi, hoverboard_event->fi_weak_lo, hoverboard_event->fi_weak_max, hoverboard_event->phase_adv_max_deg, hoverboard_event->input1_raw, hoverboard_event->input1_typ, hoverboard_event->input1_min, hoverboard_event->input1_mid, hoverboard_event->input1_max, hoverboard_event->input1_cmd, hoverboard_event->input2_raw, hoverboard_event->input2_typ, hoverboard_event->input2_min, hoverboard_event->input2_mid, hoverboard_event->input2_max, hoverboard_event->input2_cmd, hoverboard_event->aux_input1_raw, hoverboard_event->aux_input1_typ, hoverboard_event->aux_input1_min, hoverboard_event->aux_input1_mid, hoverboard_event->aux_input1_max, hoverboard_event->aux_input1_cmd, hoverboard_event->aux_input2_raw, hoverboard_event->aux_input2_typ, hoverboard_event->aux_input2_min, hoverboard_event->aux_input2_mid, hoverboard_event->aux_input2_max, hoverboard_event->aux_input2_cmd, hoverboard_event->dc_curr, hoverboard_event->rdc_curr, hoverboard_event->ldc_curr, hoverboard_event->cmdl, hoverboard_event->cmdr, hoverboard_event->spd_avg, hoverboard_event->spdl, hoverboard_event->spdr, hoverboard_event->filter_rate, hoverboard_event->spd_coef, hoverboard_event->str_coef, hoverboard_event->batv, hoverboard_event->temp);
}

/**
 * @brief Encode a hoverboard_event struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param hoverboard_event C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hoverboard_event_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_hoverboard_event_t* hoverboard_event)
{
    return mavlink_msg_hoverboard_event_pack_status(system_id, component_id, _status, msg,  hoverboard_event->ctrl_mod, hoverboard_event->ctrl_typ, hoverboard_event->cur_mot_max, hoverboard_event->rpm_mot_max, hoverboard_event->fi_weak_ena, hoverboard_event->fi_weak_hi, hoverboard_event->fi_weak_lo, hoverboard_event->fi_weak_max, hoverboard_event->phase_adv_max_deg, hoverboard_event->input1_raw, hoverboard_event->input1_typ, hoverboard_event->input1_min, hoverboard_event->input1_mid, hoverboard_event->input1_max, hoverboard_event->input1_cmd, hoverboard_event->input2_raw, hoverboard_event->input2_typ, hoverboard_event->input2_min, hoverboard_event->input2_mid, hoverboard_event->input2_max, hoverboard_event->input2_cmd, hoverboard_event->aux_input1_raw, hoverboard_event->aux_input1_typ, hoverboard_event->aux_input1_min, hoverboard_event->aux_input1_mid, hoverboard_event->aux_input1_max, hoverboard_event->aux_input1_cmd, hoverboard_event->aux_input2_raw, hoverboard_event->aux_input2_typ, hoverboard_event->aux_input2_min, hoverboard_event->aux_input2_mid, hoverboard_event->aux_input2_max, hoverboard_event->aux_input2_cmd, hoverboard_event->dc_curr, hoverboard_event->rdc_curr, hoverboard_event->ldc_curr, hoverboard_event->cmdl, hoverboard_event->cmdr, hoverboard_event->spd_avg, hoverboard_event->spdl, hoverboard_event->spdr, hoverboard_event->filter_rate, hoverboard_event->spd_coef, hoverboard_event->str_coef, hoverboard_event->batv, hoverboard_event->temp);
}

/**
 * @brief Send a hoverboard_event message
 * @param chan MAVLink channel to send the message
 *
 * @param ctrl_mod  Control mode 1:Voltage 2:Speed 3:Torque
 * @param ctrl_typ  Control type 0:Commutation 1:Sinusoidal 2:FOC
 * @param cur_mot_max [A] Max phase current
 * @param rpm_mot_max [rpm] Max motor RPM
 * @param fi_weak_ena  Enable field weakening 0:OFF 1:ON
 * @param fi_weak_hi [rpm] Field weakening high RPM
 * @param fi_weak_lo [rpm] Field weakening low RPM
 * @param fi_weak_max [A] Field weakening max current (FOC only)
 * @param phase_adv_max_deg [deg] Max phase advance angle (SIN only)
 * @param input1_raw  Input1 raw value
 * @param input1_typ  Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param input1_min  Input1 minimum value
 * @param input1_mid  Input1 middle value
 * @param input1_max  Input1 maximum value
 * @param input1_cmd  Input1 command value
 * @param input2_raw  Input2 raw value
 * @param input2_typ  Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param input2_min  Input2 minimum value
 * @param input2_mid  Input2 middle value
 * @param input2_max  Input2 maximum value
 * @param input2_cmd  Input2 command value
 * @param aux_input1_raw  Aux input1 raw value
 * @param aux_input1_typ  Aux input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 * @param aux_input1_min  Aux input1 minimum value
 * @param aux_input1_mid  Aux input1 middle value
 * @param aux_input1_max  Aux input1 maximum value
 * @param aux_input1_cmd  Aux input1 command value
 * @param aux_input2_raw  Aux input2 raw value
 * @param aux_input2_typ  Aux input2 type
 * @param aux_input2_min  Aux input2 minimum value
 * @param aux_input2_mid  Aux input2 middle value
 * @param aux_input2_max  Aux input2 maximum value
 * @param aux_input2_cmd  Aux input2 command value
 * @param dc_curr [cA] Total DC link current A *100
 * @param rdc_curr [cA] Right DC link current A *100
 * @param ldc_curr [cA] Left DC link current A *100
 * @param cmdl [rpm] Left motor command RPM
 * @param cmdr [rpm] Right motor command RPM
 * @param spd_avg [rpm] Motor measured average RPM
 * @param spdl [rpm] Left motor measured RPM
 * @param spdr [rpm] Right motor measured RPM
 * @param filter_rate [cHz] Rate *10
 * @param spd_coef  Speed coefficient *10
 * @param str_coef  Steer coefficient *10
 * @param batv [cV] Calibrated battery voltage *100
 * @param temp [cdegC] Calibrated temperature C *10
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_hoverboard_event_send(mavlink_channel_t chan, int16_t ctrl_mod, int16_t ctrl_typ, int16_t cur_mot_max, int16_t rpm_mot_max, int16_t fi_weak_ena, int16_t fi_weak_hi, int16_t fi_weak_lo, int16_t fi_weak_max, int16_t phase_adv_max_deg, int16_t input1_raw, int16_t input1_typ, int16_t input1_min, int16_t input1_mid, int16_t input1_max, int16_t input1_cmd, int16_t input2_raw, int16_t input2_typ, int16_t input2_min, int16_t input2_mid, int16_t input2_max, int16_t input2_cmd, int16_t aux_input1_raw, int16_t aux_input1_typ, int16_t aux_input1_min, int16_t aux_input1_mid, int16_t aux_input1_max, int16_t aux_input1_cmd, int16_t aux_input2_raw, uint8_t aux_input2_typ, int16_t aux_input2_min, int16_t aux_input2_mid, int16_t aux_input2_max, int16_t aux_input2_cmd, int16_t dc_curr, int16_t rdc_curr, int16_t ldc_curr, int16_t cmdl, int16_t cmdr, int16_t spd_avg, int16_t spdl, int16_t spdr, int16_t filter_rate, int16_t spd_coef, int16_t str_coef, int16_t batv, int16_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN];
    _mav_put_int16_t(buf, 0, ctrl_mod);
    _mav_put_int16_t(buf, 2, ctrl_typ);
    _mav_put_int16_t(buf, 4, cur_mot_max);
    _mav_put_int16_t(buf, 6, rpm_mot_max);
    _mav_put_int16_t(buf, 8, fi_weak_ena);
    _mav_put_int16_t(buf, 10, fi_weak_hi);
    _mav_put_int16_t(buf, 12, fi_weak_lo);
    _mav_put_int16_t(buf, 14, fi_weak_max);
    _mav_put_int16_t(buf, 16, phase_adv_max_deg);
    _mav_put_int16_t(buf, 18, input1_raw);
    _mav_put_int16_t(buf, 20, input1_typ);
    _mav_put_int16_t(buf, 22, input1_min);
    _mav_put_int16_t(buf, 24, input1_mid);
    _mav_put_int16_t(buf, 26, input1_max);
    _mav_put_int16_t(buf, 28, input1_cmd);
    _mav_put_int16_t(buf, 30, input2_raw);
    _mav_put_int16_t(buf, 32, input2_typ);
    _mav_put_int16_t(buf, 34, input2_min);
    _mav_put_int16_t(buf, 36, input2_mid);
    _mav_put_int16_t(buf, 38, input2_max);
    _mav_put_int16_t(buf, 40, input2_cmd);
    _mav_put_int16_t(buf, 42, aux_input1_raw);
    _mav_put_int16_t(buf, 44, aux_input1_typ);
    _mav_put_int16_t(buf, 46, aux_input1_min);
    _mav_put_int16_t(buf, 48, aux_input1_mid);
    _mav_put_int16_t(buf, 50, aux_input1_max);
    _mav_put_int16_t(buf, 52, aux_input1_cmd);
    _mav_put_int16_t(buf, 54, aux_input2_raw);
    _mav_put_int16_t(buf, 56, aux_input2_min);
    _mav_put_int16_t(buf, 58, aux_input2_mid);
    _mav_put_int16_t(buf, 60, aux_input2_max);
    _mav_put_int16_t(buf, 62, aux_input2_cmd);
    _mav_put_int16_t(buf, 64, dc_curr);
    _mav_put_int16_t(buf, 66, rdc_curr);
    _mav_put_int16_t(buf, 68, ldc_curr);
    _mav_put_int16_t(buf, 70, cmdl);
    _mav_put_int16_t(buf, 72, cmdr);
    _mav_put_int16_t(buf, 74, spd_avg);
    _mav_put_int16_t(buf, 76, spdl);
    _mav_put_int16_t(buf, 78, spdr);
    _mav_put_int16_t(buf, 80, filter_rate);
    _mav_put_int16_t(buf, 82, spd_coef);
    _mav_put_int16_t(buf, 84, str_coef);
    _mav_put_int16_t(buf, 86, batv);
    _mav_put_int16_t(buf, 88, temp);
    _mav_put_uint8_t(buf, 90, aux_input2_typ);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_EVENT, buf, MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_CRC);
#else
    mavlink_hoverboard_event_t packet;
    packet.ctrl_mod = ctrl_mod;
    packet.ctrl_typ = ctrl_typ;
    packet.cur_mot_max = cur_mot_max;
    packet.rpm_mot_max = rpm_mot_max;
    packet.fi_weak_ena = fi_weak_ena;
    packet.fi_weak_hi = fi_weak_hi;
    packet.fi_weak_lo = fi_weak_lo;
    packet.fi_weak_max = fi_weak_max;
    packet.phase_adv_max_deg = phase_adv_max_deg;
    packet.input1_raw = input1_raw;
    packet.input1_typ = input1_typ;
    packet.input1_min = input1_min;
    packet.input1_mid = input1_mid;
    packet.input1_max = input1_max;
    packet.input1_cmd = input1_cmd;
    packet.input2_raw = input2_raw;
    packet.input2_typ = input2_typ;
    packet.input2_min = input2_min;
    packet.input2_mid = input2_mid;
    packet.input2_max = input2_max;
    packet.input2_cmd = input2_cmd;
    packet.aux_input1_raw = aux_input1_raw;
    packet.aux_input1_typ = aux_input1_typ;
    packet.aux_input1_min = aux_input1_min;
    packet.aux_input1_mid = aux_input1_mid;
    packet.aux_input1_max = aux_input1_max;
    packet.aux_input1_cmd = aux_input1_cmd;
    packet.aux_input2_raw = aux_input2_raw;
    packet.aux_input2_min = aux_input2_min;
    packet.aux_input2_mid = aux_input2_mid;
    packet.aux_input2_max = aux_input2_max;
    packet.aux_input2_cmd = aux_input2_cmd;
    packet.dc_curr = dc_curr;
    packet.rdc_curr = rdc_curr;
    packet.ldc_curr = ldc_curr;
    packet.cmdl = cmdl;
    packet.cmdr = cmdr;
    packet.spd_avg = spd_avg;
    packet.spdl = spdl;
    packet.spdr = spdr;
    packet.filter_rate = filter_rate;
    packet.spd_coef = spd_coef;
    packet.str_coef = str_coef;
    packet.batv = batv;
    packet.temp = temp;
    packet.aux_input2_typ = aux_input2_typ;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_EVENT, (const char *)&packet, MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_CRC);
#endif
}

/**
 * @brief Send a hoverboard_event message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_hoverboard_event_send_struct(mavlink_channel_t chan, const mavlink_hoverboard_event_t* hoverboard_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_hoverboard_event_send(chan, hoverboard_event->ctrl_mod, hoverboard_event->ctrl_typ, hoverboard_event->cur_mot_max, hoverboard_event->rpm_mot_max, hoverboard_event->fi_weak_ena, hoverboard_event->fi_weak_hi, hoverboard_event->fi_weak_lo, hoverboard_event->fi_weak_max, hoverboard_event->phase_adv_max_deg, hoverboard_event->input1_raw, hoverboard_event->input1_typ, hoverboard_event->input1_min, hoverboard_event->input1_mid, hoverboard_event->input1_max, hoverboard_event->input1_cmd, hoverboard_event->input2_raw, hoverboard_event->input2_typ, hoverboard_event->input2_min, hoverboard_event->input2_mid, hoverboard_event->input2_max, hoverboard_event->input2_cmd, hoverboard_event->aux_input1_raw, hoverboard_event->aux_input1_typ, hoverboard_event->aux_input1_min, hoverboard_event->aux_input1_mid, hoverboard_event->aux_input1_max, hoverboard_event->aux_input1_cmd, hoverboard_event->aux_input2_raw, hoverboard_event->aux_input2_typ, hoverboard_event->aux_input2_min, hoverboard_event->aux_input2_mid, hoverboard_event->aux_input2_max, hoverboard_event->aux_input2_cmd, hoverboard_event->dc_curr, hoverboard_event->rdc_curr, hoverboard_event->ldc_curr, hoverboard_event->cmdl, hoverboard_event->cmdr, hoverboard_event->spd_avg, hoverboard_event->spdl, hoverboard_event->spdr, hoverboard_event->filter_rate, hoverboard_event->spd_coef, hoverboard_event->str_coef, hoverboard_event->batv, hoverboard_event->temp);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_EVENT, (const char *)hoverboard_event, MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_CRC);
#endif
}

#if MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_hoverboard_event_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  int16_t ctrl_mod, int16_t ctrl_typ, int16_t cur_mot_max, int16_t rpm_mot_max, int16_t fi_weak_ena, int16_t fi_weak_hi, int16_t fi_weak_lo, int16_t fi_weak_max, int16_t phase_adv_max_deg, int16_t input1_raw, int16_t input1_typ, int16_t input1_min, int16_t input1_mid, int16_t input1_max, int16_t input1_cmd, int16_t input2_raw, int16_t input2_typ, int16_t input2_min, int16_t input2_mid, int16_t input2_max, int16_t input2_cmd, int16_t aux_input1_raw, int16_t aux_input1_typ, int16_t aux_input1_min, int16_t aux_input1_mid, int16_t aux_input1_max, int16_t aux_input1_cmd, int16_t aux_input2_raw, uint8_t aux_input2_typ, int16_t aux_input2_min, int16_t aux_input2_mid, int16_t aux_input2_max, int16_t aux_input2_cmd, int16_t dc_curr, int16_t rdc_curr, int16_t ldc_curr, int16_t cmdl, int16_t cmdr, int16_t spd_avg, int16_t spdl, int16_t spdr, int16_t filter_rate, int16_t spd_coef, int16_t str_coef, int16_t batv, int16_t temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int16_t(buf, 0, ctrl_mod);
    _mav_put_int16_t(buf, 2, ctrl_typ);
    _mav_put_int16_t(buf, 4, cur_mot_max);
    _mav_put_int16_t(buf, 6, rpm_mot_max);
    _mav_put_int16_t(buf, 8, fi_weak_ena);
    _mav_put_int16_t(buf, 10, fi_weak_hi);
    _mav_put_int16_t(buf, 12, fi_weak_lo);
    _mav_put_int16_t(buf, 14, fi_weak_max);
    _mav_put_int16_t(buf, 16, phase_adv_max_deg);
    _mav_put_int16_t(buf, 18, input1_raw);
    _mav_put_int16_t(buf, 20, input1_typ);
    _mav_put_int16_t(buf, 22, input1_min);
    _mav_put_int16_t(buf, 24, input1_mid);
    _mav_put_int16_t(buf, 26, input1_max);
    _mav_put_int16_t(buf, 28, input1_cmd);
    _mav_put_int16_t(buf, 30, input2_raw);
    _mav_put_int16_t(buf, 32, input2_typ);
    _mav_put_int16_t(buf, 34, input2_min);
    _mav_put_int16_t(buf, 36, input2_mid);
    _mav_put_int16_t(buf, 38, input2_max);
    _mav_put_int16_t(buf, 40, input2_cmd);
    _mav_put_int16_t(buf, 42, aux_input1_raw);
    _mav_put_int16_t(buf, 44, aux_input1_typ);
    _mav_put_int16_t(buf, 46, aux_input1_min);
    _mav_put_int16_t(buf, 48, aux_input1_mid);
    _mav_put_int16_t(buf, 50, aux_input1_max);
    _mav_put_int16_t(buf, 52, aux_input1_cmd);
    _mav_put_int16_t(buf, 54, aux_input2_raw);
    _mav_put_int16_t(buf, 56, aux_input2_min);
    _mav_put_int16_t(buf, 58, aux_input2_mid);
    _mav_put_int16_t(buf, 60, aux_input2_max);
    _mav_put_int16_t(buf, 62, aux_input2_cmd);
    _mav_put_int16_t(buf, 64, dc_curr);
    _mav_put_int16_t(buf, 66, rdc_curr);
    _mav_put_int16_t(buf, 68, ldc_curr);
    _mav_put_int16_t(buf, 70, cmdl);
    _mav_put_int16_t(buf, 72, cmdr);
    _mav_put_int16_t(buf, 74, spd_avg);
    _mav_put_int16_t(buf, 76, spdl);
    _mav_put_int16_t(buf, 78, spdr);
    _mav_put_int16_t(buf, 80, filter_rate);
    _mav_put_int16_t(buf, 82, spd_coef);
    _mav_put_int16_t(buf, 84, str_coef);
    _mav_put_int16_t(buf, 86, batv);
    _mav_put_int16_t(buf, 88, temp);
    _mav_put_uint8_t(buf, 90, aux_input2_typ);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_EVENT, buf, MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_CRC);
#else
    mavlink_hoverboard_event_t *packet = (mavlink_hoverboard_event_t *)msgbuf;
    packet->ctrl_mod = ctrl_mod;
    packet->ctrl_typ = ctrl_typ;
    packet->cur_mot_max = cur_mot_max;
    packet->rpm_mot_max = rpm_mot_max;
    packet->fi_weak_ena = fi_weak_ena;
    packet->fi_weak_hi = fi_weak_hi;
    packet->fi_weak_lo = fi_weak_lo;
    packet->fi_weak_max = fi_weak_max;
    packet->phase_adv_max_deg = phase_adv_max_deg;
    packet->input1_raw = input1_raw;
    packet->input1_typ = input1_typ;
    packet->input1_min = input1_min;
    packet->input1_mid = input1_mid;
    packet->input1_max = input1_max;
    packet->input1_cmd = input1_cmd;
    packet->input2_raw = input2_raw;
    packet->input2_typ = input2_typ;
    packet->input2_min = input2_min;
    packet->input2_mid = input2_mid;
    packet->input2_max = input2_max;
    packet->input2_cmd = input2_cmd;
    packet->aux_input1_raw = aux_input1_raw;
    packet->aux_input1_typ = aux_input1_typ;
    packet->aux_input1_min = aux_input1_min;
    packet->aux_input1_mid = aux_input1_mid;
    packet->aux_input1_max = aux_input1_max;
    packet->aux_input1_cmd = aux_input1_cmd;
    packet->aux_input2_raw = aux_input2_raw;
    packet->aux_input2_min = aux_input2_min;
    packet->aux_input2_mid = aux_input2_mid;
    packet->aux_input2_max = aux_input2_max;
    packet->aux_input2_cmd = aux_input2_cmd;
    packet->dc_curr = dc_curr;
    packet->rdc_curr = rdc_curr;
    packet->ldc_curr = ldc_curr;
    packet->cmdl = cmdl;
    packet->cmdr = cmdr;
    packet->spd_avg = spd_avg;
    packet->spdl = spdl;
    packet->spdr = spdr;
    packet->filter_rate = filter_rate;
    packet->spd_coef = spd_coef;
    packet->str_coef = str_coef;
    packet->batv = batv;
    packet->temp = temp;
    packet->aux_input2_typ = aux_input2_typ;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVERBOARD_EVENT, (const char *)packet, MAVLINK_MSG_ID_HOVERBOARD_EVENT_MIN_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN, MAVLINK_MSG_ID_HOVERBOARD_EVENT_CRC);
#endif
}
#endif

#endif

// MESSAGE HOVERBOARD_EVENT UNPACKING


/**
 * @brief Get field ctrl_mod from hoverboard_event message
 *
 * @return  Control mode 1:Voltage 2:Speed 3:Torque
 */
static inline int16_t mavlink_msg_hoverboard_event_get_ctrl_mod(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  0);
}

/**
 * @brief Get field ctrl_typ from hoverboard_event message
 *
 * @return  Control type 0:Commutation 1:Sinusoidal 2:FOC
 */
static inline int16_t mavlink_msg_hoverboard_event_get_ctrl_typ(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  2);
}

/**
 * @brief Get field cur_mot_max from hoverboard_event message
 *
 * @return [A] Max phase current
 */
static inline int16_t mavlink_msg_hoverboard_event_get_cur_mot_max(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  4);
}

/**
 * @brief Get field rpm_mot_max from hoverboard_event message
 *
 * @return [rpm] Max motor RPM
 */
static inline int16_t mavlink_msg_hoverboard_event_get_rpm_mot_max(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  6);
}

/**
 * @brief Get field fi_weak_ena from hoverboard_event message
 *
 * @return  Enable field weakening 0:OFF 1:ON
 */
static inline int16_t mavlink_msg_hoverboard_event_get_fi_weak_ena(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field fi_weak_hi from hoverboard_event message
 *
 * @return [rpm] Field weakening high RPM
 */
static inline int16_t mavlink_msg_hoverboard_event_get_fi_weak_hi(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Get field fi_weak_lo from hoverboard_event message
 *
 * @return [rpm] Field weakening low RPM
 */
static inline int16_t mavlink_msg_hoverboard_event_get_fi_weak_lo(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field fi_weak_max from hoverboard_event message
 *
 * @return [A] Field weakening max current (FOC only)
 */
static inline int16_t mavlink_msg_hoverboard_event_get_fi_weak_max(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

/**
 * @brief Get field phase_adv_max_deg from hoverboard_event message
 *
 * @return [deg] Max phase advance angle (SIN only)
 */
static inline int16_t mavlink_msg_hoverboard_event_get_phase_adv_max_deg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  16);
}

/**
 * @brief Get field input1_raw from hoverboard_event message
 *
 * @return  Input1 raw value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input1_raw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  18);
}

/**
 * @brief Get field input1_typ from hoverboard_event message
 *
 * @return  Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input1_typ(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  20);
}

/**
 * @brief Get field input1_min from hoverboard_event message
 *
 * @return  Input1 minimum value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input1_min(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  22);
}

/**
 * @brief Get field input1_mid from hoverboard_event message
 *
 * @return  Input1 middle value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input1_mid(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  24);
}

/**
 * @brief Get field input1_max from hoverboard_event message
 *
 * @return  Input1 maximum value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input1_max(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  26);
}

/**
 * @brief Get field input1_cmd from hoverboard_event message
 *
 * @return  Input1 command value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input1_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  28);
}

/**
 * @brief Get field input2_raw from hoverboard_event message
 *
 * @return  Input2 raw value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input2_raw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  30);
}

/**
 * @brief Get field input2_typ from hoverboard_event message
 *
 * @return  Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input2_typ(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  32);
}

/**
 * @brief Get field input2_min from hoverboard_event message
 *
 * @return  Input2 minimum value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input2_min(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  34);
}

/**
 * @brief Get field input2_mid from hoverboard_event message
 *
 * @return  Input2 middle value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input2_mid(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  36);
}

/**
 * @brief Get field input2_max from hoverboard_event message
 *
 * @return  Input2 maximum value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input2_max(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  38);
}

/**
 * @brief Get field input2_cmd from hoverboard_event message
 *
 * @return  Input2 command value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_input2_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  40);
}

/**
 * @brief Get field aux_input1_raw from hoverboard_event message
 *
 * @return  Aux input1 raw value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input1_raw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  42);
}

/**
 * @brief Get field aux_input1_typ from hoverboard_event message
 *
 * @return  Aux input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input1_typ(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  44);
}

/**
 * @brief Get field aux_input1_min from hoverboard_event message
 *
 * @return  Aux input1 minimum value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input1_min(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  46);
}

/**
 * @brief Get field aux_input1_mid from hoverboard_event message
 *
 * @return  Aux input1 middle value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input1_mid(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  48);
}

/**
 * @brief Get field aux_input1_max from hoverboard_event message
 *
 * @return  Aux input1 maximum value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input1_max(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  50);
}

/**
 * @brief Get field aux_input1_cmd from hoverboard_event message
 *
 * @return  Aux input1 command value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input1_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  52);
}

/**
 * @brief Get field aux_input2_raw from hoverboard_event message
 *
 * @return  Aux input2 raw value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input2_raw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  54);
}

/**
 * @brief Get field aux_input2_typ from hoverboard_event message
 *
 * @return  Aux input2 type
 */
static inline uint8_t mavlink_msg_hoverboard_event_get_aux_input2_typ(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  90);
}

/**
 * @brief Get field aux_input2_min from hoverboard_event message
 *
 * @return  Aux input2 minimum value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input2_min(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  56);
}

/**
 * @brief Get field aux_input2_mid from hoverboard_event message
 *
 * @return  Aux input2 middle value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input2_mid(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  58);
}

/**
 * @brief Get field aux_input2_max from hoverboard_event message
 *
 * @return  Aux input2 maximum value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input2_max(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  60);
}

/**
 * @brief Get field aux_input2_cmd from hoverboard_event message
 *
 * @return  Aux input2 command value
 */
static inline int16_t mavlink_msg_hoverboard_event_get_aux_input2_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  62);
}

/**
 * @brief Get field dc_curr from hoverboard_event message
 *
 * @return [cA] Total DC link current A *100
 */
static inline int16_t mavlink_msg_hoverboard_event_get_dc_curr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  64);
}

/**
 * @brief Get field rdc_curr from hoverboard_event message
 *
 * @return [cA] Right DC link current A *100
 */
static inline int16_t mavlink_msg_hoverboard_event_get_rdc_curr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  66);
}

/**
 * @brief Get field ldc_curr from hoverboard_event message
 *
 * @return [cA] Left DC link current A *100
 */
static inline int16_t mavlink_msg_hoverboard_event_get_ldc_curr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  68);
}

/**
 * @brief Get field cmdl from hoverboard_event message
 *
 * @return [rpm] Left motor command RPM
 */
static inline int16_t mavlink_msg_hoverboard_event_get_cmdl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  70);
}

/**
 * @brief Get field cmdr from hoverboard_event message
 *
 * @return [rpm] Right motor command RPM
 */
static inline int16_t mavlink_msg_hoverboard_event_get_cmdr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  72);
}

/**
 * @brief Get field spd_avg from hoverboard_event message
 *
 * @return [rpm] Motor measured average RPM
 */
static inline int16_t mavlink_msg_hoverboard_event_get_spd_avg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  74);
}

/**
 * @brief Get field spdl from hoverboard_event message
 *
 * @return [rpm] Left motor measured RPM
 */
static inline int16_t mavlink_msg_hoverboard_event_get_spdl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  76);
}

/**
 * @brief Get field spdr from hoverboard_event message
 *
 * @return [rpm] Right motor measured RPM
 */
static inline int16_t mavlink_msg_hoverboard_event_get_spdr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  78);
}

/**
 * @brief Get field filter_rate from hoverboard_event message
 *
 * @return [cHz] Rate *10
 */
static inline int16_t mavlink_msg_hoverboard_event_get_filter_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  80);
}

/**
 * @brief Get field spd_coef from hoverboard_event message
 *
 * @return  Speed coefficient *10
 */
static inline int16_t mavlink_msg_hoverboard_event_get_spd_coef(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  82);
}

/**
 * @brief Get field str_coef from hoverboard_event message
 *
 * @return  Steer coefficient *10
 */
static inline int16_t mavlink_msg_hoverboard_event_get_str_coef(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  84);
}

/**
 * @brief Get field batv from hoverboard_event message
 *
 * @return [cV] Calibrated battery voltage *100
 */
static inline int16_t mavlink_msg_hoverboard_event_get_batv(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  86);
}

/**
 * @brief Get field temp from hoverboard_event message
 *
 * @return [cdegC] Calibrated temperature C *10
 */
static inline int16_t mavlink_msg_hoverboard_event_get_temp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  88);
}

/**
 * @brief Decode a hoverboard_event message into a struct
 *
 * @param msg The message to decode
 * @param hoverboard_event C-struct to decode the message contents into
 */
static inline void mavlink_msg_hoverboard_event_decode(const mavlink_message_t* msg, mavlink_hoverboard_event_t* hoverboard_event)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    hoverboard_event->ctrl_mod = mavlink_msg_hoverboard_event_get_ctrl_mod(msg);
    hoverboard_event->ctrl_typ = mavlink_msg_hoverboard_event_get_ctrl_typ(msg);
    hoverboard_event->cur_mot_max = mavlink_msg_hoverboard_event_get_cur_mot_max(msg);
    hoverboard_event->rpm_mot_max = mavlink_msg_hoverboard_event_get_rpm_mot_max(msg);
    hoverboard_event->fi_weak_ena = mavlink_msg_hoverboard_event_get_fi_weak_ena(msg);
    hoverboard_event->fi_weak_hi = mavlink_msg_hoverboard_event_get_fi_weak_hi(msg);
    hoverboard_event->fi_weak_lo = mavlink_msg_hoverboard_event_get_fi_weak_lo(msg);
    hoverboard_event->fi_weak_max = mavlink_msg_hoverboard_event_get_fi_weak_max(msg);
    hoverboard_event->phase_adv_max_deg = mavlink_msg_hoverboard_event_get_phase_adv_max_deg(msg);
    hoverboard_event->input1_raw = mavlink_msg_hoverboard_event_get_input1_raw(msg);
    hoverboard_event->input1_typ = mavlink_msg_hoverboard_event_get_input1_typ(msg);
    hoverboard_event->input1_min = mavlink_msg_hoverboard_event_get_input1_min(msg);
    hoverboard_event->input1_mid = mavlink_msg_hoverboard_event_get_input1_mid(msg);
    hoverboard_event->input1_max = mavlink_msg_hoverboard_event_get_input1_max(msg);
    hoverboard_event->input1_cmd = mavlink_msg_hoverboard_event_get_input1_cmd(msg);
    hoverboard_event->input2_raw = mavlink_msg_hoverboard_event_get_input2_raw(msg);
    hoverboard_event->input2_typ = mavlink_msg_hoverboard_event_get_input2_typ(msg);
    hoverboard_event->input2_min = mavlink_msg_hoverboard_event_get_input2_min(msg);
    hoverboard_event->input2_mid = mavlink_msg_hoverboard_event_get_input2_mid(msg);
    hoverboard_event->input2_max = mavlink_msg_hoverboard_event_get_input2_max(msg);
    hoverboard_event->input2_cmd = mavlink_msg_hoverboard_event_get_input2_cmd(msg);
    hoverboard_event->aux_input1_raw = mavlink_msg_hoverboard_event_get_aux_input1_raw(msg);
    hoverboard_event->aux_input1_typ = mavlink_msg_hoverboard_event_get_aux_input1_typ(msg);
    hoverboard_event->aux_input1_min = mavlink_msg_hoverboard_event_get_aux_input1_min(msg);
    hoverboard_event->aux_input1_mid = mavlink_msg_hoverboard_event_get_aux_input1_mid(msg);
    hoverboard_event->aux_input1_max = mavlink_msg_hoverboard_event_get_aux_input1_max(msg);
    hoverboard_event->aux_input1_cmd = mavlink_msg_hoverboard_event_get_aux_input1_cmd(msg);
    hoverboard_event->aux_input2_raw = mavlink_msg_hoverboard_event_get_aux_input2_raw(msg);
    hoverboard_event->aux_input2_min = mavlink_msg_hoverboard_event_get_aux_input2_min(msg);
    hoverboard_event->aux_input2_mid = mavlink_msg_hoverboard_event_get_aux_input2_mid(msg);
    hoverboard_event->aux_input2_max = mavlink_msg_hoverboard_event_get_aux_input2_max(msg);
    hoverboard_event->aux_input2_cmd = mavlink_msg_hoverboard_event_get_aux_input2_cmd(msg);
    hoverboard_event->dc_curr = mavlink_msg_hoverboard_event_get_dc_curr(msg);
    hoverboard_event->rdc_curr = mavlink_msg_hoverboard_event_get_rdc_curr(msg);
    hoverboard_event->ldc_curr = mavlink_msg_hoverboard_event_get_ldc_curr(msg);
    hoverboard_event->cmdl = mavlink_msg_hoverboard_event_get_cmdl(msg);
    hoverboard_event->cmdr = mavlink_msg_hoverboard_event_get_cmdr(msg);
    hoverboard_event->spd_avg = mavlink_msg_hoverboard_event_get_spd_avg(msg);
    hoverboard_event->spdl = mavlink_msg_hoverboard_event_get_spdl(msg);
    hoverboard_event->spdr = mavlink_msg_hoverboard_event_get_spdr(msg);
    hoverboard_event->filter_rate = mavlink_msg_hoverboard_event_get_filter_rate(msg);
    hoverboard_event->spd_coef = mavlink_msg_hoverboard_event_get_spd_coef(msg);
    hoverboard_event->str_coef = mavlink_msg_hoverboard_event_get_str_coef(msg);
    hoverboard_event->batv = mavlink_msg_hoverboard_event_get_batv(msg);
    hoverboard_event->temp = mavlink_msg_hoverboard_event_get_temp(msg);
    hoverboard_event->aux_input2_typ = mavlink_msg_hoverboard_event_get_aux_input2_typ(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN? msg->len : MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN;
        memset(hoverboard_event, 0, MAVLINK_MSG_ID_HOVERBOARD_EVENT_LEN);
    memcpy(hoverboard_event, _MAV_PAYLOAD(msg), len);
#endif
}
