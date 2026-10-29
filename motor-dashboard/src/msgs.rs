// Auto-generated from robot.hcl — do not edit by hand.
// Every field is `Option<T>`.
// Messages are encoded as CBOR maps keyed by field id.
// Generated on 2026-10-28 11:43:04 with template rust.tera

use anyhow::Context;

pub trait Msg: Sized + serde::Serialize + serde::de::DeserializeOwned {
    fn id() -> u32;
    fn name() -> &'static str;

    fn to_json(&self) -> anyhow::Result<String> {
        serde_json::to_string(self).context("Failed to serialize to JSON")
    }

    fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

pub const MULTICAST_PORT : u16 = 50000;
pub const MULTICAST_ADDR : &str = "224.0.0.1";


pub const BRAIN_ID: u32 = 2200474099;

pub const BROKER_ID: u32 = 2490238132;

pub const COMPASS_ID: u32 = 2753264687;

pub const HOVERBOARD_ID: u32 = 1152836275;

pub const LOGGER_ID: u32 = 2753177333;

pub const MOTOR_ID: u32 = 3404760178;

pub const MOWER_ID: u32 = 1092332049;

pub const PINGER_ID: u32 = 1295055938;

pub const PS4_ID: u32 = 360195552;

pub const SNIFFER_ID: u32 = 836480628;

pub const TUI_SNIFFER_ID: u32 = 3577618233;



use dashmap::DashMap;
use lazy_static::lazy_static;

lazy_static! {
    pub static ref ID_TO_NAME: DashMap<u32, String> = {
        let m = DashMap::new();
        m.insert(3190208493, "BrokerSubscribeRequest".to_string());
        m.insert(3197332525, "CompassEvent".to_string());
        m.insert(2637772092, "DeviceAliveEvent".to_string());
        m.insert(2371693343, "EndpointAnnounce".to_string());
        m.insert(3238220441, "EndpointAnnounceReply".to_string());
        m.insert(1228864117, "Envelope".to_string());
        m.insert(1911628039, "EnvelopeFilter".to_string());
        m.insert(2578784998, "GenericReply".to_string());
        m.insert(461737375, "HeatingEvent".to_string());
        m.insert(578653874, "HeatingRequest".to_string());
        m.insert(104988481, "HoverboardEvent".to_string());
        m.insert(2095960949, "HoverboardReply".to_string());
        m.insert(2735870956, "HoverboardRequest".to_string());
        m.insert(1802836182, "ImuEvent".to_string());
        m.insert(1361661643, "ListenEndpoint".to_string());
        m.insert(2831607083, "Max31855Event".to_string());
        m.insert(2506002638, "MotorEvent".to_string());
        m.insert(1863653678, "MotorReply".to_string());
        m.insert(929372191, "MotorRequest".to_string());
        m.insert(1594103907, "PingReply".to_string());
        m.insert(31253678, "PingRequest".to_string());
        m.insert(4282593576, "Ps4Event".to_string());
        m.insert(1992038561, "Ps4Request".to_string());
        m.insert(1736013937, "RegisterEndpoint".to_string());
        m.insert(924742914, "SysEvent".to_string());
        m.insert(2952492394, "SysReply".to_string());
        m.insert(2966412411, "SysRequest".to_string());
        m.insert(1082063571, "UsEvent".to_string());
        m.insert(3371536624, "WifiEvent".to_string());
        m.insert(2200474099, "brain".to_string());
        
        m.insert(2490238132, "broker".to_string());
        
        m.insert(2753264687, "compass".to_string());
        
        m.insert(1152836275, "hoverboard".to_string());
        
        m.insert(2753177333, "logger".to_string());
        
        m.insert(3404760178, "motor".to_string());
        
        m.insert(1092332049, "mower".to_string());
        
        m.insert(1295055938, "pinger".to_string());
        
        m.insert(360195552, "ps4".to_string());
        
        m.insert(836480628, "sniffer".to_string());
        
        m.insert(3577618233, "tui_sniffer".to_string());
        
        m
    };
}

pub fn id_to_string(msg_id: u32) -> String {
    ID_TO_NAME.get(&msg_id).map(|entry| entry.to_string()).unwrap_or_else(|| format!("Unknown({})", msg_id))
}

pub fn opt_id_to_string(msg_id: Option<u32>) -> String {
    match msg_id {
        Some(id) => id_to_string(id),
        None => "None".to_string(),
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct BrokerSubscribeRequest {
    pub src: Option<u32>,
    pub msg_type: Option<u32>,
}

impl Default for BrokerSubscribeRequest {
    fn default() -> Self {
        BrokerSubscribeRequest {
            src: None,
            msg_type: None,
        }
    }
}

impl BrokerSubscribeRequest {
    pub const MSG_ID : u32 = 3190208493;
    pub const MSG_NAME: &'static str = "BrokerSubscribeRequest";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct CompassEvent {
    pub heading: Option<f32>,// Heading in degrees
    pub pitch: Option<f32>,// Pitch in degrees
    pub roll: Option<f32>,// Roll in degrees
    pub mag_x: Option<f32>,// Magnetometer X axis in uT
    pub mag_y: Option<f32>,// Magnetometer Y axis in uT
    pub mag_z: Option<f32>,// Magnetometer Z axis in uT
    pub accel_x: Option<f32>,// Accelerometer X axis in m/s^2
    pub accel_y: Option<f32>,// Accelerometer Y axis in m/s^2
    pub accel_z: Option<f32>,// Accelerometer Z axis in m/s^2
}

impl Default for CompassEvent {
    fn default() -> Self {
        CompassEvent {
            heading: None,
            pitch: None,
            roll: None,
            mag_x: None,
            mag_y: None,
            mag_z: None,
            accel_x: None,
            accel_y: None,
            accel_z: None,
        }
    }
}

impl CompassEvent {
    pub const MSG_ID : u32 = 3197332525;
    pub const MSG_NAME: &'static str = "CompassEvent";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct DeviceAliveEvent {
    pub device: Option<String>,
    pub endpoint: Option<String>,
    pub timestamp: Option<u64>,// Timestamp in milliseconds since epoch
}

impl Default for DeviceAliveEvent {
    fn default() -> Self {
        DeviceAliveEvent {
            device: None,
            endpoint: None,
            timestamp: None,
        }
    }
}

impl DeviceAliveEvent {
    pub const MSG_ID : u32 = 2637772092;
    pub const MSG_NAME: &'static str = "DeviceAliveEvent";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct EndpointAnnounce {
    pub id: Option<u32>,// Unique identifier for the announcing endpoint
    pub name: Option<String>,// Name of the announcing endpoint
    pub description: Option<String>,// Description of the announcing endpoint
    pub services: Option<Vec<u32>>,// List of services provided by the endpoint
    pub events: Option<Vec<u32>>,// List of events emitted by the endpoint
    pub replies: Option<Vec<u32>>,// List of replies supported by the endpoint
    pub subscribes: Option<Vec<u32>>,// List of subscriptions for the endpoint
}

impl Default for EndpointAnnounce {
    fn default() -> Self {
        EndpointAnnounce {
            id: None,
            name: None,
            description: None,
            services: None,
            events: None,
            replies: None,
            subscribes: None,
        }
    }
}

impl EndpointAnnounce {
    pub const MSG_ID : u32 = 2371693343;
    pub const MSG_NAME: &'static str = "EndpointAnnounce";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct EndpointAnnounceReply {
    pub utc: Option<u64>,// Timestamp in milliseconds since epoch
}

impl Default for EndpointAnnounceReply {
    fn default() -> Self {
        EndpointAnnounceReply {
            utc: None,
        }
    }
}

impl EndpointAnnounceReply {
    pub const MSG_ID : u32 = 3238220441;
    pub const MSG_NAME: &'static str = "EndpointAnnounceReply";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct Envelope {
    pub src: Option<u32>,// Source endpoint name
    pub dst: Option<u32>,// Destination endpoint name
    pub msg_type: Option<u32>,// Message type name
    pub sequence: Option<u32>,// Request ID for matching request/reply
    pub instance_id: Option<u32>,// Instance ID for matching request/reply
    #[serde(with = "serde_bytes")]
    pub payload: Option<Vec<u8>>,// Serialized payload of the message
}

impl Default for Envelope {
    fn default() -> Self {
        Envelope {
            src: None,
            dst: None,
            msg_type: None,
            sequence: None,
            instance_id: None,
            payload: None,
        }
    }
}

impl Envelope {
    pub const MSG_ID : u32 = 1228864117;
    pub const MSG_NAME: &'static str = "Envelope";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct EnvelopeFilter {
    pub or_filter: Option<String>,// OR filter for envelopes
    pub and_filter: Option<String>,// AND filter for envelopes
}

impl Default for EnvelopeFilter {
    fn default() -> Self {
        EnvelopeFilter {
            or_filter: None,
            and_filter: None,
        }
    }
}

impl EnvelopeFilter {
    pub const MSG_ID : u32 = 1911628039;
    pub const MSG_NAME: &'static str = "EnvelopeFilter";

    pub const fn id() -> u32 {
        1911628039
    }

    pub fn name() -> &'static str {
        "EnvelopeFilter"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for EnvelopeFilter {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct GenericReply {
    pub req_id: Option<u32>,// For request/reply matching, 0 if not a request/reply
    pub error_code: Option<u32>,// Error code, 0 if no error
    pub message: Option<String>,// Error message or additional information
    pub msg_type: Option<u32>,// Message type identifier , the original request
}

impl Default for GenericReply {
    fn default() -> Self {
        GenericReply {
            req_id: None,
            error_code: None,
            message: None,
            msg_type: None,
        }
    }
}

impl GenericReply {
    pub const MSG_ID : u32 = 2578784998;
    pub const MSG_NAME: &'static str = "GenericReply";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct HeatingEvent {
    pub temperature_c: Option<f32>,// Current temperature in Celsius
    pub setpoint_c: Option<f32>,// Setpoint temperature in Celsius
    pub enabled: Option<bool>,// Heating enabled or disabled
    pub output_pct: Option<f32>,// Output percentage of the heating element
    pub heater_on: Option<bool>,// Heater is currently on or off
    pub fault: Option<bool>,// Fault detected in the heating system
    pub timestamp_ms: Option<u64>,// Timestamp in milliseconds since epoch
}

impl Default for HeatingEvent {
    fn default() -> Self {
        HeatingEvent {
            temperature_c: None,
            setpoint_c: None,
            enabled: None,
            output_pct: None,
            heater_on: None,
            fault: None,
            timestamp_ms: None,
        }
    }
}

impl HeatingEvent {
    pub const MSG_ID : u32 = 461737375;
    pub const MSG_NAME: &'static str = "HeatingEvent";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct HeatingRequest {
    pub setpoint_c: Option<f32>,// Setpoint temperature in Celsius
    pub enabled: Option<bool>,// Enable or disable heating
    pub kp: Option<f32>,// Proportional gain for PID controller
    pub ki: Option<f32>,// Integral gain for PID controller
    pub kd: Option<f32>,// Derivative gain for PID controller
    pub reset_integral: Option<bool>,// Reset the integral term of the PID controller
}

impl Default for HeatingRequest {
    fn default() -> Self {
        HeatingRequest {
            setpoint_c: None,
            enabled: None,
            kp: None,
            ki: None,
            kd: None,
            reset_integral: None,
        }
    }
}

impl HeatingRequest {
    pub const MSG_ID : u32 = 578653874;
    pub const MSG_NAME: &'static str = "HeatingRequest";

    pub const fn id() -> u32 {
        578653874
    }

    pub fn name() -> &'static str {
        "HeatingRequest"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for HeatingRequest {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct HoverboardEvent {
    pub ctrl_mod: Option<i32>,// 1:Voltage 2:Speed 3:Torque
    pub ctrl_typ: Option<i32>,// 0:Commutation 1:Sinusoidal 2:FOC
    pub cur_mot_max: Option<i32>,// Max phase current A
    pub rpm_mot_max: Option<i32>,// Max motor RPM
    pub fi_weak_ena: Option<i32>,// Enable field weak 0:OFF 1:ON
    pub fi_weak_hi: Option<i32>,// Field weak high RPM
    pub fi_weak_lo: Option<i32>,// Field weak low RPM
    pub fi_weak_max: Option<i32>,// Field weak max current A (FOC only)
    pub phase_adv_max_deg: Option<i32>,// Max Phase Adv angle Deg (SIN only)
    pub input1_raw: Option<i32>,// Input1 raw value
    pub input1_typ: Option<i32>,// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    pub input1_min: Option<i32>,// Input1 minimum value
    pub input1_mid: Option<i32>,// Input1 middle value
    pub input1_max: Option<i32>,// Input1 maximum value
    pub input1_cmd: Option<i32>,// Input1 command value
    pub input2_raw: Option<i32>,// Input2 raw value
    pub input2_typ: Option<i32>,// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    pub input2_min: Option<i32>,// Input2 minimum value
    pub input2_mid: Option<i32>,// Input2 middle value
    pub input2_max: Option<i32>,// Input2 maximum value
    pub input2_cmd: Option<i32>,// Input2 command value
    pub aux_input1_raw: Option<i32>,// Input1 raw value
    pub aux_input1_typ: Option<i32>,// Input1 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    pub aux_input1_min: Option<i32>,// Input1 minimum value
    pub aux_input1_mid: Option<i32>,// Input1 middle value
    pub aux_input1_max: Option<i32>,// Input1 maximum value
    pub aux_input1_cmd: Option<i32>,// Input1 command value
    pub aux_input2_raw: Option<i32>,// Input2 raw value
    pub aux_input2_typ: Option<i32>,// Input2 type 0:Disabled, 1:Normal Pot, 2:Middle Resting Pot, 3:Auto-detect
    pub aux_input2_min: Option<i32>,// Input2 minimum value
    pub aux_input2_mid: Option<i32>,// Input2 middle value
    pub aux_input2_max: Option<i32>,// Input2 maximum value
    pub aux_input2_cmd: Option<i32>,// Input2 command value
    pub dc_curr: Option<i32>,// Total DC Link current A *100
    pub rdc_curr: Option<i32>,// Right DC Link current A *100
    pub ldc_curr: Option<i32>,// Left DC Link current A *100
    pub cmdl: Option<i32>,// Left Motor Command RPM
    pub cmdr: Option<i32>,// Right Motor Command RPM
    pub spd_avg: Option<i32>,// Motor Measured Avg RPM
    pub spdl: Option<i32>,// Left Motor Measured RPM
    pub spdr: Option<i32>,// Right Motor Measured RPM
    pub filter_rate: Option<i32>,// Rate *10
    pub spd_coef: Option<i32>,// Speed Coefficient *10
    pub str_coef: Option<i32>,// Steer Coefficient *10
    pub batv: Option<i32>,// Calibrated Battery Voltage *100
    pub temp: Option<i32>,// Calibrated Temperature C *10
}

impl Default for HoverboardEvent {
    fn default() -> Self {
        HoverboardEvent {
            ctrl_mod: None,
            ctrl_typ: None,
            cur_mot_max: None,
            rpm_mot_max: None,
            fi_weak_ena: None,
            fi_weak_hi: None,
            fi_weak_lo: None,
            fi_weak_max: None,
            phase_adv_max_deg: None,
            input1_raw: None,
            input1_typ: None,
            input1_min: None,
            input1_mid: None,
            input1_max: None,
            input1_cmd: None,
            input2_raw: None,
            input2_typ: None,
            input2_min: None,
            input2_mid: None,
            input2_max: None,
            input2_cmd: None,
            aux_input1_raw: None,
            aux_input1_typ: None,
            aux_input1_min: None,
            aux_input1_mid: None,
            aux_input1_max: None,
            aux_input1_cmd: None,
            aux_input2_raw: None,
            aux_input2_typ: None,
            aux_input2_min: None,
            aux_input2_mid: None,
            aux_input2_max: None,
            aux_input2_cmd: None,
            dc_curr: None,
            rdc_curr: None,
            ldc_curr: None,
            cmdl: None,
            cmdr: None,
            spd_avg: None,
            spdl: None,
            spdr: None,
            filter_rate: None,
            spd_coef: None,
            str_coef: None,
            batv: None,
            temp: None,
        }
    }
}

impl HoverboardEvent {
    pub const MSG_ID : u32 = 104988481;
    pub const MSG_NAME: &'static str = "HoverboardEvent";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct HoverboardReply {
    pub speed: Option<i32>,// Speed command for the hoverboard
    pub steer: Option<i32>,// Steering command for the hoverboard
}

impl Default for HoverboardReply {
    fn default() -> Self {
        HoverboardReply {
            speed: None,
            steer: None,
        }
    }
}

impl HoverboardReply {
    pub const MSG_ID : u32 = 2095960949;
    pub const MSG_NAME: &'static str = "HoverboardReply";

    pub const fn id() -> u32 {
        2095960949
    }

    pub fn name() -> &'static str {
        "HoverboardReply"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for HoverboardReply {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct HoverboardRequest {
    pub speed: Option<i32>,// Speed command for the hoverboard
    pub steer: Option<i32>,// Steering command for the hoverboard
}

impl Default for HoverboardRequest {
    fn default() -> Self {
        HoverboardRequest {
            speed: None,
            steer: None,
        }
    }
}

impl HoverboardRequest {
    pub const MSG_ID : u32 = 2735870956;
    pub const MSG_NAME: &'static str = "HoverboardRequest";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct ImuEvent {
    pub gyro_x: Option<f32>,// Gyroscope X axis in deg/s
    pub gyro_y: Option<f32>,// Gyroscope Y axis in deg/s
    pub gyro_z: Option<f32>,// Gyroscope Z axis in deg/s
    pub accel_x: Option<f32>,// Accelerometer X axis in m/s^2
    pub accel_y: Option<f32>,// Accelerometer Y axis in m/s^2
    pub accel_z: Option<f32>,// Accelerometer Z axis in m/s^2
}

impl Default for ImuEvent {
    fn default() -> Self {
        ImuEvent {
            gyro_x: None,
            gyro_y: None,
            gyro_z: None,
            accel_x: None,
            accel_y: None,
            accel_z: None,
        }
    }
}

impl ImuEvent {
    pub const MSG_ID : u32 = 1802836182;
    pub const MSG_NAME: &'static str = "ImuEvent";

    pub const fn id() -> u32 {
        1802836182
    }

    pub fn name() -> &'static str {
        "ImuEvent"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for ImuEvent {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct ListenEndpoint {
    pub dsts: Option<u32>,// Destination ID to listen to
    pub srcs: Option<u32>,// Filter for the envelopes to listen to
    pub msg_types: Option<u32>,// Filter for the destination envelopes to listen to
}

impl Default for ListenEndpoint {
    fn default() -> Self {
        ListenEndpoint {
            dsts: None,
            srcs: None,
            msg_types: None,
        }
    }
}

impl ListenEndpoint {
    pub const MSG_ID : u32 = 1361661643;
    pub const MSG_NAME: &'static str = "ListenEndpoint";

    pub const fn id() -> u32 {
        1361661643
    }

    pub fn name() -> &'static str {
        "ListenEndpoint"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for ListenEndpoint {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct Max31855Event {
    pub thermocouple_temp: Option<f32>,// Thermocouple temperature in Celsius
    pub internal_temp: Option<f32>,// Internal temperature in Celsius
    pub fault: Option<bool>,// Fault detected
    pub fault_short_vcc: Option<bool>,// Short to VCC detected
    pub fault_short_gnd: Option<bool>,// Short to GND detected
    pub fault_open_tc: Option<bool>,// Open thermocouple detected
}

impl Default for Max31855Event {
    fn default() -> Self {
        Max31855Event {
            thermocouple_temp: None,
            internal_temp: None,
            fault: None,
            fault_short_vcc: None,
            fault_short_gnd: None,
            fault_open_tc: None,
        }
    }
}

impl Max31855Event {
    pub const MSG_ID : u32 = 2831607083;
    pub const MSG_NAME: &'static str = "Max31855Event";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct MotorEvent {
    pub enabled: Option<bool>,// Cutter enabled or disabled
    pub rpm_actual: Option<i32>,// Current speed of the cutter in RPM
    pub rpm_target: Option<i32>,// Commanded speed of the cutter in RPM
    pub pwm: Option<f32>,// PWM duty cycle for the cutter motor in percent
    pub current: Option<f32>,// Current drawn by the cutter in Amperes
    pub voltage: Option<f32>,// Voltage supplied to the cutter in Volts
    pub temperature: Option<f32>,// Temperature of the cutter in Celsius
    pub Kp: Option<f32>,// Proportional gain for the cutter motor controller
    pub Ki: Option<f32>,// Integral gain for the cutter motor controller
    pub Kd: Option<f32>,// Derivative gain for the cutter motor controller
}

impl Default for MotorEvent {
    fn default() -> Self {
        MotorEvent {
            enabled: None,
            rpm_actual: None,
            rpm_target: None,
            pwm: None,
            current: None,
            voltage: None,
            temperature: None,
            Kp: None,
            Ki: None,
            Kd: None,
        }
    }
}

impl MotorEvent {
    pub const MSG_ID : u32 = 2506002638;
    pub const MSG_NAME: &'static str = "MotorEvent";

    pub const fn id() -> u32 {
        2506002638
    }

    pub fn name() -> &'static str {
        "MotorEvent"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for MotorEvent {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct MotorReply {
    pub status_code: Option<u32>,// Error code, 0 if no error
    pub message: Option<String>,// Error message or additional information
}

impl Default for MotorReply {
    fn default() -> Self {
        MotorReply {
            status_code: None,
            message: None,
        }
    }
}

impl MotorReply {
    pub const MSG_ID : u32 = 1863653678;
    pub const MSG_NAME: &'static str = "MotorReply";

    pub const fn id() -> u32 {
        1863653678
    }

    pub fn name() -> &'static str {
        "MotorReply"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for MotorReply {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct MotorRequest {
    pub enabled: Option<bool>,// Enable or disable the cutter
    pub rpm_target: Option<i32>,// Speed command for the cutter in RPM
}

impl Default for MotorRequest {
    fn default() -> Self {
        MotorRequest {
            enabled: None,
            rpm_target: None,
        }
    }
}

impl MotorRequest {
    pub const MSG_ID : u32 = 929372191;
    pub const MSG_NAME: &'static str = "MotorRequest";

    pub const fn id() -> u32 {
        929372191
    }

    pub fn name() -> &'static str {
        "MotorRequest"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for MotorRequest {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct PingReply {
    pub req_id: Option<u32>,
    pub timestamp: Option<u64>,// Timestamp in milliseconds since epoch
}

impl Default for PingReply {
    fn default() -> Self {
        PingReply {
            req_id: None,
            timestamp: None,
        }
    }
}

impl PingReply {
    pub const MSG_ID : u32 = 1594103907;
    pub const MSG_NAME: &'static str = "PingReply";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct PingRequest {
    pub req_id: Option<u32>,
    pub timestamp: Option<u64>,// Timestamp in milliseconds since epoch
}

impl Default for PingRequest {
    fn default() -> Self {
        PingRequest {
            req_id: None,
            timestamp: None,
        }
    }
}

impl PingRequest {
    pub const MSG_ID : u32 = 31253678;
    pub const MSG_NAME: &'static str = "PingRequest";

    pub const fn id() -> u32 {
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
#[cbor(array)]
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

impl Default for Ps4Event {
    fn default() -> Self {
        Ps4Event {
            button_left: None,
            button_right: None,
            button_up: None,
            button_down: None,
            button_square: None,
            button_cross: None,
            button_circle: None,
            button_triangle: None,
            button_left_shoulder: None,
            button_right_shoulder: None,
            button_left_trigger: None,
            button_right_trigger: None,
            button_left_joystick: None,
            button_right_joystick: None,
            button_share: None,
            button_options: None,
            button_touchpad: None,
            button_ps: None,
            axis_lx: None,
            axis_ly: None,
            axis_rx: None,
            axis_ry: None,
            gyro_x: None,
            gyro_y: None,
            gyro_z: None,
            accel_x: None,
            accel_y: None,
            accel_z: None,
            connected: None,
            battery_level: None,
            bluetooth: None,
            debug: None,
            temp: None,
        }
    }
}

impl Ps4Event {
    pub const MSG_ID : u32 = 4282593576;
    pub const MSG_NAME: &'static str = "Ps4Event";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct Ps4Request {
    pub req_id: Option<u32>,// For request/reply matching, 0 if not a request/reply
    pub rumble_small: Option<i32>,
    pub rumble_large: Option<i32>,
    pub led_red: Option<i32>,
    pub led_green: Option<i32>,
    pub led_blue: Option<i32>,
    pub led_flash_on: Option<i32>,
    pub led_flash_off: Option<i32>,
}

impl Default for Ps4Request {
    fn default() -> Self {
        Ps4Request {
            req_id: None,
            rumble_small: None,
            rumble_large: None,
            led_red: None,
            led_green: None,
            led_blue: None,
            led_flash_on: None,
            led_flash_off: None,
        }
    }
}

impl Ps4Request {
    pub const MSG_ID : u32 = 1992038561;
    pub const MSG_NAME: &'static str = "Ps4Request";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct RegisterEndpoint {
    pub dst: Option<u32>,// Destination ID for the registration
}

impl Default for RegisterEndpoint {
    fn default() -> Self {
        RegisterEndpoint {
            dst: None,
        }
    }
}

impl RegisterEndpoint {
    pub const MSG_ID : u32 = 1736013937;
    pub const MSG_NAME: &'static str = "RegisterEndpoint";

    pub const fn id() -> u32 {
        1736013937
    }

    pub fn name() -> &'static str {
        "RegisterEndpoint"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for RegisterEndpoint {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct SysEvent {
    pub utc: Option<u64>,
    pub uptime: Option<u64>,
    pub free_heap: Option<u64>,
    pub flash_size: Option<u64>,
    pub cpu_board_type: Option<String>,
    pub build_date_time: Option<String>,
}

impl Default for SysEvent {
    fn default() -> Self {
        SysEvent {
            utc: None,
            uptime: None,
            free_heap: None,
            flash_size: None,
            cpu_board_type: None,
            build_date_time: None,
        }
    }
}

impl SysEvent {
    pub const MSG_ID : u32 = 924742914;
    pub const MSG_NAME: &'static str = "SysEvent";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct SysReply {
    pub req_id: Option<u32>,// For request/reply matching, 0 if not a request/reply
    pub rc: Option<i32>,
    pub message: Option<String>,
}

impl Default for SysReply {
    fn default() -> Self {
        SysReply {
            req_id: None,
            rc: None,
            message: None,
        }
    }
}

impl SysReply {
    pub const MSG_ID : u32 = 2952492394;
    pub const MSG_NAME: &'static str = "SysReply";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct SysRequest {
    pub req_id: Option<u32>,// For request/reply matching, 0 if not a request/reply
    pub set_time: Option<u64>,
    pub reboot: Option<bool>,
    pub console: Option<String>,
}

impl Default for SysRequest {
    fn default() -> Self {
        SysRequest {
            req_id: None,
            set_time: None,
            reboot: None,
            console: None,
        }
    }
}

impl SysRequest {
    pub const MSG_ID : u32 = 2966412411;
    pub const MSG_NAME: &'static str = "SysRequest";

    pub const fn id() -> u32 {
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
#[cbor(array)]
pub struct UsEvent {
    pub distance: Option<f32>,// Distance in meters
    pub temperature: Option<f32>,// Temperature in Celsius
    pub status: Option<i32>,// Status code, 0 if no error
}

impl Default for UsEvent {
    fn default() -> Self {
        UsEvent {
            distance: None,
            temperature: None,
            status: None,
        }
    }
}

impl UsEvent {
    pub const MSG_ID : u32 = 1082063571;
    pub const MSG_NAME: &'static str = "UsEvent";

    pub const fn id() -> u32 {
        1082063571
    }

    pub fn name() -> &'static str {
        "UsEvent"
    }

    pub fn from_bytes(data: &[u8]) -> anyhow::Result<Self> {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }

    pub fn to_bytes(&self) -> anyhow::Result<Vec<u8>> {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

impl Msg for UsEvent {
    fn id() -> u32 {
        Self::id()
    }

    fn name() -> &'static str {
        Self::name()
    }
}




#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
#[cbor(array)]
pub struct WifiEvent {
    pub ip: Option<String>,
    pub gateway: Option<String>,
    pub netmask: Option<String>,
    pub ssid: Option<String>,
    pub bssid: Option<String>,
    pub channel: Option<i32>,
    pub rssi: Option<i32>,
    pub mac: Option<String>,
}

impl Default for WifiEvent {
    fn default() -> Self {
        WifiEvent {
            ip: None,
            gateway: None,
            netmask: None,
            ssid: None,
            bssid: None,
            channel: None,
            rssi: None,
            mac: None,
        }
    }
}

impl WifiEvent {
    pub const MSG_ID : u32 = 3371536624;
    pub const MSG_NAME: &'static str = "WifiEvent";

    pub const fn id() -> u32 {
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


