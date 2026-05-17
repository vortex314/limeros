use serde::{Serialize, Deserialize};
use serde::de::DeserializeOwned;
use anyhow::Result;

pub trait TypedMessage : DeserializeOwned + Send + Sync +'static{
    const ID: u32;
    const MSG_TYPE: &'static str;
}
pub trait Msg  : Send + Sync {
    fn type_name(&self) -> &'static str ;
    fn type_id(&self) -> u32 ;
    fn json_serialize(&self) -> Result<Vec<u8>>;
    fn json_deserialize(v:&Vec<u8>) -> Result<Self> where Self : Sized;
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized;
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized;

    /// CBOR serialization (available with `cbor` feature flag).
    /// Default implementation delegates to JSON; override for compact CBOR encoding.
    #[cfg(feature = "cbor")]
    fn cbor_serialize(&self) -> Result<Vec<u8>> {
        self.json_serialize()
    }

    /// CBOR deserialization (available with `cbor` feature flag).
    /// Default implementation delegates to JSON; override for compact CBOR decoding.
    #[cfg(feature = "cbor")]
    fn cbor_deserialize(v: &Vec<u8>) -> Result<Self> where Self : Sized {
        Self::json_deserialize(v)
    }
}


#[derive(Debug, Clone, Serialize, Deserialize,)]
pub enum LogLevel {
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
    Alert,
}

#[derive(Debug, Clone, Serialize, Deserialize,)]
pub enum MessageType {
    SysCmd,
    SysInfo,
    WifiInfo,
    MotorInfo,
    MotorCmd,
}

#[derive(Debug, Clone, Serialize, Deserialize,)]
pub enum Toggle {
    Off,
    On,
}

#[derive(Debug, Clone, Serialize, Deserialize,)]
pub enum CtrlMod {
    Voltage,
    Speed,
    Torque,
}

#[derive(Debug, Clone, Serialize, Deserialize,)]
pub enum CtrlTyp {
    Commutation,
    Sinusoidal,
    Foc,
}

#[derive(Debug, Clone, Serialize, Deserialize,)]
pub enum InTyp {
    Disabled,
    NormalPot,
    MiddleRestingPot,
    AutoDetect,
}

#[derive(Debug, Clone, Serialize, Deserialize,)]
pub enum LawnmowerMode {
    Manual,
    Auto,
    Paused,
    EmergencyStop,
}



#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct Poly {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub typ: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub data: Option<Vec<u8>>,
}

impl TypedMessage for Poly {
    const ID: u32 = 30006;
    const MSG_TYPE: &'static str = "Poly";
}

impl Msg for Poly {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}

    #[cfg(feature = "cbor")]
    fn cbor_serialize(&self) -> Result<Vec<u8>> {Ok(serde_cbor::to_vec(self) ?)}
    #[cfg(feature = "cbor")]
    fn cbor_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_cbor::from_slice(v.as_slice()) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct UdpMessage {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dst: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub src: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub typ: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub msg: Option<serde_json::Value>,
}

impl TypedMessage for UdpMessage {
    const ID: u32 = 61718;
    const MSG_TYPE: &'static str = "UdpMessage";
}

impl Msg for UdpMessage {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct UdpMessageCbor {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dst: Option<u32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub src: Option<u32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub typ: Option<u32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub msg: Option<serde_json::Value>,
}

impl TypedMessage for UdpMessageCbor {
    const ID: u32 = 65322;
    const MSG_TYPE: &'static str = "UdpMessageCbor";
}

impl Msg for UdpMessageCbor {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}

    #[cfg(feature = "cbor")]
    fn cbor_serialize(&self) -> Result<Vec<u8>> {Ok(serde_cbor::to_vec(self) ?)}
    #[cfg(feature = "cbor")]
    fn cbor_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_cbor::from_slice(v.as_slice()) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct AliveEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub subscribes: Option<Vec<String>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub publishes: Option<Vec<String>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub services: Option<Vec<String>>,
}

impl TypedMessage for AliveEvent {
    const ID: u32 = 64775;
    const MSG_TYPE: &'static str = "AliveEvent";
}

impl Msg for AliveEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LogEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub level: Option<LogLevel>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_code: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub file: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub line: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub timestamp: Option<u64>,
}

impl TypedMessage for LogEvent {
    const ID: u32 = 29204;
    const MSG_TYPE: &'static str = "LogEvent";
}

impl Msg for LogEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct SysRequest {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub set_time: Option<u64>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub reboot: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub console: Option<String>,
}

impl TypedMessage for SysRequest {
    const ID: u32 = 54292;
    const MSG_TYPE: &'static str = "SysRequest";
}

impl Msg for SysRequest {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct SysReply {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rc: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
}

impl TypedMessage for SysReply {
    const ID: u32 = 23319;
    const MSG_TYPE: &'static str = "SysReply";
}

impl Msg for SysReply {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct SysEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub utc: Option<u64>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub uptime: Option<u64>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub free_heap: Option<u64>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub flash: Option<u64>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cpu_board: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub build_date: Option<String>,
}

impl TypedMessage for SysEvent {
    const ID: u32 = 23049;
    const MSG_TYPE: &'static str = "SysEvent";
}

impl Msg for SysEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct WifiEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ssid: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub bssid: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rssi: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ip: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub mac: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub channel: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub gateway: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub netmask: Option<String>,
}

impl TypedMessage for WifiEvent {
    const ID: u32 = 54881;
    const MSG_TYPE: &'static str = "WifiEvent";
}

impl Msg for WifiEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct MulticastEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub group: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub port: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub mtu: Option<u32>,
}

impl TypedMessage for MulticastEvent {
    const ID: u32 = 53788;
    const MSG_TYPE: &'static str = "MulticastEvent";
}

impl Msg for MulticastEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct PingRequest {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub number: Option<u32>,
}

impl TypedMessage for PingRequest {
    const ID: u32 = 35709;
    const MSG_TYPE: &'static str = "PingRequest";
}

impl Msg for PingRequest {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct PingReply {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub number: Option<u32>,
}

impl TypedMessage for PingReply {
    const ID: u32 = 62078;
    const MSG_TYPE: &'static str = "PingReply";
}

impl Msg for PingReply {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HoverboardEventRaw {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ctrl_mod: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ctrl_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cur_mot_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rpm_mot_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_ena: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_hi: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_lo: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub phase_adv_max_deg: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_raw: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_min: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_mid: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_cmd: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_raw: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_min: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_mid: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_cmd: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_raw: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_min: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_mid: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_cmd: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_raw: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_min: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_mid: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_cmd: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dc_curr: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rdc_curr: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ldc_curr: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cmdl: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cmdr: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spd_avg: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spdl: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spdr: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub filter_rate: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spd_coef: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub str_coef: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub batv: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temp: Option<i32>,
}

impl TypedMessage for HoverboardEventRaw {
    const ID: u32 = 16168;
    const MSG_TYPE: &'static str = "HoverboardEventRaw";
}

impl Msg for HoverboardEventRaw {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HoverboardEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ctrl_mod: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ctrl_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cur_mot_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rpm_mot_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_ena: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_hi: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_lo: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub phase_adv_max_deg: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_raw: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_min: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_mid: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_cmd: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_raw: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_min: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_mid: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_cmd: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_raw: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_min: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_mid: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_cmd: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_raw: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_typ: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_min: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_mid: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_max: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_cmd: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dc_curr: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rdc_curr: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ldc_curr: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cmdl: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cmdr: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spd_avg: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spdl: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spdr: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub filter_rate: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spd_coef: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub str_coef: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub batv: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temp: Option<f32>,
}

impl TypedMessage for HoverboardEvent {
    const ID: u32 = 31340;
    const MSG_TYPE: &'static str = "HoverboardEvent";
}

impl Msg for HoverboardEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HoverboardRequest {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub speed: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub steer: Option<i32>,
}

impl TypedMessage for HoverboardRequest {
    const ID: u32 = 63089;
    const MSG_TYPE: &'static str = "HoverboardRequest";
}

impl Msg for HoverboardRequest {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HoverboardReply {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_code: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
}

impl TypedMessage for HoverboardReply {
    const ID: u32 = 30066;
    const MSG_TYPE: &'static str = "HoverboardReply";
}

impl Msg for HoverboardReply {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct TouchPoint {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub active: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub id: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub x: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub y: Option<i32>,
}

impl TypedMessage for TouchPoint {
    const ID: u32 = 49173;
    const MSG_TYPE: &'static str = "TouchPoint";
}

impl Msg for TouchPoint {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct Ps4Event {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_left: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_right: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_up: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_down: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_square: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_cross: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_circle: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_triangle: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_left_shoulder: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_right_shoulder: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_left_trigger: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_right_trigger: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_left_joystick: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_right_joystick: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_share: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_options: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_touchpad: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_ps: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub axis_lx: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub axis_ly: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub axis_rx: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub axis_ry: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub gyro_x: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub gyro_y: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub gyro_z: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub accel_x: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub accel_y: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub accel_z: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub connected: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub battery_level: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub bluetooth: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub debug: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temp: Option<i32>,
}

impl TypedMessage for Ps4Event {
    const ID: u32 = 29767;
    const MSG_TYPE: &'static str = "Ps4Event";
}

impl Msg for Ps4Event {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct Ps4Request {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rumble_small: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rumble_large: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_red: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_green: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_blue: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_flash_on: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_flash_off: Option<i32>,
}

impl TypedMessage for Ps4Request {
    const ID: u32 = 5722;
    const MSG_TYPE: &'static str = "Ps4Request";
}

impl Msg for Ps4Request {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct CameraEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub width: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub height: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub format: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub data: Option<Vec<u8>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub quality: Option<i32>,
}

impl TypedMessage for CameraEvent {
    const ID: u32 = 32617;
    const MSG_TYPE: &'static str = "CameraEvent";
}

impl Msg for CameraEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct CameraRequest {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub capture_tcp_destination: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub quality: Option<i32>,
}

impl TypedMessage for CameraRequest {
    const ID: u32 = 63860;
    const MSG_TYPE: &'static str = "CameraRequest";
}

impl Msg for CameraRequest {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct CameraReply {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_code: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub data: Option<Vec<u8>>,
}

impl TypedMessage for CameraReply {
    const ID: u32 = 32887;
    const MSG_TYPE: &'static str = "CameraReply";
}

impl Msg for CameraReply {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerManualEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub speed: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub steering: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub blade: Option<bool>,
}

impl TypedMessage for LawnmowerManualEvent {
    const ID: u32 = 24124;
    const MSG_TYPE: &'static str = "LawnmowerManualEvent";
}

impl Msg for LawnmowerManualEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerManualRequest {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub speed: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub steer: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub blade: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub start_manual_control: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub stop_manual_control: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub emergency_stop: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub start_auto_mode: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub stop_auto_mode: Option<bool>,
}

impl TypedMessage for LawnmowerManualRequest {
    const ID: u32 = 47649;
    const MSG_TYPE: &'static str = "LawnmowerManualRequest";
}

impl Msg for LawnmowerManualRequest {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerManualReply {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_code: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
}

impl TypedMessage for LawnmowerManualReply {
    const ID: u32 = 22818;
    const MSG_TYPE: &'static str = "LawnmowerManualReply";
}

impl Msg for LawnmowerManualReply {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerAutoEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub started: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub stopped: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub paused: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub resumed: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub mode: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub path: Option<String>,
}

impl TypedMessage for LawnmowerAutoEvent {
    const ID: u32 = 58665;
    const MSG_TYPE: &'static str = "LawnmowerAutoEvent";
}

impl Msg for LawnmowerAutoEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerAutoRequest {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub start: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub stop: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub pause: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub resume: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub mode: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub path: Option<String>,
}

impl TypedMessage for LawnmowerAutoRequest {
    const ID: u32 = 24372;
    const MSG_TYPE: &'static str = "LawnmowerAutoRequest";
}

impl Msg for LawnmowerAutoRequest {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerStatus {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub battery_level: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub blade_status: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub current_mode: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_message: Option<String>,
}

impl TypedMessage for LawnmowerStatus {
    const ID: u32 = 21374;
    const MSG_TYPE: &'static str = "LawnmowerStatus";
}

impl Msg for LawnmowerStatus {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct MotorEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub motor_id: Option<i32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temperature: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub voltage: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub current: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub speed: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub position: Option<f32>,
}

impl TypedMessage for MotorEvent {
    const ID: u32 = 55067;
    const MSG_TYPE: &'static str = "MotorEvent";
}

impl Msg for MotorEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct Max31855Event {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub thermocouple_c: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub internal_c: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fault: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub open_circuit: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub short_to_gnd: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub short_to_vcc: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub timestamp_ms: Option<u64>,
}

impl TypedMessage for Max31855Event {
    const ID: u32 = 24126;
    const MSG_TYPE: &'static str = "Max31855Event";
}

impl Msg for Max31855Event {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HeatingRequest {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub setpoint_c: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub enabled: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub kp: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ki: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub kd: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub reset_integral: Option<bool>,
}

impl TypedMessage for HeatingRequest {
    const ID: u32 = 47125;
    const MSG_TYPE: &'static str = "HeatingRequest";
}

impl Msg for HeatingRequest {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HeatingEvent {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temperature_c: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub setpoint_c: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub output_pct: Option<f32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub enabled: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub heater_on: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fault: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub timestamp_ms: Option<u64>,
}

impl TypedMessage for HeatingEvent {
    const ID: u32 = 15368;
    const MSG_TYPE: &'static str = "HeatingEvent";
}

impl Msg for HeatingEvent {
    fn type_name(&self) -> &'static str {<Self as TypedMessage>::MSG_TYPE}
    fn type_id(&self) -> u32 {<Self as TypedMessage>::ID}
    fn json_serialize(&self) -> Result<Vec<u8>> {Ok(serde_json::to_vec(self) ?)}
    fn json_deserialize(v:& Vec<u8>) -> Result<Self> where Self : Sized {Ok(serde_json::from_slice(v.as_slice()) ?)}
    fn from_value(v:serde_json::Value) -> Result<Self> where Self : Sized {Ok(serde_json::from_value(v) ?)}
    fn to_value(&self) -> Result<serde_json::Value> where Self : Sized {Ok(serde_json::to_value(self) ?)}
}
    

