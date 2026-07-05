use anyhow::{Context, Result};
use clap::Parser;
use common::base_message::{EndpointAnnounce, Msg, UdpMessage};
use common::endpoint::Endpoint;
use common::logger;
use common::node::UdpNode;
use common::fnv::fnv1a_32;
use common::load_robot_config;
use generated::generated;
use log::info;
use std::collections::HashMap;
use std::fs::File;
use std::io::{BufWriter, Write};
use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;
use tokio::sync::Mutex;

#[derive(Parser, Debug)]
#[command(about = "Sniff all messages on the bus and log to file")]
struct Args {
    /// Endpoint name for the sniffer (used for broker handshake).
    #[arg(long, default_value = "sniffer")]
    endpoint: String,

    /// Multicast group address.
    #[arg(long, default_value = generated::MULTICAST_ADDR)]
    group: String,

    /// UDP port (defaults to generated MULTICAST_PORT when omitted).
    #[arg(long)]
    port: Option<u16>,

    /// Bind interface address.
    #[arg(long, default_value = "0.0.0.0")]
    bind: String,

    /// Output file path. Defaults to "sniffer_<ISO8601>.log".
    #[arg(short = 'o', long)]
    output: Option<String>,

    /// HCL robot config for endpoint name resolution.
    #[arg(short = 'c', long, default_value = "hcl/robot.hcl")]
    config: String,

    /// Also print messages to stdout.
    #[arg(short = 'v', long, default_value_t = false)]
    verbose: bool,
}

/// Deserialize a payload into a known message struct (Debug display),
/// falling back to CBOR diagnostic notation if the type is unknown.
fn format_payload(msg_type_id: u32, payload: Option<&[u8]>) -> String {
    use common::base_message::{EndpointAnnounce, EndpointAnnounceReply, UdpMessage};

    let bytes = match payload {
        Some(b) => b,
        None => return "<none>".into(),
    };

    macro_rules! try_deser {
        ($ty:ty) => {{
            if msg_type_id == <$ty>::id() {
                if let Ok(m) = <$ty>::from_bytes(bytes) {
                    return format!("{:?}", m);
                }
            }
        }};
    }

    try_deser!(EndpointAnnounce);
    try_deser!(EndpointAnnounceReply);
    try_deser!(UdpMessage);
    try_deser!(generated::BrokerSubscribeRequest);
    try_deser!(generated::HoverboardEvent);
    try_deser!(generated::HoverboardReply);
    try_deser!(generated::HoverboardRequest);
    try_deser!(generated::PingReply);
    try_deser!(generated::PingRequest);
    try_deser!(generated::Ps4Event);
    try_deser!(generated::Ps4Request);
    try_deser!(generated::SysEvent);
    try_deser!(generated::SysReply);
    try_deser!(generated::SysRequest);

    // Fallback: generic CBOR diagnostic
    cbor2::from_slice::<cbor2::Value>(bytes)
        .map(|v| format!("{:?}", v))
        .unwrap_or_else(|_| format!("<raw {} bytes>", bytes.len()))
}

/// Look up a symbolic name for a message type id from the known generated types.
fn msg_type_name(id: u32) -> &'static str {
    match id {
        3190208493 => "BrokerSubscribeRequest",
        2371693343 => "EndpointAnnounce",
        3238220441 => "EndpointAnnounceReply",
        1293877827 => "UdpMessage",
        104988481  => "HoverboardEvent",
        2095960949 => "HoverboardReply",
        2735870956 => "HoverboardRequest",
        1594103907 => "PingReply",
        31253678   => "PingRequest",
        4282593576 => "Ps4Event",
        1992038561 => "Ps4Request",
        924742914  => "SysEvent",
        2952492394 => "SysReply",
        2966412411 => "SysRequest",
        _ => "?",
    }
}

/// Format an endpoint ID with its resolved name if available.
fn fmt_endpoint(name_map: &HashMap<u32, String>, id: Option<u32>) -> String {
    match id {
        Some(v) => match name_map.get(&v) {
            Some(name) => format!("{}", name),
            None => v.to_string(),
        },
        None => "-".to_string(),
    }
}

/// Build the initial endpoint name map from the HCL robot config.
fn build_name_map_from_config(config_path: &str) -> HashMap<u32, String> {
    let mut map = HashMap::new();
    match load_robot_config(config_path) {
        Ok(cfg) => {
            for (dev_name, dev) in &cfg.devices {
                for ep_name in dev.endpoints.keys() {
                    // Map both the qualified codegen ID and bare name ID.
                    map.insert(fnv1a_32(&ep_name), ep_name.clone());
                }
            }
            info!("Loaded {} endpoint names from config", map.len());
        }
        Err(e) => {
            info!("Could not load config '{}' for name resolution: {}", config_path, e);
        }
    }
    map
}

#[tokio::main]
async fn main() -> Result<()> {
    logger::init();
    let args = Args::parse();

    let endpoint_id = fnv1a_32(&args.endpoint);
    let port = args.port.unwrap_or(generated::MULTICAST_PORT as u16);
    let group: Ipv4Addr = args
        .group
        .parse()
        .with_context(|| format!("failed to parse multicast address {}", args.group))?;

    // Build endpoint name map from config
    let name_map = Arc::new(Mutex::new(build_name_map_from_config(&args.config)));
    // Add our own name
    {
        let mut nm = name_map.lock().await;
        nm.insert(endpoint_id, args.endpoint.clone());
    }

    // Build UdpNode and Endpoint
    let node = UdpNode::new(endpoint_id, SocketAddr::new(group.into(), port));
    let mut endpoint = Endpoint::new_from_name(&args.endpoint, node);

    // Bind and discover broker
    endpoint.bind().await?;
    info!("Discovering broker via multicast handshake...");
    endpoint.broker_handshake().await?;
    info!("Broker found, sending wildcard subscribe request");

    // Subscribe to all messages: src=None, msg_type=None = wildcard
    let sub_req = generated::BrokerSubscribeRequest {
        src: None,
        msg_type: None,
    };
    let udp_message = UdpMessage {
        src: Some(endpoint_id),
        dst: None,
        msg_type: Some(generated::BrokerSubscribeRequest::id()),
        req_id: None,
        payload: Some(sub_req.to_bytes()?),
    };
    endpoint.send(udp_message).await?;
    info!("Wildcard subscribe sent. Listening for all messages...");

    // Open output file with timestamp
    let output_path = args.output.unwrap_or_else(|| {
        let ts = chrono::Local::now().format("%Y-%m-%dT%H_%M_%S");
        format!("sniffer_{}.log", ts)
    });
    let file = File::create(&output_path)
        .with_context(|| format!("failed to create output file {}", output_path))?;
    let writer = Arc::new(Mutex::new(BufWriter::new(file)));
    info!("Logging to {}", output_path);

    // Write header with known endpoint list
    {
        let mut w = writer.lock().await;
        let nm = name_map.lock().await;
        writeln!(
            w,
            "# Sniffer '{}' started at {}",
            args.endpoint,
            chrono::Local::now().format("%Y-%m-%dT%H:%M:%S%.3f")
        )?;
        writeln!(w, "# config={} ({} known endpoints)", args.config, nm.len())?;
        for (id, name) in nm.iter() {
            writeln!(w, "#   {} = {}", name, id)?;
        }
        writeln!(w, "# multicast={}:{}", args.group, port)?;
        writeln!(w, "# format: <timestamp> src=<name> dst=<name> msg_type=<id>(<name>) req_id=<id> payload=<cbor>")?;
    }

    let endpoint = Arc::new(endpoint);

    // Receive loop
    loop {
        let (msg, _addr) = endpoint.receive().await?;

        // Collect endpoint names from EndpointAnnounce messages
        if msg.msg_type == Some(EndpointAnnounce::id()) {
            if let Some(payload) = &msg.payload {
                if let Ok(announce) = EndpointAnnounce::from_bytes(payload) {
                    if let (Some(ep_id), Some(ep_name)) = (announce.endpoint_id, announce.endpoint_name) {
                        let mut nm = name_map.lock().await;
                        if !nm.contains_key(&ep_id) {
                            nm.insert(ep_id, ep_name.clone());
                            info!("Discovered endpoint: {} (id={})", ep_name, ep_id);
                        }
                    }
                }
            }
        }

        let timestamp = chrono::Local::now().format("%Y-%m-%dT%H:%M:%S%.3f").to_string();
        let nm = name_map.lock().await;
        let src = fmt_endpoint(&nm, msg.src);
        let dst = fmt_endpoint(&nm, msg.dst);
        drop(nm);

        let mt_id = msg.msg_type.unwrap_or(0);
        let mt_name = msg_type_name(mt_id);
        let payload_str = format_payload(mt_id, msg.payload.as_deref());
        let req_id = msg.req_id.map(|v| format!("{}", v)).unwrap_or_else(|| "-".to_string());

        let line = format!(
            "{} src={} dst={} msg_type={}({}) req_id={} payload={}",
            timestamp, src, dst, mt_id, mt_name, req_id, payload_str
        );

        if args.verbose {
            println!("{}", line);
        }

        // Write to file
        {
            let mut w = writer.lock().await;
            writeln!(w, "{}", line)?;
            w.flush()?;
        }
    }
}
