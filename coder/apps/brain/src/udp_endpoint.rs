//! UdpEndpoint actor — reusable UDP transport with broker handshake.
//!
//! Handles all UDP I/O: unicast send/receive, multicast announce, and broker
//! handshake (EndpointAnnounce/EndpointAnnounceReply).

use std::collections::HashMap;
use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;

use anyhow::Result;
use anyhow::anyhow;
use common::fnv1a_32;
use generated::generated::{EndpointAnnounce, EndpointAnnounceReply, Envelope};
use kameo::prelude::*;
use log::{info, warn};
use tokio::net::UdpSocket;
use tokio::sync::Mutex;

// ── Messages ───────────────────────────────────────────────────────────────

/// Received an Envelope from a peer, with the sender's address.
#[derive(Clone)]
pub struct RecvEnvelope {
    pub envelope: Arc<Envelope>,
    pub addr: SocketAddr,
}

/// Update the services/events/replies/subscribes lists.
pub struct AnnounceUpdate {
    pub services: Vec<u32>,
    pub events: Vec<u32>,
    pub replies: Vec<u32>,
    pub subscribes: Vec<u32>,
}

pub struct Subscribe {
    pub msg_types: Vec<u32>,
    pub recipient: Recipient<Arc<Envelope>>,
}

// ── Internal messages ──────────────────────────────────────────────────────
#[allow(dead_code)]
pub struct UdpEndpointConfig {
    pub mc_port: u16,
    pub mc_addr: Ipv4Addr,
    pub name: String,
    pub description: Option<String>,
    pub services: Vec<u32>,
    pub events: Vec<u32>,
    pub replies: Vec<u32>,
    pub subscribes: Vec<u32>,
}

/// Timer tick to send an EndpointAnnounce via multicast.
struct TickAnnounce;

// ── UdpEndpoint actor ──────────────────────────────────────────────────────

pub struct UdpEndpoint {
    config: UdpEndpointConfig,
    endpoint_id: u32,
    broker_addr: Option<SocketAddr>,
    send_socket: Arc<Mutex<Option<Arc<UdpSocket>>>>,
    pub subscribers: HashMap<u32, Vec<Recipient<Arc<Envelope>>>>,
}

impl UdpEndpoint {
    pub fn new(config: UdpEndpointConfig) -> Self {
        let endpoint_id = fnv1a_32(&config.name);
        UdpEndpoint {
            config,
            endpoint_id,
            broker_addr: None,
            send_socket: Arc::new(Mutex::new(None)),
            subscribers: HashMap::new(),
        }
    }
}

impl Actor for UdpEndpoint {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        let udp_self = actor_ref.clone();
        let send_socket = state.send_socket.clone();
        let endpoint_id = state.endpoint_id;
        let endpoint_name = state.config.name.clone();

        // Start UDP receive loop — tell ourselves so we can track handshake
        tokio::spawn(async move {
            let bind = SocketAddr::new(Ipv4Addr::UNSPECIFIED.into(), 0);
            let socket = match UdpSocket::bind(bind).await {
                Ok(s) => s,
                Err(e) => {
                    warn!("UdpEndpoint: failed to bind socket: {}", e);
                    return;
                }
            };
            let port = socket.local_addr().unwrap().port();
            info!("UdpEndpoint: unicast bound to port {}", port);

            let socket = Arc::new(socket);
            {
                let mut guard = send_socket.lock().await;
                *guard = Some(socket.clone());
            }

            let mut buf = vec![0u8; 64 * 1024];
            loop {
                match socket.recv_from(&mut buf).await {
                    Ok((len, remote)) => {
                        let packet = buf[..len].to_vec();
                        match Envelope::from_bytes(&packet) {
                            Ok(envelope) => {
                                let _ = udp_self
                                    .tell(RecvEnvelope {
                                        envelope: Arc::new(envelope),
                                        addr: remote,
                                    })
                                    .await;
                            }
                            Err(e) => {
                                warn!("UdpEndpoint: decode error from {}: {}", remote, e);
                            }
                        }
                    }
                    Err(e) => {
                        warn!("UdpEndpoint: recv error: {}", e);
                    }
                }
            }
        });

        // Periodic broker announcement timer
        let announce_ref = actor_ref.clone();
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(std::time::Duration::from_millis(1000));
            loop {
                interval.tick().await;
                let _ = announce_ref.tell(TickAnnounce).await;
            }
        });

        info!(
            "UdpEndpoint '{}' (0x{:08X}) started",
            endpoint_name, endpoint_id
        );

        Ok(state)
    }
}

// ── Handle TickAnnounce ────────────────────────────────────────────────────

impl Message<TickAnnounce> for UdpEndpoint {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        _msg: TickAnnounce,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let announce = EndpointAnnounce {
            id: Some(self.endpoint_id),
            name: Some(self.config.name.clone()),
            description: Some(self.config.description.clone().unwrap_or_default()),
            services: None,
            events: None,
            replies: None,
            subscribes: None,
        };
        let payload = match announce.to_bytes() {
            Ok(p) => p,
            Err(e) => {
                warn!("UdpEndpoint: failed to encode EndpointAnnounce: {}", e);
                return Err(anyhow!("Failed to encode EndpointAnnounce: {}", e));
            }
        };
        let envelope = Envelope {
            src: Some(self.endpoint_id),
            dst: None,
            msg_type: Some(EndpointAnnounce::id()),
            request_id: None,
            instance_id: None,
            payload: Some(payload),
        };
        let raw = match envelope.to_bytes() {
            Ok(r) => r,
            Err(e) => {
                warn!("UdpEndpoint: failed to encode Envelope: {}", e);
                return Err(anyhow!("Failed to encode Envelope: {}", e));
            }
        };
        let target = SocketAddr::new(Ipv4Addr::new(224, 0, 0, 1).into(), 50000);

        let send_socket = self.send_socket.clone();
        tokio::spawn(async move {
            let guard = send_socket.lock().await;
            if let Some(ref socket) = *guard {
                let _ = socket.send_to(&raw, target).await;
            } else {
                drop(guard);
                let bind = SocketAddr::new(Ipv4Addr::UNSPECIFIED.into(), 0);
                if let Ok(sock) = UdpSocket::bind(bind).await {
                    let _ = sock.send_to(&raw, target).await;
                }
            }
        });
        Ok(())
    }
}

// ── Handle SendEnvelope ────────────────────────────────────────────────────

impl Message<Envelope> for UdpEndpoint {
    type Reply = Result<()>;

    async fn handle(&mut self, mut msg: Envelope, _ctx: &mut Context<Self, Self::Reply>) -> Self::Reply {
        msg.src = Some(self.endpoint_id);
        let raw = msg.to_bytes()?;
        let send_socket = self.send_socket.clone();
        let broker_addr = self.broker_addr;

        let guard = send_socket.lock().await;
        if let Some(ref socket) = *guard {
            if let Some(addr) = broker_addr {
                let _ = socket.send_to(&raw, addr).await;
            }
        }
        Ok(())
    }
}

// ── Handle RecvEnvelope (forward to subscribers, handle handshake) ─

impl Message<RecvEnvelope> for UdpEndpoint {
    type Reply = Result<()>;

    async fn handle(&mut self, msg: RecvEnvelope, _ctx: &mut Context<Self, Self::Reply>) -> Self::Reply {
        let msg_type = match msg.envelope.msg_type.as_ref() {
            Some(t) => t,
            None => return Ok(()),
        };
        if *msg_type == EndpointAnnounceReply::MSG_ID {
            if self.broker_addr.is_none() || self.broker_addr != Some(msg.addr) {
                info!(
                    "UdpEndpoint: broker handshake complete — broker at {}",
                    msg.addr
                );
                self.broker_addr = Some(msg.addr);
            }
        }
        let Some(recipients) = self.subscribers.get(msg_type) else {
            return Ok(());
        };
        let env = msg.envelope.clone();
        for recipient in recipients {
            let _ = recipient.tell(env.clone()).await;
        }
        Ok(())
    }
}

impl Message<AnnounceUpdate> for UdpEndpoint {
    type Reply = ();

    async fn handle(&mut self, msg: AnnounceUpdate, _ctx: &mut Context<Self, Self::Reply>) {
        msg.services.iter().for_each(|s| {
            if !self.config.services.contains(s) {
                self.config.services.push(*s);
            }
        });
        msg.events.iter().for_each(|e| {
            if !self.config.events.contains(e) {
                self.config.events.push(*e);
            }
        });
        msg.replies.iter().for_each(|r| {
            if !self.config.replies.contains(r) {
                self.config.replies.push(*r);
            }
        });
        msg.subscribes.iter().for_each(|s| {
            if !self.config.subscribes.contains(s) {
                self.config.subscribes.push(*s);
            }
        });
        info!(
            "UdpEndpoint '{}' updated: services={:?}, events={:?}, replies={:?}, subscribes={:?}",
            self.config.name,
            self.config.services,
            self.config.events,
            self.config.replies,
            self.config.subscribes
        );
    }
}

impl Message<Subscribe> for UdpEndpoint {
    type Reply = ();

    async fn handle(&mut self, msg: Subscribe, _ctx: &mut Context<Self, Self::Reply>) {
        for msg_type in msg.msg_types {
            self.subscribers
                .entry(msg_type)
                .or_insert_with(Vec::new)
                .push(msg.recipient.clone());
        }
    }
}
