//! Unified egui dashboard for Limeros: UWB anchors/tags + ESP32 UEXT compass.
//!
//! Binds UDP 0.0.0.0:50000 (and joins multicast 239.0.0.23), decodes the
//! canonical CBOR produced by `coder` (robot.rs -> codegen_rs -> generated
//! crate) and renders:
//!   - UWB devices that announce themselves and, per tag, its anchor ranges
//!   - a compass rose pointing at the tilt-compensated heading
//!   - an attitude indicator (pitch/roll) wrapped in a rotating azimuth ring
//!   - numeric readouts for magnetometer, accelerometer and gyroscope
//!
//! On announce it replies with a DeviceRequest so devices re-point their
//! unicast events at this host (same behavior as the former uwb_anchors GUI).

use std::collections::HashMap;
use std::net::{Ipv4Addr, SocketAddr, UdpSocket};
use std::sync::mpsc;
use std::time::{Duration, Instant};

use eframe::egui;
use generated::generated::{
    id_to_string, CompassEvent, DeviceEvent, DeviceRequest, DW1000AnchorEvent, DW1000TagEvent,
    Envelope, ImuEvent, SysEvent,
};

const LISTEN_ADDR: &str = "0.0.0.0:50000";
const MULTICAST_GROUP: Ipv4Addr = Ipv4Addr::new(239, 0, 0, 23);
const SERVER_PORT: u16 = 50000;

// Instrument colors.
const SKY: egui::Color32 = egui::Color32::from_rgb(48, 110, 190);
const GROUND: egui::Color32 = egui::Color32::from_rgb(150, 92, 52);
const HORIZON: egui::Color32 = egui::Color32::WHITE;
const PLANE: egui::Color32 = egui::Color32::from_rgb(255, 205, 60);

// --- events ----------------------------------------------------------------

enum Event {
    Listening(String),
    Error(String),
    Compass { src: u32, ev: CompassEvent },
    Imu { src: u32, ev: ImuEvent },
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

// --- compass state ---------------------------------------------------------

#[derive(Default)]
struct Telemetry {
    heading: f32,
    pitch: f32,
    roll: f32,
    mag: [f32; 3],
    accel: [f32; 3],
    gyro: [f32; 3],
    src: u32,
    last_seen: Option<Instant>,
}

// --- UWB state -------------------------------------------------------------

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
    uwb_short: Option<u16>,
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
            uwb_short: None,
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

// --- helpers (ported from uwb_anchors) -------------------------------------

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

/// DW1000 short address of a device with this 8-byte EUI (last two bytes, LE).
fn uwb_short_from_mac(mac: &[u8]) -> Option<u16> {
    if mac.len() == 8 {
        Some(((mac[7] as u16) << 8) | mac[6] as u16)
    } else {
        None
    }
}

/// Natural ordering so "uwb_anchor_2" sorts before "uwb_anchor_10".
fn natural_cmp(a: &str, b: &str) -> std::cmp::Ordering {
    let split = |s: &str| {
        let digits = s.len() - s.trim_end_matches(|c: char| c.is_ascii_digit()).len();
        let (head, tail) = s.split_at(s.len() - digits);
        (head.to_lowercase(), tail.parse::<u64>().ok())
    };
    split(a).cmp(&split(b)).then_with(|| a.cmp(b))
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

// --- app -------------------------------------------------------------------

#[derive(Clone, Copy, PartialEq, Eq)]
enum Tab {
    Compass,
    Uwb,
}

struct CompassApp {
    rx: mpsc::Receiver<Event>,
    tele: Telemetry,
    devices: HashMap<u32, Device>,
    ranges: HashMap<u32, HashMap<u16, Range>>,
    listening: bool,
    listen_addr: String,
    last_error: Option<String>,
    packets: u64,
    tab: Tab,
}

impl CompassApp {
    fn new(cc: &eframe::CreationContext<'_>) -> Self {
        let (tx, rx) = mpsc::channel();
        let ctx = cc.egui_ctx.clone();
        std::thread::spawn(move || udp_loop(ctx, tx));
        Self {
            rx,
            tele: Telemetry::default(),
            devices: HashMap::new(),
            ranges: HashMap::new(),
            listening: false,
            listen_addr: LISTEN_ADDR.to_string(),
            last_error: None,
            packets: 0,
            tab: Tab::Compass,
        }
    }

    fn handle(&mut self, ev: Event) {
        let now = Instant::now();
        match ev {
            Event::Listening(addr) => {
                self.listen_addr = addr;
                self.listening = true;
            }
            Event::Error(e) => self.last_error = Some(e),
            Event::Compass { src, ev } => {
                self.packets += 1;
                self.tele.src = src;
                if let Some(v) = ev.heading {
                    self.tele.heading = v;
                }
                if let Some(v) = ev.pitch {
                    self.tele.pitch = v;
                }
                if let Some(v) = ev.roll {
                    self.tele.roll = v;
                }
                if let (Some(x), Some(y), Some(z)) = (ev.mag_x, ev.mag_y, ev.mag_z) {
                    self.tele.mag = [x, y, z];
                }
                if let (Some(x), Some(y), Some(z)) = (ev.accel_x, ev.accel_y, ev.accel_z) {
                    self.tele.accel = [x, y, z];
                }
                self.tele.last_seen = Some(now);
            }
            Event::Imu { src, ev } => {
                self.packets += 1;
                self.tele.src = src;
                if let (Some(x), Some(y), Some(z)) = (ev.gyro_x, ev.gyro_y, ev.gyro_z) {
                    self.tele.gyro = [x, y, z];
                }
                if let (Some(x), Some(y), Some(z)) = (ev.accel_x, ev.accel_y, ev.accel_z) {
                    self.tele.accel = [x, y, z];
                }
                self.tele.last_seen = Some(now);
            }
            Event::Announce {
                src,
                device,
                msg_types,
                peer_ip,
            } => {
                self.packets += 1;
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
                self.packets += 1;
                let d = self
                    .devices
                    .entry(src)
                    .or_insert_with(|| Device::new(format!("dev_{src:08X}")));
                d.uptime = uptime;
                d.last_seen = now;
            }
            Event::Anchor { src, uwb_mac } => {
                self.packets += 1;
                let d = self
                    .devices
                    .entry(src)
                    .or_insert_with(|| Device::new(format!("dev_{src:08X}")));
                d.uwb_mac = Some(hex_bytes(&uwb_mac));
                d.uwb_short = uwb_short_from_mac(&uwb_mac);
                d.kind = DeviceKind::Anchor;
                d.last_seen = now;
            }
            Event::Tag {
                src,
                uwb_mac,
                anchor,
                distance_m,
                rx_power_db,
            } => {
                self.packets += 1;
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

                self.ranges.entry(src).or_default().insert(
                    anchor,
                    Range {
                        distance_m,
                        rx_power_db,
                        last_seen: now,
                    },
                );
            }
            Event::Unknown { src, msg_type } => {
                self.packets += 1;
                self.last_error = Some(format!(
                    "undecodable {}({msg_type}) from {src:08X}",
                    id_to_string(msg_type)
                ));
            }
        }
    }

    fn anchor_name(&self, short: u16) -> Option<&str> {
        self.devices
            .values()
            .find(|d| d.uwb_short == Some(short))
            .map(|d| d.name.as_str())
    }

    fn device_label(&self, src: u32) -> String {
        self.devices
            .get(&src)
            .map(|d| d.name.clone())
            .unwrap_or_else(|| format!("{src:08X}"))
    }
}

// --- network ---------------------------------------------------------------

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
            let _ = tx.send(Event::Error(format!("bind {LISTEN_ADDR}: {e}")));
            ctx.request_repaint();
            return;
        }
    };
    if let Err(e) = socket.join_multicast_v4(&MULTICAST_GROUP, &Ipv4Addr::UNSPECIFIED) {
        let _ = tx.send(Event::Error(format!("join_multicast {MULTICAST_GROUP}: {e}")));
    }
    let _ = socket.set_nonblocking(true);
    let _ = tx.send(Event::Listening(LISTEN_ADDR.to_string()));
    ctx.request_repaint();

    let mut buf = [0u8; 2048];
    loop {
        match socket.recv_from(&mut buf) {
            Ok((n, peer)) => {
                let peer_ip = match peer {
                    SocketAddr::V4(a) => Some(a.ip().octets()),
                    _ => None,
                };
                if let Ok(env) = Envelope::from_bytes(&buf[..n]) {
                    let src = env.src.unwrap_or(0);
                    let msg_type = env.msg_type.unwrap_or(0);
                    let payload = env.payload.unwrap_or_default();

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
                std::thread::sleep(Duration::from_millis(100));
            }
        }
    }
}

fn decode_payload(src: u32, msg_type: u32, payload: &[u8], peer_ip: Option<[u8; 4]>) -> Event {
    if msg_type == CompassEvent::MSG_ID {
        if let Ok(ev) = CompassEvent::from_bytes(payload) {
            return Event::Compass { src, ev };
        }
        return Event::Unknown { src, msg_type };
    }
    if msg_type == ImuEvent::MSG_ID {
        if let Ok(ev) = ImuEvent::from_bytes(payload) {
            return Event::Imu { src, ev };
        }
        return Event::Unknown { src, msg_type };
    }
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
            Ok(s) => Event::Sys {
                src,
                uptime: s.uptime.unwrap_or(0),
            },
            Err(_) => Event::Unknown { src, msg_type },
        };
    }
    if msg_type == DW1000AnchorEvent::MSG_ID {
        return match DW1000AnchorEvent::from_bytes(payload) {
            Ok(a) => Event::Anchor {
                src,
                uwb_mac: a.uwb_mac_address.unwrap_or_default(),
            },
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

// --- rendering -------------------------------------------------------------

impl eframe::App for CompassApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        while let Ok(ev) = self.rx.try_recv() {
            self.handle(ev);
        }
        ctx.request_repaint_after(Duration::from_millis(100));

        // Drop ranging entries that haven't refreshed recently (stale anchors).
        let prune_now = Instant::now();
        self.ranges.retain(|_, anchors| {
            anchors.retain(|_, r| {
                prune_now.duration_since(r.last_seen) < std::time::Duration::from_secs(5)
            });
            !anchors.is_empty()
        });

        egui::TopBottomPanel::top("status").show(ctx, |ui| {
            ui.add_space(4.0);
            ui.horizontal(|ui| {
                ui.heading("Limero dashboard");
                ui.separator();
                if self.listening {
                    ui.colored_label(egui::Color32::GREEN, "listening");
                } else {
                    ui.colored_label(egui::Color32::YELLOW, "binding…");
                }
                ui.monospace(&self.listen_addr);
                ui.label(format!("{} pkts", self.packets));
                ui.label(format!("{} devs", self.devices.len()));
                if let Some(t) = self.tele.last_seen {
                    ui.label(format!("compass age {:.1}s", t.elapsed().as_secs_f32()));
                }
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
            .default_width(340.0)
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
            ui.horizontal(|ui| {
                ui.selectable_value(&mut self.tab, Tab::Compass, "Compass / IMU");
                ui.selectable_value(&mut self.tab, Tab::Uwb, "UWB Ranging");
            });
            ui.separator();

            match self.tab {
                Tab::Compass => {
                    if self.tele.last_seen.is_none() {
                        ui.centered_and_justified(|ui| {
                            ui.label("Waiting for CompassEvent / ImuEvent on 0.0.0.0:50000…");
                        });
                        return;
                    }
                    ui.horizontal(|ui| {
                        ui.vertical(|ui| {
                            ui.strong("Compass (heading)");
                            draw_compass(ui, self.tele.heading);
                        });
                        ui.add_space(8.0);
                        ui.vertical(|ui| {
                            ui.strong("Attitude (pitch/roll) + azimuth");
                            draw_attitude(ui, self.tele.heading, self.tele.pitch, self.tele.roll);
                        });
                    });
                    ui.separator();
                    readouts(ui, &self.tele);
                }
                Tab::Uwb => {
                    ui.heading("Ranging");
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
                                let mut rows: Vec<(Option<&str>, u16, &Range)> = anchors
                                    .iter()
                                    .map(|(&short, r)| (self.anchor_name(short), short, r))
                                    .collect();
                                rows.sort_by(|a, b| match (a.0, b.0) {
                                    (Some(x), Some(y)) => natural_cmp(x, y).then(a.1.cmp(&b.1)),
                                    (Some(_), None) => std::cmp::Ordering::Less,
                                    (None, Some(_)) => std::cmp::Ordering::Greater,
                                    (None, None) => a.1.cmp(&b.1),
                                });

                                egui::Grid::new(egui::Id::new(("tag_grid", tag)))
                                    .striped(true)
                                    .num_columns(5)
                                    .show(ui, |ui| {
                                        ui.strong("Name");
                                        ui.strong("Anchor");
                                        ui.strong("Distance");
                                        ui.strong("RX power");
                                        ui.strong("Age");
                                        ui.end_row();

                                        for (name, short, r) in rows {
                                            let age_s =
                                                now.duration_since(r.last_seen).as_secs_f32();
                                            match name {
                                                Some(n) => ui.monospace(n),
                                                None => {
                                                    ui.label(egui::RichText::new("?").weak())
                                                }
                                            };
                                            ui.monospace(hex_short(short));
                                            ui.monospace(format!("{:.2} m", r.distance_m));
                                            ui.monospace(format!("{:.1} dBm", r.rx_power_db));
                                            ui.monospace(format!("{:.1} s", age_s));
                                            ui.end_row();
                                        }
                                    });
                                ui.add_space(8.0);
                            }
                        });
                }
            }
        });
    }
}

// --- drawing (compass / attitude) ------------------------------------------

/// Compass-direction unit vector for `deg` (0 = up, clockwise on screen).
fn unit(deg: f32) -> egui::Vec2 {
    let r = deg.to_radians();
    egui::vec2(r.sin(), -r.cos())
}

fn draw_compass(ui: &mut egui::Ui, heading: f32) {
    let size = egui::vec2(230.0, 230.0);
    let (resp, painter) = ui.allocate_painter(size, egui::Sense::hover());
    let rect = resp.rect;
    let center = rect.center();
    let r = rect.width().min(rect.height()) * 0.5 - 10.0;

    painter.circle_filled(center, r, egui::Color32::from_gray(22));
    painter.circle_stroke(center, r, egui::Stroke::new(2.0_f32, egui::Color32::from_gray(120)));

    for deg in (0..360).step_by(15) {
        let d = unit(deg as f32);
        let is_card = deg % 90 == 0;
        let (len, w, c): (f32, f32, egui::Color32) = if is_card {
            (10.0, 2.5, egui::Color32::WHITE)
        } else {
            (5.0, 1.0, egui::Color32::from_gray(140))
        };
        painter.line_segment([center + d * (r - len), center + d * r], egui::Stroke::new(w, c));
    }
    for (deg, label) in [(0, "N"), (90, "E"), (180, "S"), (270, "W")] {
        let d = unit(deg as f32);
        painter.text(
            center + d * (r - 22.0),
            egui::Align2::CENTER_CENTER,
            label,
            egui::FontId::proportional(16.0),
            egui::Color32::from_gray(210),
        );
    }

    let d = unit(heading);
    painter.line_segment(
        [center - d * (r * 0.25), center + d * (r - 6.0)],
        egui::Stroke::new(3.5_f32, egui::Color32::from_rgb(255, 80, 80)),
    );
    painter.circle_filled(center, 5.0, egui::Color32::RED);
    painter.text(
        center + egui::vec2(0.0, r * 0.40),
        egui::Align2::CENTER_CENTER,
        format!("{:.1}°", heading),
        egui::FontId::proportional(20.0),
        egui::Color32::WHITE,
    );
}

/// Rotating azimuth ring: the current heading sits under the top lubber mark.
fn draw_heading_ring(painter: &egui::Painter, center: egui::Pos2, r: f32, heading: f32) {
    let ring = r + 10.0;
    for deg in (0..360).step_by(15) {
        let d = unit(deg as f32 - heading);
        let is_card = deg % 90 == 0;
        let (len, w, c): (f32, f32, egui::Color32) = if is_card {
            (9.0, 2.0, egui::Color32::WHITE)
        } else {
            (4.0, 1.0, egui::Color32::from_gray(150))
        };
        painter.line_segment(
            [center + d * (ring - len), center + d * ring],
            egui::Stroke::new(w, c),
        );
    }
    for (deg, label) in [(0, "N"), (90, "E"), (180, "S"), (270, "W")] {
        let d = unit(deg as f32 - heading);
        painter.text(
            center + d * (ring + 12.0),
            egui::Align2::CENTER_CENTER,
            label,
            egui::FontId::proportional(13.0),
            egui::Color32::from_gray(220),
        );
    }
    let top = center + egui::vec2(0.0, -(ring + 14.0));
    let tri = vec![
        top,
        top + egui::vec2(-6.0, 12.0),
        top + egui::vec2(6.0, 12.0),
    ];
    painter.add(egui::Shape::convex_polygon(
        tri,
        egui::Color32::from_rgb(255, 200, 40),
        egui::Stroke::NONE,
    ));
}

fn draw_plane(painter: &egui::Painter, center: egui::Pos2, s: f32) {
    let st = egui::Stroke::new(3.0_f32, PLANE);
    painter.line_segment(
        [center + egui::vec2(-s, 0.0), center + egui::vec2(-0.18 * s, 0.0)],
        st,
    );
    painter.line_segment(
        [center + egui::vec2(0.18 * s, 0.0), center + egui::vec2(s, 0.0)],
        st,
    );
    painter.line_segment(
        [center + egui::vec2(0.0, -0.5 * s), center + egui::vec2(0.0, 0.6 * s)],
        st,
    );
    painter.circle_filled(center, 0.07 * s, PLANE);
}

fn draw_attitude(ui: &mut egui::Ui, heading: f32, pitch: f32, roll: f32) {
    let size = egui::vec2(280.0, 280.0);
    let (resp, painter) = ui.allocate_painter(size, egui::Sense::hover());
    let rect = resp.rect;
    let center = rect.center();
    let r = rect.width().min(rect.height()) * 0.5 - 24.0;

    draw_heading_ring(&painter, center, r, heading);

    let clip = egui::Rect::from_center_size(center, egui::vec2(r * 2.0, r * 2.0));
    let hp = ui.painter_at(clip);

    let th = roll.to_radians();
    let up = egui::vec2(-th.sin(), -th.cos());
    let d = egui::vec2(th.cos(), -th.sin());
    let po = pitch.clamp(-60.0, 60.0) / 60.0 * r * 0.9;
    let p = center - up * po; // horizon line passes through p, tilted by roll
    let w = r * 3.0;
    let h = r * 3.0;

    hp.circle_filled(center, r, GROUND);
    hp.add(egui::Shape::convex_polygon(
        vec![p - d * w, p + d * w, p + d * w + up * h, p - d * w + up * h],
        SKY,
        egui::Stroke::NONE,
    ));
    hp.line_segment([p - d * w, p + d * w], egui::Stroke::new(2.0_f32, HORIZON));

    draw_plane(&hp, center, r * 0.55);

    painter.text(
        rect.left_center() + egui::vec2(10.0, 0.0),
        egui::Align2::LEFT_CENTER,
        format!("Az {:.0}°", heading),
        egui::FontId::proportional(14.0),
        egui::Color32::from_gray(200),
    );
}

fn readouts(ui: &mut egui::Ui, t: &Telemetry) {
    ui.group(|ui| {
        ui.strong("Orientation");
        ui.monospace(format!("Heading  {:>7.1} °", t.heading));
        ui.monospace(format!("Pitch    {:>7.1} °", t.pitch));
        ui.monospace(format!("Roll     {:>7.1} °", t.roll));
    });
    ui.add_space(6.0);
    ui.group(|ui| {
        ui.strong("Magnetometer (uT)");
        ui.monospace(format!("X {:>8.2}", t.mag[0]));
        ui.monospace(format!("Y {:>8.2}", t.mag[1]));
        ui.monospace(format!("Z {:>8.2}", t.mag[2]));
    });
    ui.add_space(6.0);
    ui.group(|ui| {
        ui.strong("Accelerometer (m/s²)");
        ui.monospace(format!("X {:>8.2}", t.accel[0]));
        ui.monospace(format!("Y {:>8.2}", t.accel[1]));
        ui.monospace(format!("Z {:>8.2}", t.accel[2]));
    });
    ui.add_space(6.0);
    ui.group(|ui| {
        ui.strong("Gyroscope (deg/s)");
        ui.monospace(format!("X {:>8.2}", t.gyro[0]));
        ui.monospace(format!("Y {:>8.2}", t.gyro[1]));
        ui.monospace(format!("Z {:>8.2}", t.gyro[2]));
    });
}

fn main() {
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_inner_size([1200.0, 760.0]),
        ..Default::default()
    };

    eframe::run_native(
        "Limero dashboard (compass + UWB)",
        options,
        Box::new(|cc| Box::new(CompassApp::new(cc))),
    )
    .expect("failed to start eframe");
}
