//! UDP unicast actor — receives and sends unicast UDP Envelopes.

use anyhow::Result;
use generated::generated::{EndpointAnnounce, Envelope};
use kameo::prelude::*;
use log::{info, warn};
use std::collections::HashMap;
use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;
use tokio::net::UdpSocket;

use crate::brain::{EnvelopeHandlerEvent, EnvelopeHandlerRequest, ResultLog};
use crate::multicast::McastReceive;
use crate::router::{FromDevice, RouterActor, ToDevice};

// ── Messages ───────────────────────────────────────────────────────────────
pub struct AddUdpTarget {
    pub ep_info: EndpointAnnounce,
    pub addr: SocketAddr,
}
/// Start listening for unicast UDP on the given port.
pub struct StartUnicast {
    pub port: u16,
    pub bind_addr: Ipv4Addr,
}

/// Send a raw envelope to a specific UDP address.
pub struct UdpSend {
    pub raw: Vec<u8>,
    pub addr: SocketAddr,
}

// ── UDP actor ──────────────────────────────────────────────────────────────

pub struct UdpActor {
    socket: Option<Arc<UdpSocket>>,
    targets: HashMap<u32, SocketAddr>,
    router : ActorRef<RouterActor>,
}

impl UdpActor {
    pub fn new(router: ActorRef<RouterActor>) -> Self {
        UdpActor {
            socket: None,
            targets: HashMap::new(),
            router,
        }
    }
}

impl Actor for UdpActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        Ok(state)
    }
}

impl Message<StartUnicast> for UdpActor {
    type Reply = ();

    async fn handle(&mut self, msg: StartUnicast, ctx: &mut Context<Self, Self::Reply>) {
        let bind = SocketAddr::new(msg.bind_addr.into(), msg.port);

        // Spawn the bind as a detached task so we can await it
        let socket = match UdpSocket::bind(bind).await {
            Ok(s) => s,
            Err(e) => {
                warn!("Failed to bind unicast socket on {}: {}", bind, e);
                return;
            }
        };

        let socket = Arc::new(socket);
        self.socket = Some(socket.clone());

        info!("Unicast actor listening on {}", bind);

        let read_socket = socket.clone();
        let my_ref = ctx.actor_ref().clone();
        let router_ref = self.router.clone();

        tokio::spawn(async move {
            let mut buf = vec![0u8; 64 * 1024];
            loop {
                match read_socket.recv_from(&mut buf).await {
                    Ok((len, from)) => {
                        let packet = buf[..len].to_vec();
                        match Envelope::from_bytes(&packet) {
                            Ok(envelope) => {
                                let _ = router_ref
                                    .clone()
                                    .tell(FromDevice {
                                        id: envelope.src.unwrap_or(0),
                                        envelope: Arc::new(envelope),
                                    })
                                    .await;
                            }
                            Err(e) => {
                                warn!(
                                    "Failed to decode unicast packet[{}] from {}: {}",
                                    len, from, e
                                );
                            }
                        }
                    }
                    Err(e) => {
                        warn!("recv error: {}", e);
                    }
                }
            }
        });
    }
}

impl Message<UdpSend> for UdpActor {
    type Reply = ();

    async fn handle(&mut self, msg: UdpSend, _ctx: &mut Context<Self, Self::Reply>) {
        let addr = msg.addr;
        if let Some(ref socket) = self.socket {
            if let Err(e) = socket.try_send_to(&msg.raw, addr) {
                warn!("Failed to send UDP packet to {}: {}", addr, e);
            }
        } else {
            warn!("UDP socket not initialized");
        }
    }
}

impl Message<AddUdpTarget> for UdpActor {
    type Reply = ();

    async fn handle(&mut self, msg: AddUdpTarget, _ctx: &mut Context<Self, Self::Reply>) {
        if let Some(id) = msg.ep_info.id {
            self.targets.insert(id, msg.addr);
            info!("Added UDP target: id={}, addr={}", id, msg.addr);
        } else {
            warn!("AddUdpTarget received without an endpoint id");
        }
    }
}

impl Message<McastReceive> for UdpActor {
    type Reply = ();

    async fn handle(&mut self, msg: McastReceive, ctx: &mut Context<Self, Self::Reply>) {
        let addr = msg.addr;
        let packet = msg.raw;
        match Envelope::from_bytes(&packet) {
            Ok(envelope) => {
                if let Some(src) = envelope.src {
                    self.router.tell( FromDevice {
                        id: src,
                        envelope: Arc::new(envelope.clone()),
                    }).await.log_error("Failed to send FromDevice message to router");
                }
            }
            Err(e) => {
                warn!(
                    "Failed to decode multicast packet[{}] from {}: {}",
                    packet.len(),
                    addr,
                    e
                );
            }
        }
    }
}

impl Message<ToDevice> for UdpActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: ToDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        if let Some(&target) = self.targets.get(&msg.id) {
            let raw = match msg.envelope.to_bytes() {
                Ok(r) => r,
                Err(e) => {
                    warn!("Failed to serialize envelope for UDP send to {}: {}", msg.id, e);
                    return;
                }
            };
            if let Some(ref socket) = self.socket {
                if let Err(e) = socket.try_send_to(&raw, target) {
                    warn!("Failed to send UDP packet to {}: {}", msg.id, e);
                }
            } else {
                warn!("UDP socket not initialized, dropping dst={}", msg.id);
            }
        } else {
            warn!("No UDP target found for id {}", msg.id);
        }
    }
}