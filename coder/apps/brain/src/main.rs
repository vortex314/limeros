//! brain — Limeros control app.
//!
//! Architecture:
//!   UdpEndpoint    — UDP transport with broker handshake
//!   HoverboardActor — sends HoverboardRequest at 2Hz

mod hoverboard;
mod udp_endpoint;

use std::net::Ipv4Addr;

use clap::Parser;
use common::fnv1a_32;
use kameo::prelude::*;
use log::info;

use hoverboard::HoverboardActor;
use udp_endpoint::{UdpEndpoint, UdpEndpointConfig};

// ── CLI ────────────────────────────────────────────────────────────────────

#[derive(Parser, Debug)]
#[command(about = "Brain — Limeros control app")]
struct Args {
    /// Endpoint name (used for broker discovery).
    #[arg(long, default_value = "brain")]
    endpoint: String,

    /// Multicast group address.
    #[arg(long, default_value = "224.0.0.1")]
    group: String,

    /// Multicast/unicast port.
    #[arg(long, default_value_t = 50000)]
    port: u16,
}

// ── Main ───────────────────────────────────────────────────────────────────

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    common::logger::init();
    let args = Args::parse();
    info!("Starting brain '{}'...", args.endpoint);

    let group: Ipv4Addr = args.group.parse()?;

    let udp_config = UdpEndpointConfig {
        name: args.endpoint.clone(),
        description: Some("Brain UDP endpoint".to_string()),
        services: vec![],
        events: vec![],
        replies: vec![],
        subscribes: vec![],
        mc_addr: group,
        mc_port: args.port,
    };

    let udp_ref = UdpEndpoint::spawn(UdpEndpoint::new(udp_config));

    let endpoint_id = fnv1a_32(&args.endpoint);
    let _hoverboard_ref = HoverboardActor::spawn(HoverboardActor::new(endpoint_id, udp_ref.clone()));

    info!("Brain running. Press Ctrl+C to stop.");
    tokio::signal::ctrl_c().await?;
    info!("Shutting down.");
    Ok(())
}
