use anyhow::{Context, Result};
use clap::Parser;
use fnv::FnvHasher;
use std::collections::{HashMap, HashSet};
use std::hash::{Hash, Hasher};
use std::net::{Ipv4Addr, SocketAddr};
use tokio::net::UdpSocket;

use common::{RobotConfig, load_robot_config};
use generated::{BROKER_ID, BytesSerde, EndpointAnnounce, UdpMessage, generated};

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

fn decode_endpoint_announce(payload: Option<&[u8]>) -> Option<EndpointAnnounce> {
    let data = payload?;
    Some(EndpointAnnounce::from_bytes(data)?)
}

fn decode_udp_message(packet: &[u8]) -> Option<UdpMessage> {
    Some(UdpMessage::from_bytes(packet)?)
}

fn target_ids(msg: &UdpMessage, subscriptions: &[Subscription]) -> HashSet<u32> {
    subscriptions
        .iter()
        .filter(|s| matches_subscription(msg, s))
        .filter_map(|s| s.dst)
        .collect()
}

struct Node {
    id: u32,
    udp_addr: Option<SocketAddr>,
    mc_addr: SocketAddr,
    udp_socket: Option<UdpSocket>,
    mc_socket: Option<UdpSocket>,
}

impl Node {
    fn new(id: u32, mc_addr: SocketAddr) -> Self {
        Node {
            id,
            udp_addr: None,
            mc_addr,
            udp_socket: None,
            mc_socket: None,
        }
    }
    async fn bind_udp(&mut self, bind_addr: &str) -> Result<()> {
        let socket = UdpSocket::bind(bind_addr)
            .await
            .with_context(|| format!("failed to bind UDP socket to {}", bind_addr))?;
        self.udp_addr = Some(socket.local_addr()?);
        self.udp_socket = Some(socket);
        Ok(())
    }

    async fn join_multicast(&mut self, group: Ipv4Addr) -> Result<()> {
        if let Some(socket) = &self.udp_socket {
            socket
                .join_multicast_v4(group, Ipv4Addr::UNSPECIFIED)
                .with_context(|| format!("failed to join multicast group {}", group))?;
            self.mc_socket = Some(*socket.clone());
        }
        Ok(())
    }

    async fn send_multicast(&self, data: &[u8]) -> Result<()> {
        if let Some(socket) = &self.mc_socket {
            socket
                .send_to(data, self.mc_addr)
                .await
                .with_context(|| format!("failed to send multicast to {}", self.mc_addr))?;
        }
        Ok(())
    }

    async fn send_udp(&self, data: &[u8], addr: SocketAddr) -> Result<()> {
        if let Some(socket) = &self.udp_socket {
            socket
                .send_to(data, addr)
                .await
                .with_context(|| format!("failed to send UDP to {}", addr))?;
        }
        Ok(())
    }

    async fn receive_udp(&self, buf: &mut [u8]) -> Result<(usize, SocketAddr)> {
        if let Some(socket) = &self.udp_socket {
            let (len, addr) = socket
                .recv_from(buf)
                .await
                .with_context(|| "failed to receive UDP packet")?;
            Ok((len, addr))
        } else {
            Err(anyhow::anyhow!("UDP socket is not bound"))
        }
    }

    async fn receive_multicast(&self, buf: &mut [u8]) -> Result<(usize, SocketAddr)> {
        if let Some(socket) = &self.mc_socket {
            let (len, addr) = socket
                .recv_from(buf)
                .await
                .with_context(|| "failed to receive multicast packet")?;
            Ok((len, addr))
        } else {
            Err(anyhow::anyhow!("Multicast socket is not joined"))
        }
    }
}

async fn my_broker_loop(node: &mut Node, subscriptions: &[Subscription]) -> Result<UdpMessage> {
    let mut announce_buf = vec![0_u8; 64 * 1024];
    let mut route_buf = vec![0_u8; 64 * 1024];
    let last_msg: Result<UdpMessage> ;

    tokio::select! {
        recv = node.receive_multicast(&mut announce_buf) => {
            let (len, addr) = recv?;

            println!("Received multicast packet of length {} from {}", len, addr);
            // Process the multicast packet here
            // update known endpoints, forward to subscribed endpoints, etc.
            let packet = &announce_buf[..len];
            last_msg = UdpMessage::from_bytes(packet).ok_or_else(|| anyhow::anyhow!("Failed to decode multicast packet"));
    }
        recv = node.receive_udp(&mut route_buf) => {
            let (len, addr) = recv?;
            println!("Received UDP packet of length {} from {}", len, addr);
            // Process the UDP packet here
            // check subscriptions and forward
            let packet = &route_buf[..len];
            last_msg = UdpMessage::from_bytes(packet).ok_or_else(|| anyhow::anyhow!("Failed to decode route packet"));
    }
        }
    last_msg
}

#[tokio::main]
async fn main() -> Result<()> {
    let args = Args::parse();

    let cfg =
        load_robot_config(&args.input).with_context(|| format!("failed to load {}", args.input))?;

    let port = args.port.unwrap_or(generated::MULTICAST_PORT as u16);
    let bind_addr = format!("{}:{}", args.bind, port);
    let group: Ipv4Addr = args.group.parse().context("invalid multicast group")?;

    let node = Node::new(BROKER_ID, SocketAddr::new(group.into(), port));

    let multicast_socket = UdpSocket::bind(&bind_addr)
        .await
        .with_context(|| format!("failed to bind {}", bind_addr))?;
    multicast_socket
        .join_multicast_v4(group, Ipv4Addr::UNSPECIFIED)
        .context("failed to join multicast group")?;

    // Routing traffic is handled on an independent UDP socket bound to a random local port.
    let route_socket = UdpSocket::bind(format!("{}:0", args.bind))
        .await
        .context("failed to bind routing udp socket")?;
    let route_local = route_socket
        .local_addr()
        .context("failed to read routing socket local addr")?;

    let subscriptions = parse_subscriptions(&cfg);
    let mut known_endpoints: HashMap<u32, KnownEndpoint> = HashMap::new();
    let mut seen_ids: HashSet<u32> = HashSet::new();
    let endpoint_announce_id = generated::EndpointAnnounce::ID;

    println!(
        "announce socket={} (group {}), route socket={}, subscriptions={}",
        bind_addr,
        group,
        route_local,
        subscriptions.len()
    );

    let mut announce_buf = vec![0_u8; 64 * 1024];
    let mut route_buf = vec![0_u8; 64 * 1024];

    loop {
        tokio::select! {
            recv = multicast_socket.recv_from(&mut announce_buf) => {
                let (len, peer): (usize, SocketAddr) = recv?;
                let packet = &announce_buf[..len];
                let msg: UdpMessage = match decode_udp_message(packet) {
                    Some(v) => v,
                    None => {
                        eprintln!("drop invalid announce packet from {}", peer);
                        continue;
                    }
                };

                if msg.msg_type == Some(endpoint_announce_id) {
                    if let Some(announce) = decode_endpoint_announce(msg.payload.as_deref()) {
                        if let Some(endpoint_id) = announce.endpoint_id {
                            known_endpoints.insert(
                                endpoint_id,
                                KnownEndpoint {
                                    announce: announce.clone(),
                                    addr: peer,
                                },
                            );
                            if seen_ids.insert(endpoint_id) {
                                println!(
                                    "endpoint announce id={} name={:?} device={:?} addr={}",
                                    endpoint_id, announce.endpoint_name, announce.device_name, peer
                                );
                            }
                        }
                    }
                }
            }
            recv = route_socket.recv_from(&mut route_buf) => {
                let (len, peer): (usize, SocketAddr) = recv?;
                let packet = &route_buf[..len];
                let msg: UdpMessage = match decode_udp_message(packet) {
                    Some(v) => v,
                    None => {
                        eprintln!("drop invalid route packet from {}", peer);
                        continue;
                    }
                };

                if msg.msg_type == Some(endpoint_announce_id) {
                    // Announce traffic must be handled on multicast socket only.
                    continue;
                }

                let target_ids = target_ids(&msg, &subscriptions);
                if !target_ids.is_empty() {
                    println!(
                        "route src={:?} dst={:?} type={:?} req_id={:?}",
                        msg.src, msg.dst, msg.msg_type, msg.req_id
                    );
                }

                for endpoint_id in target_ids {
                    if let Some(target) = known_endpoints.get(&endpoint_id) {
                        if let Err(e) = route_socket.send_to(packet, target.addr).await {
                            eprintln!(
                                "forward to endpoint {} at {} ({:?}) failed: {}",
                                endpoint_id, target.addr, target.announce.endpoint_name, e
                            );
                        }
                    } else {
                        eprintln!("no known endpoint address for destination id {}", endpoint_id);
                    }
                }
            }
        }
    }
}
