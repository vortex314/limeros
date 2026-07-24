//! UDP unicast actor — receives and sends unicast UDP Envelopes.

use std::net::{Ipv4Addr, SocketAddr};

use actix::prelude::*;
use generated::generated::Envelope;
use log::{info, warn};
use tokio::net::UdpSocket;

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

// ── UDP actor ──────────────────────────────────────────────────────────────

pub struct UdpActor {
    router: Addr<crate::actors::router::Router>,
}

impl UdpActor {
    pub fn new(router: Addr<crate::actors::router::Router>) -> Self {
        UdpActor { router }
    }
}

impl Actor for UdpActor {
    type Context = Context<Self>;
}

impl Handler<StartUnicast> for UdpActor {
    type Result = anyhow::Result<()>;

    fn handle(&mut self, msg: StartUnicast, _ctx: &mut Context<Self>) -> Self::Result {
        let router = self.router.clone();
        let bind = SocketAddr::new(msg.bind_addr.into(), msg.port);
        let my_addr =_ctx.address();

        tokio::spawn(async move {
            let socket = match UdpSocket::bind(bind).await {
                Ok(s) => s,
                Err(e) => {
                    warn!("Failed to bind unicast socket on {}: {}", bind, e);
                    return;
                }
            };

            info!("UDP unicast actor listening on {}", bind);

            let mut buf = vec![0u8; 64 * 1024];
            loop {
                match socket.recv_from(&mut buf).await {
                    Ok((len, addr)) => {
                        let packet = &buf[..len];
                        match Envelope::from_bytes(packet) {
                            Ok(envelope) => {
                                if let Ok(raw) = envelope.to_bytes() {
                                    router.do_send(IncomingEnvelope {
                                        envelope,
                                        raw,
                                    });
                                }
                            }
                            Err(e) => {
                                warn!("Failed to decode unicast packet[{}] from {}: {}", len, addr, e);
                                // display cbor
                                cbor2::from_reader(&mut &buf[..len]).map(|v: String| info!("CBOR: {:?}", v))
                                    .unwrap_or_else(|_| info!("CBOR: {:02X?}", &buf[..len]));

                            }
                        }
                    }
                    Err(e) => {
                        warn!("Unicast recv error: {}", e);
                    }
                }
            }
        });

        Ok(())
    }
}

impl Handler<UdpSend> for UdpActor {
    type Result = anyhow::Result<()>;

    fn handle(&mut self, msg: UdpSend, _ctx: &mut Context<Self>) -> Self::Result {
        let addr = msg.addr;
        tokio::spawn(async move {
            let socket = match UdpSocket::bind("0.0.0.0:0").await {
                Ok(s) => s,
                Err(e) => {
                    warn!("Failed to bind send socket: {}", e);
                    return;
                }
            };
            if let Err(e) = socket.send_to(&msg.raw, addr).await {
                warn!("Failed to send UDP to {}: {}", addr, e);
            }
        });
        Ok(())
    }
}
