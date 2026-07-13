use anyhow::{Context, Result};
use cbor2;

pub trait Msg<T> {
    fn from_bytes(data: &[u8]) -> Result<T>
    where
        T: Sized + serde::de::DeserializeOwned,
    {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }
    fn to_bytes(&self) -> Result<Vec<u8>>
    where
        Self: serde::Serialize,
    {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
    fn id() -> u32;
    fn name() -> &'static str;
}


pub fn show_cbor_bytes(bytes: &[u8]) -> String {
    cbor2::from_slice::<cbor2::Value>(bytes)
        .map(|v| format!("{:?}", v))
        .unwrap_or_else(|_| format!("Invalid CBOR: {:?}", bytes))
}
/* 
impl Msg<UdpMessage> for UdpMessage {
    fn id() -> u32 {
        1293877827
    }
    fn name() -> &'static str {
        "UdpMessage"
    }
}

#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct UdpMessage {
    #[cbor(key = 0)]
    pub src: Option<u32>,
    #[cbor(key = 1)]
    pub dst: Option<u32>,
    #[cbor(key = 2)]
    pub msg_type: Option<u32>,
    #[cbor(key = 3)]
    pub req_id: Option<u32>,
    #[cbor(key = 4)]
    pub payload: Option<Vec<u8>>,
}


impl Msg<EndpointAnnounce> for EndpointAnnounce {
    fn id() -> u32 {
        2371693343
    }
    fn name() -> &'static str {
        "EndpointAnnounce"
    }
}

#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct EndpointAnnounce {
    #[cbor(key = 0)]
    pub endpoint_id: Option<u32>,
    #[cbor(key = 1)]
    pub endpoint_name: Option<String>,
}


impl Msg<EndpointAnnounceReply> for EndpointAnnounceReply {
    fn id() -> u32 {
        3238220441
    }
    fn name() -> &'static str {
        "EndpointAnnounceReply"
    }
}

#[derive(Debug, Clone, PartialEq, cbor2::Cbor)]
pub struct EndpointAnnounceReply {
    #[cbor(key = 0)]
    pub broker_id: Option<u32>,
}

*/