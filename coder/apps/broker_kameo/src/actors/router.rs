//! Central routing actor — receives Envelopes from transports and forwards
//! them based on dst/src/msg_type subscription matching.

use std::sync::Arc;

use dashmap::DashMap;
use generated::generated::{EndpointAnnounce, Envelope, id_to_string, opt_id_to_string};
use kameo::prelude::*;
use log::{debug, info};
use std::fmt::Display;
use std::net::SocketAddr;

// ── Types ───────────────────────────────────────────────────────────────────

/// Where a message came from or should go to.
#[derive(Debug, Clone)]
pub enum EndpointAddress {
    UdpEndpoint(ActorRef<crate::actors::udp::UdpActor>, SocketAddr),
    SerialEndpoint(ActorRef<crate::actors::serial::SerialActor>, String),
    EnvelopeActor(Recipient<IncomingEnvelope>),
}

// Manual PartialEq — compares the SocketAddr/port path and ignores the ActorRef
impl PartialEq for EndpointAddress {
    fn eq(&self, other: &Self) -> bool {
        match (self, other) {
            (EndpointAddress::UdpEndpoint(_, a), EndpointAddress::UdpEndpoint(_, b)) => a == b,
            (EndpointAddress::SerialEndpoint(_, a), EndpointAddress::SerialEndpoint(_, b)) => {
                a == b
            }
            (EndpointAddress::EnvelopeActor(a), EndpointAddress::EnvelopeActor(b)) => a == b,
            _ => false,
        }
    }
}

impl Display for EndpointAddress {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            EndpointAddress::UdpEndpoint(_, addr) => write!(f, "udp://{}", addr),
            EndpointAddress::SerialEndpoint(_, port) => write!(f, "serial://{}", port),
            EndpointAddress::EnvelopeActor(_) => write!(f, "envelope://actor"),
        }
    }
}

/// A known endpoint registered via EndpointAnnounce.
#[derive(Debug, Clone)]
#[allow(dead_code)]
pub struct KnownEndpoint {
    pub announce: EndpointAnnounce,
    pub addr: EndpointAddress,
}

/// A subscription rule parsed from robot.hcl.
#[derive(Debug, Clone)]
pub struct Subscription {
    pub src: Option<u32>,
    pub msg_type: Option<u32>,
    pub dst: Option<u32>,
}

/// Incoming envelope from any transport.
/// Both fields are `Arc` so forwarding to N destinations only bumps refcounts.
#[derive(Clone)]
pub struct IncomingEnvelope {
    pub envelope: Arc<Envelope>,
    pub raw: Arc<Vec<u8>>,
}

/// Register a serial actor's address with the router.
pub struct EndpointUpdate {
    pub envelope: Envelope,
    pub ep_addr: EndpointAddress,
    pub ep_announce: EndpointAnnounce,
}

pub struct AddRecipient {
    pub id: u32,
    pub name: String,
    pub description : Option<String>,
    pub recipient: Recipient<IncomingEnvelope>,
    pub subscription: Subscription,
}

// ── Router actor ───────────────────────────────────────────────────────────

#[derive(Actor)]
#[allow(dead_code)]
pub struct Router {
    /// Known endpoints: id → info.
    known_endpoints: DashMap<u32, KnownEndpoint>,
    /// Static subscriptions from config.
    subscriptions: Vec<Subscription>,
    /// Dynamic subscriptions: subscriber_id → [Subscription].
    dynamic_subscriptions: DashMap<u32, Vec<Subscription>>,
    /// Serial actor addresses: port_path → ActorRef<SerialActor>.
    serial_addrs: DashMap<String, ActorRef<crate::actors::serial::SerialActor>>,
    /// Our broker id.
    broker_id: u32,
}

impl Router {
    pub fn new(broker_id: u32, subscriptions: Vec<Subscription>) -> Self {
        Router {
            known_endpoints: DashMap::new(),
            subscriptions,
            dynamic_subscriptions: DashMap::new(),
            serial_addrs: DashMap::new(),
            broker_id,
        }
    }

    fn matches_subscription(msg: &Envelope, sub: &Subscription) -> bool {
        let src_ok = sub.src.is_none() || sub.src == msg.src;
        let typ_ok = sub.msg_type.is_none() || sub.msg_type == msg.msg_type;
        let dst_ok = sub.dst.is_none() || sub.dst == msg.dst;
        src_ok && typ_ok && dst_ok
    }

    /// Resolve a u32 endpoint id into an EndpointAddress.
    fn resolve_dst(&self, id: u32) -> Option<EndpointAddress> {
        if let Some(ep) = self.known_endpoints.get(&id) {
            return Some(ep.addr.clone());
        }
        None
    }

    /// Get all target addresses that should receive a message.
    fn target_addrs(&self, msg: &Envelope) -> Vec<EndpointAddress> {
        let mut addrs: Vec<EndpointAddress> = Vec::new();

        // Match static subscriptions
        for sub in &self.subscriptions {
            if Self::matches_subscription(msg, sub) {
                if let Some(dst_id) = sub.dst {
                    if let Some(addr) = self.resolve_dst(dst_id) {
                        addrs.push(addr);
                    }
                } else if let Some(dst_id) = msg.dst {
                    if let Some(addr) = self.resolve_dst(dst_id) {
                        addrs.push(addr);
                    }
                }
            }
        }

        // Match dynamic subscriptions
        for entry in self.dynamic_subscriptions.iter() {
            let subscriber_id = *entry.key();
            if entry.value().iter().any(|s| {
                let src_ok = s.src.is_none() || s.src == msg.src;
                let typ_ok = s.msg_type.is_none() || s.msg_type == msg.msg_type;
                src_ok && typ_ok
            }) {
                if let Some(addr) = self.resolve_dst(subscriber_id) {
                    addrs.push(addr);
                }
            }
        }

        // Always forward to explicit dst if present
        if let Some(dst_id) = msg.dst {
            if let Some(addr) = self.resolve_dst(dst_id) {
                if !addrs.contains(&addr) {
                    addrs.push(addr);
                }
            }
        }

        addrs
    }
}

// ── Handle IncomingEnvelope ────────────────────────────────────────────────

impl Message<IncomingEnvelope> for Router {
    type Reply = ();

    async fn handle(&mut self, msg: IncomingEnvelope, _ctx: &mut Context<Self, Self::Reply>) {
        // Forward to matching subscribers
        let targets = self.target_addrs(&msg.envelope);
        if targets.is_empty() {
            debug!(
                "dropping src={} dst={} msg_type={} (no matching subscribers)",
                opt_id_to_string(msg.envelope.src),
                opt_id_to_string(msg.envelope.dst),
                opt_id_to_string(msg.envelope.msg_type)
            );
        }
        for addr in targets {
            debug!(
                "src={} dst={} msg_type={} to {}",
                opt_id_to_string(msg.envelope.src),
                opt_id_to_string(msg.envelope.dst),
                opt_id_to_string(msg.envelope.msg_type),
                addr
            );
            match addr {
                EndpointAddress::SerialEndpoint(actor, _port) => {
                    let _ = actor
                        .tell(crate::actors::serial::SerialSend {
                            frame: msg.raw.to_vec(),
                        })
                        .await;
                }
                EndpointAddress::UdpEndpoint(actor, udp_addr) => {
                    let _ = actor
                        .tell(crate::actors::udp::UdpSend {
                            raw: msg.raw.to_vec(),
                            addr: udp_addr,
                        })
                        .await;
                }
                EndpointAddress::EnvelopeActor(recipient) => {
                    let _ = recipient.tell(msg.clone()).await;
                }
            }
        }
    }
}

impl Message<EndpointUpdate> for Router {
    type Reply = ();

    async fn handle(&mut self, msg: EndpointUpdate, _ctx: &mut Context<Self, Self::Reply>) {
        if let Some(endpoint_id) = msg.envelope.src {
            let is_new = !self.known_endpoints.contains_key(&endpoint_id);
            self.known_endpoints.insert(
                endpoint_id,
                KnownEndpoint {
                    announce: msg.ep_announce.clone(),
                    addr: msg.ep_addr.clone(),
                },
            );
            if is_new {
                info!(
                    "Discovered endpoint '{}' at {}",
                    id_to_string(endpoint_id),
                    msg.ep_addr
                );
            }
        }
    }
}

impl Message<AddRecipient> for Router {
    type Reply = ();

    async fn handle(&mut self, msg: AddRecipient, _ctx: &mut Context<Self, Self::Reply>) {
        self.known_endpoints.insert(
            msg.id,
            KnownEndpoint {
                announce: EndpointAnnounce {
                    id: Some(msg.id),
                    name: Some(msg.name.clone()),
                    description: msg.description.clone(),
                    services: None,
                    events: None,
                    replies: None,
                    subscribes: None,
                },
                addr: EndpointAddress::EnvelopeActor(msg.recipient),
            },
        );
        self.dynamic_subscriptions.insert(msg.id, vec![msg.subscription]);
    }
}   
     