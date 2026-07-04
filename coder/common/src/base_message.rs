use serde_tuple::{Deserialize_tuple, Serialize_tuple};
use anyhow::{Context, Result};

pub trait BytesSerde<T> {
    fn from_bytes(data: &[u8]) -> Result<T> where T : Sized + serde::de::DeserializeOwned {
        cbor2::from_reader(data).context("Failed to deserialize from CBOR")
    }
    fn to_bytes(&self) -> Result<Vec<u8>> where Self : serde::Serialize {
        cbor2::to_vec(self).context("Failed to serialize to CBOR")
    }
}

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




impl EndpointAnnounce {
    pub const ID: u32 = 2371693343;
    pub const NAME: &'static str = "EndpointAnnounce";
}

#[derive(Debug, Clone,Serialize_tuple,Deserialize_tuple)]
pub struct EndpointAnnounce {
    pub endpoint_id: Option<u32>,
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

