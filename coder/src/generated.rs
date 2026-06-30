// Auto-generated from robot.hcl — do not edit by hand.
// Every field is `Option<T>`; missing fields serialize as CBOR null.
// Messages are encoded as CBOR arrays (positional, not keyed by name).

use serde::{Deserialize, Serialize};

pub static const MULTICAST_PORT : i16 = 12345;


pub static const HOVERBOARD_ID: u32 = 1152836275;

pub static const LOGGER_ID: u32 = 2753177333;

pub static const MOWER_ID: u32 = 1092332049;

pub static const PS4_ID: u32 = 360195552;





impl SysEvent {
    pub const ID: u32 = 924742914;
    pub const NAME: &'static str = "SysEvent";
}

#[derive(Debug, Clone)]
pub struct SysEvent {
    pub utc: Option<u64>,
    pub uptime: Option<u64>,
    pub free_heap: Option<u64>,
    pub flash: Option<u64>,
    pub cpu_board: Option<String>,
    pub build_date: Option<String>,
}

impl serde::Serialize for SysEvent {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        use serde::ser::SerializeSeq;
        let mut seq = serializer.serialize_seq(Some(6))?;
        seq.serialize_element(&self.utc)?;
        seq.serialize_element(&self.uptime)?;
        seq.serialize_element(&self.free_heap)?;
        seq.serialize_element(&self.flash)?;
        seq.serialize_element(&self.cpu_board)?;
        seq.serialize_element(&self.build_date)?;
        seq.end()
    }
}

#[derive(Deserialize)]
struct SysEventSerdeHelper(Option<u64>, Option<u64>, Option<u64>, Option<u64>, Option<String>, Option<String>);

impl From<SysEventSerdeHelper> for SysEvent {
    fn from(h: SysEventSerdeHelper) -> Self {
        let (utc, uptime, free_heap, flash, cpu_board, build_date) = (h.0, h.1, h.2, h.3, h.4, h.5);
        SysEvent {
            utc,
            uptime,
            free_heap,
            flash,
            cpu_board,
            build_date,
        }
    }
}



impl SysReply {
    pub const ID: u32 = 2952492394;
    pub const NAME: &'static str = "SysReply";
}

#[derive(Debug, Clone)]
pub struct SysReply {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub rc: Option<i32>,
    pub message: Option<String>,
}

impl serde::Serialize for SysReply {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        use serde::ser::SerializeSeq;
        let mut seq = serializer.serialize_seq(Some(3))?;
        seq.serialize_element(&self.req_id)?;
        seq.serialize_element(&self.rc)?;
        seq.serialize_element(&self.message)?;
        seq.end()
    }
}

#[derive(Deserialize)]
struct SysReplySerdeHelper(Option<u32>, Option<i32>, Option<String>);

impl From<SysReplySerdeHelper> for SysReply {
    fn from(h: SysReplySerdeHelper) -> Self {
        let (req_id, rc, message) = (h.0, h.1, h.2);
        SysReply {
            req_id,
            rc,
            message,
        }
    }
}



impl SysRequest {
    pub const ID: u32 = 2966412411;
    pub const NAME: &'static str = "SysRequest";
}

#[derive(Debug, Clone)]
pub struct SysRequest {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub set_time: Option<u64>,
    pub reboot: Option<bool>,
    pub console: Option<String>,
}

impl serde::Serialize for SysRequest {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        use serde::ser::SerializeSeq;
        let mut seq = serializer.serialize_seq(Some(4))?;
        seq.serialize_element(&self.req_id)?;
        seq.serialize_element(&self.set_time)?;
        seq.serialize_element(&self.reboot)?;
        seq.serialize_element(&self.console)?;
        seq.end()
    }
}

#[derive(Deserialize)]
struct SysRequestSerdeHelper(Option<u32>, Option<u64>, Option<bool>, Option<String>);

impl From<SysRequestSerdeHelper> for SysRequest {
    fn from(h: SysRequestSerdeHelper) -> Self {
        let (req_id, set_time, reboot, console) = (h.0, h.1, h.2, h.3);
        SysRequest {
            req_id,
            set_time,
            reboot,
            console,
        }
    }
}



impl HoverboardEvent {
    pub const ID: u32 = 104988481;
    pub const NAME: &'static str = "HoverboardEvent";
}

#[derive(Debug, Clone)]
pub struct HoverboardEvent {
    /// 1:Voltage 2:Speed 3:Torque
    pub ctrl_mod: Option<i32>,
    /// 0:Commutation 1:Sinusoidal 2:FOC
    pub ctrl_typ: Option<i32>,
    /// Max phase current A
    pub cur_mot_max: Option<i32>,
    /// Max motor RPM
    pub rpm_mot_max: Option<i32>,
    /// Enable field weak 0:OFF 1:ON
    pub fi_weak_ena: Option<i32>,
    /// Field weak high RPM
    pub fi_weak_hi: Option<i32>,
    /// Field weak low RPM
    pub fi_weak_lo: Option<i32>,
    /// Field weak max current A (FOC only)
    pub fi_weak_max: Option<i32>,
    /// Max Phase Adv angle Deg (SIN only)
    pub phase_adv_max_deg: Option<i32>,
    /// Input1 raw value
    pub input1_raw: Option<i32>,
    /// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    pub input1_typ: Option<i32>,
    /// Input1 minimum value
    pub input1_min: Option<i32>,
    /// Input1 middle value
    pub input1_mid: Option<i32>,
    /// Input1 maximum value
    pub input1_max: Option<i32>,
    /// Input1 command value
    pub input1_cmd: Option<i32>,
    /// Input2 raw value
    pub input2_raw: Option<i32>,
    /// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    pub input2_typ: Option<i32>,
    /// Input2 minimum value
    pub input2_min: Option<i32>,
    /// Input2 middle value
    pub input2_mid: Option<i32>,
    /// Input2 maximum value
    pub input2_max: Option<i32>,
    /// Input2 command value
    pub input2_cmd: Option<i32>,
    /// Input1 raw value
    pub aux_input1_raw: Option<i32>,
    /// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    pub aux_input1_typ: Option<i32>,
    /// Input1 minimum value
    pub aux_input1_min: Option<i32>,
    /// Input1 middle value
    pub aux_input1_mid: Option<i32>,
    /// Input1 maximum value
    pub aux_input1_max: Option<i32>,
    /// Input1 command value
    pub aux_input1_cmd: Option<i32>,
    /// Input2 raw value
    pub aux_input2_raw: Option<i32>,
    /// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    pub aux_input2_typ: Option<i32>,
    /// Input2 minimum value
    pub aux_input2_min: Option<i32>,
    /// Input2 middle value
    pub aux_input2_mid: Option<i32>,
    /// Input2 maximum value
    pub aux_input2_max: Option<i32>,
    /// Input2 command value
    pub aux_input2_cmd: Option<i32>,
    /// Total DC Link current A *100
    pub dc_curr: Option<f32>,
    /// Right DC Link current A *100
    pub rdc_curr: Option<f32>,
    /// Left DC Link current A *100
    pub ldc_curr: Option<f32>,
    /// Left Motor Command RPM
    pub cmdl: Option<i32>,
    /// Right Motor Command RPM
    pub cmdr: Option<i32>,
    /// Motor Measured Avg RPM
    pub spd_avg: Option<i32>,
    /// Left Motor Measured RPM
    pub spdl: Option<i32>,
    /// Right Motor Measured RPM
    pub spdr: Option<i32>,
    /// Rate *10
    pub filter_rate: Option<i32>,
    /// Speed Coefficient *10
    pub spd_coef: Option<i32>,
    /// Steer Coefficient *10
    pub str_coef: Option<i32>,
    /// Calibrated Battery Voltage *100
    pub batv: Option<f32>,
    /// Calibrated Temperature C *10
    pub temp: Option<f32>,
}

impl serde::Serialize for HoverboardEvent {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        use serde::ser::SerializeSeq;
        let mut seq = serializer.serialize_seq(Some(46))?;
        seq.serialize_element(&self.ctrl_mod)?;
        seq.serialize_element(&self.ctrl_typ)?;
        seq.serialize_element(&self.cur_mot_max)?;
        seq.serialize_element(&self.rpm_mot_max)?;
        seq.serialize_element(&self.fi_weak_ena)?;
        seq.serialize_element(&self.fi_weak_hi)?;
        seq.serialize_element(&self.fi_weak_lo)?;
        seq.serialize_element(&self.fi_weak_max)?;
        seq.serialize_element(&self.phase_adv_max_deg)?;
        seq.serialize_element(&self.input1_raw)?;
        seq.serialize_element(&self.input1_typ)?;
        seq.serialize_element(&self.input1_min)?;
        seq.serialize_element(&self.input1_mid)?;
        seq.serialize_element(&self.input1_max)?;
        seq.serialize_element(&self.input1_cmd)?;
        seq.serialize_element(&self.input2_raw)?;
        seq.serialize_element(&self.input2_typ)?;
        seq.serialize_element(&self.input2_min)?;
        seq.serialize_element(&self.input2_mid)?;
        seq.serialize_element(&self.input2_max)?;
        seq.serialize_element(&self.input2_cmd)?;
        seq.serialize_element(&self.aux_input1_raw)?;
        seq.serialize_element(&self.aux_input1_typ)?;
        seq.serialize_element(&self.aux_input1_min)?;
        seq.serialize_element(&self.aux_input1_mid)?;
        seq.serialize_element(&self.aux_input1_max)?;
        seq.serialize_element(&self.aux_input1_cmd)?;
        seq.serialize_element(&self.aux_input2_raw)?;
        seq.serialize_element(&self.aux_input2_typ)?;
        seq.serialize_element(&self.aux_input2_min)?;
        seq.serialize_element(&self.aux_input2_mid)?;
        seq.serialize_element(&self.aux_input2_max)?;
        seq.serialize_element(&self.aux_input2_cmd)?;
        seq.serialize_element(&self.dc_curr)?;
        seq.serialize_element(&self.rdc_curr)?;
        seq.serialize_element(&self.ldc_curr)?;
        seq.serialize_element(&self.cmdl)?;
        seq.serialize_element(&self.cmdr)?;
        seq.serialize_element(&self.spd_avg)?;
        seq.serialize_element(&self.spdl)?;
        seq.serialize_element(&self.spdr)?;
        seq.serialize_element(&self.filter_rate)?;
        seq.serialize_element(&self.spd_coef)?;
        seq.serialize_element(&self.str_coef)?;
        seq.serialize_element(&self.batv)?;
        seq.serialize_element(&self.temp)?;
        seq.end()
    }
}

#[derive(Deserialize)]
struct HoverboardEventSerdeHelper(Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<f32>, Option<f32>, Option<f32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<f32>, Option<f32>);

impl From<HoverboardEventSerdeHelper> for HoverboardEvent {
    fn from(h: HoverboardEventSerdeHelper) -> Self {
        let (ctrl_mod, ctrl_typ, cur_mot_max, rpm_mot_max, fi_weak_ena, fi_weak_hi, fi_weak_lo, fi_weak_max, phase_adv_max_deg, input1_raw, input1_typ, input1_min, input1_mid, input1_max, input1_cmd, input2_raw, input2_typ, input2_min, input2_mid, input2_max, input2_cmd, aux_input1_raw, aux_input1_typ, aux_input1_min, aux_input1_mid, aux_input1_max, aux_input1_cmd, aux_input2_raw, aux_input2_typ, aux_input2_min, aux_input2_mid, aux_input2_max, aux_input2_cmd, dc_curr, rdc_curr, ldc_curr, cmdl, cmdr, spd_avg, spdl, spdr, filter_rate, spd_coef, str_coef, batv, temp) = (h.0, h.1, h.2, h.3, h.4, h.5, h.6, h.7, h.8, h.9, h.10, h.11, h.12, h.13, h.14, h.15, h.16, h.17, h.18, h.19, h.20, h.21, h.22, h.23, h.24, h.25, h.26, h.27, h.28, h.29, h.30, h.31, h.32, h.33, h.34, h.35, h.36, h.37, h.38, h.39, h.40, h.41, h.42, h.43, h.44, h.45);
        HoverboardEvent {
            ctrl_mod,
            ctrl_typ,
            cur_mot_max,
            rpm_mot_max,
            fi_weak_ena,
            fi_weak_hi,
            fi_weak_lo,
            fi_weak_max,
            phase_adv_max_deg,
            input1_raw,
            input1_typ,
            input1_min,
            input1_mid,
            input1_max,
            input1_cmd,
            input2_raw,
            input2_typ,
            input2_min,
            input2_mid,
            input2_max,
            input2_cmd,
            aux_input1_raw,
            aux_input1_typ,
            aux_input1_min,
            aux_input1_mid,
            aux_input1_max,
            aux_input1_cmd,
            aux_input2_raw,
            aux_input2_typ,
            aux_input2_min,
            aux_input2_mid,
            aux_input2_max,
            aux_input2_cmd,
            dc_curr,
            rdc_curr,
            ldc_curr,
            cmdl,
            cmdr,
            spd_avg,
            spdl,
            spdr,
            filter_rate,
            spd_coef,
            str_coef,
            batv,
            temp,
        }
    }
}



impl HoverboardReply {
    pub const ID: u32 = 2095960949;
    pub const NAME: &'static str = "HoverboardReply";
}

#[derive(Debug, Clone)]
pub struct HoverboardReply {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub error_code: Option<i32>,
    pub message: Option<String>,
}

impl serde::Serialize for HoverboardReply {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        use serde::ser::SerializeSeq;
        let mut seq = serializer.serialize_seq(Some(3))?;
        seq.serialize_element(&self.req_id)?;
        seq.serialize_element(&self.error_code)?;
        seq.serialize_element(&self.message)?;
        seq.end()
    }
}

#[derive(Deserialize)]
struct HoverboardReplySerdeHelper(Option<u32>, Option<i32>, Option<String>);

impl From<HoverboardReplySerdeHelper> for HoverboardReply {
    fn from(h: HoverboardReplySerdeHelper) -> Self {
        let (req_id, error_code, message) = (h.0, h.1, h.2);
        HoverboardReply {
            req_id,
            error_code,
            message,
        }
    }
}



impl HoverboardRequest {
    pub const ID: u32 = 2735870956;
    pub const NAME: &'static str = "HoverboardRequest";
}

#[derive(Debug, Clone)]
pub struct HoverboardRequest {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    /// Speed command for the hoverboard
    pub speed: Option<i32>,
    /// Steering command for the hoverboard
    pub steer: Option<i32>,
}

impl serde::Serialize for HoverboardRequest {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        use serde::ser::SerializeSeq;
        let mut seq = serializer.serialize_seq(Some(3))?;
        seq.serialize_element(&self.req_id)?;
        seq.serialize_element(&self.speed)?;
        seq.serialize_element(&self.steer)?;
        seq.end()
    }
}

#[derive(Deserialize)]
struct HoverboardRequestSerdeHelper(Option<u32>, Option<i32>, Option<i32>);

impl From<HoverboardRequestSerdeHelper> for HoverboardRequest {
    fn from(h: HoverboardRequestSerdeHelper) -> Self {
        let (req_id, speed, steer) = (h.0, h.1, h.2);
        HoverboardRequest {
            req_id,
            speed,
            steer,
        }
    }
}



impl Ps4Event {
    pub const ID: u32 = 4282593576;
    pub const NAME: &'static str = "Ps4Event";
}

#[derive(Debug, Clone)]
pub struct Ps4Event {
    pub button_left: Option<bool>,
    pub button_right: Option<bool>,
    pub button_up: Option<bool>,
    pub button_down: Option<bool>,
    pub button_square: Option<bool>,
    pub button_cross: Option<bool>,
    pub button_circle: Option<bool>,
    pub button_triangle: Option<bool>,
    pub button_left_shoulder: Option<bool>,
    pub button_right_shoulder: Option<bool>,
    pub button_left_trigger: Option<bool>,
    pub button_right_trigger: Option<bool>,
    pub button_left_joystick: Option<bool>,
    pub button_right_joystick: Option<bool>,
    pub button_share: Option<bool>,
    pub button_options: Option<bool>,
    pub button_touchpad: Option<bool>,
    pub button_ps: Option<bool>,
    pub axis_lx: Option<i32>,
    pub axis_ly: Option<i32>,
    pub axis_rx: Option<i32>,
    pub axis_ry: Option<i32>,
    pub gyro_x: Option<i32>,
    pub gyro_y: Option<i32>,
    pub gyro_z: Option<i32>,
    pub accel_x: Option<i32>,
    pub accel_y: Option<i32>,
    pub accel_z: Option<i32>,
    pub connected: Option<bool>,
    pub battery_level: Option<i32>,
    pub bluetooth: Option<bool>,
    pub debug: Option<String>,
    pub temp: Option<i32>,
}

impl serde::Serialize for Ps4Event {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        use serde::ser::SerializeSeq;
        let mut seq = serializer.serialize_seq(Some(33))?;
        seq.serialize_element(&self.button_left)?;
        seq.serialize_element(&self.button_right)?;
        seq.serialize_element(&self.button_up)?;
        seq.serialize_element(&self.button_down)?;
        seq.serialize_element(&self.button_square)?;
        seq.serialize_element(&self.button_cross)?;
        seq.serialize_element(&self.button_circle)?;
        seq.serialize_element(&self.button_triangle)?;
        seq.serialize_element(&self.button_left_shoulder)?;
        seq.serialize_element(&self.button_right_shoulder)?;
        seq.serialize_element(&self.button_left_trigger)?;
        seq.serialize_element(&self.button_right_trigger)?;
        seq.serialize_element(&self.button_left_joystick)?;
        seq.serialize_element(&self.button_right_joystick)?;
        seq.serialize_element(&self.button_share)?;
        seq.serialize_element(&self.button_options)?;
        seq.serialize_element(&self.button_touchpad)?;
        seq.serialize_element(&self.button_ps)?;
        seq.serialize_element(&self.axis_lx)?;
        seq.serialize_element(&self.axis_ly)?;
        seq.serialize_element(&self.axis_rx)?;
        seq.serialize_element(&self.axis_ry)?;
        seq.serialize_element(&self.gyro_x)?;
        seq.serialize_element(&self.gyro_y)?;
        seq.serialize_element(&self.gyro_z)?;
        seq.serialize_element(&self.accel_x)?;
        seq.serialize_element(&self.accel_y)?;
        seq.serialize_element(&self.accel_z)?;
        seq.serialize_element(&self.connected)?;
        seq.serialize_element(&self.battery_level)?;
        seq.serialize_element(&self.bluetooth)?;
        seq.serialize_element(&self.debug)?;
        seq.serialize_element(&self.temp)?;
        seq.end()
    }
}

#[derive(Deserialize)]
struct Ps4EventSerdeHelper(Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<bool>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<bool>, Option<i32>, Option<bool>, Option<String>, Option<i32>);

impl From<Ps4EventSerdeHelper> for Ps4Event {
    fn from(h: Ps4EventSerdeHelper) -> Self {
        let (button_left, button_right, button_up, button_down, button_square, button_cross, button_circle, button_triangle, button_left_shoulder, button_right_shoulder, button_left_trigger, button_right_trigger, button_left_joystick, button_right_joystick, button_share, button_options, button_touchpad, button_ps, axis_lx, axis_ly, axis_rx, axis_ry, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, connected, battery_level, bluetooth, debug, temp) = (h.0, h.1, h.2, h.3, h.4, h.5, h.6, h.7, h.8, h.9, h.10, h.11, h.12, h.13, h.14, h.15, h.16, h.17, h.18, h.19, h.20, h.21, h.22, h.23, h.24, h.25, h.26, h.27, h.28, h.29, h.30, h.31, h.32);
        Ps4Event {
            button_left,
            button_right,
            button_up,
            button_down,
            button_square,
            button_cross,
            button_circle,
            button_triangle,
            button_left_shoulder,
            button_right_shoulder,
            button_left_trigger,
            button_right_trigger,
            button_left_joystick,
            button_right_joystick,
            button_share,
            button_options,
            button_touchpad,
            button_ps,
            axis_lx,
            axis_ly,
            axis_rx,
            axis_ry,
            gyro_x,
            gyro_y,
            gyro_z,
            accel_x,
            accel_y,
            accel_z,
            connected,
            battery_level,
            bluetooth,
            debug,
            temp,
        }
    }
}



impl Ps4Request {
    pub const ID: u32 = 1992038561;
    pub const NAME: &'static str = "Ps4Request";
}

#[derive(Debug, Clone)]
pub struct Ps4Request {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub rumble_small: Option<i32>,
    pub rumble_large: Option<i32>,
    pub led_red: Option<i32>,
    pub led_green: Option<i32>,
    pub led_blue: Option<i32>,
    pub led_flash_on: Option<i32>,
    pub led_flash_off: Option<i32>,
}

impl serde::Serialize for Ps4Request {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        use serde::ser::SerializeSeq;
        let mut seq = serializer.serialize_seq(Some(8))?;
        seq.serialize_element(&self.req_id)?;
        seq.serialize_element(&self.rumble_small)?;
        seq.serialize_element(&self.rumble_large)?;
        seq.serialize_element(&self.led_red)?;
        seq.serialize_element(&self.led_green)?;
        seq.serialize_element(&self.led_blue)?;
        seq.serialize_element(&self.led_flash_on)?;
        seq.serialize_element(&self.led_flash_off)?;
        seq.end()
    }
}

#[derive(Deserialize)]
struct Ps4RequestSerdeHelper(Option<u32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>, Option<i32>);

impl From<Ps4RequestSerdeHelper> for Ps4Request {
    fn from(h: Ps4RequestSerdeHelper) -> Self {
        let (req_id, rumble_small, rumble_large, led_red, led_green, led_blue, led_flash_on, led_flash_off) = (h.0, h.1, h.2, h.3, h.4, h.5, h.6, h.7);
        Ps4Request {
            req_id,
            rumble_small,
            rumble_large,
            led_red,
            led_green,
            led_blue,
            led_flash_on,
            led_flash_off,
        }
    }
}


