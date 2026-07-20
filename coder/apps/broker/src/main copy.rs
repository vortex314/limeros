use anyhow::{Context, Result};
use clap::Parser;
use dashmap::DashMap;
use log::{debug, info, warn};
use std::collections::HashSet;
use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;
use tokio::sync::mpsc::{Receiver, Sender};
use tokio::sync::Mutex;

use common::node::UdpNode;
use common::{RobotConfig, fnv1a_32, load_robot_config, logger};
use generated::generated as msgs;
use msgs::{BrokerSubscribeRequest, EndpointAnnounce, EndpointAnnounceReply, Envelope};

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

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
        
enum EndpointAddress {
    SocketAddr(SocketAddr),
    SerialPort(String),
}

#[derive(Debug, Clone)]
struct KnownEndpoint {
    announce: EndpointAnnounce,
    addr: EndpointAddress,
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
    for ep in cfg.endpoints.values() {
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
    subs
}

fn matches_subscription(msg: &Envelope, sub: &Subscription) -> bool {
    let src_ok = sub.src.is_none() || sub.src == msg.src;
    let typ_ok = sub.msg_type.is_none() || sub.msg_type == msg.msg_type;
    let dst_ok = sub.dst.is_none() || sub.dst == msg.dst;
    src_ok && typ_ok && dst_ok
}

fn target_ids(msg: &Envelope, subscriptions: &[Subscription]) -> HashSet<u32> {
    let mut hash_set: HashSet<u32> = subscriptions
        .iter()
        .filter(|s| matches_subscription(msg, s))
        .filter_map(|s| s.dst)
        .collect();
    if let Some(dst) = msg.dst {
        hash_set.insert(dst);
    };
    hash_set
}

struct Broker {
    node: Arc<UdpNode>,
    subscriptions: Vec<Subscription>,
    known_endpoints: DashMap<u32, KnownEndpoint>,
    dynamic_subscriptions: DashMap<u32, Vec<Subscription>>,
    rx_message: Mutex<Receiver<MetaMessage>>,
    tx_message: Sender<MetaMessage>,
}

struct MetaMessage {
    msg: Envelope,
    raw: Vec<u8>,
}

impl Broker {
    fn new(node: UdpNode, subscriptions: Vec<Subscription>) -> Self {
        let node = Arc::new(node);
        let (tx_message, rx_message) = tokio::sync::mpsc::channel(100);

        Broker {
            node,
            subscriptions,
            known_endpoints: DashMap::new(),
            dynamic_subscriptions: DashMap::new(),
            rx_message: Mutex::new(rx_message),
            tx_message,
        }
    }

    async fn handle_multicast_packet(&self, packet: &[u8], addr: SocketAddr) -> Result<()> {
        let udp_message = Envelope::from_bytes(packet)?;
        let udp_message_clone = udp_message.clone();
        if udp_message.msg_type == Some(EndpointAnnounce::id()) {
            let payload = udp_message
                .payload
                .ok_or_else(|| anyhow::anyhow!("Missing payload"))?;
            let announce = EndpointAnnounce::from_bytes(&payload)?;
            if let Some(endpoint_id) = announce.id {
                if self.known_endpoints.contains_key(&endpoint_id) {
                    if self.known_endpoints.get(&endpoint_id).unwrap().addr != EndpointAddress::SocketAddr(addr) {
                        info!(
                            "Endpoint {} ( {} ) already known, updating address to {}",announce.name.clone().unwrap_or_default(),
                            endpoint_id, addr
                        );
                        self.known_endpoints.insert(
                            endpoint_id,
                            KnownEndpoint {
                                announce: announce.clone(),
                                addr: EndpointAddress::SocketAddr(addr),
                            },
                        );
                    }
                } else {
                    info!("New endpoint {} ( {} ) discovered at {}", announce.name.clone().unwrap_or_default(), endpoint_id, addr);
                    self.known_endpoints.insert(
                        endpoint_id,
                        KnownEndpoint {
                            announce: announce.clone(),
                            addr: EndpointAddress::SocketAddr(addr),
                        },
                    );
                }
            }
            let reply_payload = EndpointAnnounceReply {
                utc: Some(chrono::Utc::now().timestamp_millis() as u64),
            }
            .to_bytes()?;
            let reply_message = Envelope {
                src: Some(fnv1a_32("broker")),
                dst: udp_message.src,
                msg_type: Some(EndpointAnnounceReply::id()),
                request_id: udp_message.request_id,
                instance_id: udp_message.instance_id,
                payload: Some(reply_payload),
            };
            self.node
                .send_unicast(&reply_message.to_bytes()?, addr)
                .await?;

            // Also forward the announce to subscribers via the publisher channel
            self.tx_message
                .send(MetaMessage {
                    msg: udp_message_clone,
                    raw: packet.to_vec(),
                })
                .await
                .map_err(|e| anyhow::anyhow!("Failed to forward announce to channel: {}", e))?;
        }

        Ok(())
    }

    async fn handle_udp_packet(&self, packet: &[u8], addr: SocketAddr) -> Result<()> {
        let udp_message = Envelope::from_bytes(packet)?;
        // Handle dynamic subscribe requests
        if udp_message.msg_type == Some(BrokerSubscribeRequest::id()) {
            if let Some(payload) = &udp_message.payload {
                if let Ok(sub_req) = BrokerSubscribeRequest::from_bytes(payload) {
                    let endpoint_id = udp_message.src.unwrap_or(0);
                    let sub = Subscription {
                        src: sub_req.src,
                        msg_type: sub_req.msg_type,
                        dst: None, // target is the DashMap key, not a msg.dst filter
                    };
                    self.dynamic_subscriptions
                        .entry(endpoint_id)
                        .or_default()
                        .push(sub);
                    info!(
                        "Dynamic subscribe from endpoint {}: src={:?} msg_type={:?}",
                        endpoint_id, sub_req.src, sub_req.msg_type
                    );

                    // Replay known endpoint announces to the new subscriber
                    for entry in self.known_endpoints.iter() {
                        let ep_id = *entry.key();
                        if ep_id == endpoint_id {
                            continue;
                        }
                        if let Ok(announce_bytes) = entry.value().announce.to_bytes() {
                            let fwd = Envelope {
                                src: Some(entry.value().announce.id.unwrap_or(0)),
                                dst: udp_message.src,
                                msg_type: Some(EndpointAnnounce::id()),
                                request_id: None,
                                instance_id: None,
                                payload: Some(announce_bytes),
                            };
                            if let Ok(pkt) = fwd.to_bytes() {
                                let _ = self.node.send_unicast(&pkt, addr).await;
                            }
                        }
                    }
                }
            }
            return Ok(()); // don't forward subscribe requests
        }
        self.tx_message
            .send(MetaMessage {
                msg: udp_message,
                raw: packet.to_vec(),
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
                debug!("Waiting to receive unicast packet on unicast  ....");
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
                let mut target_ids = target_ids(&meta.msg, &self_pub.subscriptions);
                // Also include dynamic subscriptions that match
                // Skip sub.dst comparison — the DashMap key IS the target endpoint,
                // and sub.dst is not a message filter for dynamic subs.
                for dyn_subs in self_pub.dynamic_subscriptions.iter() {
                    if dyn_subs.value().iter().any(|s| {
                        let src_ok = s.src.is_none() || s.src == meta.msg.src;
                        let typ_ok = s.msg_type.is_none() || s.msg_type == meta.msg.msg_type;
                        src_ok && typ_ok
                    }) {
                        target_ids.insert(*dyn_subs.key());
                    }
                }
                for endpoint_id in target_ids {
                    if let Some(target) = self_pub.known_endpoints.get(&endpoint_id) {
                        if let Err(e) = self.node.send_unicast(&meta.raw, match &target.addr {
                            EndpointAddress::SocketAddr(addr) => *addr,
                            EndpointAddress::SerialPort(_) => {
                                warn!("Cannot send unicast to serial port endpoint");
                                continue;
                            }
                        }).await {
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

    let port = args.port.unwrap_or(msgs::MULTICAST_PORT as u16);
    let mc_addr_str = cfg
        .multicast_addr
        .clone()
        .unwrap_or("225.0.0.0".to_string());
    let group: Ipv4Addr = mc_addr_str
        .parse()
        .with_context(|| format!("failed to parse multicast address {}", mc_addr_str))?;
    info!("Using multicast group {} and port {}", group, port);

    let mut node = UdpNode::new(msgs::BROKER_ID, SocketAddr::new(group.into(), port));
    node.bind_unicast_with_port(50001).await?;
    node.bind_multicast().await?;
    let broker = Arc::new(Broker::new(node, parse_subscriptions(&cfg)));

    broker.run().await;
    Ok(())
}
