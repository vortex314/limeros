// Auto-generated from robot.hcl — do not edit by hand.
// Every field is `Option<T>`; missing fields serialize as CBOR null.
// Messages are encoded as CBOR arrays (positional, not keyed by name).
use serde_tuple::{Deserialize_tuple, Serialize_tuple};

pub trait BytesSerde<T> {
    fn from_bytes(data: &[u8]) -> Option<T> where T : Sized + serde::de::DeserializeOwned {
        cbor2::from_reader(data).ok()
    }
    fn to_bytes(&self) -> Option<Vec<u8>> where Self : serde::Serialize {
        cbor2::to_vec(self).ok()
    }
}

pub const MULTICAST_PORT : u16 = 50000;
pub const MULTICAST_ADDR : &str = "224.0.0.1";


pub const BROKER_ID: u32 = 1261903363;

pub const HOVERBOARD_ID: u32 = 126669173;

pub const LOGGER_ID: u32 = 3862246649;

pub const MOWER_ID: u32 = 2050343113;

pub const PS4_ID: u32 = 2501583945;





impl EndpointAnnounce {
    pub const ID: u32 = 2371693343;
    pub const NAME: &'static str = "EndpointAnnounce";
}

#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct EndpointAnnounce {
    pub endpoint_id: Option<u32>,
    pub endpoint_name: Option<String>,
    pub device_name: Option<String>,
    pub description: Option<String>,
}

impl BytesSerde<EndpointAnnounce> for EndpointAnnounce {}



impl EndpointAnnounceReply {
    pub const ID: u32 = 3238220441;
    pub const NAME: &'static str = "EndpointAnnounceReply";
}

#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct EndpointAnnounceReply {
    pub broker_id: Option<u32>,
    pub endpoint_name: Option<String>,
    pub device_name: Option<String>,
    pub description: Option<String>,
}

impl BytesSerde<EndpointAnnounceReply> for EndpointAnnounceReply {}



impl HoverboardEvent {
    pub const ID: u32 = 104988481;
    pub const NAME: &'static str = "HoverboardEvent";
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

impl BytesSerde<HoverboardEvent> for HoverboardEvent {}



impl HoverboardReply {
    pub const ID: u32 = 2095960949;
    pub const NAME: &'static str = "HoverboardReply";
}

#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct HoverboardReply {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub error_code: Option<i32>,
    pub message: Option<String>,
}

impl BytesSerde<HoverboardReply> for HoverboardReply {}



impl HoverboardRequest {
    pub const ID: u32 = 2735870956;
    pub const NAME: &'static str = "HoverboardRequest";
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

impl BytesSerde<HoverboardRequest> for HoverboardRequest {}



impl PingReply {
    pub const ID: u32 = 1594103907;
    pub const NAME: &'static str = "PingReply";
}

#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct PingReply {
    pub req_id: Option<u32>,
}

impl BytesSerde<PingReply> for PingReply {}



impl PingRequest {
    pub const ID: u32 = 31253678;
    pub const NAME: &'static str = "PingRequest";
}

#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct PingRequest {
    pub req_id: Option<u32>,
}

impl BytesSerde<PingRequest> for PingRequest {}



impl Ps4Event {
    pub const ID: u32 = 4282593576;
    pub const NAME: &'static str = "Ps4Event";
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

impl BytesSerde<Ps4Event> for Ps4Event {}



impl Ps4Request {
    pub const ID: u32 = 1992038561;
    pub const NAME: &'static str = "Ps4Request";
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

impl BytesSerde<Ps4Request> for Ps4Request {}



impl SysEvent {
    pub const ID: u32 = 924742914;
    pub const NAME: &'static str = "SysEvent";
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

impl BytesSerde<SysEvent> for SysEvent {}



impl SysReply {
    pub const ID: u32 = 2952492394;
    pub const NAME: &'static str = "SysReply";
}

#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct SysReply {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub rc: Option<i32>,
    pub message: Option<String>,
}

impl BytesSerde<SysReply> for SysReply {}



impl SysRequest {
    pub const ID: u32 = 2966412411;
    pub const NAME: &'static str = "SysRequest";
}

#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct SysRequest {
    /// For request/reply matching, 0 if not a request/reply
    pub req_id: Option<u32>,
    pub set_time: Option<u64>,
    pub reboot: Option<bool>,
    pub console: Option<String>,
}

impl BytesSerde<SysRequest> for SysRequest {}



impl UdpMessage {
    pub const ID: u32 = 1293877827;
    pub const NAME: &'static str = "UdpMessage";
}

#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct UdpMessage {
    pub src: Option<u32>,
    pub dst: Option<u32>,
    pub msg_type: Option<u32>,
    pub req_id: Option<u32>,
    pub payload: Option<Vec<u8>>,
}

impl BytesSerde<UdpMessage> for UdpMessage {}


