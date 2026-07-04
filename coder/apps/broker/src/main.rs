use anyhow::{Context, Result};
use clap::Parser;
use dashmap::DashMap;
use fnv::FnvHasher;
use log::{info, warn};
use std::collections::HashSet;
use std::hash::{Hash, Hasher};
use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;
use tokio::net::UdpSocket;
use tokio::sync::Mutex;
use tokio::sync::mpsc::{Receiver, Sender};

use common::base_message::{BytesSerde, EndpointAnnounceReply, UdpMessage};
use common::node::UdpNode;
use common::{RobotConfig, load_robot_config, logger};
use generated::{BROKER_ID, generated};
use common::base_message::{EndpointAnnounce};

#[derive(Parser, Debug)]
#[command(about = "Multicast UDP broker receiver")]
struct Args {
    /// HCL config path.
    #[arg(short = 'i', long, default_value = "hcl/robot.hcl")]
    input: String,

    /// Multicast group address.
    #[arg(long, default_value = "239.255.0.1")]
    group: String,

    /// UDP port (defaults to generated MULTICAST_PORT when omitted).
    #[arg(long)]
    port: Option<u16>,

    /// Bind interface address.
    #[arg(long, default_value = "0.0.0.0")]
    bind: String,
}

#[derive(Debug, Clone)]
struct Subscription {
    src: Option<u32>,
    msg_type: Option<u32>,
    dst: Option<u32>,
}

#[derive(Debug, Clone)]
struct KnownEndpoint {
    announce: EndpointAnnounce,
    addr: SocketAddr,
}

fn fnv1a_32(s: &str) -> u32 {
    let mut hasher = FnvHasher::default();
    s.hash(&mut hasher);
    hasher.finish() as u32
}

fn normalize_symbol(value: &str) -> String {
    value
        .split('.')
        .next_back()
        .unwrap_or(value)
        .trim()
        .to_string()
}

fn parse_subscriptions(cfg: &RobotConfig) -> Vec<Subscription> {
    let mut subs = Vec::new();
    for endpoint_map in cfg.devices.values().map(|d| &d.endpoints) {
        for ep in endpoint_map.values() {
            for sub in &ep.subscribes {
                let src = sub
                    .src
                    .as_deref()
                    .map(normalize_symbol)
                    .map(|v| fnv1a_32(&v));
                let msg_type = sub
                    .msg_type
                    .as_deref()
                    .map(normalize_symbol)
                    .map(|v| fnv1a_32(&v));
                let dst = sub
                    .dst
                    .as_deref()
                    .map(normalize_symbol)
                    .map(|v| fnv1a_32(&v));
                subs.push(Subscription { src, msg_type, dst });
            }
        }
    }
    subs
}

fn matches_subscription(msg: &UdpMessage, sub: &Subscription) -> bool {
    let src_ok = sub.src.is_none() || sub.src == msg.src;
    let typ_ok = sub.msg_type.is_none() || sub.msg_type == msg.msg_type;
    let dst_ok = sub.dst.is_none() || sub.dst == msg.dst;
    src_ok && typ_ok && dst_ok
}



fn target_ids(msg: &UdpMessage, subscriptions: &[Subscription]) -> HashSet<u32> {
    subscriptions
        .iter()
        .filter(|s| matches_subscription(msg, s))
        .filter_map(|s| s.dst)
        .collect()
}

struct Broker {
    node: Arc<UdpNode>,
    subscriptions: Vec<Subscription>,
    known_endpoints: DashMap<u32, KnownEndpoint>,
    rx_message: Mutex<Receiver<MetaMessage>>,
    tx_message: Sender<MetaMessage>,
}

struct MetaMessage {
    msg: UdpMessage,
    src_addr: SocketAddr,
}

fn show_cbor_bytes(bytes: &[u8]) -> String {
    cbor2::from_slice::<cbor2::Value>(bytes)
        .map(|v| format!("{:?}", v))
        .unwrap_or_else(|_| format!("Invalid CBOR: {:?}", bytes))
}

impl Broker {
    fn new(node: UdpNode, subscriptions: Vec<Subscription>) -> Self {
        let node = Arc::new(node);
        let (tx_message, rx_message) = tokio::sync::mpsc::channel(100);

        Broker {
            node,
            subscriptions,
            known_endpoints: DashMap::new(),
            rx_message: Mutex::new(rx_message),
            tx_message,
        }
    }

    async fn handle_multicast_packet(&self, packet: &[u8], addr: SocketAddr) -> Result<()> {
        let udp_message = UdpMessage::from_bytes(packet)?;
        if udp_message.msg_type == Some(EndpointAnnounce::ID) {
            let payload = udp_message.payload.ok_or_else(|| anyhow::anyhow!("Missing payload"))?;
            let announce = EndpointAnnounce::from_bytes(&payload)?;
            if let Some(endpoint_id) = announce.endpoint_id {
                self.known_endpoints.insert(
                    endpoint_id,
                    KnownEndpoint {
                        announce: announce.clone(),
                        addr,
                    },
                );
            }
            let reply_payload = EndpointAnnounceReply {
                broker_id: Some(BROKER_ID),
            }
            .to_bytes()?;
            let reply_message = UdpMessage {
                src: Some(BROKER_ID),
                dst: Some(udp_message.src.unwrap_or(0)),
                msg_type: Some(EndpointAnnounceReply::ID),
                req_id: udp_message.req_id,
                payload: Some(reply_payload),
            };
            self.node.send_unicast(&reply_message.to_bytes()?, addr).await?;
        }

        Ok(())
    }

    async fn handle_udp_packet(&self, packet: &[u8], addr: SocketAddr) -> Result<()> {
        let udp_message = UdpMessage::from_bytes(packet)?;
        self.tx_message
            .send(MetaMessage {
                msg: udp_message,
                src_addr: addr,
            })
            .await
            .map_err(|e| anyhow::anyhow!("Failed to send message to channel: {}", e))?;
        Ok(())
    }

    pub async fn run(self: Arc<Self>) {
        let self_mc = Arc::clone(&self);
        let node_mc: Arc<UdpNode> = Arc::clone(&self.node);
        // spawn multicast receive loop
        let t1 = tokio::spawn(async move {
            loop {
                let mut buf = vec![0_u8; 64 * 1024];
                let (len, addr) = match node_mc.receive_multicast(&mut buf).await {
                    Ok(v) => v,
                    Err(e) => {
                        warn!("Error receiving multicast packet: {:?}", e);
                        continue;
                    }
                };
                let packet = &buf[..len];

                if let Err(e) = self_mc.handle_multicast_packet(&packet, addr).await {
                    warn!("Error handling multicast packet from {}: {:?}", addr, e);
                }
            }
        });

        let self_udp = Arc::clone(&self);
        let node_udp: Arc<UdpNode> = Arc::clone(&self.node);
        // spawn unicast receive loop
        let t2 = tokio::spawn(async move {
            loop {
                let mut buf = vec![0_u8; 64 * 1024];
                let r: Result<(usize, SocketAddr)> = node_udp.receive_unicast(&mut buf).await;
                if r.is_err() {
                    warn!("Error receiving UDP packet: {:?}", r.err());
                    continue;
                }
                let (len, addr) = r.unwrap();
                let packet = &buf[..len];
                if let Err(e) = self_udp.handle_udp_packet(packet, addr).await {
                    warn!("Error handling UDP packet from {}: {:?}", addr, e);
                }
            }
        });

        let self_pub = Arc::clone(&self);
        // spawn publisher task to forward messages to subscribed endpoints
        let t3 = tokio::spawn(async move {
            let mut rx = self_pub.rx_message.lock().await;
            while let Some(meta) = rx.recv().await {
                let target_ids = target_ids(&meta.msg, &self_pub.subscriptions);
                for endpoint_id in target_ids {
                    if let Some(target) = self_pub.known_endpoints.get(&endpoint_id) {
                        if let Err(e) = self
                            .node
                            .send_unicast(&meta.msg.to_bytes().unwrap(), target.addr)
                            .await
                        {
                            warn!(
                                "forward to endpoint {} at ({:?}) failed: {}",
                                endpoint_id, target.addr, e
                            );
                        }
                    } else {
                        warn!(
                            "no known endpoint address for destination id {}",
                            endpoint_id
                        );
                    }
                }
            }
        });
        // wait one of termonation t1,t2,t3
        tokio::select! {
            _ = t1 => {
                warn!("Multicast receive loop terminated");
            }
            _ = t2 => {
                warn!("Unicast receive loop terminated");
            }
            _ = t3 => {
                warn!("Publisher task terminated");
            }
        }
    }
}

#[tokio::main]
async fn main() -> Result<()> {
    logger::init();
    info!("Starting broker...");
    let args = Args::parse();

    let cfg =
        load_robot_config(&args.input).with_context(|| format!("failed to load {}", args.input))?;

    let port = args.port.unwrap_or(generated::MULTICAST_PORT as u16);
    let bind_addr = format!("{}:{}", args.bind, port);
    let mc_addr_str = cfg
        .multicast_addr
        .clone()
        .unwrap_or("225.0.0.0".to_string());
    let group: Ipv4Addr = mc_addr_str
        .parse()
        .with_context(|| format!("failed to parse multicast address {}", mc_addr_str))?;
    info!("Using multicast group {} and port {}", group, port);

    let mut node = UdpNode::new(BROKER_ID, SocketAddr::new(group.into(), port));
    node.bind_unicast  ().await?;
    node.bind_multicast().await?;
    let broker = Arc::new(Broker::new(node, parse_subscriptions(&cfg)));

    broker.run().await;
    Ok(())
}
