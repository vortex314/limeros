use anyhow::{Context, Result};
use clap::Parser;
use common::base_message::{BytesSerde, UdpMessage};
use common::{endpoint, fnv::fnv1a_32, logger, node::UdpNode};
use generated::{BROKER_ID,generated};
use log::{info, warn};
use std::{
    net::{Ipv4Addr, SocketAddr},
    time::Duration,
};
use common::base_message::{EndpointAnnounce, EndpointAnnounceReply};

#[derive(Parser, Debug)]
#[command(about = "Pinger endpoint announce client")]
struct Args {
    /// Multicast group address.
    #[arg(long, default_value = generated::MULTICAST_ADDR)]
    group: String,

    /// UDP port (defaults to generated MULTICAST_PORT when omitted).
    #[arg(long)]
    port: Option<u16>,

    /// Bind interface address.
    #[arg(long, default_value = "0.0.0.0")]
    bind: String,

    /// Device name announced to the broker.
    #[arg(long, default_value = "pinger")]
    device: String,

    /// Endpoint name announced to the broker.
    #[arg(long, default_value = "pinger")]
    endpoint: String,

    /// Description announced to the broker.
    #[arg(long, default_value = "Ping client endpoint")]
    description: String,

    /// Re-announce period in milliseconds while waiting for broker reply.
    #[arg(long, default_value_t = 1000)]
    announce_ms: u64,
}

#[tokio::main]
async fn main() -> Result<()> {
    logger::init();
    let args = Args::parse();

    let endpoint_id = fnv1a_32(&args.endpoint);

    let port = 50000; // Use the generated MULTICAST_PORT constant
    let mc_addr_str = "224.0.0.1"; // Use the generated MULTICAST_ADDR constant
   let group: Ipv4Addr = mc_addr_str
        .parse()
        .with_context(|| format!("failed to parse multicast address {}", mc_addr_str))?;
    let mut node = UdpNode::new(endpoint_id, SocketAddr::new(group.into(), port));

    let mut pinger = endpoint::Endpoint::new(endpoint_id, node);
    pinger.bind().await?;
    pinger.broker_handshake().await?;

    Ok(())
}
