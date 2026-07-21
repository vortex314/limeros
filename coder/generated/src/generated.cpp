// Auto-generated from robot.hcl — do not edit by hand.
// Every field is `std::optional<T>`; missing fields serialize as CBOR null.
// Messages are encoded as CBOR arrays (positional, not keyed by name).
// Uses tinycbor for CBOR encoding/decoding.

#pragma once

#include <cbor.h>
#include <cstdint>
#include <optional>
#include <string>






struct BrokerSubscribeRequest {
    std::optional<uint32_t> src;
    std::optional<uint32_t> msg_type;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 2);
        if (err != CborNoError) return err;
        if (src.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, (uint64_t)src.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (msg_type.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, (uint64_t)msg_type.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a BrokerSubscribeRequest from a CBOR array value.
    static BrokerSubscribeRequest fromCbor(CborValue& it) {
        BrokerSubscribeRequest msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { uint64_t _v; cbor_value_get_uint(&array, &_v); msg.src = (uint32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { uint64_t _v; cbor_value_get_uint(&array, &_v); msg.msg_type = (uint32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct HoverboardEvent {
    /// 1:Voltage 2:Speed 3:Torque
    std::optional<int32_t> ctrl_mod;
    /// 0:Commutation 1:Sinusoidal 2:FOC
    std::optional<int32_t> ctrl_typ;
    /// Max phase current A
    std::optional<int32_t> cur_mot_max;
    /// Max motor RPM
    std::optional<int32_t> rpm_mot_max;
    /// Enable field weak 0:OFF 1:ON
    std::optional<int32_t> fi_weak_ena;
    /// Field weak high RPM
    std::optional<int32_t> fi_weak_hi;
    /// Field weak low RPM
    std::optional<int32_t> fi_weak_lo;
    /// Field weak max current A (FOC only)
    std::optional<int32_t> fi_weak_max;
    /// Max Phase Adv angle Deg (SIN only)
    std::optional<int32_t> phase_adv_max_deg;
    /// Input1 raw value
    std::optional<int32_t> input1_raw;
    /// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    std::optional<int32_t> input1_typ;
    /// Input1 minimum value
    std::optional<int32_t> input1_min;
    /// Input1 middle value
    std::optional<int32_t> input1_mid;
    /// Input1 maximum value
    std::optional<int32_t> input1_max;
    /// Input1 command value
    std::optional<int32_t> input1_cmd;
    /// Input2 raw value
    std::optional<int32_t> input2_raw;
    /// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    std::optional<int32_t> input2_typ;
    /// Input2 minimum value
    std::optional<int32_t> input2_min;
    /// Input2 middle value
    std::optional<int32_t> input2_mid;
    /// Input2 maximum value
    std::optional<int32_t> input2_max;
    /// Input2 command value
    std::optional<int32_t> input2_cmd;
    /// Input1 raw value
    std::optional<int32_t> aux_input1_raw;
    /// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    std::optional<int32_t> aux_input1_typ;
    /// Input1 minimum value
    std::optional<int32_t> aux_input1_min;
    /// Input1 middle value
    std::optional<int32_t> aux_input1_mid;
    /// Input1 maximum value
    std::optional<int32_t> aux_input1_max;
    /// Input1 command value
    std::optional<int32_t> aux_input1_cmd;
    /// Input2 raw value
    std::optional<int32_t> aux_input2_raw;
    /// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    std::optional<int32_t> aux_input2_typ;
    /// Input2 minimum value
    std::optional<int32_t> aux_input2_min;
    /// Input2 middle value
    std::optional<int32_t> aux_input2_mid;
    /// Input2 maximum value
    std::optional<int32_t> aux_input2_max;
    /// Input2 command value
    std::optional<int32_t> aux_input2_cmd;
    /// Total DC Link current A *100
    std::optional<float> dc_curr;
    /// Right DC Link current A *100
    std::optional<float> rdc_curr;
    /// Left DC Link current A *100
    std::optional<float> ldc_curr;
    /// Left Motor Command RPM
    std::optional<int32_t> cmdl;
    /// Right Motor Command RPM
    std::optional<int32_t> cmdr;
    /// Motor Measured Avg RPM
    std::optional<int32_t> spd_avg;
    /// Left Motor Measured RPM
    std::optional<int32_t> spdl;
    /// Right Motor Measured RPM
    std::optional<int32_t> spdr;
    /// Rate *10
    std::optional<int32_t> filter_rate;
    /// Speed Coefficient *10
    std::optional<int32_t> spd_coef;
    /// Steer Coefficient *10
    std::optional<int32_t> str_coef;
    /// Calibrated Battery Voltage *100
    std::optional<float> batv;
    /// Calibrated Temperature C *10
    std::optional<float> temp;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 46);
        if (err != CborNoError) return err;
        if (ctrl_mod.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)ctrl_mod.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (ctrl_typ.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)ctrl_typ.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (cur_mot_max.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)cur_mot_max.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (rpm_mot_max.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)rpm_mot_max.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (fi_weak_ena.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)fi_weak_ena.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (fi_weak_hi.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)fi_weak_hi.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (fi_weak_lo.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)fi_weak_lo.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (fi_weak_max.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)fi_weak_max.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (phase_adv_max_deg.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)phase_adv_max_deg.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input1_raw.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input1_raw.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input1_typ.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input1_typ.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input1_min.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input1_min.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input1_mid.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input1_mid.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input1_max.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input1_max.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input1_cmd.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input1_cmd.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input2_raw.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input2_raw.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input2_typ.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input2_typ.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input2_min.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input2_min.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input2_mid.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input2_mid.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input2_max.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input2_max.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (input2_cmd.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)input2_cmd.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input1_raw.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input1_raw.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input1_typ.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input1_typ.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input1_min.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input1_min.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input1_mid.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input1_mid.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input1_max.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input1_max.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input1_cmd.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input1_cmd.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input2_raw.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input2_raw.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input2_typ.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input2_typ.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input2_min.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input2_min.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input2_mid.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input2_mid.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input2_max.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input2_max.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (aux_input2_cmd.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)aux_input2_cmd.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (dc_curr.has_value()) {

            err = cbor_encode_float(&arrayEncoder, dc_curr.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (rdc_curr.has_value()) {

            err = cbor_encode_float(&arrayEncoder, rdc_curr.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (ldc_curr.has_value()) {

            err = cbor_encode_float(&arrayEncoder, ldc_curr.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (cmdl.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)cmdl.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (cmdr.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)cmdr.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (spd_avg.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)spd_avg.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (spdl.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)spdl.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (spdr.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)spdr.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (filter_rate.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)filter_rate.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (spd_coef.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)spd_coef.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (str_coef.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)str_coef.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (batv.has_value()) {

            err = cbor_encode_float(&arrayEncoder, batv.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (temp.has_value()) {

            err = cbor_encode_float(&arrayEncoder, temp.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a HoverboardEvent from a CBOR array value.
    static HoverboardEvent fromCbor(CborValue& it) {
        HoverboardEvent msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.ctrl_mod = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.ctrl_typ = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.cur_mot_max = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.rpm_mot_max = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.fi_weak_ena = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.fi_weak_hi = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.fi_weak_lo = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.fi_weak_max = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.phase_adv_max_deg = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input1_raw = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input1_typ = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input1_min = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input1_mid = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input1_max = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input1_cmd = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input2_raw = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input2_typ = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input2_min = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input2_mid = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input2_max = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.input2_cmd = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input1_raw = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input1_typ = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input1_min = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input1_mid = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input1_max = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input1_cmd = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input2_raw = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input2_typ = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input2_min = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input2_mid = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input2_max = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.aux_input2_cmd = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_float(&array, &msg.dc_curr.emplace());
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_float(&array, &msg.rdc_curr.emplace());
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_float(&array, &msg.ldc_curr.emplace());
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.cmdl = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.cmdr = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.spd_avg = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.spdl = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.spdr = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.filter_rate = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.spd_coef = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.str_coef = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_float(&array, &msg.batv.emplace());
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_float(&array, &msg.temp.emplace());
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct HoverboardReply {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    std::optional<int32_t> error_code;
    std::optional<std::string> message;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 3);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, (uint64_t)req_id.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (error_code.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)error_code.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (message.has_value()) {

            err = cbor_encode_text_string(&arrayEncoder,
                message.value().c_str(),
                message.value().size());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a HoverboardReply from a CBOR array value.
    static HoverboardReply fromCbor(CborValue& it) {
        HoverboardReply msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { uint64_t _v; cbor_value_get_uint(&array, &_v); msg.req_id = (uint32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.error_code = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { char* _buf; size_t _len; cbor_value_dup_text_string(&array, &_buf, &_len, nullptr); msg.message = std::string(_buf, _len); free(_buf); }
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct HoverboardRequest {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    /// Speed command for the hoverboard
    std::optional<int32_t> speed;
    /// Steering command for the hoverboard
    std::optional<int32_t> steer;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 3);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, (uint64_t)req_id.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (speed.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)speed.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (steer.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)steer.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a HoverboardRequest from a CBOR array value.
    static HoverboardRequest fromCbor(CborValue& it) {
        HoverboardRequest msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { uint64_t _v; cbor_value_get_uint(&array, &_v); msg.req_id = (uint32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.speed = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.steer = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct PingReply {
    std::optional<uint32_t> req_id;
    /// Timestamp in milliseconds since epoch
    std::optional<uint64_t> timestamp;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 2);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, (uint64_t)req_id.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (timestamp.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, timestamp.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a PingReply from a CBOR array value.
    static PingReply fromCbor(CborValue& it) {
        PingReply msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { uint64_t _v; cbor_value_get_uint(&array, &_v); msg.req_id = (uint32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_uint(&array, &msg.timestamp.emplace());
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct PingRequest {
    std::optional<uint32_t> req_id;
    /// Timestamp in milliseconds since epoch
    std::optional<uint64_t> timestamp;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 2);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, (uint64_t)req_id.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (timestamp.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, timestamp.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a PingRequest from a CBOR array value.
    static PingRequest fromCbor(CborValue& it) {
        PingRequest msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { uint64_t _v; cbor_value_get_uint(&array, &_v); msg.req_id = (uint32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_uint(&array, &msg.timestamp.emplace());
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct Ps4Event {
    std::optional<bool> button_left;
    std::optional<bool> button_right;
    std::optional<bool> button_up;
    std::optional<bool> button_down;
    std::optional<bool> button_square;
    std::optional<bool> button_cross;
    std::optional<bool> button_circle;
    std::optional<bool> button_triangle;
    std::optional<bool> button_left_shoulder;
    std::optional<bool> button_right_shoulder;
    std::optional<bool> button_left_trigger;
    std::optional<bool> button_right_trigger;
    std::optional<bool> button_left_joystick;
    std::optional<bool> button_right_joystick;
    std::optional<bool> button_share;
    std::optional<bool> button_options;
    std::optional<bool> button_touchpad;
    std::optional<bool> button_ps;
    std::optional<int32_t> axis_lx;
    std::optional<int32_t> axis_ly;
    std::optional<int32_t> axis_rx;
    std::optional<int32_t> axis_ry;
    std::optional<int32_t> gyro_x;
    std::optional<int32_t> gyro_y;
    std::optional<int32_t> gyro_z;
    std::optional<int32_t> accel_x;
    std::optional<int32_t> accel_y;
    std::optional<int32_t> accel_z;
    std::optional<bool> connected;
    std::optional<int32_t> battery_level;
    std::optional<bool> bluetooth;
    std::optional<std::string> debug;
    std::optional<int32_t> temp;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 33);
        if (err != CborNoError) return err;
        if (button_left.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_left.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_right.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_right.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_up.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_up.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_down.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_down.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_square.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_square.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_cross.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_cross.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_circle.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_circle.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_triangle.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_triangle.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_left_shoulder.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_left_shoulder.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_right_shoulder.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_right_shoulder.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_left_trigger.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_left_trigger.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_right_trigger.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_right_trigger.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_left_joystick.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_left_joystick.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_right_joystick.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_right_joystick.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_share.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_share.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_options.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_options.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_touchpad.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_touchpad.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (button_ps.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, button_ps.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (axis_lx.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)axis_lx.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (axis_ly.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)axis_ly.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (axis_rx.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)axis_rx.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (axis_ry.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)axis_ry.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (gyro_x.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)gyro_x.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (gyro_y.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)gyro_y.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (gyro_z.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)gyro_z.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (accel_x.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)accel_x.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (accel_y.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)accel_y.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (accel_z.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)accel_z.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (connected.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, connected.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (battery_level.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)battery_level.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (bluetooth.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, bluetooth.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (debug.has_value()) {

            err = cbor_encode_text_string(&arrayEncoder,
                debug.value().c_str(),
                debug.value().size());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (temp.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)temp.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a Ps4Event from a CBOR array value.
    static Ps4Event fromCbor(CborValue& it) {
        Ps4Event msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_left = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_right = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_up = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_down = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_square = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_cross = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_circle = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_triangle = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_left_shoulder = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_right_shoulder = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_left_trigger = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_right_trigger = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_left_joystick = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_right_joystick = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_share = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_options = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_touchpad = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.button_ps = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.axis_lx = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.axis_ly = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.axis_rx = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.axis_ry = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.gyro_x = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.gyro_y = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.gyro_z = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.accel_x = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.accel_y = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.accel_z = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.connected = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.battery_level = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.bluetooth = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { char* _buf; size_t _len; cbor_value_dup_text_string(&array, &_buf, &_len, nullptr); msg.debug = std::string(_buf, _len); free(_buf); }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.temp = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct Ps4Request {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    std::optional<int32_t> rumble_small;
    std::optional<int32_t> rumble_large;
    std::optional<int32_t> led_red;
    std::optional<int32_t> led_green;
    std::optional<int32_t> led_blue;
    std::optional<int32_t> led_flash_on;
    std::optional<int32_t> led_flash_off;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 8);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, (uint64_t)req_id.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (rumble_small.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)rumble_small.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (rumble_large.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)rumble_large.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (led_red.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)led_red.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (led_green.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)led_green.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (led_blue.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)led_blue.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (led_flash_on.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)led_flash_on.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (led_flash_off.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)led_flash_off.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a Ps4Request from a CBOR array value.
    static Ps4Request fromCbor(CborValue& it) {
        Ps4Request msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { uint64_t _v; cbor_value_get_uint(&array, &_v); msg.req_id = (uint32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.rumble_small = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.rumble_large = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.led_red = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.led_green = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.led_blue = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.led_flash_on = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.led_flash_off = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct SysEvent {
    std::optional<uint64_t> utc;
    std::optional<uint64_t> uptime;
    std::optional<uint64_t> free_heap;
    std::optional<uint64_t> flash;
    std::optional<std::string> cpu_board;
    std::optional<std::string> build_date;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 6);
        if (err != CborNoError) return err;
        if (utc.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, utc.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (uptime.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, uptime.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (free_heap.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, free_heap.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (flash.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, flash.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (cpu_board.has_value()) {

            err = cbor_encode_text_string(&arrayEncoder,
                cpu_board.value().c_str(),
                cpu_board.value().size());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (build_date.has_value()) {

            err = cbor_encode_text_string(&arrayEncoder,
                build_date.value().c_str(),
                build_date.value().size());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a SysEvent from a CBOR array value.
    static SysEvent fromCbor(CborValue& it) {
        SysEvent msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_uint(&array, &msg.utc.emplace());
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_uint(&array, &msg.uptime.emplace());
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_uint(&array, &msg.free_heap.emplace());
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_uint(&array, &msg.flash.emplace());
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { char* _buf; size_t _len; cbor_value_dup_text_string(&array, &_buf, &_len, nullptr); msg.cpu_board = std::string(_buf, _len); free(_buf); }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { char* _buf; size_t _len; cbor_value_dup_text_string(&array, &_buf, &_len, nullptr); msg.build_date = std::string(_buf, _len); free(_buf); }
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct SysReply {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    std::optional<int32_t> rc;
    std::optional<std::string> message;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 3);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, (uint64_t)req_id.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (rc.has_value()) {

            err = cbor_encode_int(&arrayEncoder, (int64_t)rc.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (message.has_value()) {

            err = cbor_encode_text_string(&arrayEncoder,
                message.value().c_str(),
                message.value().size());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a SysReply from a CBOR array value.
    static SysReply fromCbor(CborValue& it) {
        SysReply msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { uint64_t _v; cbor_value_get_uint(&array, &_v); msg.req_id = (uint32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { int64_t _v; cbor_value_get_int(&array, &_v); msg.rc = (int32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { char* _buf; size_t _len; cbor_value_dup_text_string(&array, &_buf, &_len, nullptr); msg.message = std::string(_buf, _len); free(_buf); }
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


struct SysRequest {
    /// For request/reply matching, 0 if not a request/reply
    std::optional<uint32_t> req_id;
    std::optional<uint64_t> set_time;
    std::optional<bool> reboot;
    std::optional<std::string> console;

    /// Serialize this message into a CBOR array as a child of `parentEncoder`.
    CborError toCbor(CborEncoder& parentEncoder) const {
        CborEncoder arrayEncoder;
        CborError err = cbor_encoder_create_array(
            &parentEncoder, &arrayEncoder, 4);
        if (err != CborNoError) return err;
        if (req_id.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, (uint64_t)req_id.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (set_time.has_value()) {

            err = cbor_encode_uint(&arrayEncoder, set_time.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (reboot.has_value()) {

            err = cbor_encode_boolean(&arrayEncoder, reboot.value());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        if (console.has_value()) {

            err = cbor_encode_text_string(&arrayEncoder,
                console.value().c_str(),
                console.value().size());
        } else {
            err = cbor_encode_null(&arrayEncoder);
        }
        if (err != CborNoError) return err;
        return cbor_encoder_close_container(&parentEncoder, &arrayEncoder);
    }

    /// Deserialize a SysRequest from a CBOR array value.
    static SysRequest fromCbor(CborValue& it) {
        SysRequest msg;
        CborValue array;
        if (cbor_value_is_container(&it) &&
            cbor_value_enter_container(&it, &array) == CborNoError)
        {
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { uint64_t _v; cbor_value_get_uint(&array, &_v); msg.req_id = (uint32_t)_v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    cbor_value_get_uint(&array, &msg.set_time.emplace());
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { bool _v; cbor_value_get_boolean(&array, &_v); msg.reboot = _v; }
                }
                cbor_value_advance(&array);
            }
            if (!cbor_value_at_end(&array)) {
                if (!cbor_value_is_null(&array)) {

                    { char* _buf; size_t _len; cbor_value_dup_text_string(&array, &_buf, &_len, nullptr); msg.console = std::string(_buf, _len); free(_buf); }
                }
                cbor_value_advance(&array);
            }
            cbor_value_leave_container(&it, &array);
        }
        return msg;
    }
};


