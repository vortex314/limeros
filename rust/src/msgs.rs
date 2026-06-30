use serde::{Serialize, Deserialize};
use anyhow::Result;



pub trait Msg {
    const ID: u32;
    const NAME: &'static str;
    fn to_bytes(&self) -> Result<Vec<u8>>;
    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized;
    fn to_readable(&self) -> String;
}


#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum LogLevel {
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
    Alert,
}


#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum MessageType {
    SysCmd,
    SysInfo,
    WifiInfo,
    MotorInfo,
    MotorCmd,
}


#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum Toggle {
    Off,
    On,
}


#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum CtrlMod {
    Voltage,
    Speed,
    Torque,
}


#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum CtrlTyp {
    Commutation,
    Sinusoidal,
    Foc,
}


#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum InTyp {
    Disabled,
    NormalPot,
    MiddleRestingPot,
    AutoDetect,
}


#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum LawnmowerMode {
    Manual,
    Auto,
    Paused,
    EmergencyStop,
}




#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct UdpMessage {
    #[serde(rename="3542317724")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dst: Option<u32>,
    #[serde(rename="3543982537")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub src: Option<u32>,
    #[serde(rename="694873664")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub msg_typ: Option<u32>,
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="856651685")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub payload: Option<Vec<u8>>,
}

impl Msg for UdpMessage {
    const ID: u32 = 1293877827;
    const NAME: &'static str = "UdpMessage";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.dst.as_ref().map(|v| { format!("  dst: {:?}", v) }),
            self.src.as_ref().map(|v| { format!("  src: {:?}", v) }),
            self.msg_typ.as_ref().map(|v| { format!("  msg_typ: {:?}", v) }),
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.payload.as_ref().map(|v| { format!("  payload: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct BrokerAnnouncement {
    #[serde(rename="3686535603")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub src_ip: Option<String>,
}

impl Msg for BrokerAnnouncement {
    const ID: u32 = 883525243;
    const NAME: &'static str = "BrokerAnnouncement";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.src_ip.as_ref().map(|v| { format!("  src_ip: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct DeviceAliveEvent {
    #[serde(rename="1815859273")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub device_id: Option<String>,
    #[serde(rename="2831556715")]
    pub objects: Vec<String>,
    #[serde(rename="3861851728")]
    pub subscribes: Vec<String>,
    #[serde(rename="2907205612")]
    pub publishes: Vec<String>,
    #[serde(rename="1722637011")]
    pub requests: Vec<String>,
    #[serde(rename="4257930943")]
    pub replies: Vec<String>,
}

impl Msg for DeviceAliveEvent {
    const ID: u32 = 2637772092;
    const NAME: &'static str = "DeviceAliveEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.device_id.as_ref().map(|v| { format!("  device_id: {:?}", v) }),
            (!self.objects.is_empty()).then(|| format!("  objects: {:?},", self.objects)),
            (!self.subscribes.is_empty()).then(|| format!("  subscribes: {:?},", self.subscribes)),
            (!self.publishes.is_empty()).then(|| format!("  publishes: {:?},", self.publishes)),
            (!self.requests.is_empty()).then(|| format!("  requests: {:?},", self.requests)),
            (!self.replies.is_empty()).then(|| format!("  replies: {:?},", self.replies)),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct BrokerSubscribeRequest {
    #[serde(rename="1070661349")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub src_str: Option<String>,
    #[serde(rename="3660214244")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dst_str: Option<String>,
    #[serde(rename="2773705352")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub msg_typ_str: Option<String>,
    #[serde(rename="4022087983")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub src_id: Option<u32>,
    #[serde(rename="3161019324")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dst_id: Option<u32>,
    #[serde(rename="1581777240")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub msg_typ_id: Option<u32>,
}

impl Msg for BrokerSubscribeRequest {
    const ID: u32 = 3190208493;
    const NAME: &'static str = "BrokerSubscribeRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.src_str.as_ref().map(|v| { format!("  src_str: {:?}", v) }),
            self.dst_str.as_ref().map(|v| { format!("  dst_str: {:?}", v) }),
            self.msg_typ_str.as_ref().map(|v| { format!("  msg_typ_str: {:?}", v) }),
            self.src_id.as_ref().map(|v| { format!("  src_id: {:?}", v) }),
            self.dst_id.as_ref().map(|v| { format!("  dst_id: {:?}", v) }),
            self.msg_typ_id.as_ref().map(|v| { format!("  msg_typ_id: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct BrokerUnsubscribeRequest {
    #[serde(rename="1070661349")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub src_str: Option<String>,
    #[serde(rename="3660214244")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dst_str: Option<String>,
    #[serde(rename="2773705352")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub msg_typ_str: Option<String>,
    #[serde(rename="4022087983")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub src_id: Option<u32>,
    #[serde(rename="3161019324")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dst_id: Option<u32>,
    #[serde(rename="1581777240")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub msg_typ_id: Option<u32>,
}

impl Msg for BrokerUnsubscribeRequest {
    const ID: u32 = 2677628270;
    const NAME: &'static str = "BrokerUnsubscribeRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.src_str.as_ref().map(|v| { format!("  src_str: {:?}", v) }),
            self.dst_str.as_ref().map(|v| { format!("  dst_str: {:?}", v) }),
            self.msg_typ_str.as_ref().map(|v| { format!("  msg_typ_str: {:?}", v) }),
            self.src_id.as_ref().map(|v| { format!("  src_id: {:?}", v) }),
            self.dst_id.as_ref().map(|v| { format!("  dst_id: {:?}", v) }),
            self.msg_typ_id.as_ref().map(|v| { format!("  msg_typ_id: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct BrokerSubscribeReply {
    #[serde(rename="1582595778")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rc: Option<i32>,
    #[serde(rename="619841764")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
}

impl Msg for BrokerSubscribeReply {
    const ID: u32 = 1412940656;
    const NAME: &'static str = "BrokerSubscribeReply";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.rc.as_ref().map(|v| { format!("  rc: {:?}", v) }),
            self.message.as_ref().map(|v| { format!("  message: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct BrokerEvent {
    #[serde(rename="3974112489")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub messages_routed: Option<u64>,
    #[serde(rename="1559764480")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub messages_dropped: Option<u64>,
    #[serde(rename="3202884639")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dictionary_size: Option<u32>,
    #[serde(rename="1942086824")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub subscription_count: Option<u32>,
    #[serde(rename="3933932640")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub endpoint_count: Option<u32>,
    #[serde(rename="1772062464")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub uptime_secs: Option<u64>,
}

impl Msg for BrokerEvent {
    const ID: u32 = 634317388;
    const NAME: &'static str = "BrokerEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.messages_routed.as_ref().map(|v| { format!("  messages_routed: {:?}", v) }),
            self.messages_dropped.as_ref().map(|v| { format!("  messages_dropped: {:?}", v) }),
            self.dictionary_size.as_ref().map(|v| { format!("  dictionary_size: {:?}", v) }),
            self.subscription_count.as_ref().map(|v| { format!("  subscription_count: {:?}", v) }),
            self.endpoint_count.as_ref().map(|v| { format!("  endpoint_count: {:?}", v) }),
            self.uptime_secs.as_ref().map(|v| { format!("  uptime_secs: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct BrokerRequest {
    #[serde(rename="3760613617")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub add_word: Option<String>,
}

impl Msg for BrokerRequest {
    const ID: u32 = 902291917;
    const NAME: &'static str = "BrokerRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.add_word.as_ref().map(|v| { format!("  add_word: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LogEvent {
    #[serde(rename="2610554845")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub level: Option<LogLevel>,
    #[serde(rename="619841764")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
    #[serde(rename="352063879")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_code: Option<i32>,
    #[serde(rename="2867484483")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub file: Option<String>,
    #[serde(rename="400234023")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub line: Option<i32>,
    #[serde(rename="2994984227")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub timestamp: Option<u64>,
}

impl Msg for LogEvent {
    const ID: u32 = 1963762699;
    const NAME: &'static str = "LogEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.level.as_ref().map(|v| { format!("  level: {:?}", v) }),
            self.message.as_ref().map(|v| { format!("  message: {:?}", v) }),
            self.error_code.as_ref().map(|v| { format!("  error_code: {:?}", v) }),
            self.file.as_ref().map(|v| { format!("  file: {:?}", v) }),
            self.line.as_ref().map(|v| { format!("  line: {:?}", v) }),
            self.timestamp.as_ref().map(|v| { format!("  timestamp: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct SysRequest {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="900236405")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub set_time: Option<u64>,
    #[serde(rename="2272436152")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub reboot: Option<bool>,
    #[serde(rename="775068458")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub console: Option<String>,
}

impl Msg for SysRequest {
    const ID: u32 = 2966412411;
    const NAME: &'static str = "SysRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.set_time.as_ref().map(|v| { format!("  set_time: {:?}", v) }),
            self.reboot.as_ref().map(|v| { format!("  reboot: {:?}", v) }),
            self.console.as_ref().map(|v| { format!("  console: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct SysReply {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="1582595778")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rc: Option<i32>,
    #[serde(rename="619841764")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
}

impl Msg for SysReply {
    const ID: u32 = 2952492394;
    const NAME: &'static str = "SysReply";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.rc.as_ref().map(|v| { format!("  rc: {:?}", v) }),
            self.message.as_ref().map(|v| { format!("  message: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct SysEvent {
    #[serde(rename="1033099933")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub utc: Option<u64>,
    #[serde(rename="4240833753")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub uptime: Option<u64>,
    #[serde(rename="3986257610")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub free_heap: Option<u64>,
    #[serde(rename="2944773417")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub flash: Option<u64>,
    #[serde(rename="3517703682")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cpu_board: Option<String>,
    #[serde(rename="2863606096")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub build_date: Option<String>,
}

impl Msg for SysEvent {
    const ID: u32 = 924742914;
    const NAME: &'static str = "SysEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.utc.as_ref().map(|v| { format!("  utc: {:?}", v) }),
            self.uptime.as_ref().map(|v| { format!("  uptime: {:?}", v) }),
            self.free_heap.as_ref().map(|v| { format!("  free_heap: {:?}", v) }),
            self.flash.as_ref().map(|v| { format!("  flash: {:?}", v) }),
            self.cpu_board.as_ref().map(|v| { format!("  cpu_board: {:?}", v) }),
            self.build_date.as_ref().map(|v| { format!("  build_date: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct WifiEvent {
    #[serde(rename="3291061806")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ssid: Option<String>,
    #[serde(rename="3223905102")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub bssid: Option<String>,
    #[serde(rename="2985115822")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rssi: Option<i32>,
    #[serde(rename="1261996636")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ip: Option<String>,
    #[serde(rename="3970106408")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub mac: Option<String>,
    #[serde(rename="566383268")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub channel: Option<i32>,
    #[serde(rename="3888792057")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub gateway: Option<String>,
    #[serde(rename="162736944")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub netmask: Option<String>,
}

impl Msg for WifiEvent {
    const ID: u32 = 3371536624;
    const NAME: &'static str = "WifiEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.ssid.as_ref().map(|v| { format!("  ssid: {:?}", v) }),
            self.bssid.as_ref().map(|v| { format!("  bssid: {:?}", v) }),
            self.rssi.as_ref().map(|v| { format!("  rssi: {:?}", v) }),
            self.ip.as_ref().map(|v| { format!("  ip: {:?}", v) }),
            self.mac.as_ref().map(|v| { format!("  mac: {:?}", v) }),
            self.channel.as_ref().map(|v| { format!("  channel: {:?}", v) }),
            self.gateway.as_ref().map(|v| { format!("  gateway: {:?}", v) }),
            self.netmask.as_ref().map(|v| { format!("  netmask: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct MulticastEvent {
    #[serde(rename="1605967500")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub group: Option<String>,
    #[serde(rename="2078544550")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub port: Option<i32>,
    #[serde(rename="3464659267")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub mtu: Option<u32>,
}

impl Msg for MulticastEvent {
    const ID: u32 = 579249307;
    const NAME: &'static str = "MulticastEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.group.as_ref().map(|v| { format!("  group: {:?}", v) }),
            self.port.as_ref().map(|v| { format!("  port: {:?}", v) }),
            self.mtu.as_ref().map(|v| { format!("  mtu: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct PingRequest {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="467038368")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub number: Option<u32>,
}

impl Msg for PingRequest {
    const ID: u32 = 31253678;
    const NAME: &'static str = "PingRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.number.as_ref().map(|v| { format!("  number: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct PingReply {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="467038368")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub number: Option<u32>,
}

impl Msg for PingReply {
    const ID: u32 = 1594103907;
    const NAME: &'static str = "PingReply";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.number.as_ref().map(|v| { format!("  number: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HoverboardEventRaw {
    #[serde(rename="3392182895")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ctrl_mod: Option<i32>,
    #[serde(rename="2385037750")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ctrl_typ: Option<i32>,
    #[serde(rename="1331200639")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cur_mot_max: Option<i32>,
    #[serde(rename="1758636612")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rpm_mot_max: Option<i32>,
    #[serde(rename="1232202040")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_ena: Option<i32>,
    #[serde(rename="1424660061")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_hi: Option<i32>,
    #[serde(rename="1861466535")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_lo: Option<i32>,
    #[serde(rename="4048649340")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_max: Option<i32>,
    #[serde(rename="1755172188")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub phase_adv_max_deg: Option<i32>,
    #[serde(rename="2937864083")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_raw: Option<i32>,
    #[serde(rename="1036041868")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_typ: Option<i32>,
    #[serde(rename="4254203333")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_min: Option<i32>,
    #[serde(rename="59901751")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_mid: Option<i32>,
    #[serde(rename="128292155")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_max: Option<i32>,
    #[serde(rename="52753209")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_cmd: Option<i32>,
    #[serde(rename="410488530")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_raw: Option<i32>,
    #[serde(rename="2312413913")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_typ: Option<i32>,
    #[serde(rename="2004308528")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_min: Option<i32>,
    #[serde(rename="2172084718")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_mid: Option<i32>,
    #[serde(rename="1835252410")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_max: Option<i32>,
    #[serde(rename="1105465644")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_cmd: Option<i32>,
    #[serde(rename="1476824232")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_raw: Option<i32>,
    #[serde(rename="1911254079")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_typ: Option<i32>,
    #[serde(rename="3150477658")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_min: Option<i32>,
    #[serde(rename="2982701468")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_mid: Option<i32>,
    #[serde(rename="2782649968")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_max: Option<i32>,
    #[serde(rename="3894824798")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_cmd: Option<i32>,
    #[serde(rename="3565524949")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_raw: Option<i32>,
    #[serde(rename="3177791518")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_typ: Option<i32>,
    #[serde(rename="637114339")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_min: Option<i32>,
    #[serde(rename="804890529")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_mid: Option<i32>,
    #[serde(rename="470618077")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_max: Option<i32>,
    #[serde(rename="2583074967")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_cmd: Option<i32>,
    #[serde(rename="36155229")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dc_curr: Option<i32>,
    #[serde(rename="4155600845")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rdc_curr: Option<i32>,
    #[serde(rename="3591431219")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ldc_curr: Option<i32>,
    #[serde(rename="2942290309")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cmdl: Option<i32>,
    #[serde(rename="3244287451")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cmdr: Option<i32>,
    #[serde(rename="1676374509")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spd_avg: Option<i32>,
    #[serde(rename="687126222")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spdl: Option<i32>,
    #[serde(rename="318018604")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spdr: Option<i32>,
    #[serde(rename="4218586042")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub filter_rate: Option<i32>,
    #[serde(rename="2563210242")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spd_coef: Option<i32>,
    #[serde(rename="3252853448")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub str_coef: Option<i32>,
    #[serde(rename="1321952890")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub batv: Option<i32>,
    #[serde(rename="3223044039")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temp: Option<i32>,
}

impl Msg for HoverboardEventRaw {
    const ID: u32 = 2949786301;
    const NAME: &'static str = "HoverboardEventRaw";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.ctrl_mod.as_ref().map(|v| { format!("  ctrl_mod: {:?}", v) }),
            self.ctrl_typ.as_ref().map(|v| { format!("  ctrl_typ: {:?}", v) }),
            self.cur_mot_max.as_ref().map(|v| { format!("  cur_mot_max: {:?}", v) }),
            self.rpm_mot_max.as_ref().map(|v| { format!("  rpm_mot_max: {:?}", v) }),
            self.fi_weak_ena.as_ref().map(|v| { format!("  fi_weak_ena: {:?}", v) }),
            self.fi_weak_hi.as_ref().map(|v| { format!("  fi_weak_hi: {:?}", v) }),
            self.fi_weak_lo.as_ref().map(|v| { format!("  fi_weak_lo: {:?}", v) }),
            self.fi_weak_max.as_ref().map(|v| { format!("  fi_weak_max: {:?}", v) }),
            self.phase_adv_max_deg.as_ref().map(|v| { format!("  phase_adv_max_deg: {:?}", v) }),
            self.input1_raw.as_ref().map(|v| { format!("  input1_raw: {:?}", v) }),
            self.input1_typ.as_ref().map(|v| { format!("  input1_typ: {:?}", v) }),
            self.input1_min.as_ref().map(|v| { format!("  input1_min: {:?}", v) }),
            self.input1_mid.as_ref().map(|v| { format!("  input1_mid: {:?}", v) }),
            self.input1_max.as_ref().map(|v| { format!("  input1_max: {:?}", v) }),
            self.input1_cmd.as_ref().map(|v| { format!("  input1_cmd: {:?}", v) }),
            self.input2_raw.as_ref().map(|v| { format!("  input2_raw: {:?}", v) }),
            self.input2_typ.as_ref().map(|v| { format!("  input2_typ: {:?}", v) }),
            self.input2_min.as_ref().map(|v| { format!("  input2_min: {:?}", v) }),
            self.input2_mid.as_ref().map(|v| { format!("  input2_mid: {:?}", v) }),
            self.input2_max.as_ref().map(|v| { format!("  input2_max: {:?}", v) }),
            self.input2_cmd.as_ref().map(|v| { format!("  input2_cmd: {:?}", v) }),
            self.aux_input1_raw.as_ref().map(|v| { format!("  aux_input1_raw: {:?}", v) }),
            self.aux_input1_typ.as_ref().map(|v| { format!("  aux_input1_typ: {:?}", v) }),
            self.aux_input1_min.as_ref().map(|v| { format!("  aux_input1_min: {:?}", v) }),
            self.aux_input1_mid.as_ref().map(|v| { format!("  aux_input1_mid: {:?}", v) }),
            self.aux_input1_max.as_ref().map(|v| { format!("  aux_input1_max: {:?}", v) }),
            self.aux_input1_cmd.as_ref().map(|v| { format!("  aux_input1_cmd: {:?}", v) }),
            self.aux_input2_raw.as_ref().map(|v| { format!("  aux_input2_raw: {:?}", v) }),
            self.aux_input2_typ.as_ref().map(|v| { format!("  aux_input2_typ: {:?}", v) }),
            self.aux_input2_min.as_ref().map(|v| { format!("  aux_input2_min: {:?}", v) }),
            self.aux_input2_mid.as_ref().map(|v| { format!("  aux_input2_mid: {:?}", v) }),
            self.aux_input2_max.as_ref().map(|v| { format!("  aux_input2_max: {:?}", v) }),
            self.aux_input2_cmd.as_ref().map(|v| { format!("  aux_input2_cmd: {:?}", v) }),
            self.dc_curr.as_ref().map(|v| { format!("  dc_curr: {:?}", v) }),
            self.rdc_curr.as_ref().map(|v| { format!("  rdc_curr: {:?}", v) }),
            self.ldc_curr.as_ref().map(|v| { format!("  ldc_curr: {:?}", v) }),
            self.cmdl.as_ref().map(|v| { format!("  cmdl: {:?}", v) }),
            self.cmdr.as_ref().map(|v| { format!("  cmdr: {:?}", v) }),
            self.spd_avg.as_ref().map(|v| { format!("  spd_avg: {:?}", v) }),
            self.spdl.as_ref().map(|v| { format!("  spdl: {:?}", v) }),
            self.spdr.as_ref().map(|v| { format!("  spdr: {:?}", v) }),
            self.filter_rate.as_ref().map(|v| { format!("  filter_rate: {:?}", v) }),
            self.spd_coef.as_ref().map(|v| { format!("  spd_coef: {:?}", v) }),
            self.str_coef.as_ref().map(|v| { format!("  str_coef: {:?}", v) }),
            self.batv.as_ref().map(|v| { format!("  batv: {:?}", v) }),
            self.temp.as_ref().map(|v| { format!("  temp: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HoverboardEvent {
    #[serde(rename="3392182895")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ctrl_mod: Option<i32>,
    #[serde(rename="2385037750")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ctrl_typ: Option<i32>,
    #[serde(rename="1331200639")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cur_mot_max: Option<i32>,
    #[serde(rename="1758636612")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rpm_mot_max: Option<i32>,
    #[serde(rename="1232202040")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_ena: Option<i32>,
    #[serde(rename="1424660061")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_hi: Option<i32>,
    #[serde(rename="1861466535")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_lo: Option<i32>,
    #[serde(rename="4048649340")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fi_weak_max: Option<i32>,
    #[serde(rename="1755172188")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub phase_adv_max_deg: Option<i32>,
    #[serde(rename="2937864083")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_raw: Option<i32>,
    #[serde(rename="1036041868")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_typ: Option<i32>,
    #[serde(rename="4254203333")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_min: Option<i32>,
    #[serde(rename="59901751")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_mid: Option<i32>,
    #[serde(rename="128292155")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_max: Option<i32>,
    #[serde(rename="52753209")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input1_cmd: Option<i32>,
    #[serde(rename="410488530")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_raw: Option<i32>,
    #[serde(rename="2312413913")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_typ: Option<i32>,
    #[serde(rename="2004308528")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_min: Option<i32>,
    #[serde(rename="2172084718")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_mid: Option<i32>,
    #[serde(rename="1835252410")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_max: Option<i32>,
    #[serde(rename="1105465644")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub input2_cmd: Option<i32>,
    #[serde(rename="1476824232")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_raw: Option<i32>,
    #[serde(rename="1911254079")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_typ: Option<i32>,
    #[serde(rename="3150477658")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_min: Option<i32>,
    #[serde(rename="2982701468")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_mid: Option<i32>,
    #[serde(rename="2782649968")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_max: Option<i32>,
    #[serde(rename="3894824798")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input1_cmd: Option<i32>,
    #[serde(rename="3565524949")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_raw: Option<i32>,
    #[serde(rename="3177791518")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_typ: Option<i32>,
    #[serde(rename="637114339")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_min: Option<i32>,
    #[serde(rename="804890529")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_mid: Option<i32>,
    #[serde(rename="470618077")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_max: Option<i32>,
    #[serde(rename="2583074967")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aux_input2_cmd: Option<i32>,
    #[serde(rename="36155229")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub dc_curr: Option<f32>,
    #[serde(rename="4155600845")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rdc_curr: Option<f32>,
    #[serde(rename="3591431219")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ldc_curr: Option<f32>,
    #[serde(rename="2942290309")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cmdl: Option<i32>,
    #[serde(rename="3244287451")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cmdr: Option<i32>,
    #[serde(rename="1676374509")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spd_avg: Option<i32>,
    #[serde(rename="687126222")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spdl: Option<i32>,
    #[serde(rename="318018604")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spdr: Option<i32>,
    #[serde(rename="4218586042")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub filter_rate: Option<i32>,
    #[serde(rename="2563210242")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub spd_coef: Option<i32>,
    #[serde(rename="3252853448")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub str_coef: Option<i32>,
    #[serde(rename="1321952890")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub batv: Option<f32>,
    #[serde(rename="3223044039")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temp: Option<f32>,
}

impl Msg for HoverboardEvent {
    const ID: u32 = 104988481;
    const NAME: &'static str = "HoverboardEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.ctrl_mod.as_ref().map(|v| { format!("  ctrl_mod: {:?}", v) }),
            self.ctrl_typ.as_ref().map(|v| { format!("  ctrl_typ: {:?}", v) }),
            self.cur_mot_max.as_ref().map(|v| { format!("  cur_mot_max: {:?}", v) }),
            self.rpm_mot_max.as_ref().map(|v| { format!("  rpm_mot_max: {:?}", v) }),
            self.fi_weak_ena.as_ref().map(|v| { format!("  fi_weak_ena: {:?}", v) }),
            self.fi_weak_hi.as_ref().map(|v| { format!("  fi_weak_hi: {:?}", v) }),
            self.fi_weak_lo.as_ref().map(|v| { format!("  fi_weak_lo: {:?}", v) }),
            self.fi_weak_max.as_ref().map(|v| { format!("  fi_weak_max: {:?}", v) }),
            self.phase_adv_max_deg.as_ref().map(|v| { format!("  phase_adv_max_deg: {:?}", v) }),
            self.input1_raw.as_ref().map(|v| { format!("  input1_raw: {:?}", v) }),
            self.input1_typ.as_ref().map(|v| { format!("  input1_typ: {:?}", v) }),
            self.input1_min.as_ref().map(|v| { format!("  input1_min: {:?}", v) }),
            self.input1_mid.as_ref().map(|v| { format!("  input1_mid: {:?}", v) }),
            self.input1_max.as_ref().map(|v| { format!("  input1_max: {:?}", v) }),
            self.input1_cmd.as_ref().map(|v| { format!("  input1_cmd: {:?}", v) }),
            self.input2_raw.as_ref().map(|v| { format!("  input2_raw: {:?}", v) }),
            self.input2_typ.as_ref().map(|v| { format!("  input2_typ: {:?}", v) }),
            self.input2_min.as_ref().map(|v| { format!("  input2_min: {:?}", v) }),
            self.input2_mid.as_ref().map(|v| { format!("  input2_mid: {:?}", v) }),
            self.input2_max.as_ref().map(|v| { format!("  input2_max: {:?}", v) }),
            self.input2_cmd.as_ref().map(|v| { format!("  input2_cmd: {:?}", v) }),
            self.aux_input1_raw.as_ref().map(|v| { format!("  aux_input1_raw: {:?}", v) }),
            self.aux_input1_typ.as_ref().map(|v| { format!("  aux_input1_typ: {:?}", v) }),
            self.aux_input1_min.as_ref().map(|v| { format!("  aux_input1_min: {:?}", v) }),
            self.aux_input1_mid.as_ref().map(|v| { format!("  aux_input1_mid: {:?}", v) }),
            self.aux_input1_max.as_ref().map(|v| { format!("  aux_input1_max: {:?}", v) }),
            self.aux_input1_cmd.as_ref().map(|v| { format!("  aux_input1_cmd: {:?}", v) }),
            self.aux_input2_raw.as_ref().map(|v| { format!("  aux_input2_raw: {:?}", v) }),
            self.aux_input2_typ.as_ref().map(|v| { format!("  aux_input2_typ: {:?}", v) }),
            self.aux_input2_min.as_ref().map(|v| { format!("  aux_input2_min: {:?}", v) }),
            self.aux_input2_mid.as_ref().map(|v| { format!("  aux_input2_mid: {:?}", v) }),
            self.aux_input2_max.as_ref().map(|v| { format!("  aux_input2_max: {:?}", v) }),
            self.aux_input2_cmd.as_ref().map(|v| { format!("  aux_input2_cmd: {:?}", v) }),
            self.dc_curr.as_ref().map(|v| { format!("  dc_curr: {:?}", v) }),
            self.rdc_curr.as_ref().map(|v| { format!("  rdc_curr: {:?}", v) }),
            self.ldc_curr.as_ref().map(|v| { format!("  ldc_curr: {:?}", v) }),
            self.cmdl.as_ref().map(|v| { format!("  cmdl: {:?}", v) }),
            self.cmdr.as_ref().map(|v| { format!("  cmdr: {:?}", v) }),
            self.spd_avg.as_ref().map(|v| { format!("  spd_avg: {:?}", v) }),
            self.spdl.as_ref().map(|v| { format!("  spdl: {:?}", v) }),
            self.spdr.as_ref().map(|v| { format!("  spdr: {:?}", v) }),
            self.filter_rate.as_ref().map(|v| { format!("  filter_rate: {:?}", v) }),
            self.spd_coef.as_ref().map(|v| { format!("  spd_coef: {:?}", v) }),
            self.str_coef.as_ref().map(|v| { format!("  str_coef: {:?}", v) }),
            self.batv.as_ref().map(|v| { format!("  batv: {:?}", v) }),
            self.temp.as_ref().map(|v| { format!("  temp: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HoverboardRequest {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="2072037248")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub speed: Option<i32>,
    #[serde(rename="318877302")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub steer: Option<i32>,
}

impl Msg for HoverboardRequest {
    const ID: u32 = 2735870956;
    const NAME: &'static str = "HoverboardRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.speed.as_ref().map(|v| { format!("  speed: {:?}", v) }),
            self.steer.as_ref().map(|v| { format!("  steer: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HoverboardReply {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="352063879")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_code: Option<i32>,
    #[serde(rename="619841764")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
}

impl Msg for HoverboardReply {
    const ID: u32 = 2095960949;
    const NAME: &'static str = "HoverboardReply";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.error_code.as_ref().map(|v| { format!("  error_code: {:?}", v) }),
            self.message.as_ref().map(|v| { format!("  message: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct TouchPoint {
    #[serde(rename="3648362799")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub active: Option<bool>,
    #[serde(rename="926444256")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub id: Option<i32>,
    #[serde(rename="4245442695")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub x: Option<i32>,
    #[serde(rename="4228665076")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub y: Option<i32>,
}

impl Msg for TouchPoint {
    const ID: u32 = 3463201870;
    const NAME: &'static str = "TouchPoint";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.active.as_ref().map(|v| { format!("  active: {:?}", v) }),
            self.id.as_ref().map(|v| { format!("  id: {:?}", v) }),
            self.x.as_ref().map(|v| { format!("  x: {:?}", v) }),
            self.y.as_ref().map(|v| { format!("  y: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct Ps4Event {
    #[serde(rename="1747553511")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_left: Option<bool>,
    #[serde(rename="3439317144")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_right: Option<bool>,
    #[serde(rename="4011736455")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_up: Option<bool>,
    #[serde(rename="2887647170")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_down: Option<bool>,
    #[serde(rename="2698463033")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_square: Option<bool>,
    #[serde(rename="3330582218")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_cross: Option<bool>,
    #[serde(rename="3154770978")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_circle: Option<bool>,
    #[serde(rename="770341826")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_triangle: Option<bool>,
    #[serde(rename="3968804096")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_left_shoulder: Option<bool>,
    #[serde(rename="3901035901")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_right_shoulder: Option<bool>,
    #[serde(rename="3934254276")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_left_trigger: Option<bool>,
    #[serde(rename="4042356867")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_right_trigger: Option<bool>,
    #[serde(rename="790015118")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_left_joystick: Option<bool>,
    #[serde(rename="1414428719")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_right_joystick: Option<bool>,
    #[serde(rename="2890681153")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_share: Option<bool>,
    #[serde(rename="945736196")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_options: Option<bool>,
    #[serde(rename="907838114")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_touchpad: Option<bool>,
    #[serde(rename="4027675431")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub button_ps: Option<bool>,
    #[serde(rename="944310989")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub axis_lx: Option<i32>,
    #[serde(rename="927533370")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub axis_ly: Option<i32>,
    #[serde(rename="738742419")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub axis_rx: Option<i32>,
    #[serde(rename="721964800")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub axis_ry: Option<i32>,
    #[serde(rename="22698073")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub gyro_x: Option<i32>,
    #[serde(rename="5920454")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub gyro_y: Option<i32>,
    #[serde(rename="4284110131")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub gyro_z: Option<i32>,
    #[serde(rename="1062702548")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub accel_x: Option<i32>,
    #[serde(rename="1079480167")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub accel_y: Option<i32>,
    #[serde(rename="1096257786")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub accel_z: Option<i32>,
    #[serde(rename="1424938192")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub connected: Option<bool>,
    #[serde(rename="523888211")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub battery_level: Option<i32>,
    #[serde(rename="1682838045")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub bluetooth: Option<bool>,
    #[serde(rename="1483009432")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub debug: Option<String>,
    #[serde(rename="3223044039")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temp: Option<i32>,
}

impl Msg for Ps4Event {
    const ID: u32 = 4282593576;
    const NAME: &'static str = "Ps4Event";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.button_left.as_ref().map(|v| { format!("  button_left: {:?}", v) }),
            self.button_right.as_ref().map(|v| { format!("  button_right: {:?}", v) }),
            self.button_up.as_ref().map(|v| { format!("  button_up: {:?}", v) }),
            self.button_down.as_ref().map(|v| { format!("  button_down: {:?}", v) }),
            self.button_square.as_ref().map(|v| { format!("  button_square: {:?}", v) }),
            self.button_cross.as_ref().map(|v| { format!("  button_cross: {:?}", v) }),
            self.button_circle.as_ref().map(|v| { format!("  button_circle: {:?}", v) }),
            self.button_triangle.as_ref().map(|v| { format!("  button_triangle: {:?}", v) }),
            self.button_left_shoulder.as_ref().map(|v| { format!("  button_left_shoulder: {:?}", v) }),
            self.button_right_shoulder.as_ref().map(|v| { format!("  button_right_shoulder: {:?}", v) }),
            self.button_left_trigger.as_ref().map(|v| { format!("  button_left_trigger: {:?}", v) }),
            self.button_right_trigger.as_ref().map(|v| { format!("  button_right_trigger: {:?}", v) }),
            self.button_left_joystick.as_ref().map(|v| { format!("  button_left_joystick: {:?}", v) }),
            self.button_right_joystick.as_ref().map(|v| { format!("  button_right_joystick: {:?}", v) }),
            self.button_share.as_ref().map(|v| { format!("  button_share: {:?}", v) }),
            self.button_options.as_ref().map(|v| { format!("  button_options: {:?}", v) }),
            self.button_touchpad.as_ref().map(|v| { format!("  button_touchpad: {:?}", v) }),
            self.button_ps.as_ref().map(|v| { format!("  button_ps: {:?}", v) }),
            self.axis_lx.as_ref().map(|v| { format!("  axis_lx: {:?}", v) }),
            self.axis_ly.as_ref().map(|v| { format!("  axis_ly: {:?}", v) }),
            self.axis_rx.as_ref().map(|v| { format!("  axis_rx: {:?}", v) }),
            self.axis_ry.as_ref().map(|v| { format!("  axis_ry: {:?}", v) }),
            self.gyro_x.as_ref().map(|v| { format!("  gyro_x: {:?}", v) }),
            self.gyro_y.as_ref().map(|v| { format!("  gyro_y: {:?}", v) }),
            self.gyro_z.as_ref().map(|v| { format!("  gyro_z: {:?}", v) }),
            self.accel_x.as_ref().map(|v| { format!("  accel_x: {:?}", v) }),
            self.accel_y.as_ref().map(|v| { format!("  accel_y: {:?}", v) }),
            self.accel_z.as_ref().map(|v| { format!("  accel_z: {:?}", v) }),
            self.connected.as_ref().map(|v| { format!("  connected: {:?}", v) }),
            self.battery_level.as_ref().map(|v| { format!("  battery_level: {:?}", v) }),
            self.bluetooth.as_ref().map(|v| { format!("  bluetooth: {:?}", v) }),
            self.debug.as_ref().map(|v| { format!("  debug: {:?}", v) }),
            self.temp.as_ref().map(|v| { format!("  temp: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct Ps4Request {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="3134091592")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rumble_small: Option<i32>,
    #[serde(rename="2673761152")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub rumble_large: Option<i32>,
    #[serde(rename="1563830470")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_red: Option<i32>,
    #[serde(rename="2295245222")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_green: Option<i32>,
    #[serde(rename="797885595")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_blue: Option<i32>,
    #[serde(rename="2033544681")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_flash_on: Option<i32>,
    #[serde(rename="1311799717")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led_flash_off: Option<i32>,
}

impl Msg for Ps4Request {
    const ID: u32 = 1992038561;
    const NAME: &'static str = "Ps4Request";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.rumble_small.as_ref().map(|v| { format!("  rumble_small: {:?}", v) }),
            self.rumble_large.as_ref().map(|v| { format!("  rumble_large: {:?}", v) }),
            self.led_red.as_ref().map(|v| { format!("  led_red: {:?}", v) }),
            self.led_green.as_ref().map(|v| { format!("  led_green: {:?}", v) }),
            self.led_blue.as_ref().map(|v| { format!("  led_blue: {:?}", v) }),
            self.led_flash_on.as_ref().map(|v| { format!("  led_flash_on: {:?}", v) }),
            self.led_flash_off.as_ref().map(|v| { format!("  led_flash_off: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct CameraEvent {
    #[serde(rename="2508680735")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub width: Option<i32>,
    #[serde(rename="3585981250")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub height: Option<i32>,
    #[serde(rename="3114108242")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub format: Option<String>,
    #[serde(rename="3631407781")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub data: Option<Vec<u8>>,
    #[serde(rename="1080748746")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led: Option<bool>,
    #[serde(rename="2597670950")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub quality: Option<i32>,
}

impl Msg for CameraEvent {
    const ID: u32 = 2485305690;
    const NAME: &'static str = "CameraEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.width.as_ref().map(|v| { format!("  width: {:?}", v) }),
            self.height.as_ref().map(|v| { format!("  height: {:?}", v) }),
            self.format.as_ref().map(|v| { format!("  format: {:?}", v) }),
            self.data.as_ref().map(|v| { format!("  data: {:?}", v) }),
            self.led.as_ref().map(|v| { format!("  led: {:?}", v) }),
            self.quality.as_ref().map(|v| { format!("  quality: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct CameraRequest {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="1080748746")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub led: Option<bool>,
    #[serde(rename="2458837278")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub capture_tcp_destination: Option<String>,
    #[serde(rename="2597670950")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub quality: Option<i32>,
}

impl Msg for CameraRequest {
    const ID: u32 = 3491086723;
    const NAME: &'static str = "CameraRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.led.as_ref().map(|v| { format!("  led: {:?}", v) }),
            self.capture_tcp_destination.as_ref().map(|v| { format!("  capture_tcp_destination: {:?}", v) }),
            self.quality.as_ref().map(|v| { format!("  quality: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct CameraReply {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="352063879")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_code: Option<i32>,
    #[serde(rename="619841764")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
    #[serde(rename="3631407781")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub data: Option<Vec<u8>>,
}

impl Msg for CameraReply {
    const ID: u32 = 4038246066;
    const NAME: &'static str = "CameraReply";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.error_code.as_ref().map(|v| { format!("  error_code: {:?}", v) }),
            self.message.as_ref().map(|v| { format!("  message: {:?}", v) }),
            self.data.as_ref().map(|v| { format!("  data: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerManualEvent {
    #[serde(rename="2072037248")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub speed: Option<i32>,
    #[serde(rename="3635427802")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub steering: Option<i32>,
    #[serde(rename="1552352849")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub blade: Option<bool>,
}

impl Msg for LawnmowerManualEvent {
    const ID: u32 = 3961874191;
    const NAME: &'static str = "LawnmowerManualEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.speed.as_ref().map(|v| { format!("  speed: {:?}", v) }),
            self.steering.as_ref().map(|v| { format!("  steering: {:?}", v) }),
            self.blade.as_ref().map(|v| { format!("  blade: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerManualRequest {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="2072037248")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub speed: Option<f32>,
    #[serde(rename="318877302")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub steer: Option<f32>,
    #[serde(rename="1552352849")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub blade: Option<bool>,
    #[serde(rename="3226366932")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub start_manual_control: Option<bool>,
    #[serde(rename="3612618358")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub stop_manual_control: Option<bool>,
    #[serde(rename="1921368063")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub emergency_stop: Option<bool>,
    #[serde(rename="751408827")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub start_auto_mode: Option<bool>,
    #[serde(rename="3579739313")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub stop_auto_mode: Option<bool>,
}

impl Msg for LawnmowerManualRequest {
    const ID: u32 = 3783098050;
    const NAME: &'static str = "LawnmowerManualRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.speed.as_ref().map(|v| { format!("  speed: {:?}", v) }),
            self.steer.as_ref().map(|v| { format!("  steer: {:?}", v) }),
            self.blade.as_ref().map(|v| { format!("  blade: {:?}", v) }),
            self.start_manual_control.as_ref().map(|v| { format!("  start_manual_control: {:?}", v) }),
            self.stop_manual_control.as_ref().map(|v| { format!("  stop_manual_control: {:?}", v) }),
            self.emergency_stop.as_ref().map(|v| { format!("  emergency_stop: {:?}", v) }),
            self.start_auto_mode.as_ref().map(|v| { format!("  start_auto_mode: {:?}", v) }),
            self.stop_auto_mode.as_ref().map(|v| { format!("  stop_auto_mode: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerManualReply {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="352063879")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_code: Option<i32>,
    #[serde(rename="619841764")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub message: Option<String>,
}

impl Msg for LawnmowerManualReply {
    const ID: u32 = 1558853623;
    const NAME: &'static str = "LawnmowerManualReply";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.error_code.as_ref().map(|v| { format!("  error_code: {:?}", v) }),
            self.message.as_ref().map(|v| { format!("  message: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerAutoEvent {
    #[serde(rename="2153339806")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub started: Option<bool>,
    #[serde(rename="1860367182")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub stopped: Option<bool>,
    #[serde(rename="169449419")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub paused: Option<bool>,
    #[serde(rename="3585847050")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub resumed: Option<bool>,
    #[serde(rename="3966689298")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub mode: Option<String>,
    #[serde(rename="2223459638")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub path: Option<String>,
}

impl Msg for LawnmowerAutoEvent {
    const ID: u32 = 3727910832;
    const NAME: &'static str = "LawnmowerAutoEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.started.as_ref().map(|v| { format!("  started: {:?}", v) }),
            self.stopped.as_ref().map(|v| { format!("  stopped: {:?}", v) }),
            self.paused.as_ref().map(|v| { format!("  paused: {:?}", v) }),
            self.resumed.as_ref().map(|v| { format!("  resumed: {:?}", v) }),
            self.mode.as_ref().map(|v| { format!("  mode: {:?}", v) }),
            self.path.as_ref().map(|v| { format!("  path: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerAutoRequest {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="1697318111")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub start: Option<bool>,
    #[serde(rename="3411225317")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub stop: Option<bool>,
    #[serde(rename="1887753101")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub pause: Option<bool>,
    #[serde(rename="4281967466")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub resume: Option<bool>,
    #[serde(rename="3966689298")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub mode: Option<String>,
    #[serde(rename="2223459638")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub path: Option<String>,
}

impl Msg for LawnmowerAutoRequest {
    const ID: u32 = 3118861241;
    const NAME: &'static str = "LawnmowerAutoRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.start.as_ref().map(|v| { format!("  start: {:?}", v) }),
            self.stop.as_ref().map(|v| { format!("  stop: {:?}", v) }),
            self.pause.as_ref().map(|v| { format!("  pause: {:?}", v) }),
            self.resume.as_ref().map(|v| { format!("  resume: {:?}", v) }),
            self.mode.as_ref().map(|v| { format!("  mode: {:?}", v) }),
            self.path.as_ref().map(|v| { format!("  path: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LawnmowerStatus {
    #[serde(rename="523888211")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub battery_level: Option<i32>,
    #[serde(rename="1913274388")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub blade_status: Option<bool>,
    #[serde(rename="1996314776")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub current_mode: Option<String>,
    #[serde(rename="977701329")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub error_message: Option<String>,
}

impl Msg for LawnmowerStatus {
    const ID: u32 = 3070837701;
    const NAME: &'static str = "LawnmowerStatus";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.battery_level.as_ref().map(|v| { format!("  battery_level: {:?}", v) }),
            self.blade_status.as_ref().map(|v| { format!("  blade_status: {:?}", v) }),
            self.current_mode.as_ref().map(|v| { format!("  current_mode: {:?}", v) }),
            self.error_message.as_ref().map(|v| { format!("  error_message: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct MotorEvent {
    #[serde(rename="4111653210")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub motor_id: Option<i32>,
    #[serde(rename="3924994357")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temperature: Option<f32>,
    #[serde(rename="3784531813")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub voltage: Option<f32>,
    #[serde(rename="3647323098")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub current: Option<f32>,
    #[serde(rename="2072037248")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub speed: Option<f32>,
    #[serde(rename="2471448074")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub position: Option<f32>,
}

impl Msg for MotorEvent {
    const ID: u32 = 2506002638;
    const NAME: &'static str = "MotorEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.motor_id.as_ref().map(|v| { format!("  motor_id: {:?}", v) }),
            self.temperature.as_ref().map(|v| { format!("  temperature: {:?}", v) }),
            self.voltage.as_ref().map(|v| { format!("  voltage: {:?}", v) }),
            self.current.as_ref().map(|v| { format!("  current: {:?}", v) }),
            self.speed.as_ref().map(|v| { format!("  speed: {:?}", v) }),
            self.position.as_ref().map(|v| { format!("  position: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct Max31855Event {
    #[serde(rename="4285487460")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub thermocouple_c: Option<f32>,
    #[serde(rename="2410396330")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub internal_c: Option<f32>,
    #[serde(rename="2936529791")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fault: Option<bool>,
    #[serde(rename="3167585005")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub open_circuit: Option<bool>,
    #[serde(rename="2345477587")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub short_to_gnd: Option<bool>,
    #[serde(rename="2218820948")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub short_to_vcc: Option<bool>,
    #[serde(rename="464087480")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub timestamp_ms: Option<u64>,
}

impl Msg for Max31855Event {
    const ID: u32 = 2831607083;
    const NAME: &'static str = "Max31855Event";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.thermocouple_c.as_ref().map(|v| { format!("  thermocouple_c: {:?}", v) }),
            self.internal_c.as_ref().map(|v| { format!("  internal_c: {:?}", v) }),
            self.fault.as_ref().map(|v| { format!("  fault: {:?}", v) }),
            self.open_circuit.as_ref().map(|v| { format!("  open_circuit: {:?}", v) }),
            self.short_to_gnd.as_ref().map(|v| { format!("  short_to_gnd: {:?}", v) }),
            self.short_to_vcc.as_ref().map(|v| { format!("  short_to_vcc: {:?}", v) }),
            self.timestamp_ms.as_ref().map(|v| { format!("  timestamp_ms: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HeatingRequest {
    #[serde(rename="400946249")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub req_id: Option<u32>,
    #[serde(rename="1690578701")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub setpoint_c: Option<f32>,
    #[serde(rename="49525662")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub enabled: Option<bool>,
    #[serde(rename="1463622254")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub kp: Option<f32>,
    #[serde(rename="1077737017")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ki: Option<f32>,
    #[serde(rename="1262290826")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub kd: Option<f32>,
    #[serde(rename="314866981")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub reset_integral: Option<bool>,
}

impl Msg for HeatingRequest {
    const ID: u32 = 578653874;
    const NAME: &'static str = "HeatingRequest";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.req_id.as_ref().map(|v| { format!("  req_id: {:?}", v) }),
            self.setpoint_c.as_ref().map(|v| { format!("  setpoint_c: {:?}", v) }),
            self.enabled.as_ref().map(|v| { format!("  enabled: {:?}", v) }),
            self.kp.as_ref().map(|v| { format!("  kp: {:?}", v) }),
            self.ki.as_ref().map(|v| { format!("  ki: {:?}", v) }),
            self.kd.as_ref().map(|v| { format!("  kd: {:?}", v) }),
            self.reset_integral.as_ref().map(|v| { format!("  reset_integral: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}


#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct HeatingEvent {
    #[serde(rename="2266483847")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub temperature_c: Option<f32>,
    #[serde(rename="1690578701")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub setpoint_c: Option<f32>,
    #[serde(rename="642138204")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub output_pct: Option<f32>,
    #[serde(rename="49525662")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub enabled: Option<bool>,
    #[serde(rename="2096980360")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub heater_on: Option<bool>,
    #[serde(rename="2936529791")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fault: Option<bool>,
    #[serde(rename="464087480")]
    #[serde(skip_serializing_if = "Option::is_none")]
    pub timestamp_ms: Option<u64>,
}

impl Msg for HeatingEvent {
    const ID: u32 = 461737375;
    const NAME: &'static str = "HeatingEvent";

    fn to_bytes(&self) -> Result<Vec<u8>> {
        // Use serde_cbor to produce CBOR bytes
        let bytes = cbor2::to_vec(self)?;
        Ok(bytes)
    }   

    fn from_bytes(v:&Vec<u8>) -> Result<Self> where Self : Sized {
        // Use serde_cbor to parse CBOR bytes
        let m = cbor2::from_slice(v.as_slice())?;
        Ok(m)
    }

    fn to_readable(&self) -> String {
        let fields: Vec<String> = [
            self.temperature_c.as_ref().map(|v| { format!("  temperature_c: {:?}", v) }),
            self.setpoint_c.as_ref().map(|v| { format!("  setpoint_c: {:?}", v) }),
            self.output_pct.as_ref().map(|v| { format!("  output_pct: {:?}", v) }),
            self.enabled.as_ref().map(|v| { format!("  enabled: {:?}", v) }),
            self.heater_on.as_ref().map(|v| { format!("  heater_on: {:?}", v) }),
            self.fault.as_ref().map(|v| { format!("  fault: {:?}", v) }),
            self.timestamp_ms.as_ref().map(|v| { format!("  timestamp_ms: {:?}", v) }),
        ].into_iter().flatten().collect();
        Self::NAME.to_string() + " { " + &fields.join(", ") + " }"
    }
}



pub static MSG_IDS: &[(&str, u32)] = &[
    ("UdpMessage", 1293877827),
    ("dst", 3542317724),
    ("src", 3543982537),
    ("msg_typ", 694873664),
    ("req_id", 400946249),
    ("payload", 856651685),
    ("BrokerAnnouncement", 883525243),
    ("src_ip", 3686535603),
    ("DeviceAliveEvent", 2637772092),
    ("device_id", 1815859273),
    ("objects", 2831556715),
    ("subscribes", 3861851728),
    ("publishes", 2907205612),
    ("requests", 1722637011),
    ("replies", 4257930943),
    ("BrokerSubscribeRequest", 3190208493),
    ("src_str", 1070661349),
    ("dst_str", 3660214244),
    ("msg_typ_str", 2773705352),
    ("src_id", 4022087983),
    ("dst_id", 3161019324),
    ("msg_typ_id", 1581777240),
    ("BrokerUnsubscribeRequest", 2677628270),
    ("src_str", 1070661349),
    ("dst_str", 3660214244),
    ("msg_typ_str", 2773705352),
    ("src_id", 4022087983),
    ("dst_id", 3161019324),
    ("msg_typ_id", 1581777240),
    ("BrokerSubscribeReply", 1412940656),
    ("rc", 1582595778),
    ("message", 619841764),
    ("BrokerEvent", 634317388),
    ("messages_routed", 3974112489),
    ("messages_dropped", 1559764480),
    ("dictionary_size", 3202884639),
    ("subscription_count", 1942086824),
    ("endpoint_count", 3933932640),
    ("uptime_secs", 1772062464),
    ("BrokerRequest", 902291917),
    ("add_word", 3760613617),
    ("LogEvent", 1963762699),
    ("level", 2610554845),
    ("message", 619841764),
    ("error_code", 352063879),
    ("file", 2867484483),
    ("line", 400234023),
    ("timestamp", 2994984227),
    ("SysRequest", 2966412411),
    ("req_id", 400946249),
    ("set_time", 900236405),
    ("reboot", 2272436152),
    ("console", 775068458),
    ("SysReply", 2952492394),
    ("req_id", 400946249),
    ("rc", 1582595778),
    ("message", 619841764),
    ("SysEvent", 924742914),
    ("utc", 1033099933),
    ("uptime", 4240833753),
    ("free_heap", 3986257610),
    ("flash", 2944773417),
    ("cpu_board", 3517703682),
    ("build_date", 2863606096),
    ("WifiEvent", 3371536624),
    ("ssid", 3291061806),
    ("bssid", 3223905102),
    ("rssi", 2985115822),
    ("ip", 1261996636),
    ("mac", 3970106408),
    ("channel", 566383268),
    ("gateway", 3888792057),
    ("netmask", 162736944),
    ("MulticastEvent", 579249307),
    ("group", 1605967500),
    ("port", 2078544550),
    ("mtu", 3464659267),
    ("PingRequest", 31253678),
    ("req_id", 400946249),
    ("number", 467038368),
    ("PingReply", 1594103907),
    ("req_id", 400946249),
    ("number", 467038368),
    ("HoverboardEventRaw", 2949786301),
    ("ctrl_mod", 3392182895),
    ("ctrl_typ", 2385037750),
    ("cur_mot_max", 1331200639),
    ("rpm_mot_max", 1758636612),
    ("fi_weak_ena", 1232202040),
    ("fi_weak_hi", 1424660061),
    ("fi_weak_lo", 1861466535),
    ("fi_weak_max", 4048649340),
    ("phase_adv_max_deg", 1755172188),
    ("input1_raw", 2937864083),
    ("input1_typ", 1036041868),
    ("input1_min", 4254203333),
    ("input1_mid", 59901751),
    ("input1_max", 128292155),
    ("input1_cmd", 52753209),
    ("input2_raw", 410488530),
    ("input2_typ", 2312413913),
    ("input2_min", 2004308528),
    ("input2_mid", 2172084718),
    ("input2_max", 1835252410),
    ("input2_cmd", 1105465644),
    ("aux_input1_raw", 1476824232),
    ("aux_input1_typ", 1911254079),
    ("aux_input1_min", 3150477658),
    ("aux_input1_mid", 2982701468),
    ("aux_input1_max", 2782649968),
    ("aux_input1_cmd", 3894824798),
    ("aux_input2_raw", 3565524949),
    ("aux_input2_typ", 3177791518),
    ("aux_input2_min", 637114339),
    ("aux_input2_mid", 804890529),
    ("aux_input2_max", 470618077),
    ("aux_input2_cmd", 2583074967),
    ("dc_curr", 36155229),
    ("rdc_curr", 4155600845),
    ("ldc_curr", 3591431219),
    ("cmdl", 2942290309),
    ("cmdr", 3244287451),
    ("spd_avg", 1676374509),
    ("spdl", 687126222),
    ("spdr", 318018604),
    ("filter_rate", 4218586042),
    ("spd_coef", 2563210242),
    ("str_coef", 3252853448),
    ("batv", 1321952890),
    ("temp", 3223044039),
    ("HoverboardEvent", 104988481),
    ("ctrl_mod", 3392182895),
    ("ctrl_typ", 2385037750),
    ("cur_mot_max", 1331200639),
    ("rpm_mot_max", 1758636612),
    ("fi_weak_ena", 1232202040),
    ("fi_weak_hi", 1424660061),
    ("fi_weak_lo", 1861466535),
    ("fi_weak_max", 4048649340),
    ("phase_adv_max_deg", 1755172188),
    ("input1_raw", 2937864083),
    ("input1_typ", 1036041868),
    ("input1_min", 4254203333),
    ("input1_mid", 59901751),
    ("input1_max", 128292155),
    ("input1_cmd", 52753209),
    ("input2_raw", 410488530),
    ("input2_typ", 2312413913),
    ("input2_min", 2004308528),
    ("input2_mid", 2172084718),
    ("input2_max", 1835252410),
    ("input2_cmd", 1105465644),
    ("aux_input1_raw", 1476824232),
    ("aux_input1_typ", 1911254079),
    ("aux_input1_min", 3150477658),
    ("aux_input1_mid", 2982701468),
    ("aux_input1_max", 2782649968),
    ("aux_input1_cmd", 3894824798),
    ("aux_input2_raw", 3565524949),
    ("aux_input2_typ", 3177791518),
    ("aux_input2_min", 637114339),
    ("aux_input2_mid", 804890529),
    ("aux_input2_max", 470618077),
    ("aux_input2_cmd", 2583074967),
    ("dc_curr", 36155229),
    ("rdc_curr", 4155600845),
    ("ldc_curr", 3591431219),
    ("cmdl", 2942290309),
    ("cmdr", 3244287451),
    ("spd_avg", 1676374509),
    ("spdl", 687126222),
    ("spdr", 318018604),
    ("filter_rate", 4218586042),
    ("spd_coef", 2563210242),
    ("str_coef", 3252853448),
    ("batv", 1321952890),
    ("temp", 3223044039),
    ("HoverboardRequest", 2735870956),
    ("req_id", 400946249),
    ("speed", 2072037248),
    ("steer", 318877302),
    ("HoverboardReply", 2095960949),
    ("req_id", 400946249),
    ("error_code", 352063879),
    ("message", 619841764),
    ("TouchPoint", 3463201870),
    ("active", 3648362799),
    ("id", 926444256),
    ("x", 4245442695),
    ("y", 4228665076),
    ("Ps4Event", 4282593576),
    ("button_left", 1747553511),
    ("button_right", 3439317144),
    ("button_up", 4011736455),
    ("button_down", 2887647170),
    ("button_square", 2698463033),
    ("button_cross", 3330582218),
    ("button_circle", 3154770978),
    ("button_triangle", 770341826),
    ("button_left_shoulder", 3968804096),
    ("button_right_shoulder", 3901035901),
    ("button_left_trigger", 3934254276),
    ("button_right_trigger", 4042356867),
    ("button_left_joystick", 790015118),
    ("button_right_joystick", 1414428719),
    ("button_share", 2890681153),
    ("button_options", 945736196),
    ("button_touchpad", 907838114),
    ("button_ps", 4027675431),
    ("axis_lx", 944310989),
    ("axis_ly", 927533370),
    ("axis_rx", 738742419),
    ("axis_ry", 721964800),
    ("gyro_x", 22698073),
    ("gyro_y", 5920454),
    ("gyro_z", 4284110131),
    ("accel_x", 1062702548),
    ("accel_y", 1079480167),
    ("accel_z", 1096257786),
    ("connected", 1424938192),
    ("battery_level", 523888211),
    ("bluetooth", 1682838045),
    ("debug", 1483009432),
    ("temp", 3223044039),
    ("Ps4Request", 1992038561),
    ("req_id", 400946249),
    ("rumble_small", 3134091592),
    ("rumble_large", 2673761152),
    ("led_red", 1563830470),
    ("led_green", 2295245222),
    ("led_blue", 797885595),
    ("led_flash_on", 2033544681),
    ("led_flash_off", 1311799717),
    ("CameraEvent", 2485305690),
    ("width", 2508680735),
    ("height", 3585981250),
    ("format", 3114108242),
    ("data", 3631407781),
    ("led", 1080748746),
    ("quality", 2597670950),
    ("CameraRequest", 3491086723),
    ("req_id", 400946249),
    ("led", 1080748746),
    ("capture_tcp_destination", 2458837278),
    ("quality", 2597670950),
    ("CameraReply", 4038246066),
    ("req_id", 400946249),
    ("error_code", 352063879),
    ("message", 619841764),
    ("data", 3631407781),
    ("LawnmowerManualEvent", 3961874191),
    ("speed", 2072037248),
    ("steering", 3635427802),
    ("blade", 1552352849),
    ("LawnmowerManualRequest", 3783098050),
    ("req_id", 400946249),
    ("speed", 2072037248),
    ("steer", 318877302),
    ("blade", 1552352849),
    ("start_manual_control", 3226366932),
    ("stop_manual_control", 3612618358),
    ("emergency_stop", 1921368063),
    ("start_auto_mode", 751408827),
    ("stop_auto_mode", 3579739313),
    ("LawnmowerManualReply", 1558853623),
    ("req_id", 400946249),
    ("error_code", 352063879),
    ("message", 619841764),
    ("LawnmowerAutoEvent", 3727910832),
    ("started", 2153339806),
    ("stopped", 1860367182),
    ("paused", 169449419),
    ("resumed", 3585847050),
    ("mode", 3966689298),
    ("path", 2223459638),
    ("LawnmowerAutoRequest", 3118861241),
    ("req_id", 400946249),
    ("start", 1697318111),
    ("stop", 3411225317),
    ("pause", 1887753101),
    ("resume", 4281967466),
    ("mode", 3966689298),
    ("path", 2223459638),
    ("LawnmowerStatus", 3070837701),
    ("battery_level", 523888211),
    ("blade_status", 1913274388),
    ("current_mode", 1996314776),
    ("error_message", 977701329),
    ("MotorEvent", 2506002638),
    ("motor_id", 4111653210),
    ("temperature", 3924994357),
    ("voltage", 3784531813),
    ("current", 3647323098),
    ("speed", 2072037248),
    ("position", 2471448074),
    ("Max31855Event", 2831607083),
    ("thermocouple_c", 4285487460),
    ("internal_c", 2410396330),
    ("fault", 2936529791),
    ("open_circuit", 3167585005),
    ("short_to_gnd", 2345477587),
    ("short_to_vcc", 2218820948),
    ("timestamp_ms", 464087480),
    ("HeatingRequest", 578653874),
    ("req_id", 400946249),
    ("setpoint_c", 1690578701),
    ("enabled", 49525662),
    ("kp", 1463622254),
    ("ki", 1077737017),
    ("kd", 1262290826),
    ("reset_integral", 314866981),
    ("HeatingEvent", 461737375),
    ("temperature_c", 2266483847),
    ("setpoint_c", 1690578701),
    ("output_pct", 642138204),
    ("enabled", 49525662),
    ("heater_on", 2096980360),
    ("fault", 2936529791),
    ("timestamp_ms", 464087480),
];