//! UdpEndpoint — simplified UDP transport for ps4_bridge.
//!
//! Sends Envelopes to the broker and announces itself via multicast.
//! No subscriber/routing logic — this binary only publishes Ps4Events.

use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;

use anyhow::Result;
use common::fnv1a_32;
use generated::generated::{EndpointAnnounce, Envelope};
use kameo::prelude::*;
use log::info;
use tokio::net::UdpSocket;
use tokio::sync::Mutex;

// ── Config ─────────────────────────────────────────────────────────────────

pub struct UdpEndpointConfig {
    pub name: String,
    pub description: Option<String>,
    pub mc_addr: Ipv4Addr,
    pub mc_port: u16,
}

// ── Internal ───────────────────────────────────────────────────────────────

struct TickAnnounce;

// ── UdpEndpoint actor ──────────────────────────────────────────────────────

pub struct UdpEndpoint {
    config: UdpEndpointConfig,
    endpoint_id: u32,
    send_socket: Arc<Mutex<Option<Arc<UdpSocket>>>>,
}

impl UdpEndpoint {
    pub fn new(config: UdpEndpointConfig) -> Self {
        let endpoint_id = fnv1a_32(&config.name);
        UdpEndpoint {
            config,
            endpoint_id,
            send_socket: Arc::new(Mutex::new(None)),
        }
    }
}

impl Actor for UdpEndpoint {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        let send_socket = state.send_socket.clone();
        let endpoint_id = state.endpoint_id;
        let name = state.config.name.clone();

        // Bind a UDP socket for sending
        tokio::spawn(async move {
            let bind = SocketAddr::new(Ipv4Addr::UNSPECIFIED.into(), 0);
            let socket = match UdpSocket::bind(bind).await {
                Ok(s) => s,
                Err(e) => {
                    log::warn!("UdpEndpoint: failed to bind socket: {e}");
                    return;
                }
            };
            let port = socket.local_addr().unwrap().port();
            info!("UdpEndpoint: bound to port {port}");

            let socket = Arc::new(socket);
            {
                let mut guard = send_socket.lock().await;
                *guard = Some(socket);
            }
        });

        // Periodic multicast announcement timer
        let announce_ref = actor_ref.clone();
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(std::time::Duration::from_secs(1));
            loop {
                interval.tick().await;
                let _ = announce_ref.tell(TickAnnounce).await;
            }
        });

        info!("UdpEndpoint '{name}' (0x{endpoint_id:08X}) started");
        Ok(state)
    }
}

// ── Handle TickAnnounce ────────────────────────────────────────────────────

impl Message<TickAnnounce> for UdpEndpoint {
    type Reply = ();

    async fn handle(&mut self, _msg: TickAnnounce, _ctx: &mut Context<Self, Self::Reply>) {
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
                log::warn!("Failed to encode EndpointAnnounce: {e}");
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
                log::warn!("Failed to encode Envelope: {e}");
                return;
            }
        };
        let target = SocketAddr::new(self.config.mc_addr.into(), self.config.mc_port);

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

// ── Handle Envelope (send to broker on unicast) ────────────────────────────

impl Message<Envelope> for UdpEndpoint {
    type Reply = ();

    async fn handle(&mut self, mut msg: Envelope, _ctx: &mut Context<Self, Self::Reply>) {
        msg.src = Some(self.endpoint_id);
        let raw = match msg.to_bytes() {
            Ok(r) => r,
            Err(e) => {
                log::warn!("Failed to encode Envelope: {e}");
                return;
            }
        };
        let send_socket = self.send_socket.clone();
        // Send to the multicast group — the broker is listening there
        let target = SocketAddr::new(self.config.mc_addr.into(), self.config.mc_port);

        let guard = send_socket.lock().await;
        if let Some(ref socket) = *guard {
            let _ = socket.send_to(&raw, target).await;
        }
    }
}
