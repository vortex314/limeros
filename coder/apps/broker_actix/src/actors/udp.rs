//! UDP unicast actor — receives and sends unicast UDP Envelopes.

use actix::prelude::*;
use generated::generated::Envelope;
use log::{info, warn};
use std::{
    net::{Ipv4Addr, SocketAddr},
    sync::Arc,
};
use tokio::{net::UdpSocket, runtime::Runtime, sync::futures};

use crate::actors::router::{EndpointAddress, IncomingEnvelope};

// ── Messages ───────────────────────────────────────────────────────────────

/// Start listening for unicast UDP on the given port.
#[derive(Message, Debug)]
#[rtype(result = "anyhow::Result<()>")]
pub struct StartUnicast {
    pub port: u16,
    pub bind_addr: Ipv4Addr,
}

/// Send a raw envelope to a specific UDP address.
#[derive(Message, Debug)]
#[rtype(result = "anyhow::Result<()>")]
pub struct UdpSend {
    pub raw: Vec<u8>,
    pub addr: SocketAddr,
}

/// Receive a raw envelope from a specific UDP address.
#[derive(Message, Debug)]
#[rtype(result = "anyhow::Result<()>")]
pub struct UdpReceive {
    pub raw: Vec<u8>,
    pub addr: SocketAddr,
}

// ── UDP actor ──────────────────────────────────────────────────────────────

pub struct UdpActor {
    router: Addr<crate::actors::router::Router>,
    socket: Option<Arc<UdpSocket>>,
}

impl UdpActor {
    pub fn new(router: Addr<crate::actors::router::Router>) -> Self {
        UdpActor {
            router,
            socket: None,
        }
    }
}

impl Actor for UdpActor {
    type Context = Context<Self>;
}

impl Handler<StartUnicast> for UdpActor {
    type Result = anyhow::Result<()>;

    fn handle(&mut self, msg: StartUnicast, ctx: &mut Context<Self>) -> Self::Result {
        let router = self.router.clone();
        let bind = SocketAddr::new(msg.bind_addr.into(), msg.port);

        // We need the bind to finish before returning Ok from this
        // sync handler, but bind() is async. Spawn it, and have the
        // spawned task report success/failure back via a message.
        ctx.spawn(
            async move {
                match UdpSocket::bind(bind).await {
                    Ok(s) => Some(s),
                    Err(e) => {
                        warn!("Failed to bind unicast socket on {}: {}", bind, e);
                        None
                    }
                }
            }
            .into_actor(self)
            .map(move |socket_opt, act, ctx| {
                if let Some(socket) = socket_opt {
                    let socket = Arc::new(socket);
                    act.socket = Some(socket.clone());

                    info!("Unicast actor listening on {}", bind);

                    // spawn the read loop on the tokio runtime, feeding
                    // received packets back to the actor via messages
                    let router = router.clone();
                    let read_socket = socket.clone();
                    tokio::spawn(async move {
                        let mut buf = vec![0u8; 64 * 1024];
                        loop {
                            match read_socket.recv_from(&mut buf).await {
                                Ok((len, from)) => {
                                    let packet = buf[..len].to_vec();
                                    // do decode here, or forward raw + addr to router/actor
                                    match Envelope::from_bytes(&packet) {
                                        Ok(envelope) => {
                                            router.do_send(IncomingEnvelope {
                                                envelope,
                                                raw: packet,
                                            });
                                        }
                                        Err(e) => {
                                            warn!(
                                                "Failed to decode unicast packet[{}] from {}: {}",
                                                len, from, e
                                            );
                                            // display cbor
                                            cbor2::from_reader(&mut &buf[..len])
                                                .map(|v: String| info!("CBOR: {:?}", v))
                                                .unwrap_or_else(|_| {
                                                    info!("CBOR: {:02X?}", &buf[..len])
                                                });
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
            }),
        );

        Ok(())
    }
}

impl Handler<UdpSend> for UdpActor {
    type Result = anyhow::Result<()>;

    fn handle(&mut self, msg: UdpSend, _ctx: &mut Context<Self>) -> Self::Result {
        let addr = msg.addr;
        self.socket
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("UDP socket not initialized"))?
            .try_send_to(&msg.raw, addr)
            .map_err(|e| anyhow::anyhow!("Failed to send UDP packet to {}: {}", addr, e))?;
        Ok(())
    }
}
