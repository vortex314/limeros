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
use kameo::prelude::*;
use log::info;

use actors::logger::LoggerActor;
use actors::multicast::{MulticastActor, StartMulticast};
use actors::router::{Router, Subscription};
use actors::serial::SerialActor;
use actors::udp::{StartUnicast, UdpActor};
use serde_json::json;

use crate::actors::logger::{Flush, Log, LogRecord, OpenObserveConfig};

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

    let logger_actor = LoggerActor::new(OpenObserveConfig {
        endpoint: "http://192.168.0.240:5080".to_string(),
        organization: "default".to_string(),
        stream: "broker".to_string(),
        email: "root@example.com".to_string(),
        password: "Complexpass#123".to_string(),
    });

    let logger_ref = LoggerActor::spawn(logger_actor);

    let cfg =
        load_robot_config(&args.input).with_context(|| format!("failed to load {}", args.input))?;

    let broker_id = common::fnv1a_32("broker");
    let subscriptions = parse_subscriptions(&cfg);

    // Start Router actor
    let router_actor = Router::spawn(Router::new(broker_id, subscriptions, logger_ref.clone()));

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

    for i in [0, 10, 20, 30, 40, 50, 60, 70, 80, 90].iter() {
        logger_ref
            .tell(Log(LogRecord {
                level: "info".to_string(),
                message: format!("Test log message {}", i),
                fields: json!({
                    "a": "hello",   // string
                    "b": 3.14,       // float
                    "i": i,           // integer
                    "flag": true,     // boolean
                }),
            }))
            .send()
            .await?;
        logger_ref.tell(Flush).send().await?;
    }

    // Start Serial actors for endpoints with a transport setting
    for (name, ep_cfg) in &cfg.endpoints {
        if let Some(ref transport) = ep_cfg.transport {
            info!(
                "Starting serial actor for endpoint '{}' on {}",
                name, transport
            );
            let _serial =
                SerialActor::spawn(SerialActor::new(transport.clone(), router_actor.clone()));
            // Serial actor registers itself with the router via EndpointAnnounce
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
