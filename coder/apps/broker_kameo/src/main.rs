//! broker_kameo — Limeros message broker built on kameo actors.
//!
//! Architecture:
//!   MulticastActor — receives multicast UDP, discovers endpoints
//!   UdpActor       — receives/sends unicast UDP
//!   SerialActor    — one per serial port (e.g. /dev/ttyUSB0)
//!   Router         — central routing hub, subscription matching
//!
//! Configuration: robot.hcl

mod actors;
mod codec;

use std::net::Ipv4Addr;

use anyhow::Context;
use clap::Parser;
use common::{RobotConfig, load_robot_config};
use env_logger::Logger;
use generated::generated::{Envelope, opt_id_to_string};
use kameo::prelude::*;
use log::info;

use actors::logger::LoggerActor;
use actors::multicast::{MulticastActor, StartMulticast};
use actors::router::{Router, Subscription};
use actors::serial::SerialActor;
use actors::udp::{StartUnicast, UdpActor};
use serde_json::json;

use crate::actors::logger::{Flush, Log, LogRecord, OpenObserveConfig};

pub fn display_envelope(envelope: &Envelope, context: &str) {
    info!(
        "{}: src={} dst={} msg_type={} payload_len={}",
        context,
        opt_id_to_string(envelope.src),
        opt_id_to_string(envelope.dst),
        opt_id_to_string(envelope.msg_type),
        envelope.payload.as_ref().map(|p| p.len()).unwrap_or(0)
    );
}

// ── CLI ────────────────────────────────────────────────────────────────────

#[derive(Parser, Debug)]
#[command(about = "Limeros broker — kameo-based message routing")]
struct Args {
    /// HCL config path.
    #[arg(short = 'i', long, default_value = "../hcl/robot.hcl")]
    input: String,

    /// Multicast group address.
    #[arg(long, default_value = "224.0.0.1")]
    group: String,

    /// Multicast port (defaults to config multicast_port).
    #[arg(long)]
    port: Option<u16>,

    /// Bind interface address.
    #[arg(long, default_value = "0.0.0.0")]
    bind: String,

    /// Broker unicast port.
    #[arg(long, default_value_t = 50001)]
    broker_port: u16,
}

// ── Helpers ────────────────────────────────────────────────────────────────

fn parse_subscriptions(cfg: &RobotConfig) -> Vec<Subscription> {
    let mut subs = Vec::new();
    for ep in cfg.endpoints.values() {
        for sub in &ep.subscribes {
            let src = sub
                .src
                .as_deref()
                .and_then(|v| v.parse::<u32>().ok().or_else(|| Some(common::fnv1a_32(v))));
            let msg_type = sub
                .msg_type
                .as_deref()
                .and_then(|v| v.parse::<u32>().ok().or_else(|| Some(common::fnv1a_32(v))));
            let dst = sub
                .dst
                .as_deref()
                .and_then(|v| v.parse::<u32>().ok().or_else(|| Some(common::fnv1a_32(v))));
            subs.push(Subscription { src, msg_type, dst });
        }
    }
    subs
}

// ── Main ───────────────────────────────────────────────────────────────────

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    common::logger::init();

    let args = Args::parse();
    info!("Starting broker_kameo...");

    let cfg =
        load_robot_config(&args.input).with_context(|| format!("failed to load {}", args.input))?;

    let broker_id = common::fnv1a_32("broker");
    let subscriptions = parse_subscriptions(&cfg);

    // Start Router actor
    let router_actor = Router::spawn(Router::new(broker_id, subscriptions));

    let logger_actor = LoggerActor::new(
        OpenObserveConfig {
            endpoint: "http://192.168.0.240:5080".to_string(),
            organization: "default".to_string(),
            stream: "broker".to_string(),
            email: "root@example.com".to_string(),
            password: "Complexpass#123".to_string(),
        },
        router_actor.clone(),
    );
    LoggerActor::spawn(logger_actor);

    // Start UDP unicast actor
    let udp_actor = UdpActor::spawn(UdpActor::new(router_actor.clone()));
    udp_actor
        .tell(StartUnicast {
            port: cfg.broker_port,
            bind_addr: "0.0.0.0".parse()?,
        })
        .await?;

    // Start Multicast actor
    let mc_group: Ipv4Addr = cfg.multicast_addr.unwrap().parse()?;
    let mc_port = cfg.multicast_port;
    let bind_addr: Ipv4Addr = "0.0.0.0".parse()?;

    let multicast_actor =
        MulticastActor::spawn(MulticastActor::new(router_actor.clone(), udp_actor.clone()));
    multicast_actor
        .tell(StartMulticast {
            group: mc_group,
            port: mc_port,
            bind_addr,
        })
        .await?;

    // Start Serial actors for endpoints with a transport setting
    if let Some(ports) = &cfg.serial_ports {
        for port_name in ports {
            SerialActor::spawn(SerialActor::new(port_name.clone(), router_actor.clone()));
        }
    }

    info!(
        "Broker running: multicast {}:{}, unicast :{}, {} serial port(s)",
        mc_group,
        mc_port,
        cfg.broker_port,
        cfg.endpoints
            .values()
            .filter(|ep| ep.transport.is_some())
            .count()
    );

    // Keep the system alive
    tokio::signal::ctrl_c().await?;
    info!("Shutting down.");
    Ok(())
}
