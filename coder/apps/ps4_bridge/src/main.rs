//! ps4_bridge — standalone binary that reads a DualShock 4 controller via HID
//! and publishes Ps4Event messages to the Limeros broker.
//!
//! Architecture:
//!   UdpEndpoint — announces itself & sends Ps4Event Envelopes via UDP
//!   Ps4Reader  — reads HID input reports, parses them into Ps4Event, forwards
//!                to UdpEndpoint

mod ps4_reader;
mod udp_endpoint;

use std::net::Ipv4Addr;

use clap::Parser;
use common::fnv1a_32;
use kameo::prelude::*;
use log::info;

use ps4_reader::Ps4Reader;
use udp_endpoint::{UdpEndpoint, UdpEndpointConfig};

// ── CLI ────────────────────────────────────────────────────────────────────

#[derive(Parser, Debug)]
#[command(about = "PS4 Bridge — reads DS4 controller and sends Ps4Events to broker")]
struct Args {
    /// Endpoint name (used for broker discovery).
    #[arg(long, default_value = "ps4")]
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
    info!("Starting ps4_bridge '{}'...", args.endpoint);

    let group: Ipv4Addr = args.group.parse()?;

    let udp_config = UdpEndpointConfig {
        name: args.endpoint.clone(),
        description: Some("PS4 Bridge UDP endpoint".to_string()),
        mc_addr: group,
        mc_port: args.port,
    };

    let udp_ref = UdpEndpoint::spawn(UdpEndpoint::new(udp_config));

    let endpoint_id = fnv1a_32(&args.endpoint);
    let _reader_ref = Ps4Reader::spawn(Ps4Reader::new(endpoint_id, udp_ref.clone()));

    info!("ps4_bridge running. Press Ctrl+C to stop.");
    tokio::signal::ctrl_c().await?;
    info!("Shutting down.");
    Ok(())
}
