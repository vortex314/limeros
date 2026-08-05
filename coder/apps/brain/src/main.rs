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

use crate::{
    brain::{BrainData, ResultLog}, compass::CompassActor, cutter::CutterActor, hoverboard_proxy::HoverboardProxy, imu::ImuActor, serial::SerialActor, udp::UdpActor,
};

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
    let brain_prepared = BrainActor::prepare();

    let hb_ref = HoverboardProxy::spawn(HoverboardProxy::new(brain_prepared.actor_ref().clone(),router_ref.clone()));
    let cutter_ref = CutterActor::spawn(CutterActor::new(brain_prepared.actor_ref().clone(),router_ref.clone()));
    let compass_ref = CompassActor::spawn(CompassActor::new(brain_prepared.actor_ref().clone(),router_ref.clone()));
    let imu_ref = ImuActor::spawn(ImuActor::new(brain_prepared.actor_ref().clone(),router_ref.clone()));
    let serial_usb0 = SerialActor::spawn(SerialActor::new("/dev/ttyUSB0", router_ref.clone()));
    let serial_usb1 = SerialActor::spawn(SerialActor::new("/dev/ttyUSB1", router_ref.clone()));
    let udp_ref = UdpActor::spawn(UdpActor::new(router_ref.clone()));
    let mc_ref = multicast::MulticastActor::spawn(multicast::MulticastActor::new(
        udp_ref.clone(),
        router_ref.clone(),
    ));

    let ps4_proxy = ps4_proxy::Ps4Proxy::spawn(ps4_proxy::Ps4Proxy::new(
        brain_prepared.actor_ref().clone().recipient(),
    ));
    let ps4_reader =
        ps4_reader::Ps4Bridge::spawn(ps4_reader::Ps4Bridge::new(ps4_proxy.clone().recipient()));

    let brain_ref = brain_prepared.actor_ref().clone();
    brain_prepared.spawn(BrainActor {
        machine: None,
        hoverboard_ref: hb_ref.clone(),
        cutter_ref: cutter_ref.clone(),
        compass_ref: compass_ref.clone(),
        imu_ref: imu_ref.clone(),
        ps4_ref: ps4_proxy.clone(),
    });



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

    brain_ref.link(&hb_ref).await;
    brain_ref.link(&cutter_ref).await;
    brain_ref.link(&compass_ref).await;
    brain_ref.link(&imu_ref).await;
    brain_ref.link(&ps4_proxy).await;
    brain_ref.link(&ps4_reader).await;
    brain_ref.link(&udp_ref).await;
    brain_ref.link(&mc_ref).await;
    brain_ref.link(&serial_usb0).await;
    brain_ref.link(&serial_usb1).await; 

    info!("Brain running. Press Ctrl+C to stop.");
    tokio::signal::ctrl_c().await?;
    info!("Shutting down.");
    // Stop actors
    brain_ref.stop_gracefully().await.log_error("stop 1");
    hb_ref.stop_gracefully().await.log_error("stop 1");
    cutter_ref.stop_gracefully().await.log_error("stop 1");
    compass_ref.stop_gracefully().await.log_error("stop 1");
    imu_ref.stop_gracefully().await.log_error("stop 1");
    ps4_proxy.stop_gracefully().await.log_error("stop 1");
    ps4_reader.stop_gracefully().await.log_error("stop 1");
    udp_ref.stop_gracefully().await.log_error("stop 1");
    mc_ref.stop_gracefully().await.log_error("stop 1");
    serial_usb0.stop_gracefully().await.log_error("stop 1");
    serial_usb1.stop_gracefully().await.log_error("stop 1");
    info!("Shutdown complete.");
    Ok(())
}
