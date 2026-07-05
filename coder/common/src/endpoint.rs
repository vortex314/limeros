use std::sync::Arc;
use tokio::sync::Mutex;
use tokio::time::sleep;
use std::time::Duration;

use crate::base_message::{Msg, EndpointAnnounce, EndpointAnnounceReply, UdpMessage};
use crate::node::UdpNode;
use crate::fnv::fnv1a_32;
use anyhow::Result;
use log::{info, warn};

pub struct Endpoint {
    id: u32,
    name: String,
    node: UdpNode,
    broker_addr: Arc<Mutex<Option<std::net::SocketAddr>>>,
}

impl Endpoint {


    pub fn new_from_name(name: &str, node: UdpNode) -> Self {
        let id = fnv1a_32(name);
        info!("Creating Endpoint with name '{}' and id {}", name, id);
        Endpoint {
            id,
            name: name.to_string(),
            node,
            broker_addr: Arc::new(Mutex::new(None)),
        }
    }

    pub async fn bind(&mut self,) -> Result<()> {
        self.node.bind_unicast().await
    }

    pub async fn broker_handshake(&mut self) -> Result<()> {
        loop {
            let announce_payload = EndpointAnnounce {
                endpoint_id: Some(self.id),
                endpoint_name: Some(self.name.clone()),
            }
            .to_bytes()?;
            let udp_message = UdpMessage {
                src: Some(self.id),
                dst: None,
                msg_type: Some(EndpointAnnounce::id()),
                req_id: None,
                payload: Some(announce_payload),
            };
            self.node.send_multicast(&udp_message.to_bytes()?).await?;
            tokio::select!{
                _ = sleep(Duration::from_millis(1000)) => {
                    info!("Re-announcing endpoint to broker...");
                }
                result = self.receive() => {
                    match result {
                        Ok((reply, addr)) => {
                            if reply.msg_type == Some(EndpointAnnounceReply::id()) {
                                let reply_payload = EndpointAnnounceReply::from_bytes(&reply.payload.unwrap())?;
                                info!("Received EndpointAnnounceReply from broker: {:?}", reply_payload);
                                self.broker_addr.lock().await.replace(addr);
                                info!("Received EndpointAnnounceReply from broker {}",addr);
                                break;
                            } else {
                                warn!("Received unexpected message type from broker: {:?}", reply);
                            }
                        }
                        Err(e) => {
                            warn!("Error receiving message from broker: {:?}", e);
                        }
                    }
                }
            }
;
        }
        Ok(())
    }

    pub async fn announce(&self, announce: EndpointAnnounce) -> Result<()> {
        let payload = announce.to_bytes()?;
        let envelope = UdpMessage {
            src: Some(self.id),
            dst: None,
            msg_type: Some(EndpointAnnounce::id()),
            req_id: None,
            payload: Some(payload),
        };
        let packet = envelope.to_bytes()?;
        self.node.send_multicast(&packet).await
    }

    pub async fn receive(&self) -> Result<(UdpMessage, std::net::SocketAddr)> {
        let mut buf = [0u8; 1500];
        let (len, addr) = self.node.receive_unicast(&mut buf).await?;
        let packet = &buf[..len];
        if let Ok(message) = UdpMessage::from_bytes(packet) {
            if message.msg_type == Some(EndpointAnnounceReply::id()) {
                if let Ok(reply) =
                    EndpointAnnounceReply::from_bytes(message.payload.as_deref().unwrap_or(&[]))
                {
                    info!("Received EndpointAnnounceReply from {}: {:?}", addr, reply);
                    self.broker_addr.lock().await.replace(addr);
                } else {
                    warn!("Failed to decode EndpointAnnounceReply from {}", addr);
                }
                Ok((message, addr))
            } else {
                Ok((message, addr))
            }
        } else {
            warn!("Failed to decode UDP message from {}", addr);
            Err(anyhow::anyhow!(
                "Failed to decode UDP message from {}",
                addr
            ))
        }
    }

    pub async fn send(&self, message: UdpMessage) -> Result<()> {
        if let Some(broker_addr) = *self.broker_addr.lock().await {
            let packet = message.to_bytes()?;
            self.node.send_unicast(&packet, broker_addr).await
        } else {
            Err(anyhow::anyhow!(
                "No broker address known, cannot send message"
            ))
        }
    }

    pub async fn close(&mut self) -> Result<()> {
        self.node.close().await
    }
}
