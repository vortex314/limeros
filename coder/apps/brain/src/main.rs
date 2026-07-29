//! brain — Limeros control app.
//!
//! Architecture:
//!   UdpEndpoint     — UDP transport with broker handshake
//!   HoverboardActor — digital twin of hoverboard (2Hz command)
//!   CutterActor     — digital twin of cutter (2Hz command)
//!   CompassActor    — digital twin of compass sensor (event-only)
//!   ImuActor        — digital twin of IMU sensor (event-only)

mod brain;
mod compass;
mod cutter;
mod hoverboard;
mod imu;
mod ps4;
mod udp_endpoint;

use std::net::Ipv4Addr;

use clap::Parser;
use common::fnv1a_32;
use generated::generated::{Envelope, opt_id_to_string};
use kameo::prelude::*;
use log::info;

use brain::BrainActor;
use compass::CompassActor;
use cutter::CutterActor;
use hoverboard::HoverboardActor;
use imu::ImuActor;
use ps4::Ps4Actor;
use udp_endpoint::{UdpEndpoint, UdpEndpointConfig};

pub fn display_envelope(envelope: &Envelope, context: &str) {
    info!(
        "{} src={} dst={} msg_type={} payload_len={} ",
        context,
        opt_id_to_string(envelope.src),
        opt_id_to_string(envelope.dst),
        opt_id_to_string(envelope.msg_type),
        envelope.payload.as_ref().map(|p| p.len()).unwrap_or(0),
    );
}

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
    let hoverboard_ref = HoverboardActor::spawn(HoverboardActor::new(endpoint_id, udp_ref.clone()));
    let cutter_ref = CutterActor::spawn(CutterActor::new(endpoint_id, udp_ref.clone()));
    let compass_ref = CompassActor::spawn(CompassActor::new(endpoint_id, udp_ref.clone()));
    let imu_ref = ImuActor::spawn(ImuActor::new(endpoint_id, udp_ref.clone()));
    let ps4_ref = Ps4Actor::spawn(Ps4Actor::new(endpoint_id, udp_ref.clone()));

    let _brain_ref = BrainActor::spawn(BrainActor::new(
        udp_ref.clone(),
        hoverboard_ref,
        cutter_ref,
        compass_ref,
        imu_ref,
        ps4_ref,
    ));

    info!("Brain running. Press Ctrl+C to stop.");
    tokio::signal::ctrl_c().await?;
    info!("Shutting down.");
    Ok(())
}
