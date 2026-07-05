use anyhow::{Context, Result};
use clap::Parser;
use common::base_message::{Msg, UdpMessage};
use common::{endpoint, fnv::fnv1a_32, logger, node::UdpNode};
use generated::generated;
use log::{debug, info, warn};
use tokio::join;
use std::{
    net::{Ipv4Addr, SocketAddr},
    sync::Arc,
};

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

    let port = args.port.unwrap_or(generated::MULTICAST_PORT as u16);
    let mc_addr_str = &args.group;
    let group: Ipv4Addr = mc_addr_str
        .parse()
        .with_context(|| format!("failed to parse multicast address {}", mc_addr_str))?;
    let node = UdpNode::new(endpoint_id, SocketAddr::new(group.into(), port));

    let mut pinger = endpoint::Endpoint::new_from_name(&args.endpoint, node);
    pinger.bind().await?;
    pinger.broker_handshake().await?;
    let pinger = Arc::new(pinger);
    let pinger1 = pinger.clone();

        let t1 = tokio::spawn(async move {
            loop {
            let ping_request = generated::PingRequest {
                req_id: Some(1),
                timestamp: Some(chrono::Utc::now().timestamp_millis() as u64),
            };
            let udp_message = UdpMessage {
                src: Some(endpoint_id),
                dst: Some(endpoint_id),
                msg_type: Some(generated::PingRequest::id()),
                req_id: ping_request.req_id,
                payload: Some(ping_request.to_bytes().unwrap()),
            };
            pinger1.send(udp_message).await.unwrap();
            debug!("Sent PingRequest to broker");
            tokio::time::sleep(std::time::Duration::from_secs(1)).await
            }

        });

        let t2 = tokio::spawn(async move {
            loop {
                let (reply, addr) = pinger.receive().await.unwrap();
                if reply.msg_type == Some(generated::PingRequest::id()) {
                    let ping_request = generated::PingRequest::from_bytes(&reply.payload.unwrap()).unwrap();
                    debug!("Received PingRequest from broker at {}: {:?}", addr, ping_request);let ping_reply = generated::PingReply {
                        req_id: ping_request.req_id,
                        timestamp: Some(chrono::Utc::now().timestamp_micros() as u64),
                    };
                    let reply_message = UdpMessage {
                        src: Some(endpoint_id),
                        dst: Some(reply.src.unwrap_or(0)),
                        msg_type: Some(generated::PingReply::id()),
                        req_id: ping_reply.req_id,
                        payload: Some(ping_reply.to_bytes().unwrap()),
                    };
                    let _ = pinger.send(reply_message).await;
                } else if reply.msg_type == Some(generated::PingReply::id()) {
                    let ping_reply = generated::PingReply::from_bytes(&reply.payload.unwrap()).unwrap();
                    debug!("Received PingReply from broker at {}: {:?}", addr, ping_reply);
                    let delta_ts = chrono::Utc::now().timestamp_micros() as u64 - ping_reply.timestamp.unwrap_or(0);
                    info!("Ping round-trip time: {} µs", delta_ts);
                } else {
                    warn!("Received unexpected message type from broker: {:?}", reply);
                }
            }
        });

        let r = join!(t1, t2);
        info!("Pinger tasks completed: {:?}", r);
    Ok(())
}
