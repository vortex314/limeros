//! UDP unicast actor — receives and sends unicast UDP Envelopes.

use generated::generated::Envelope;
use kameo::prelude::*;
use log::{info, warn};
use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;
use tokio::net::UdpSocket;

// ── Messages ───────────────────────────────────────────────────────────────

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

#[derive(Actor)]
pub struct UdpActor {
    router: ActorRef<crate::actors::router::Router>,
    socket: Option<Arc<UdpSocket>>,
}

impl UdpActor {
    pub fn new(router: ActorRef<crate::actors::router::Router>) -> Self {
        UdpActor {
            router,
            socket: None,
        }
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
                                let _ = router_clone
                                    .tell(crate::actors::router::IncomingEnvelope {
                                        envelope: Arc::new(envelope),
                                        raw: Arc::new(packet),
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
