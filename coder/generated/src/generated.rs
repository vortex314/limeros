// Auto-generated from robot.hcl — do not edit by hand.
// Every field is `Option<T>`; missing fields serialize as CBOR null.
// Messages are encoded as CBOR arrays (positional, not keyed by name).
use serde_tuple::{Deserialize_tuple, Serialize_tuple};
use common::base_message::Msg;

pub const MULTICAST_PORT : u16 = 50000;
pub const MULTICAST_ADDR : &str = "224.0.0.1";


pub const BROKER_ID: u32 = 2490238132;

pub const HOVERBOARD_ID: u32 = 1152836275;

pub const LOGGER_ID: u32 = 2753177333;

pub const MOWER_ID: u32 = 1092332049;

pub const PINGER_ID: u32 = 1295055938;

pub const PS4_ID: u32 = 360195552;

pub const SNIFFER_ID: u32 = 836480628;

pub const TUI_SNIFFER_ID: u32 = 3577618233;





#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct BrokerSubscribeRequest {
    pub src: Option<u32>,
    pub msg_type: Option<u32>,
}

impl Msg<BrokerSubscribeRequest> for BrokerSubscribeRequest {
    fn id() -> u32 {
        3190208493
    }
    fn name() -> &'static str {
        "BrokerSubscribeRequest"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
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

impl Msg<HoverboardEvent> for HoverboardEvent {
    fn id() -> u32 {
        104988481
    }
    fn name() -> &'static str {
        "HoverboardEvent"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct HoverboardReply {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub error_code: Option<i32>,
    pub message: Option<String>,
}

impl Msg<HoverboardReply> for HoverboardReply {
    fn id() -> u32 {
        2095960949
    }
    fn name() -> &'static str {
        "HoverboardReply"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct HoverboardRequest {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    /// Speed command for the hoverboard
    pub speed: Option<i32>,
    /// Steering command for the hoverboard
    pub steer: Option<i32>,
}

impl Msg<HoverboardRequest> for HoverboardRequest {
    fn id() -> u32 {
        2735870956
    }
    fn name() -> &'static str {
        "HoverboardRequest"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct PingReply {
    pub req_id: Option<u32>,
    /// Timestamp in milliseconds since epoch
    pub timestamp: Option<u64>,
}

impl Msg<PingReply> for PingReply {
    fn id() -> u32 {
        1594103907
    }
    fn name() -> &'static str {
        "PingReply"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct PingRequest {
    pub req_id: Option<u32>,
    /// Timestamp in milliseconds since epoch
    pub timestamp: Option<u64>,
}

impl Msg<PingRequest> for PingRequest {
    fn id() -> u32 {
        31253678
    }
    fn name() -> &'static str {
        "PingRequest"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
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

impl Msg<Ps4Event> for Ps4Event {
    fn id() -> u32 {
        4282593576
    }
    fn name() -> &'static str {
        "Ps4Event"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
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

impl Msg<Ps4Request> for Ps4Request {
    fn id() -> u32 {
        1992038561
    }
    fn name() -> &'static str {
        "Ps4Request"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct SysEvent {
    pub utc: Option<u64>,
    pub uptime: Option<u64>,
    pub free_heap: Option<u64>,
    pub flash: Option<u64>,
    pub cpu_board: Option<String>,
    pub build_date: Option<String>,
}

impl Msg<SysEvent> for SysEvent {
    fn id() -> u32 {
        924742914
    }
    fn name() -> &'static str {
        "SysEvent"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct SysReply {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub rc: Option<i32>,
    pub message: Option<String>,
}

impl Msg<SysReply> for SysReply {
    fn id() -> u32 {
        2952492394
    }
    fn name() -> &'static str {
        "SysReply"
    }
}



#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct SysRequest {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub set_time: Option<u64>,
    pub reboot: Option<bool>,
    pub console: Option<String>,
}

impl Msg<SysRequest> for SysRequest {
    fn id() -> u32 {
        2966412411
    }
    fn name() -> &'static str {
        "SysRequest"
    }
}


