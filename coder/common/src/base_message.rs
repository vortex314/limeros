use anyhow::{Context, Result};
use serde_tuple::{Deserialize_tuple, Serialize_tuple};

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

impl Msg<UdpMessage> for UdpMessage {
    fn id() -> u32 {
        1293877827
    }
    fn name() -> &'static str {
        "UdpMessage"
    }
}

#[derive(Debug, Clone, Serialize_tuple, Deserialize_tuple)]
pub struct UdpMessage {
    pub src: Option<u32>,
    pub dst: Option<u32>,
    pub msg_type: Option<u32>,
    pub req_id: Option<u32>,
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

#[derive(Debug, Clone, Serialize_tuple, Deserialize_tuple)]
pub struct EndpointAnnounce {
    pub endpoint_id: Option<u32>,
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

#[derive(Debug, Clone, Serialize_tuple, Deserialize_tuple)]
pub struct EndpointAnnounceReply {
    pub broker_id: Option<u32>,
}

