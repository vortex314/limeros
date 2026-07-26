//! UdpEndpoint actor — reusable UDP transport with broker handshake.
//!
//! Handles all UDP I/O: unicast send/receive, multicast announce, and broker
//! handshake (EndpointAnnounce/EndpointAnnounceReply).
//!
//! Messages:
//!   SendEnvelope — send an Envelope (to broker, or multicast if addr is None)
//!   RecvEnvelope — emitted to the recipient for every received Envelope

use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;

use common::fnv1a_32;
use generated::generated::{EndpointAnnounce, EndpointAnnounceReply, Envelope};
use kameo::prelude::*;
use log::{info, warn};
use tokio::net::UdpSocket;
use tokio::sync::Mutex;

// ── Messages ───────────────────────────────────────────────────────────────

/// Send an Envelope to the broker.
pub struct SendEnvelope {
    pub envelope: Envelope,
}

/// Received an Envelope from a peer, with the sender's address.
pub struct RecvEnvelope {
    pub envelope: Envelope,
    pub addr: SocketAddr,
}

/// Update the services/events/replies/subscribes lists.
pub struct AnnounceUpdate {
    pub services: Vec<u32>,
    pub events: Vec<u32>,
    pub replies: Vec<u32>,
    pub subscribes: Vec<u32>,
}

// ── Config ─────────────────────────────────────────────────────────────────

#[allow(dead_code)]
pub struct UdpEndpointConfig {
    pub endpoint_name: String,
    pub multicast_group: Ipv4Addr,
    pub multicast_port: u16,
    pub bind_addr: Ipv4Addr,
}

// ── Internal messages ──────────────────────────────────────────────────────

/// Timer tick to send an EndpointAnnounce via multicast.
struct TickAnnounce;

// ── UdpEndpoint actor ──────────────────────────────────────────────────────

pub struct UdpEndpoint {
    endpoint_id: u32,
    endpoint_name: String,
    #[allow(dead_code)]
    endpoint_description: String,
    services: Vec<u32>,
    events: Vec<u32>,
    replies: Vec<u32>,
    subscribes: Vec<u32>,
    broker_addr: Option<SocketAddr>,
    /// Where to forward received Envelopes.
    pinger: ActorRef<crate::PingerActor>,
    /// Shared UDP socket for sending.
    send_socket: Arc<Mutex<Option<Arc<UdpSocket>>>>,
}

impl UdpEndpoint {
    pub fn new(config: UdpEndpointConfig, pinger: ActorRef<crate::PingerActor>) -> Self {
        let endpoint_id = fnv1a_32(&config.endpoint_name);
        UdpEndpoint {
            endpoint_id,
            endpoint_name: config.endpoint_name,
            endpoint_description: "UdpEndpoint actor".into(),
            services: Vec::new(),
            events: Vec::new(),
            replies: Vec::new(),
            subscribes: Vec::new(),
            broker_addr: None,
            pinger,
            send_socket: Arc::new(Mutex::new(None)),
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
        let endpoint_name = state.endpoint_name.clone();

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
                                        envelope,
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
    type Reply = ();

    async fn handle(&mut self, _msg: TickAnnounce, _ctx: &mut Context<Self, Self::Reply>) {
        let announce = EndpointAnnounce {
            id: Some(self.endpoint_id),
            name: Some(self.endpoint_name.clone()),
            description: Some("UdpEndpoint actor".into()),
            services: None,
            events: None,
            replies: None,
            subscribes: None,
        };
        let payload = match announce.to_bytes() {
            Ok(p) => p,
            Err(e) => {
                warn!("UdpEndpoint: failed to encode EndpointAnnounce: {}", e);
                return;
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
                return;
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
    }
}

// ── Handle SendEnvelope ────────────────────────────────────────────────────

impl Message<SendEnvelope> for UdpEndpoint {
    type Reply = ();

    async fn handle(&mut self, msg: SendEnvelope, _ctx: &mut Context<Self, Self::Reply>) {
        let raw = match msg.envelope.to_bytes() {
            Ok(r) => r,
            Err(e) => {
                warn!("UdpEndpoint: failed to encode Envelope: {}", e);
                return;
            }
        };
        let send_socket = self.send_socket.clone();
        let broker_addr = self.broker_addr;

//        tokio::spawn(async move {
            let guard = send_socket.lock().await;
            if let Some(ref socket) = *guard {
                if let Some(addr) = broker_addr {
                    let _ = socket.send_to(&raw, addr).await;
                }
            }
//        });
    }
}

// ── Handle RecvEnvelope (forward to pinger, handle handshake) ─

impl Message<RecvEnvelope> for UdpEndpoint {
    type Reply = ();

    async fn handle(&mut self, msg: RecvEnvelope, _ctx: &mut Context<Self, Self::Reply>) {
        // Process broker handshake replies internally
        if msg.envelope.msg_type == Some(EndpointAnnounceReply::id()) {
            if self.broker_addr.is_none() || self.broker_addr != Some(msg.addr) {
                info!(
                    "UdpEndpoint: broker handshake complete — broker at {}",
                    msg.addr
                );
                self.broker_addr = Some(msg.addr);
            }
        }

        // Forward to pinger
        let _ = self.pinger.tell(msg).await;
    }
}

impl Message<AnnounceUpdate> for UdpEndpoint {
    type Reply = ();

    async fn handle(&mut self, msg: AnnounceUpdate, _ctx: &mut Context<Self, Self::Reply>) {
        msg.services.iter().for_each(|s| {
            if !self.services.contains(s) {
                self.services.push(*s);
            }
        });
        msg.events.iter().for_each(|e| {
            if !self.events.contains(e) {
                self.events.push(*e);
            }
        });
        msg.replies.iter().for_each(|r| {
            if !self.replies.contains(r) {
                self.replies.push(*r);
            }
        });
        msg.subscribes.iter().for_each(|s| {
            if !self.subscribes.contains(s) {
                self.subscribes.push(*s);
            }
        });
        info!(
            "UdpEndpoint '{}' updated: services={:?}, events={:?}, replies={:?}, subscribes={:?}",
            self.endpoint_name, self.services, self.events, self.replies, self.subscribes
        );
    }
}
