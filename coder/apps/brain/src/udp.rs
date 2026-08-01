//! UDP unicast actor — receives and sends unicast UDP Envelopes.

use generated::generated::{EndpointAnnounce, Envelope};
use kameo::prelude::*;
use log::{info, warn};
use std::collections::HashMap;
use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;
use tokio::net::UdpSocket;
use anyhow::Result;

use crate::router::{Register, RouterActor};

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
    router: ActorRef<RouterActor>,
    socket: Option<Arc<UdpSocket>>,
    targets: HashMap<u32, SocketAddr>,
}

impl UdpActor {
    pub fn new(router: ActorRef<RouterActor>) -> Self {
        UdpActor {
            router,
            socket: None,
            targets: HashMap::new(),
        }
    }
}

impl Actor for UdpActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        state
            .router
            .tell(Register {
                actor_ref: actor_ref.recipient(),
                description: "UdpActor".to_string(),
            })
            .await?;
        Ok(state)
    }
}

impl Message<StartUnicast> for UdpActor {
    type Reply = ();

    async fn handle(&mut self, msg: StartUnicast, _ctx: &mut Context<Self, Self::Reply>) {

        let router_clone = self.router.clone();
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

        tokio::spawn(async move {
            let mut buf = vec![0u8; 64 * 1024];
            loop {
                match read_socket.recv_from(&mut buf).await {
                    Ok((len, from)) => {
                        let packet = buf[..len].to_vec();
                        match Envelope::from_bytes(&packet) {
                            Ok(envelope) => {
                                let _ = router_clone.tell(Arc::new(envelope)).await;
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

impl Message<Arc<Envelope>> for UdpActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: Arc<Envelope>,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        // The router broadcasts every envelope to all listeners, so only
        // send when the envelope has a destination we know about.
        let Some(dst) = msg.dst else {
            return; // broadcast / not addressed — ignore
        };
        let Some(&target) = self.targets.get(&dst) else {
 //           warn!("No UDP target for dst={}, dropping", dst);
            return;
        };
        let Some(ref socket) = self.socket else {
            warn!("UDP socket not initialized, dropping dst={}", dst);
            return;
        };
        let raw = msg
            .to_bytes()
            .map_err(|e| anyhow::anyhow!("Failed to serialize envelope for UDP send: {}", e));
        if let Ok(raw) = raw {
            if let Err(e) = socket.try_send_to(&raw, target) {
                warn!("Failed to send UDP packet to {}: {}", dst, e);
            }
        } else {
            warn!("Failed to serialize envelope for UDP send to {}: {}", dst, raw.err().unwrap());
        }
        return;
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