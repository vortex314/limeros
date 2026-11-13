//! egui dashboard that acts as the central UDP server for the UWB devices.
//!
//! It binds UDP port 50000, joins the multicast group 239.0.0.23:50000, and
//! renders the devices that announce themselves plus, for every tag, the
//! anchors it is ranging with their latest distance and RX power.
//!
//! The wire format is the canonical CBOR produced by `coder` (robot.rs ->
//! codegen_rs -> generated crate). This crate decodes with that same crate so
//! it always matches the firmware (uwb_proto.h / msgs.cpp).

use std::collections::HashMap;
use std::net::{Ipv4Addr, SocketAddr, UdpSocket};
use std::sync::mpsc;
use std::time::{Duration, Instant};

use eframe::egui;
use generated::generated::{
    id_to_string, DeviceEvent, DeviceRequest, DW1000AnchorEvent, DW1000TagEvent, Envelope, SysEvent,
};

const LISTEN_ADDR: &str = "0.0.0.0:50000";
const MULTICAST_GROUP: Ipv4Addr = Ipv4Addr::new(239, 0, 0, 23);
const SERVER_PORT: u16 = 50000;

// --- events ----------------------------------------------------------------

#[derive(Debug)]
enum Event {
    Listening { addr: String },
    Error(String),
    Announce {
        src: u32,
        device: String,
        msg_types: Vec<u32>,
        peer_ip: Option<[u8; 4]>,
    },
    Sys { src: u32, uptime: u64 },
    Anchor { src: u32, uwb_mac: Vec<u8> },
    Tag {
        src: u32,
        uwb_mac: Vec<u8>,
        anchor: u16,
        distance_m: f32,
        rx_power_db: f32,
    },
    Unknown { src: u32, msg_type: u32 },
}

fn decode_payload(src: u32, msg_type: u32, payload: &[u8], peer_ip: Option<[u8; 4]>) -> Event {
    if msg_type == DeviceEvent::MSG_ID {
        return match DeviceEvent::from_bytes(payload) {
            Ok(d) => Event::Announce {
                src,
                device: d.device_name.unwrap_or_default(),
                msg_types: d.request_types.unwrap_or_default(),
                peer_ip,
            },
            Err(_) => Event::Unknown { src, msg_type },
        };
    }
    if msg_type == SysEvent::MSG_ID {
        return match SysEvent::from_bytes(payload) {
            Ok(s) => Event::Sys { src, uptime: s.uptime.unwrap_or(0) },
            Err(_) => Event::Unknown { src, msg_type },
        };
    }
    if msg_type == DW1000AnchorEvent::MSG_ID {
        return match DW1000AnchorEvent::from_bytes(payload) {
            Ok(a) => Event::Anchor { src, uwb_mac: a.uwb_mac_address.unwrap_or_default() },
            Err(_) => Event::Unknown { src, msg_type },
        };
    }
    if msg_type == DW1000TagEvent::MSG_ID {
        return match DW1000TagEvent::from_bytes(payload) {
            Ok(t) => Event::Tag {
                src,
                uwb_mac: t.uwb_mac_address.unwrap_or_default(),
                anchor: t.anchor.unwrap_or(0),
                distance_m: t.distance_m.unwrap_or(0.0),
                rx_power_db: t.rx_power_db.unwrap_or(0.0),
            },
            Err(_) => Event::Unknown { src, msg_type },
        };
    }
    Event::Unknown { src, msg_type }
}

// --- helpers ---------------------------------------------------------------

fn fmt_ip(ip: Option<[u8; 4]>) -> String {
    match ip {
        Some(b) => format!("{}.{}.{}.{}", b[0], b[1], b[2], b[3]),
        None => "?".to_string(),
    }
}

fn hex_bytes(b: &[u8]) -> String {
    b.iter().map(|x| format!("{x:02X}")).collect::<Vec<_>>().join(":")
}

fn hex_short(v: u16) -> String {
    format!("0x{v:04X}")
}

/// Local IPv4 for the interface that reaches `peer` (set-broker reply).
fn local_ipv4_for(peer: SocketAddr) -> Option<[u8; 4]> {
    let s = UdpSocket::bind("0.0.0.0:0").ok()?;
    s.connect(peer).ok()?;
    match s.local_addr().ok()? {
        SocketAddr::V4(a) => Some(a.ip().octets()),
        _ => None,
    }
}

// --- live state ------------------------------------------------------------

#[derive(Clone, Copy, PartialEq, Eq)]
enum DeviceKind {
    Tag,
    Anchor,
    Unknown,
}

struct Device {
    name: String,
    kind: DeviceKind,
    ip: Option<[u8; 4]>,
    msg_types: Vec<u32>,
    uwb_mac: Option<String>,
    uptime: u64,
    last_seen: Instant,
}

impl Device {
    fn new(name: String) -> Self {
        Self {
            name,
            kind: DeviceKind::Unknown,
            ip: None,
            msg_types: Vec::new(),
            uwb_mac: None,
            uptime: 0,
            last_seen: Instant::now(),
        }
    }
}

struct Range {
    distance_m: f32,
    rx_power_db: f32,
    last_seen: Instant,
}

struct UwbApp {
    rx: mpsc::Receiver<Event>,
    devices: HashMap<u32, Device>,
    ranges: HashMap<u32, HashMap<String, Range>>,
    listening: bool,
    listen_addr: String,
    last_error: Option<String>,
    packets: u64,
}

impl UwbApp {
    fn new(cc: &eframe::CreationContext<'_>) -> Self {
        let (tx, rx) = mpsc::channel();
        let ctx = cc.egui_ctx.clone();
        std::thread::spawn(move || udp_loop(ctx, tx));

        Self {
            rx,
            devices: HashMap::new(),
            ranges: HashMap::new(),
            listening: false,
            listen_addr: LISTEN_ADDR.to_string(),
            last_error: None,
            packets: 0,
        }
    }

    fn handle(&mut self, ev: Event) {
        self.packets += 1;
        let now = Instant::now();
        match ev {
            Event::Listening { addr } => {
                self.listen_addr = addr;
                self.listening = true;
            }
            Event::Error(e) => self.last_error = Some(e),
            Event::Announce { src, device, msg_types, peer_ip } => {
                let d = self
                    .devices
                    .entry(src)
                    .or_insert_with(|| Device::new(format!("dev_{src:08X}")));
                d.name = device;
                d.ip = peer_ip;
                d.msg_types = msg_types;
                d.kind = if d.msg_types.contains(&DW1000TagEvent::MSG_ID) {
                    DeviceKind::Tag
                } else if d.msg_types.contains(&DW1000AnchorEvent::MSG_ID) {
                    DeviceKind::Anchor
                } else {
                    DeviceKind::Unknown
                };
                d.last_seen = now;
            }
            Event::Sys { src, uptime } => {
                let d = self
                    .devices
                    .entry(src)
                    .or_insert_with(|| Device::new(format!("dev_{src:08X}")));
                d.uptime = uptime;
                d.last_seen = now;
            }
            Event::Anchor { src, uwb_mac } => {
                let d = self
                    .devices
                    .entry(src)
                    .or_insert_with(|| Device::new(format!("dev_{src:08X}")));
                d.uwb_mac = Some(hex_bytes(&uwb_mac));
                d.kind = DeviceKind::Anchor;
                d.last_seen = now;
            }
            Event::Tag { src, uwb_mac, anchor, distance_m, rx_power_db } => {
                let mac_hex = hex_bytes(&uwb_mac);
                let d = self
                    .devices
                    .entry(src)
                    .or_insert_with(|| Device::new(mac_hex.clone()));
                d.uwb_mac = Some(mac_hex.clone());
                d.kind = DeviceKind::Tag;
                if d.name.starts_with("dev_") {
                    d.name = mac_hex.clone();
                }
                d.last_seen = now;

                let key = hex_short(anchor);
                self.ranges.entry(src).or_default().insert(
                    key,
                    Range {
                        distance_m,
                        rx_power_db,
                        last_seen: now,
                    },
                );
            }
            Event::Unknown { src, msg_type } => {
                self.last_error = Some(format!(
                    "undecodable {}({msg_type}) from {src:08X}",
                    id_to_string(msg_type)
                ));
            }
        }
    }

    fn device_label(&self, src: u32) -> String {
        self.devices
            .get(&src)
            .map(|d| d.name.clone())
            .unwrap_or_else(|| format!("{src:08X}"))
    }
}

// --- network ---------------------------------------------------------------

/// On announce, reply with a canonical DeviceRequest so the device re-points
/// its unicast events at this GUI's ip:port.
fn send_set_broker(socket: &UdpSocket, peer: SocketAddr, my_ip: [u8; 4]) {
    let req = DeviceRequest {
        ip_address: Some(my_ip.to_vec()),
        port: Some(SERVER_PORT as u32),
        device_name: None,
    };
    let env = Envelope {
        src: Some(0),
        dst: None,
        msg_type: Some(DeviceRequest::MSG_ID),
        sequence: None,
        instance_id: None,
        payload: Some(req.to_bytes().unwrap_or_default()),
    };
    if let Ok(bytes) = env.to_bytes() {
        let _ = socket.send_to(&bytes, peer);
    }
}

fn udp_loop(ctx: egui::Context, tx: mpsc::Sender<Event>) {
    let socket = match UdpSocket::bind(LISTEN_ADDR) {
        Ok(s) => s,
        Err(e) => {
            eprintln!("UDP bind failed: {e}");
            let _ = tx.send(Event::Error(format!("bind {LISTEN_ADDR}: {e}")));
            ctx.request_repaint();
            return;
        }
    };
    if let Err(e) = socket.join_multicast_v4(&MULTICAST_GROUP, &Ipv4Addr::UNSPECIFIED) {
        let _ = tx.send(Event::Error(format!("join_multicast {MULTICAST_GROUP}: {e}")));
    }
    if let Err(e) = socket.set_nonblocking(true) {
        let _ = tx.send(Event::Error(format!("set_nonblocking: {e}")));
    }

    let _ = tx.send(Event::Listening { addr: LISTEN_ADDR.to_string() });
    ctx.request_repaint();

    let mut buf = [0u8; 2048];
    loop {
        match socket.recv_from(&mut buf) {
            Ok((n, peer)) => {
                let peer_ip = match peer {
                    SocketAddr::V4(a) => Some(a.ip().octets()),
                    _ => None,
                };
                if let Ok(envelope) = Envelope::from_bytes(&buf[..n]) {
                    let src = envelope.src.unwrap_or(0);
                    let msg_type = envelope.msg_type.unwrap_or(0);
                    let payload = envelope.payload.unwrap_or_default();

                    if msg_type == DeviceEvent::MSG_ID {
                        if let Some(ip) = local_ipv4_for(peer) {
                            send_set_broker(&socket, peer, ip);
                        }
                    }

                    let ev = decode_payload(src, msg_type, &payload, peer_ip);
                    let _ = tx.send(ev);
                    ctx.request_repaint();
                }
            }
            Err(ref e) if e.kind() == std::io::ErrorKind::WouldBlock => {
                std::thread::sleep(Duration::from_millis(1));
            }
            Err(e) => {
                let _ = tx.send(Event::Error(format!("recv: {e}")));
                ctx.request_repaint();
                std::thread::sleep(Duration::from_millis(100));
            }
        }
    }
}

impl eframe::App for UwbApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        while let Ok(ev) = self.rx.try_recv() {
            self.handle(ev);
        }

        egui::TopBottomPanel::top("status").show(ctx, |ui| {
            ui.add_space(4.0);
            ui.horizontal(|ui| {
                ui.heading("UWB UDP dashboard");
                ui.separator();
                ui.label("UDP");
                if self.listening {
                    ui.colored_label(egui::Color32::GREEN, "listening");
                } else {
                    ui.colored_label(egui::Color32::YELLOW, "binding…");
                }
                ui.monospace(&self.listen_addr);
                ui.label(format!("{} pkts", self.packets));
            });
            if let Some(e) = &self.last_error {
                ui.horizontal(|ui| {
                    ui.colored_label(egui::Color32::LIGHT_RED, "last event:");
                    ui.label(e);
                });
            }
            ui.add_space(4.0);
        });

        egui::SidePanel::left("devices")
            .resizable(true)
            .default_width(360.0)
            .show(ctx, |ui| {
                ui.heading("Devices");
                ui.separator();
                if self.devices.is_empty() {
                    ui.label("No devices announced yet.");
                    return;
                }

                let mut ids: Vec<u32> = self.devices.keys().copied().collect();
                ids.sort();

                egui::ScrollArea::vertical()
                    .auto_shrink([false, false])
                    .show(ui, |ui| {
                        for id in ids {
                            let d = &self.devices[&id];
                            ui.group(|ui| {
                                ui.horizontal(|ui| {
                                    ui.strong(&d.name);
                                    let (color, label) = match d.kind {
                                        DeviceKind::Tag => (egui::Color32::LIGHT_BLUE, "tag"),
                                        DeviceKind::Anchor => (egui::Color32::LIGHT_GREEN, "anchor"),
                                        DeviceKind::Unknown => (egui::Color32::GRAY, "?"),
                                    };
                                    ui.colored_label(color, label);
                                    ui.with_layout(
                                        egui::Layout::right_to_left(egui::Align::Center),
                                        |ui| {
                                            ui.monospace(format!(
                                                "{:.0}s",
                                                d.last_seen.elapsed().as_secs_f32()
                                            ));
                                        },
                                    );
                                });
                                ui.monospace(format!("ip {}", fmt_ip(d.ip)));
                                ui.label(format!("up {} s", d.uptime / 1000));
                                if let Some(mac) = &d.uwb_mac {
                                    ui.monospace(format!("uwb {mac}"));
                                }
                                if !d.msg_types.is_empty() {
                                    let s = d
                                        .msg_types
                                        .iter()
                                        .map(|&t| id_to_string(t))
                                        .collect::<Vec<_>>()
                                        .join(", ");
                                    ui.label(egui::RichText::new(s).weak());
                                }
                            });
                            ui.add_space(4.0);
                        }
                    });
            });

        egui::CentralPanel::default().show(ctx, |ui| {
            ui.heading("Ranging");
            ui.separator();
            if self.ranges.is_empty() {
                ui.label("Waiting for UWB tag events…");
                return;
            }

            let mut tags: Vec<u32> = self.ranges.keys().copied().collect();
            tags.sort();

            let now = Instant::now();
            egui::ScrollArea::vertical()
                .auto_shrink([false, false])
                .show(ui, |ui| {
                    for tag in tags {
                        ui.strong(format!("tag {}", self.device_label(tag)));
                        let anchors = &self.ranges[&tag];
                        let mut keys: Vec<&String> = anchors.keys().collect();
                        keys.sort();

                        egui::Grid::new(egui::Id::new(("tag_grid", tag)))
                            .striped(true)
                            .num_columns(4)
                            .show(ui, |ui| {
                                ui.strong("Anchor");
                                ui.strong("Distance");
                                ui.strong("RX power");
                                ui.strong("Age");
                                ui.end_row();

                                for key in keys {
                                    let r = &anchors[key];
                                    let age_s = now.duration_since(r.last_seen).as_secs_f32();
                                    ui.monospace(key.as_str());
                                    ui.monospace(format!("{:.2} m", r.distance_m));
                                    ui.monospace(format!("{:.1} dBm", r.rx_power_db));
                                    ui.monospace(format!("{:.1} s", age_s));
                                    ui.end_row();
                                }
                            });
                        ui.add_space(8.0);
                    }
                });
        });
    }
}

fn main() {
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_inner_size([900.0, 600.0]),
        ..Default::default()
    };

    eframe::run_native(
        "UWB UDP dashboard",
        options,
        Box::new(|cc| Box::new(UwbApp::new(cc))),
    )
    .expect("failed to start eframe");
}

// ---------------------------------------------------------------------------
// tests: decode the firmware's actual CBOR bytes (uwb_proto.h / msgs.cpp)
// ---------------------------------------------------------------------------

#[cfg(test)]
mod tests {
    use super::*;

    fn hex_decode(s: &str) -> Vec<u8> {
        (0..s.len())
            .step_by(2)
            .map(|i| u8::from_str_radix(&s[i..i + 2], 16).expect("hex"))
            .collect()
    }

    #[test]
    fn decode_firmware_announce_bytes() {
        let bytes = hex_decode(
            "861a1b805b83f61a8036fa81f6f65824836c7577625f616e63686f725f31821a371e75021ab0385794821a371e75021ab0385794",
        );
        let env = Envelope::from_bytes(&bytes).expect("decode envelope");
        assert_eq!(env.msg_type, Some(DeviceEvent::MSG_ID));

        let dev = DeviceEvent::from_bytes(&env.payload.unwrap()).expect("decode payload");
        assert_eq!(dev.device_name.as_deref(), Some("uwb_anchor_1"));
        assert_eq!(
            dev.request_types,
            Some(vec![SysEvent::MSG_ID, DW1000AnchorEvent::MSG_ID])
        );
        assert_eq!(
            dev.event_types,
            Some(vec![SysEvent::MSG_ID, DW1000AnchorEvent::MSG_ID])
        );
    }

    #[test]
    fn decode_firmware_tag_event_bytes() {
        let bytes = hex_decode(
            "861a67113736f61a81758649f6f65784487d0022ea82603b9c19df97fa3e947ae1fac2a83333",
        );
        let env = Envelope::from_bytes(&bytes).expect("decode envelope");
        assert_eq!(env.msg_type, Some(DW1000TagEvent::MSG_ID));

        let tag = DW1000TagEvent::from_bytes(&env.payload.unwrap()).expect("decode payload");
        assert_eq!(tag.anchor, Some(0xdf97));
        assert!((tag.distance_m.unwrap() - 0.29).abs() < 0.001);
        assert!((tag.rx_power_db.unwrap() - (-84.1)).abs() < 0.01);
    }
}