use anyhow::{Context, Result};
use clap::Parser;
use common::{
    base_message::show_cbor_bytes, endpoint, fnv::fnv1a_32, logger, node::UdpNode, RobotConfig,
};
use generated::generated as msgs;
use log::{debug, info, warn};
use msgs::{Envelope, PingReply, PingRequest};
use std::{
    net::{Ipv4Addr, SocketAddr},
    sync::Arc,
};
use tokio::join;

#[derive(Parser, Debug)]
#[command(about = "Pinger endpoint announce client")]
struct Args {
    /// Multicast group address.
    #[arg(long, default_value = msgs::MULTICAST_ADDR)]
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

fn decipher_message(cfg: &RobotConfig, msg: Envelope) -> String {
    let bytes = match msg.payload {
        Some(b) => b,
        None => return "<none>".into(),
    };

    macro_rules! try_deser {
        ($ty:ty) => {{
            if msg.msg_type == Some(<$ty>::id()) {
                return format!(
                    " {} -> {} {} {:?}",
                    cfg.id_to_name(msg.src),
                    cfg.id_to_name(msg.dst),
                    cfg.id_to_name(msg.msg_type),
                    <$ty>::from_bytes(&bytes).unwrap()
                );
            }
        }};
    }

    try_deser!(PingRequest);
    try_deser!(PingReply);
    try_deser!(msgs::EndpointAnnounceReply);

    // Fallback to CBOR diagnostic notation
    return format!(
        " {} -> {} {} {}",
        cfg.id_to_name(msg.src),
        cfg.id_to_name(msg.dst),
        cfg.id_to_name(msg.msg_type),
        show_cbor_bytes(&bytes)
    );
}

#[tokio::main]
async fn main() -> Result<()> {
    logger::init();
    let args = Args::parse();

    let endpoint_id = fnv1a_32(&args.endpoint);

    let port = args.port.unwrap_or(msgs::MULTICAST_PORT as u16);
    let mc_addr_str = &args.group;
    let group: Ipv4Addr = mc_addr_str
        .parse()
        .with_context(|| format!("failed to parse multicast address {}", mc_addr_str))?;
    let node = UdpNode::new(endpoint_id, SocketAddr::new(group.into(), port));
    let config_path = "hcl/robot.hcl";
    let robot_config = RobotConfig::load_from_file(config_path).context(format!(
        "failed to load robot configuration from {}",
        config_path
    ))?;

    let mut pinger = endpoint::Endpoint::new_from_name(&args.endpoint, node);
    pinger.bind().await?;

    let pinger = Arc::new(pinger);
    let pinger2 = pinger.clone();
    let pinger1 = pinger.clone();

    tokio::spawn(async move {
        loop {
            if let Err(e) = pinger2.broker_handshake().await {
                warn!("Broker handshake failed: {:?}", e);
            }
            tokio::time::sleep(std::time::Duration::from_millis(args.announce_ms)).await;
        }
    });
    let endpoint_name = args.endpoint.clone();
    let endpoint_name_2 = endpoint_name.clone();

    let t1 = tokio::spawn(async move {
        loop {
            let ping_request = PingRequest {
                req_id: Some(1),
                timestamp: Some(chrono::Utc::now().timestamp_millis() as u64),
            };
            let udp_message = Envelope {
                src: Some(endpoint_id),
                dst: Some(endpoint_id),
                msg_type: Some(PingRequest::id()),
                request_id: ping_request.req_id,
                instance_id: None,
                payload: Some(ping_request.to_bytes().unwrap()),
            };
            pinger1.send(udp_message).await.unwrap();
            debug!("Sent PingRequest to broker");
            tokio::time::sleep(std::time::Duration::from_secs(1)).await;
        }
    });

    let t2 = tokio::spawn(async move {
        loop {
            let (reply, addr) = pinger.receive().await.unwrap();
            if reply.msg_type == Some(PingRequest::id()) {
                let ping_request = PingRequest::from_bytes(&reply.payload.unwrap()).unwrap();
                debug!(
                    "Received PingRequest from broker at {}: {:?}",
                    addr, ping_request
                );
                let ping_reply = PingReply {
                    req_id: ping_request.req_id,
                    timestamp: Some(chrono::Utc::now().timestamp_micros() as u64),
                };
                let reply_message = Envelope {
                    src: Some(fnv1a_32(&endpoint_name_2)),
                    dst: reply.src.clone(),
                    msg_type: Some(PingReply::id()),
                    request_id: ping_reply.req_id,
                    instance_id: None,
                    payload: Some(ping_reply.to_bytes().unwrap()),
                };
                let _ = pinger.send(reply_message).await;
            } else if reply.msg_type == Some(PingReply::id()) {
                let ping_reply = PingReply::from_bytes(&reply.payload.unwrap()).unwrap();
                debug!(
                    "Received PingReply from broker at {}: {:?}",
                    addr, ping_reply
                );
                let delta_ts = chrono::Utc::now().timestamp_micros() as u64
                    - ping_reply.timestamp.unwrap_or(0);
                info!("Ping round-trip time: {} µs", delta_ts);
            } else if reply.msg_type != Some(msgs::EndpointAnnounceReply::id()) {
                warn!(
                    "Received unexpected message type {}",
                    decipher_message(&robot_config, reply)
                );
            }
        }
    });

    let r = join!(t1, t2);
    info!("Pinger tasks completed: {:?}", r);
    Ok(())
}
