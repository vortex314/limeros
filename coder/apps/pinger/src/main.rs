//! pinger — kameo-based network reachability tester for Limeros.
//!
//! Sends PingRequest messages and measures round-trip time from PingReply.
//! Uses the UdpEndpoint actor for all UDP I/O and broker handshake.
//!
//! Architecture:
//!   UdpEndpoint — handles UDP send/receive, broker announcement
//!   PingerActor — ping logic, RTT measurement

mod udp_endpoint;

use std::{net::Ipv4Addr, time::Duration};
use std::sync::Arc;

use anyhow::Result;
use clap::Parser;
use generated::generated::{Envelope, PINGER_ID, PingReply, PingRequest};
use kameo::prelude::*;
use log::{debug, info, warn};

use udp_endpoint::{ UdpEndpoint, UdpEndpointConfig};
use tokio::time::interval;
use tokio_stream::wrappers::IntervalStream;

use crate::udp_endpoint::Subscribe;

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


// ── PingerActor ────────────────────────────────────────────────────────────
pub struct PingerActor {
    gateway: ActorRef<UdpEndpoint>,
    interval_ms: u64,
}

impl PingerActor {
    fn new(gateway: ActorRef<UdpEndpoint>) -> Self {
        PingerActor {
            gateway,
            interval_ms: 1000,
        }
    }

    async fn handle_ping_request(&mut self, req: PingRequest, env: Arc<Envelope>) -> Result<()> {
        debug!(
            "Received PingRequest req_id={:?} from {}",
            req.req_id,
            env.src.unwrap_or(0)
        );
        let reply = PingReply {
            req_id: req.req_id,
            timestamp: Some(chrono::Utc::now().timestamp_micros() as u64),
        };
        if let Ok(payload) = reply.to_bytes() {
            let reply_env = Envelope {
                src: None,
                dst: env.src,
                msg_type: Some(PingReply::id()),
                request_id: reply.req_id,
                instance_id: None,
                payload: Some(payload),
            };
            let _ = self.gateway.tell(reply_env).await;
        }
        Ok(())
    }
    async fn handle_ping_reply(&mut self, reply: PingReply, env: Arc<Envelope>) -> Result<()> {
        let now_us = chrono::Utc::now().timestamp_micros() as u64;
        let sent_us = reply.timestamp.unwrap_or(0);
        let rtt_us = now_us.saturating_sub(sent_us);
        info!(
            "PingReply from {} — RTT: {} µs ({:.2} ms)",
            env.src.unwrap_or(0),
            rtt_us,
            rtt_us as f64 / 1000.0
        );
        Ok(())
    }

    async fn handle_tick_ping(&mut self) -> Result<()> {
        let req = PingRequest {
            req_id: Some(1),
            timestamp: Some(chrono::Utc::now().timestamp_millis() as u64),
        };
        let payload = match req.to_bytes() {
            Ok(p) => p,
            Err(e) => {
                warn!("Failed to encode PingRequest: {}", e);
                return Ok(());
            }
        };
        let envelope = Envelope {
            src: Some(PINGER_ID),
            dst: Some(PINGER_ID),
            msg_type : Some(PingRequest::MSG_ID),
            request_id: req.req_id,
            instance_id: None,
            payload: Some(payload),
        };
        let _ = self.gateway.tell(envelope).await;
        Ok(())
    }
}


impl Actor for PingerActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        // Periodic PingRequest timer
        let tick_ref = actor_ref.clone();
        let interval_ms = state.interval_ms;
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(std::time::Duration::from_millis(interval_ms));
            loop {
                interval.tick().await;
                let _ = tick_ref.tell(TickPing).await;
            }
        });

        Ok(state)
    }
}

// ── Handle RecvEnvelope (incoming from UdpEndpoint) ───────────────────────

impl Message<Arc<Envelope>> for PingerActor {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        msg: Arc<Envelope>,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let env = msg;
        let msg_type = env.msg_type.unwrap_or(0);

        match msg_type {
            PingRequest::MSG_ID => {
                let payload = match &env.payload {
                    Some(p) => p,
                    None => return Ok(()),
                };
                if let Ok(req) = PingRequest::from_bytes(payload) {
                    self.handle_ping_request(req, env.clone()).await?;
                }
            }
            PingReply::MSG_ID => {
                let payload = match &env.payload {
                    Some(p) => p,
                    None => return Ok(()),
                };
                if let Ok(reply) = PingReply::from_bytes(payload) {
                    self.handle_ping_reply(reply, env.clone()).await?;
                }
            }
            _ => {
                warn!(
                    "Received unknown message type {} from {}",
                    msg_type,
                    env.src.unwrap_or(0)
                );
            }
        }
        Ok(())
    }
}

// ── Handle TickPing ───────────────────────────────────────────────────────

impl Message<TickPing> for PingerActor {
    type Reply = ();

    async fn handle(&mut self, _msg: TickPing, _ctx: &mut Context<Self, Self::Reply>) {
        self.handle_tick_ping().await.unwrap_or_else(|e| {
            warn!("Failed to send PingRequest: {}", e);
        });
    }
}

// ── Main ───────────────────────────────────────────────────────────────────

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    common::logger::init();
    let args = Args::parse();
    info!("Starting pinger '{}'...", args.endpoint);

    let group: Ipv4Addr = args.group.parse()?;
    let _bind: Ipv4Addr = args.bind.parse()?;

    let udp_config = UdpEndpointConfig {
        name: args.endpoint.clone(),
        description: Some("Pinger UDP endpoint".to_string()),
        services: vec![PingRequest::MSG_ID],
        events: vec![],
        replies: vec![PingReply::MSG_ID],
        subscribes: vec![],
        mc_addr: group,
        mc_port: args.port,
    };
    let udp_ref = UdpEndpoint::spawn(UdpEndpoint::new(udp_config));
    let pinger_ref = PingerActor::spawn(PingerActor::new(udp_ref.clone()));

    let _ = udp_ref
        .tell(Subscribe {
            msg_type: PingRequest::MSG_ID,
            recipient: pinger_ref.clone().recipient::<Arc<Envelope>>(),
        })
        .await;
    let _ = udp_ref
        .tell(Subscribe {
            msg_type: PingReply::MSG_ID,
            recipient: pinger_ref.clone().recipient::<Arc<Envelope>>(),
        })
        .await;


    tokio::signal::ctrl_c().await?;
    info!("Shutting down.");
    Ok(())
}
