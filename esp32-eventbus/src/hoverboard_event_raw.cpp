
#include <hoverboard_event_raw.h>

HoverboardEventRaw::HoverboardEventRaw() : Msg(HoverboardEventRaw::name_value)
{
    // Initialize all optional fields to std::nullopt
    ctrl_mod = std::nullopt;
    ctrl_typ = std::nullopt;
    cur_mot_max = std::nullopt;
    rpm_mot_max = std::nullopt;
    fi_weak_ena = std::nullopt;
    fi_weak_hi = std::nullopt;
    fi_weak_lo = std::nullopt;
    fi_weak_max = std::nullopt;
    phase_adv_max_deg = std::nullopt;
    input1_raw = std::nullopt;
    input1_typ = std::nullopt;
    input1_min = std::nullopt;
    input1_mid = std::nullopt;
    input1_max = std::nullopt;
    input1_cmd = std::nullopt;
    input2_raw = std::nullopt;
    input2_typ = std::nullopt;
    input2_min = std::nullopt;
    input2_mid = std::nullopt;
    input2_max = std::nullopt;
    input2_cmd = std::nullopt;
    aux_input1_raw = std::nullopt;
    aux_input1_typ = std::nullopt;
    aux_input1_min = std::nullopt;
    aux_input1_mid = std::nullopt;
    aux_input1_max = std::nullopt;
    aux_input1_cmd = std::nullopt;
    aux_input2_raw = std::nullopt;
    aux_input2_typ = std::nullopt;
    aux_input2_min = std::nullopt;
    aux_input2_mid = std::nullopt;
    aux_input2_max = std::nullopt;
    aux_input2_cmd = std::nullopt;
    dc_curr = std::nullopt; 
    dc_volt = std::nullopt;
    temperature = std::nullopt;

}