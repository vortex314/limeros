//! UDP unicast actor — receives and sends unicast UDP Envelopes.

use anyhow::Result;
use common::{envelope_to_string};
use dashmap::DashMap;
use generated::generated::{opt_id_to_string, EndpointAnnounce, Envelope};
use kameo::prelude::*;
use log::{debug, info, warn};
use std::collections::HashMap;
use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;
use tokio::net::UdpSocket;

use crate::brain::ResultLog;
use crate::multicast::McastReceive;
use crate::router::{AddListener, EnvelopeFilter, RegisterWithRouter, RouterActor, ToDevice};

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

pub struct UdpConfig {
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
    targets: Arc<DashMap<u32, SocketAddr>>,
    router: ActorRef<RouterActor>,
    udp_config: UdpConfig,
}

impl UdpActor {
    pub fn new(router: ActorRef<RouterActor>, udp_config: UdpConfig) -> Self {
        UdpActor {
            socket: None,
            targets: Arc::new(DashMap::new()),
            router,
            udp_config,
        }
    }

    pub async fn bind(&mut self, actor_ref: ActorRef<Self>) -> Result<()> {
        let bind = SocketAddr::new(self.udp_config.bind_addr.into(), self.udp_config.port);
        let socket = Arc::new(UdpSocket::bind(bind).await?);
        self.socket = Some(socket.clone());

        info!("Unicast actor listening on {}", bind);

        Self::spawn_reader(
            socket,
            self.router.clone(),
            self.targets.clone(),
            actor_ref.recipient(),
        );
        Ok(())
    }

    fn spawn_reader(
        socket: Arc<UdpSocket>,
        router_ref: ActorRef<RouterActor>,
        targets: Arc<DashMap<u32, SocketAddr>>,
        actor_ref: Recipient<Envelope>,
    ) {
        let targets = targets.clone();
        tokio::spawn(async move {
            let mut buf = vec![0u8; 64 * 1024];
            loop {
                match socket.recv_from(&mut buf).await {
                    Ok((len, from)) => {
                        debug!("Received packet from {}", from);
                        let packet = buf[..len].to_vec();
                        match Envelope::from_bytes(&packet) {
                            Ok(envelope) => {
                                if targets.get(&envelope.src.unwrap_or(0)).is_none() {
                                    info!(
                                        "Adding new UDP target {}",
                                        envelope_to_string(&envelope)
                                    );
                                    targets.insert(envelope.src.unwrap_or(0), from);
                                    let _ = router_ref.tell(RegisterWithRouter {
                                        dst: envelope.src.unwrap_or(0),
                                        recipient: actor_ref.clone(),
                                        url: format!("UDP://{}", from),
                                    }).await;
                                    router_ref
                                        .tell(AddListener {
                                            dst: envelope.src.unwrap_or(0),
                                            envelope_filter: EnvelopeFilter {
                                                dst: None,
                                                src: envelope.src,
                                                msg_type: envelope.msg_type,
                                            },
                                        })
                                        .await
                                        .log_error("Failed to add listen for UDP packet");
                                }

                                let _ = router_ref.tell(envelope).await;
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

impl Actor for UdpActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(
        mut state: Self::Args,
        actor_ref: ActorRef<Self>,
    ) -> Result<Self, Self::Error> {
        state.bind(actor_ref).await?;
        Ok(state)
    }
}

impl Message<Envelope> for UdpActor {
    type Reply = Result<(), anyhow::Error>;

    async fn handle(
        &mut self,
        msg: Envelope,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Result<(), anyhow::Error> {
        let dst = msg
            .dst
            .ok_or(anyhow::anyhow!("Envelope message missing destination"))?;
        let socket = self
            .socket
            .as_ref()
            .ok_or(anyhow::anyhow!("UDP socket not initialized"))?;
        let addr = self
            .targets
            .get(&dst)
            .ok_or(anyhow::anyhow!("No UDP target found for id {}", dst))?;
        let raw = msg.to_bytes()?;
        socket
            .try_send_to(&raw, *addr)
            .map_err(|e| anyhow::anyhow!("Failed to send UDP packet to {:?}: {}", addr, e))?;
        Ok(())
    }
}

/*impl Message<StartUnicast> for UdpActor {
    type Reply = ();

    async fn handle(&mut self, msg: StartUnicast, _ctx: &mut Context<Self, Self::Reply>) {
        let bind = SocketAddr::new(msg.bind_addr.into(), msg.port);

        // Binding happens in `on_start`; only bind here if it hasn't happened yet.
        if let Some(ref socket) = self.socket {
            match socket.local_addr() {
                Ok(current) if current == bind => {
                    info!("Unicast already listening on {}", bind);
                }
                Ok(current) => {
                    warn!(
                        "Unicast already listening on {}, ignoring request for {}",
                        current, bind
                    );
                }
                Err(e) => warn!("Unable to read unicast local address: {}", e),
            }
            return;
        }

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

        Self::spawn_reader(socket, self.router.clone(), self.targets.clone());
    }
}*/

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

    async fn handle(&mut self, msg: McastReceive, _ctx: &mut Context<Self, Self::Reply>) {
        let addr = msg.addr;
        let packet = msg.raw;
        match Envelope::from_bytes(&packet) {
            Ok(envelope) => {
                if let Some(src) = envelope.src {
                    self.router
                        .tell(envelope)
                        .await
                        .log_error("Failed to send FromDevice message to router");
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
        if let Some(target) = self.targets.get(&msg.id) {
            let raw = match msg.envelope.to_bytes() {
                Ok(r) => r,
                Err(e) => {
                    warn!(
                        "Failed to serialize envelope for UDP send to {}: {}",
                        msg.id, e
                    );
                    return;
                }
            };
            if let Some(ref socket) = self.socket {
                if let Err(e) = socket.try_send_to(&raw, *target) {
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
