// MESSAGE HOVERBOARD_EVENT support class

#pragma once

namespace mavlink {
namespace robot {
namespace msg {

/**
 * @brief HOVERBOARD_EVENT message
 *
 * Telemetry emitted by the hoverboard
 */
struct HOVERBOARD_EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 40004;
    static constexpr size_t LENGTH = 91;
    static constexpr size_t MIN_LENGTH = 91;
    static constexpr uint8_t CRC_EXTRA = 194;
    static constexpr auto NAME = "HOVERBOARD_EVENT";


    int16_t ctrl_mod; /*<  Control mode 1:Voltage 2:Speed 3:Torque */
    int16_t ctrl_typ; /*<  Control type 0:Commutation 1:Sinusoidal 2:FOC */
    int16_t cur_mot_max; /*< [A] Max phase current */
    int16_t rpm_mot_max; /*< [rpm] Max motor RPM */
    int16_t fi_weak_ena; /*<  Enable field weakening 0:OFF 1:ON */
    int16_t fi_weak_hi; /*< [rpm] Field weakening high RPM */
    int16_t fi_weak_lo; /*< [rpm] Field weakening low RPM */
    int16_t fi_weak_max; /*< [A] Field weakening max current (FOC only) */
    int16_t phase_adv_max_deg; /*< [deg] Max phase advance angle (SIN only) */
    int16_t input1_raw; /*<  Input1 raw value */
    int16_t input1_typ; /*<  Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect */
    int16_t input1_min; /*<  Input1 minimum value */
    int16_t input1_mid; /*<  Input1 middle value */
    int16_t input1_max; /*<  Input1 maximum value */
    int16_t input1_cmd; /*<  Input1 command value */
    int16_t input2_raw; /*<  Input2 raw value */
    int16_t input2_typ; /*<  Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect */
    int16_t input2_min; /*<  Input2 minimum value */
    int16_t input2_mid; /*<  Input2 middle value */
    int16_t input2_max; /*<  Input2 maximum value */
    int16_t input2_cmd; /*<  Input2 command value */
    int16_t aux_input1_raw; /*<  Aux input1 raw value */
    int16_t aux_input1_typ; /*<  Aux input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect */
    int16_t aux_input1_min; /*<  Aux input1 minimum value */
    int16_t aux_input1_mid; /*<  Aux input1 middle value */
    int16_t aux_input1_max; /*<  Aux input1 maximum value */
    int16_t aux_input1_cmd; /*<  Aux input1 command value */
    int16_t aux_input2_raw; /*<  Aux input2 raw value */
    uint8_t aux_input2_typ; /*<  Aux input2 type */
    int16_t aux_input2_min; /*<  Aux input2 minimum value */
    int16_t aux_input2_mid; /*<  Aux input2 middle value */
    int16_t aux_input2_max; /*<  Aux input2 maximum value */
    int16_t aux_input2_cmd; /*<  Aux input2 command value */
    int16_t dc_curr; /*< [cA] Total DC link current A *100 */
    int16_t rdc_curr; /*< [cA] Right DC link current A *100 */
    int16_t ldc_curr; /*< [cA] Left DC link current A *100 */
    int16_t cmdl; /*< [rpm] Left motor command RPM */
    int16_t cmdr; /*< [rpm] Right motor command RPM */
    int16_t spd_avg; /*< [rpm] Motor measured average RPM */
    int16_t spdl; /*< [rpm] Left motor measured RPM */
    int16_t spdr; /*< [rpm] Right motor measured RPM */
    int16_t filter_rate; /*< [cHz] Rate *10 */
    int16_t spd_coef; /*<  Speed coefficient *10 */
    int16_t str_coef; /*<  Steer coefficient *10 */
    int16_t batv; /*< [cV] Calibrated battery voltage *100 */
    int16_t temp; /*< [cdegC] Calibrated temperature C *10 */


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
        ss << "  ctrl_mod: " << ctrl_mod << std::endl;
        ss << "  ctrl_typ: " << ctrl_typ << std::endl;
        ss << "  cur_mot_max: " << cur_mot_max << std::endl;
        ss << "  rpm_mot_max: " << rpm_mot_max << std::endl;
        ss << "  fi_weak_ena: " << fi_weak_ena << std::endl;
        ss << "  fi_weak_hi: " << fi_weak_hi << std::endl;
        ss << "  fi_weak_lo: " << fi_weak_lo << std::endl;
        ss << "  fi_weak_max: " << fi_weak_max << std::endl;
        ss << "  phase_adv_max_deg: " << phase_adv_max_deg << std::endl;
        ss << "  input1_raw: " << input1_raw << std::endl;
        ss << "  input1_typ: " << input1_typ << std::endl;
        ss << "  input1_min: " << input1_min << std::endl;
        ss << "  input1_mid: " << input1_mid << std::endl;
        ss << "  input1_max: " << input1_max << std::endl;
        ss << "  input1_cmd: " << input1_cmd << std::endl;
        ss << "  input2_raw: " << input2_raw << std::endl;
        ss << "  input2_typ: " << input2_typ << std::endl;
        ss << "  input2_min: " << input2_min << std::endl;
        ss << "  input2_mid: " << input2_mid << std::endl;
        ss << "  input2_max: " << input2_max << std::endl;
        ss << "  input2_cmd: " << input2_cmd << std::endl;
        ss << "  aux_input1_raw: " << aux_input1_raw << std::endl;
        ss << "  aux_input1_typ: " << aux_input1_typ << std::endl;
        ss << "  aux_input1_min: " << aux_input1_min << std::endl;
        ss << "  aux_input1_mid: " << aux_input1_mid << std::endl;
        ss << "  aux_input1_max: " << aux_input1_max << std::endl;
        ss << "  aux_input1_cmd: " << aux_input1_cmd << std::endl;
        ss << "  aux_input2_raw: " << aux_input2_raw << std::endl;
        ss << "  aux_input2_typ: " << +aux_input2_typ << std::endl;
        ss << "  aux_input2_min: " << aux_input2_min << std::endl;
        ss << "  aux_input2_mid: " << aux_input2_mid << std::endl;
        ss << "  aux_input2_max: " << aux_input2_max << std::endl;
        ss << "  aux_input2_cmd: " << aux_input2_cmd << std::endl;
        ss << "  dc_curr: " << dc_curr << std::endl;
        ss << "  rdc_curr: " << rdc_curr << std::endl;
        ss << "  ldc_curr: " << ldc_curr << std::endl;
        ss << "  cmdl: " << cmdl << std::endl;
        ss << "  cmdr: " << cmdr << std::endl;
        ss << "  spd_avg: " << spd_avg << std::endl;
        ss << "  spdl: " << spdl << std::endl;
        ss << "  spdr: " << spdr << std::endl;
        ss << "  filter_rate: " << filter_rate << std::endl;
        ss << "  spd_coef: " << spd_coef << std::endl;
        ss << "  str_coef: " << str_coef << std::endl;
        ss << "  batv: " << batv << std::endl;
        ss << "  temp: " << temp << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << ctrl_mod;                      // offset: 0
        map << ctrl_typ;                      // offset: 2
        map << cur_mot_max;                   // offset: 4
        map << rpm_mot_max;                   // offset: 6
        map << fi_weak_ena;                   // offset: 8
        map << fi_weak_hi;                    // offset: 10
        map << fi_weak_lo;                    // offset: 12
        map << fi_weak_max;                   // offset: 14
        map << phase_adv_max_deg;             // offset: 16
        map << input1_raw;                    // offset: 18
        map << input1_typ;                    // offset: 20
        map << input1_min;                    // offset: 22
        map << input1_mid;                    // offset: 24
        map << input1_max;                    // offset: 26
        map << input1_cmd;                    // offset: 28
        map << input2_raw;                    // offset: 30
        map << input2_typ;                    // offset: 32
        map << input2_min;                    // offset: 34
        map << input2_mid;                    // offset: 36
        map << input2_max;                    // offset: 38
        map << input2_cmd;                    // offset: 40
        map << aux_input1_raw;                // offset: 42
        map << aux_input1_typ;                // offset: 44
        map << aux_input1_min;                // offset: 46
        map << aux_input1_mid;                // offset: 48
        map << aux_input1_max;                // offset: 50
        map << aux_input1_cmd;                // offset: 52
        map << aux_input2_raw;                // offset: 54
        map << aux_input2_min;                // offset: 56
        map << aux_input2_mid;                // offset: 58
        map << aux_input2_max;                // offset: 60
        map << aux_input2_cmd;                // offset: 62
        map << dc_curr;                       // offset: 64
        map << rdc_curr;                      // offset: 66
        map << ldc_curr;                      // offset: 68
        map << cmdl;                          // offset: 70
        map << cmdr;                          // offset: 72
        map << spd_avg;                       // offset: 74
        map << spdl;                          // offset: 76
        map << spdr;                          // offset: 78
        map << filter_rate;                   // offset: 80
        map << spd_coef;                      // offset: 82
        map << str_coef;                      // offset: 84
        map << batv;                          // offset: 86
        map << temp;                          // offset: 88
        map << aux_input2_typ;                // offset: 90
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> ctrl_mod;                      // offset: 0
        map >> ctrl_typ;                      // offset: 2
        map >> cur_mot_max;                   // offset: 4
        map >> rpm_mot_max;                   // offset: 6
        map >> fi_weak_ena;                   // offset: 8
        map >> fi_weak_hi;                    // offset: 10
        map >> fi_weak_lo;                    // offset: 12
        map >> fi_weak_max;                   // offset: 14
        map >> phase_adv_max_deg;             // offset: 16
        map >> input1_raw;                    // offset: 18
        map >> input1_typ;                    // offset: 20
        map >> input1_min;                    // offset: 22
        map >> input1_mid;                    // offset: 24
        map >> input1_max;                    // offset: 26
        map >> input1_cmd;                    // offset: 28
        map >> input2_raw;                    // offset: 30
        map >> input2_typ;                    // offset: 32
        map >> input2_min;                    // offset: 34
        map >> input2_mid;                    // offset: 36
        map >> input2_max;                    // offset: 38
        map >> input2_cmd;                    // offset: 40
        map >> aux_input1_raw;                // offset: 42
        map >> aux_input1_typ;                // offset: 44
        map >> aux_input1_min;                // offset: 46
        map >> aux_input1_mid;                // offset: 48
        map >> aux_input1_max;                // offset: 50
        map >> aux_input1_cmd;                // offset: 52
        map >> aux_input2_raw;                // offset: 54
        map >> aux_input2_min;                // offset: 56
        map >> aux_input2_mid;                // offset: 58
        map >> aux_input2_max;                // offset: 60
        map >> aux_input2_cmd;                // offset: 62
        map >> dc_curr;                       // offset: 64
        map >> rdc_curr;                      // offset: 66
        map >> ldc_curr;                      // offset: 68
        map >> cmdl;                          // offset: 70
        map >> cmdr;                          // offset: 72
        map >> spd_avg;                       // offset: 74
        map >> spdl;                          // offset: 76
        map >> spdr;                          // offset: 78
        map >> filter_rate;                   // offset: 80
        map >> spd_coef;                      // offset: 82
        map >> str_coef;                      // offset: 84
        map >> batv;                          // offset: 86
        map >> temp;                          // offset: 88
        map >> aux_input2_typ;                // offset: 90
    }
};

} // namespace msg
} // namespace robot
} // namespace mavlink
