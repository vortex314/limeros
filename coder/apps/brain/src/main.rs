//! brain — Limeros control app.
//!
//! Architecture:
//!   UdpEndpoint     — UDP transport with broker handshake
//!   HoverboardActor — digital twin of hoverboard (2Hz command)
//!   CutterActor     — digital twin of cutter (2Hz command)
//!   CompassActor    — digital twin of compass sensor (event-only)
//!   ImuActor        — digital twin of IMU sensor (event-only)

mod brain;
mod codec;
mod compass;
mod cutter;
mod hoverboard_proxy;
mod imu;
mod multicast;
mod ps4_proxy;
mod ps4_reader;
mod router;
mod serial;
mod udp;

use std::net::Ipv4Addr;

use clap::Parser;
use common::fnv;
use generated::generated::{opt_id_to_string, Envelope};
use kameo::prelude::*;
use log::info;

use brain::BrainActor;

use crate::{hoverboard_proxy::HoverboardProxy, serial::SerialActor, udp::UdpActor};

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
    let _console = kameo::console::serve("127.0.0.1:9999").await?;
    console_subscriber::init();

    let args = Args::parse();
    info!("Starting brain '{}'...", args.endpoint);

    let group: Ipv4Addr = args.group.parse()?;

    let router_ref = router::RouterActor::spawn(router::RouterActor::new());
    let udp_ref = UdpActor::spawn(UdpActor::new(router_ref.clone()));
    let mc_ref = multicast::MulticastActor::spawn(multicast::MulticastActor::new(
        router_ref.clone().recipient(),
        udp_ref.clone(),
    ));
    let hb_ref = HoverboardProxy::prepare().actor_ref();
    let _brain_ref = BrainActor::spawn(BrainActor::new(hb_ref.clone()));
    let _serial_usb0 = SerialActor::spawn(SerialActor::new("/dev/ttyUSB0"));
    let _serial_usb1 = SerialActor::spawn(SerialActor::new("/dev/ttyUSB1"));
    let _hoverboard_proxy = hb_ref.spawn(HoverboardProxy::new(
        router_ref.clone(),
        _brain_ref.clone(),
    ));
    _serial_usb0
        .tell(brain::EnvelopeHandlerRequest::SetListener {
            endpoint: fnv::fnv1a_32("hoverboard"),
            recipient: _hoverboard_proxy.clone().recipient(),
        })
        .await?;
    _serial_usb1
        .tell(brain::EnvelopeHandlerRequest::SetListener {
            endpoint: fnv::fnv1a_32("hoverboard"),
            recipient: _hoverboard_proxy.clone().recipient(),
        })
        .await?;

    let _ps4_proxy =
        ps4_proxy::Ps4Proxy::spawn(ps4_proxy::Ps4Proxy::new(_brain_ref.clone().recipient()));
    let _ps4_reader =
        ps4_reader::Ps4Bridge::spawn(ps4_reader::Ps4Bridge::new(_ps4_proxy.clone().recipient()));

    // Start transports
    mc_ref
        .tell(multicast::StartMulticast {
            group,
            port: args.port,
            bind_addr: Ipv4Addr::UNSPECIFIED,
        })
        .await?;
    udp_ref
        .tell(udp::StartUnicast {
            port: args.port,
            bind_addr: Ipv4Addr::UNSPECIFIED,
        })
        .await?;

    info!("Brain running. Press Ctrl+C to stop.");
    tokio::signal::ctrl_c().await?;
    info!("Shutting down.");
    Ok(())
}
