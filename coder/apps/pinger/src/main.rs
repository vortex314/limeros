//! pinger — kameo-based network reachability tester for Limeros.
//!
//! Sends PingRequest messages and measures round-trip time from PingReply.
//! Uses the UdpEndpoint actor for all UDP I/O and broker handshake.
//!
//! Architecture:
//!   UdpEndpoint — handles UDP send/receive, broker announcement
//!   PingerActor — ping logic, RTT measurement

mod udp_endpoint;

use std::net::Ipv4Addr;

use clap::Parser;
use common::fnv1a_32;
use generated::generated::{Envelope, PingReply, PingRequest};
use kameo::prelude::*;
use log::{debug, info, warn};

use udp_endpoint::{RecvEnvelope, SendEnvelope, UdpEndpoint, UdpEndpointConfig};

// ── CLI ────────────────────────────────────────────────────────────────────

#[derive(Parser, Debug)]
#[command(about = "Pinger — kameo-based network reachability tester")]
struct Args {
    /// Multicast group address.
    #[arg(long, default_value = "224.0.0.1")]
    group: String,

    /// UDP port (default: 50000).
    #[arg(long, default_value_t = 50000)]
    port: u16,

    /// Bind interface address.
    #[arg(long, default_value = "0.0.0.0")]
    bind: String,

    /// Endpoint name.
    #[arg(long, default_value = "pinger")]
    endpoint: String,

    /// Ping interval in milliseconds.
    #[arg(long, default_value_t = 1000)]
    interval_ms: u64,
}

// ── Messages ───────────────────────────────────────────────────────────────

/// Timer tick to send a PingRequest.
struct TickPing;

/// Set the transport address after construction (resolves circular dependency).
struct SetTransport {
    transport: ActorRef<UdpEndpoint>,
}

// ── PingerActor ────────────────────────────────────────────────────────────

pub struct PingerActor {
    endpoint_id: u32,
    endpoint_name: String,
    /// The UdpEndpoint we use for all network I/O (set after startup).
    transport: Option<ActorRef<UdpEndpoint>>,
}

impl PingerActor {
    fn new(endpoint_name: String) -> Self {
        let endpoint_id = fnv1a_32(&endpoint_name);
        PingerActor {
            endpoint_id,
            endpoint_name,
            transport: None,
        }
    }
}

impl Actor for PingerActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        let endpoint_name = state.endpoint_name.clone();
        let endpoint_id = state.endpoint_id;

        // Periodic PingRequest timer
        let tick_ref = actor_ref.clone();
        let interval_ms = 1000u64;
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(std::time::Duration::from_millis(interval_ms));
            loop {
                interval.tick().await;
                let _ = tick_ref.tell(TickPing).await;
            }
        });

        info!(
            "PingerActor '{}' (0x{:08X}) started",
            endpoint_name, endpoint_id
        );

        Ok(state)
    }
}

// ── Handle RecvEnvelope (incoming from UdpEndpoint) ───────────────────────

impl Message<RecvEnvelope> for PingerActor {
    type Reply = ();

    async fn handle(&mut self, msg: RecvEnvelope, _ctx: &mut Context<Self, Self::Reply>) {
        let env = &msg.envelope;
        let msg_type = env.msg_type.unwrap_or(0);

        // EndpointAnnounceReply is handled by UdpEndpoint internally,
        // but may also arrive here. Ignore.
        if msg_type == generated::generated::EndpointAnnounceReply::id() {
            return;
        }

        if msg_type == PingRequest::id() {
            let payload = match &env.payload {
                Some(p) => p,
                None => return,
            };
            if let Ok(req) = PingRequest::from_bytes(payload) {
                debug!(
                    "Received PingRequest req_id={:?} from {}",
                    req.req_id, msg.addr
                );
                let reply = PingReply {
                    req_id: req.req_id,
                    timestamp: Some(chrono::Utc::now().timestamp_micros() as u64),
                };
                if let Ok(payload) = reply.to_bytes() {
                    let reply_env = Envelope {
                        src: Some(self.endpoint_id),
                        dst: env.src,
                        msg_type: Some(PingReply::id()),
                        request_id: reply.req_id,
                        instance_id: None,
                        payload: Some(payload),
                    };
                    if let Some(ref transport) = self.transport {
                        let _ = transport
                            .tell(SendEnvelope {
                                envelope: reply_env,
                            })
                            .await;
                    }
                }
            }
            return;
        }

        if msg_type == PingReply::id() {
            let payload = match &env.payload {
                Some(p) => p,
                None => return,
            };
            if let Ok(reply) = PingReply::from_bytes(payload) {
                let now_us = chrono::Utc::now().timestamp_micros() as u64;
                let sent_us = reply.timestamp.unwrap_or(0);
                let rtt_us = now_us.saturating_sub(sent_us);
                info!(
                    "PingReply from {} — RTT: {} µs ({:.2} ms)",
                    msg.addr,
                    rtt_us,
                    rtt_us as f64 / 1000.0
                );
            }
        }
    }
}

// ── Handle SetTransport ────────────────────────────────────────────────────

impl Message<SetTransport> for PingerActor {
    type Reply = ();

    async fn handle(&mut self, msg: SetTransport, _ctx: &mut Context<Self, Self::Reply>) {
        self.transport = Some(msg.transport);
        info!("PingerActor transport set");
    }
}

// ── Handle TickPing ───────────────────────────────────────────────────────

impl Message<TickPing> for PingerActor {
    type Reply = ();

    async fn handle(&mut self, _msg: TickPing, _ctx: &mut Context<Self, Self::Reply>) {
        let req = PingRequest {
            req_id: Some(1),
            timestamp: Some(chrono::Utc::now().timestamp_millis() as u64),
        };
        let payload = match req.to_bytes() {
            Ok(p) => p,
            Err(e) => {
                warn!("Failed to encode PingRequest: {}", e);
                return;
            }
        };
        let envelope = Envelope {
            src: Some(self.endpoint_id),
            dst: Some(self.endpoint_id),
            msg_type: Some(PingRequest::id()),
            request_id: req.req_id,
            instance_id: None,
            payload: Some(payload),
        };
        if let Some(ref transport) = self.transport {
            let _ = transport
                .tell(SendEnvelope {
                    envelope,
                })
                .await;
        }
    }
}

// ── Main ───────────────────────────────────────────────────────────────────

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    common::logger::init();
    let args = Args::parse();
    info!("Starting pinger '{}'...", args.endpoint);

    let group: Ipv4Addr = args.group.parse()?;
    let bind: Ipv4Addr = args.bind.parse()?;

    // Resolve circular dependency: UdpEndpoint needs PingerActor's ActorRef,
    // PingerActor needs UdpEndpoint's ActorRef.
    //
    // 1. Create PingerActor with no transport yet, then spawn it
    let pinger_actor = PingerActor::new(args.endpoint.clone());
    let pinger_ref = kameo::actor::Spawn::spawn(pinger_actor);

    // 2. Create UdpEndpoint pointing at PingerActor, then spawn it
    let udp_config = UdpEndpointConfig {
        endpoint_name: args.endpoint.clone(),
        multicast_group: group,
        multicast_port: args.port,
        bind_addr: bind,
    };
    let udp_actor = UdpEndpoint::new(udp_config, pinger_ref.clone());
    let udp_ref = kameo::actor::Spawn::spawn(udp_actor);

    // 3. Give the real UdpEndpoint ActorRef to PingerActor
    let _ = pinger_ref.tell(SetTransport { transport: udp_ref }).await;

    tokio::signal::ctrl_c().await?;
    info!("Shutting down.");
    Ok(())
}
