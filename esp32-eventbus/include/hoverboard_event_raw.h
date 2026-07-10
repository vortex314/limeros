#include <msgs.h>



typedef enum HbVar {
    CTRL_MOD = 0,
    CTRL_TYP,
    CUR_MOT_MAX,
    RPM_MOT_MAX,
    FI_WEAK_ENA,
    FI_WEAK_HI,
    FI_WEAK_LO,
    FI_WEAK_MAX,
    PHASE_ADV_MAX_DEG,
    IN1_RAW,
    IN1_TYP,
    IN1_MIN,
    IN1_MID,
    IN1_MAX,
    IN1_CMD,
    IN2_RAW,
    IN2_TYP,
    IN2_MIN,
    IN2_MID,
    IN2_MAX,
    IN2_CMD,
    AUX_IN1_RAW,
    AUX_IN1_TYP,
    AUX_IN1_MIN,
    AUX_IN1_MID,
    AUX_IN1_MAX,
    AUX_IN1_CMD,
    AUX_IN2_RAW,
    AUX_IN2_TYP,
    AUX_IN2_MIN,
    AUX_IN2_MID,
    AUX_IN2_MAX,
    AUX_IN2_CMD,
    DC_CURR,
    RDC_CURR,
    LDC_CURR,
    CMDL,
    CMDR,
    SPD_AVG,
    SPDL,
    SPDR,
    FILTER_RATE,
    SPD_COEF,
    STR_COEF,
    BATV,
    TEMP,
} HbVar;


class HoverboardEventRaw : public Msg 
{
public: 
    std::optional<int32_t> ctrl_mod;
    std::optional<int32_t> ctrl_typ;
    std::optional<int32_t> cur_mot_max;
    std::optional<int32_t> rpm_mot_max;
    std::optional<int32_t> fi_weak_ena;
    std::optional<int32_t> fi_weak_hi;
    std::optional<int32_t> fi_weak_lo;
    std::optional<int32_t> fi_weak_max;
    std::optional<int32_t> phase_adv_max_deg;
    std::optional<int32_t> input1_raw;
    std::optional<int32_t> input1_typ;
    std::optional<int32_t> input1_min;
    std::optional<int32_t> input1_mid;
    std::optional<int32_t> input1_max;
    std::optional<int32_t> input1_cmd;
    std::optional<int32_t> input2_raw;
    std::optional<int32_t> input2_typ;
    std::optional<int32_t> input2_min;
    std::optional<int32_t> input2_mid;
    std::optional<int32_t> input2_max;
    std::optional<int32_t> input2_cmd;
    std::optional<int32_t> aux_input1_raw;
    std::optional<int32_t> aux_input1_typ;
    std::optional<int32_t> aux_input1_min;
    std::optional<int32_t> aux_input1_mid;
    std::optional<int32_t> aux_input1_max;
    std::optional<int32_t> aux_input1_cmd;      
    std::optional<int32_t> aux_input2_raw;
    std::optional<int32_t> aux_input2_typ;
    std::optional<int32_t> aux_input2_min;
    std::optional<int32_t> aux_input2_mid;
    std::optional<int32_t> aux_input2_max;
    std::optional<int32_t> aux_input2_cmd;
    std::optional<float> dc_curr;   
    std::optional<float> dc_volt;
    std::optional<float> temperature;

    Result<Bytes> serialize() const;
    HoverboardEventRaw* deserialize(const Bytes& bytes);
    std::string to_string() const;
};