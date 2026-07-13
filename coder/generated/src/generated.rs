// Auto-generated from robot.hcl — do not edit by hand.
// Every field is `Option<T>`.
// Messages are encoded as CBOR maps keyed by field id.
use anyhow::Context;

pub trait Msg: Sized + serde::Serialize + serde::de::DeserializeOwned {
    fn id() -> u32;
    fn name() -> &'static str;

    fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

pub const MULTICAST_PORT : u16 = 50000;
pub const MULTICAST_ADDR : &str = "224.0.0.1";


pub const BROKER_ID: u32 = 2490238132;

pub const COMPASS_ID: u32 = 2753264687;

pub const HOVERBOARD_ID: u32 = 1152836275;

pub const LOGGER_ID: u32 = 2753177333;

pub const MOWER_ID: u32 = 1092332049;

pub const PINGER_ID: u32 = 1295055938;

pub const PS4_ID: u32 = 360195552;

pub const SNIFFER_ID: u32 = 836480628;

pub const TUI_SNIFFER_ID: u32 = 3577618233;





#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct BrokerSubscribeRequest {
    #[cbor(key = 0)]
    pub src: Option<u32>,
    #[cbor(key = 1)]
    pub msg_type: Option<u32>,
}

impl BrokerSubscribeRequest {
    pub fn id() -> u32 {
        3190208493
    }

    pub fn name() -> &'static str {
        "BrokerSubscribeRequest"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for BrokerSubscribeRequest {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct CompassEvent {
    /// Heading in degrees
    #[cbor(key = 0)]
    pub heading: Option<f32>,
    /// Pitch in degrees
    #[cbor(key = 1)]
    pub pitch: Option<f32>,
    /// Roll in degrees
    #[cbor(key = 2)]
    pub roll: Option<f32>,
    /// Magnetometer X axis in uT
    #[cbor(key = 3)]
    pub mag_x: Option<f32>,
    /// Magnetometer Y axis in uT
    #[cbor(key = 4)]
    pub mag_y: Option<f32>,
    /// Magnetometer Z axis in uT
    #[cbor(key = 5)]
    pub mag_z: Option<f32>,
    /// Accelerometer X axis in m/s^2
    #[cbor(key = 6)]
    pub accel_x: Option<f32>,
    /// Accelerometer Y axis in m/s^2
    #[cbor(key = 7)]
    pub accel_y: Option<f32>,
    /// Accelerometer Z axis in m/s^2
    #[cbor(key = 8)]
    pub accel_z: Option<f32>,
}

impl CompassEvent {
    pub fn id() -> u32 {
        3197332525
    }

    pub fn name() -> &'static str {
        "CompassEvent"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for CompassEvent {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct DeviceAliveEvent {
    #[cbor(key = 0)]
    pub device: Option<String>,
    #[cbor(key = 1)]
    pub endpoint: Option<String>,
    /// Timestamp in milliseconds since epoch
    #[cbor(key = 2)]
    pub timestamp: Option<u64>,
}

impl DeviceAliveEvent {
    pub fn id() -> u32 {
        2637772092
    }

    pub fn name() -> &'static str {
        "DeviceAliveEvent"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for DeviceAliveEvent {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct EndpointAnnounce {
    /// Unique identifier for the announcing endpoint
    #[cbor(key = 0)]
    pub id: Option<u32>,
    /// Name of the announcing endpoint
    #[cbor(key = 1)]
    pub name: Option<String>,
    /// List of services provided by the endpoint
    #[cbor(key = 2)]
    pub services: Option<Vec<u32>>,
    /// List of events emitted by the endpoint
    #[cbor(key = 3)]
    pub events: Option<Vec<u32>>,
    /// List of replies supported by the endpoint
    #[cbor(key = 4)]
    pub replies: Option<Vec<u32>>,
    /// List of subscriptions for the endpoint
    #[cbor(key = 5)]
    pub subscribes: Option<Vec<u32>>,
}

impl EndpointAnnounce {
    pub fn id() -> u32 {
        2371693343
    }

    pub fn name() -> &'static str {
        "EndpointAnnounce"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for EndpointAnnounce {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct EndpointAnnounceReply {
    /// Timestamp in milliseconds since epoch
    #[cbor(key = 0)]
    pub utc: Option<u64>,
}

impl EndpointAnnounceReply {
    pub fn id() -> u32 {
        3238220441
    }

    pub fn name() -> &'static str {
        "EndpointAnnounceReply"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for EndpointAnnounceReply {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct Envelope {
    /// Source endpoint name
    #[cbor(key = 0)]
    pub src: Option<u32>,
    /// Destination endpoint name
    #[cbor(key = 1)]
    pub dst: Option<u32>,
    /// Message type name
    #[cbor(key = 2)]
    pub msg_type: Option<u32>,
    /// Request ID for matching request/reply
    #[cbor(key = 3)]
    pub request_id: Option<u32>,
    /// Instance ID for matching request/reply
    #[cbor(key = 4)]
    pub instance_id: Option<u32>,
    /// Serialized payload of the message
    #[cbor(key = 5)]
    pub payload: Option<Vec<u8>>,
}

impl Envelope {
    pub fn id() -> u32 {
        1228864117
    }

    pub fn name() -> &'static str {
        "Envelope"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for Envelope {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct GenericReply {
    /// For request/reply matching, 0 if not a request/reply
    #[cbor(key = 0)]
    pub req_id: Option<u32>,
    /// Error code, 0 if no error
    #[cbor(key = 1)]
    pub error_code: Option<u32>,
    /// Error message or additional information
    #[cbor(key = 2)]
    pub message: Option<String>,
    /// Message type identifier , the original request
    #[cbor(key = 3)]
    pub msg_type: Option<u32>,
}

impl GenericReply {
    pub fn id() -> u32 {
        2578784998
    }

    pub fn name() -> &'static str {
        "GenericReply"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for GenericReply {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct HeatingEvent {
    /// Current temperature in Celsius
    #[cbor(key = 0)]
    pub temperature: Option<f32>,
    /// Setpoint temperature in Celsius
    #[cbor(key = 1)]
    pub setpoint: Option<f32>,
    /// Heating status
    #[cbor(key = 2)]
    pub heating: Option<bool>,
}

impl HeatingEvent {
    pub fn id() -> u32 {
        461737375
    }

    pub fn name() -> &'static str {
        "HeatingEvent"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for HeatingEvent {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct HoverboardEvent {
    /// 1:Voltage 2:Speed 3:Torque
    #[cbor(key = 0)]
    pub ctrl_mod: Option<i32>,
    /// 0:Commutation 1:Sinusoidal 2:FOC
    #[cbor(key = 1)]
    pub ctrl_typ: Option<i32>,
    /// Max phase current A
    #[cbor(key = 2)]
    pub cur_mot_max: Option<i32>,
    /// Max motor RPM
    #[cbor(key = 3)]
    pub rpm_mot_max: Option<i32>,
    /// Enable field weak 0:OFF 1:ON
    #[cbor(key = 4)]
    pub fi_weak_ena: Option<i32>,
    /// Field weak high RPM
    #[cbor(key = 5)]
    pub fi_weak_hi: Option<i32>,
    /// Field weak low RPM
    #[cbor(key = 6)]
    pub fi_weak_lo: Option<i32>,
    /// Field weak max current A (FOC only)
    #[cbor(key = 7)]
    pub fi_weak_max: Option<i32>,
    /// Max Phase Adv angle Deg (SIN only)
    #[cbor(key = 8)]
    pub phase_adv_max_deg: Option<i32>,
    /// Input1 raw value
    #[cbor(key = 9)]
    pub input1_raw: Option<i32>,
    /// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    #[cbor(key = 10)]
    pub input1_typ: Option<i32>,
    /// Input1 minimum value
    #[cbor(key = 11)]
    pub input1_min: Option<i32>,
    /// Input1 middle value
    #[cbor(key = 12)]
    pub input1_mid: Option<i32>,
    /// Input1 maximum value
    #[cbor(key = 13)]
    pub input1_max: Option<i32>,
    /// Input1 command value
    #[cbor(key = 14)]
    pub input1_cmd: Option<i32>,
    /// Input2 raw value
    #[cbor(key = 15)]
    pub input2_raw: Option<i32>,
    /// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    #[cbor(key = 16)]
    pub input2_typ: Option<i32>,
    /// Input2 minimum value
    #[cbor(key = 17)]
    pub input2_min: Option<i32>,
    /// Input2 middle value
    #[cbor(key = 18)]
    pub input2_mid: Option<i32>,
    /// Input2 maximum value
    #[cbor(key = 19)]
    pub input2_max: Option<i32>,
    /// Input2 command value
    #[cbor(key = 20)]
    pub input2_cmd: Option<i32>,
    /// Input1 raw value
    #[cbor(key = 21)]
    pub aux_input1_raw: Option<i32>,
    /// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    #[cbor(key = 22)]
    pub aux_input1_typ: Option<i32>,
    /// Input1 minimum value
    #[cbor(key = 23)]
    pub aux_input1_min: Option<i32>,
    /// Input1 middle value
    #[cbor(key = 24)]
    pub aux_input1_mid: Option<i32>,
    /// Input1 maximum value
    #[cbor(key = 25)]
    pub aux_input1_max: Option<i32>,
    /// Input1 command value
    #[cbor(key = 26)]
    pub aux_input1_cmd: Option<i32>,
    /// Input2 raw value
    #[cbor(key = 27)]
    pub aux_input2_raw: Option<i32>,
    /// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    #[cbor(key = 28)]
    pub aux_input2_typ: Option<i32>,
    /// Input2 minimum value
    #[cbor(key = 29)]
    pub aux_input2_min: Option<i32>,
    /// Input2 middle value
    #[cbor(key = 30)]
    pub aux_input2_mid: Option<i32>,
    /// Input2 maximum value
    #[cbor(key = 31)]
    pub aux_input2_max: Option<i32>,
    /// Input2 command value
    #[cbor(key = 32)]
    pub aux_input2_cmd: Option<i32>,
    /// Total DC Link current A *100
    #[cbor(key = 33)]
    pub dc_curr: Option<f32>,
    /// Right DC Link current A *100
    #[cbor(key = 34)]
    pub rdc_curr: Option<f32>,
    /// Left DC Link current A *100
    #[cbor(key = 35)]
    pub ldc_curr: Option<f32>,
    /// Left Motor Command RPM
    #[cbor(key = 36)]
    pub cmdl: Option<i32>,
    /// Right Motor Command RPM
    #[cbor(key = 37)]
    pub cmdr: Option<i32>,
    /// Motor Measured Avg RPM
    #[cbor(key = 38)]
    pub spd_avg: Option<i32>,
    /// Left Motor Measured RPM
    #[cbor(key = 39)]
    pub spdl: Option<i32>,
    /// Right Motor Measured RPM
    #[cbor(key = 40)]
    pub spdr: Option<i32>,
    /// Rate *10
    #[cbor(key = 41)]
    pub filter_rate: Option<i32>,
    /// Speed Coefficient *10
    #[cbor(key = 42)]
    pub spd_coef: Option<i32>,
    /// Steer Coefficient *10
    #[cbor(key = 43)]
    pub str_coef: Option<i32>,
    /// Calibrated Battery Voltage *100
    #[cbor(key = 44)]
    pub batv: Option<f32>,
    /// Calibrated Temperature C *10
    #[cbor(key = 45)]
    pub temp: Option<f32>,
}

impl HoverboardEvent {
    pub fn id() -> u32 {
        104988481
    }

    pub fn name() -> &'static str {
        "HoverboardEvent"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for HoverboardEvent {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct HoverboardRequest {
    /// For request/reply matching, 0 if not a request/reply
    #[cbor(key = 0)]
    pub req_id: Option<u32>,
    /// Speed command for the hoverboard
    #[cbor(key = 1)]
    pub speed: Option<i32>,
    /// Steering command for the hoverboard
    #[cbor(key = 2)]
    pub steer: Option<i32>,
}

impl HoverboardRequest {
    pub fn id() -> u32 {
        2735870956
    }

    pub fn name() -> &'static str {
        "HoverboardRequest"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for HoverboardRequest {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct Max31855Event {
    /// Thermocouple temperature in Celsius
    #[cbor(key = 0)]
    pub thermocouple_temp: Option<f32>,
    /// Internal temperature in Celsius
    #[cbor(key = 1)]
    pub internal_temp: Option<f32>,
    /// Fault detected
    #[cbor(key = 2)]
    pub fault: Option<bool>,
    /// Short to VCC detected
    #[cbor(key = 3)]
    pub fault_short_vcc: Option<bool>,
    /// Short to GND detected
    #[cbor(key = 4)]
    pub fault_short_gnd: Option<bool>,
    /// Open thermocouple detected
    #[cbor(key = 5)]
    pub fault_open_tc: Option<bool>,
}

impl Max31855Event {
    pub fn id() -> u32 {
        2831607083
    }

    pub fn name() -> &'static str {
        "Max31855Event"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for Max31855Event {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct PingReply {
    #[cbor(key = 0)]
    pub req_id: Option<u32>,
    /// Timestamp in milliseconds since epoch
    #[cbor(key = 1)]
    pub timestamp: Option<u64>,
}

impl PingReply {
    pub fn id() -> u32 {
        1594103907
    }

    pub fn name() -> &'static str {
        "PingReply"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for PingReply {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct PingRequest {
    #[cbor(key = 0)]
    pub req_id: Option<u32>,
    /// Timestamp in milliseconds since epoch
    #[cbor(key = 1)]
    pub timestamp: Option<u64>,
}

impl PingRequest {
    pub fn id() -> u32 {
        31253678
    }

    pub fn name() -> &'static str {
        "PingRequest"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for PingRequest {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct Ps4Event {
    #[cbor(key = 0)]
    pub button_left: Option<bool>,
    #[cbor(key = 1)]
    pub button_right: Option<bool>,
    #[cbor(key = 2)]
    pub button_up: Option<bool>,
    #[cbor(key = 3)]
    pub button_down: Option<bool>,
    #[cbor(key = 4)]
    pub button_square: Option<bool>,
    #[cbor(key = 5)]
    pub button_cross: Option<bool>,
    #[cbor(key = 6)]
    pub button_circle: Option<bool>,
    #[cbor(key = 7)]
    pub button_triangle: Option<bool>,
    #[cbor(key = 8)]
    pub button_left_shoulder: Option<bool>,
    #[cbor(key = 9)]
    pub button_right_shoulder: Option<bool>,
    #[cbor(key = 10)]
    pub button_left_trigger: Option<bool>,
    #[cbor(key = 11)]
    pub button_right_trigger: Option<bool>,
    #[cbor(key = 12)]
    pub button_left_joystick: Option<bool>,
    #[cbor(key = 13)]
    pub button_right_joystick: Option<bool>,
    #[cbor(key = 14)]
    pub button_share: Option<bool>,
    #[cbor(key = 15)]
    pub button_options: Option<bool>,
    #[cbor(key = 16)]
    pub button_touchpad: Option<bool>,
    #[cbor(key = 17)]
    pub button_ps: Option<bool>,
    #[cbor(key = 18)]
    pub axis_lx: Option<i32>,
    #[cbor(key = 19)]
    pub axis_ly: Option<i32>,
    #[cbor(key = 20)]
    pub axis_rx: Option<i32>,
    #[cbor(key = 21)]
    pub axis_ry: Option<i32>,
    #[cbor(key = 22)]
    pub gyro_x: Option<i32>,
    #[cbor(key = 23)]
    pub gyro_y: Option<i32>,
    #[cbor(key = 24)]
    pub gyro_z: Option<i32>,
    #[cbor(key = 25)]
    pub accel_x: Option<i32>,
    #[cbor(key = 26)]
    pub accel_y: Option<i32>,
    #[cbor(key = 27)]
    pub accel_z: Option<i32>,
    #[cbor(key = 28)]
    pub connected: Option<bool>,
    #[cbor(key = 29)]
    pub battery_level: Option<i32>,
    #[cbor(key = 30)]
    pub bluetooth: Option<bool>,
    #[cbor(key = 31)]
    pub debug: Option<String>,
    #[cbor(key = 32)]
    pub temp: Option<i32>,
}

impl Ps4Event {
    pub fn id() -> u32 {
        4282593576
    }

    pub fn name() -> &'static str {
        "Ps4Event"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for Ps4Event {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct Ps4Request {
    /// For request/reply matching, 0 if not a request/reply
    #[cbor(key = 0)]
    pub req_id: Option<u32>,
    #[cbor(key = 1)]
    pub rumble_small: Option<i32>,
    #[cbor(key = 2)]
    pub rumble_large: Option<i32>,
    #[cbor(key = 3)]
    pub led_red: Option<i32>,
    #[cbor(key = 4)]
    pub led_green: Option<i32>,
    #[cbor(key = 5)]
    pub led_blue: Option<i32>,
    #[cbor(key = 6)]
    pub led_flash_on: Option<i32>,
    #[cbor(key = 7)]
    pub led_flash_off: Option<i32>,
}

impl Ps4Request {
    pub fn id() -> u32 {
        1992038561
    }

    pub fn name() -> &'static str {
        "Ps4Request"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for Ps4Request {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct SysEvent {
    #[cbor(key = 0)]
    pub utc: Option<u64>,
    #[cbor(key = 1)]
    pub uptime: Option<u64>,
    #[cbor(key = 2)]
    pub free_heap: Option<u64>,
    #[cbor(key = 3)]
    pub flash: Option<u64>,
    #[cbor(key = 4)]
    pub cpu_board: Option<String>,
    #[cbor(key = 5)]
    pub build_date: Option<String>,
}

impl SysEvent {
    pub fn id() -> u32 {
        924742914
    }

    pub fn name() -> &'static str {
        "SysEvent"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for SysEvent {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct SysReply {
    /// For request/reply matching, 0 if not a request/reply
    #[cbor(key = 0)]
    pub req_id: Option<u32>,
    #[cbor(key = 1)]
    pub rc: Option<i32>,
    #[cbor(key = 2)]
    pub message: Option<String>,
}

impl SysReply {
    pub fn id() -> u32 {
        2952492394
    }

    pub fn name() -> &'static str {
        "SysReply"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for SysReply {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct SysRequest {
    /// For request/reply matching, 0 if not a request/reply
    #[cbor(key = 0)]
    pub req_id: Option<u32>,
    #[cbor(key = 1)]
    pub set_time: Option<u64>,
    #[cbor(key = 2)]
    pub reboot: Option<bool>,
    #[cbor(key = 3)]
    pub console: Option<String>,
}

impl SysRequest {
    pub fn id() -> u32 {
        2966412411
    }

    pub fn name() -> &'static str {
        "SysRequest"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for SysRequest {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}



#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct WifiEvent {
    #[cbor(key = 0)]
    pub ip: Option<String>,
    #[cbor(key = 1)]
    pub gateway: Option<String>,
    #[cbor(key = 2)]
    pub netmask: Option<String>,
    #[cbor(key = 3)]
    pub ssid: Option<String>,
    #[cbor(key = 4)]
    pub bssid: Option<String>,
    #[cbor(key = 5)]
    pub channel: Option<i32>,
    #[cbor(key = 6)]
    pub rssi: Option<i32>,
    #[cbor(key = 7)]
    pub mac: Option<String>,
}

impl WifiEvent {
    pub fn id() -> u32 {
        3371536624
    }

    pub fn name() -> &'static str {
        "WifiEvent"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for WifiEvent {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}


