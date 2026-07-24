// serial_bridge — egui dashboard for hoverboard telemetry and control over a serial port.
//
// Wire protocol (mirrors hoverboard_actor.cpp / codec.cpp):
//   SEND:  Envelope → CBOR → CRC-16(CCITT, big-endian) → COBS → 0x00 → serial
//   RECV:  serial → buffer till 0x00 → COBS decode → verify CRC-16 → CBOR decode Envelope → payload

mod codec;

use std::io;
use std::io::Write;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::{Duration, Instant};

use anyhow::Context;
use clap::Parser;
use eframe::egui;
use generated::generated::{
    Envelope, HoverboardEvent, HoverboardRequest, Msg,
};

// ── CLI ────────────────────────────────────────────────────────────────────

#[derive(Parser, Debug)]
#[command(about = "Serial bridge for Limeros eventbus over UART — egui dashboard")]
struct Args {
    /// Serial port device, e.g. /dev/ttyUSB0
    #[arg(short, long)]
    port: String,

    /// Baud rate (default: 115200)
    #[arg(short, long, default_value_t = 115200)]
    baud: u32,
}

// ── Shared state ───────────────────────────────────────────────────────────

/// App state shared between serial threads and the egui UI.
struct AppState {
    /// (speed, steer) for periodic HoverboardRequest sending.
    hb_state: Arc<Mutex<(i32, i32)>>,
    /// Latest HoverboardEvent received from serial.
    latest_event: Arc<Mutex<Option<HoverboardEvent>>>,
    /// Timestamp of last received event.
    _last_event_time: Arc<Mutex<Option<Instant>>>,
    /// Whether the serial link is alive (event within 5s).
    connected: Arc<Mutex<bool>>,
}

// ── main ───────────────────────────────────────────────────────────────────

fn main() -> anyhow::Result<()> {
    env_logger::init();
    let args = Args::parse();

    let running = Arc::new(AtomicBool::new(true));

    // Shared state
    let hb_state = Arc::new(Mutex::new((0i32, 0i32)));
    let latest_event = Arc::new(Mutex::new(None::<HoverboardEvent>));
    let last_event_time = Arc::new(Mutex::new(None::<Instant>));
    let connected = Arc::new(Mutex::new(false));

    // Open serial port
    let port = serialport::new(&args.port, args.baud)
        .timeout(Duration::from_millis(50))
        .open()
        .with_context(|| format!("Failed to open serial port {}", args.port))?;

    let mut reader = port.try_clone().context("Failed to clone serial port (reader)")?;
    let mut sender_port = port.try_clone().context("Failed to clone serial port (sender)")?;

    let src_id = common_hash("serial_bridge");

    // ── Reader thread ──
    let reader_running = running.clone();
    let reader_event = latest_event.clone();
    let reader_time = last_event_time.clone();
    let reader_connected = connected.clone();
    thread::spawn(move || {
        let mut buf: Vec<u8> = Vec::with_capacity(1024);
        let mut tmp = [0u8; 256];
        while reader_running.load(Ordering::Relaxed) {
            match reader.read(&mut tmp) {
                Ok(0) => continue,
                Ok(n) => {
                    for &b in &tmp[..n] {
                        if b == 0x00 {
                            if buf.is_empty() {
                                continue;
                            }
                            match codec::decode_frame(&buf) {
                                Ok(envelope) => {
                                    handle_envelope(
                                        &envelope,
                                        &reader_event,
                                        &reader_time,
                                        &reader_connected,
                                    );
                                }
                                Err(e) => log::warn!("Frame decode error: {e}"),
                            }
                            buf.clear();
                        } else {
                            buf.push(b);
                            if buf.len() > 4096 {
                                log::warn!("Oversized frame (>4096), discarding");
                                buf.clear();
                            }
                        }
                    }
                }
                Err(ref e) if e.kind() == io::ErrorKind::TimedOut => continue,
                Err(e) => {
                    log::error!("Serial read error: {e}");
                    break;
                }
            }
        }
    });

    // ── Periodic sender thread ──
    let sender_running = running.clone();
    let sender_hb = hb_state.clone();
    thread::spawn(move || {
        while sender_running.load(Ordering::Relaxed) {
            let (speed, steer) = {
                let guard = sender_hb.lock().unwrap();
                *guard
            };
            let req = HoverboardRequest {
                speed: Some(speed),
                steer: Some(steer),
                req_id: None,
            };
            if let Err(e) = send_message(&mut sender_port, &req, src_id) {
                log::error!("Periodic send error: {e}");
            }
            thread::sleep(Duration::from_millis(500));
        }
    });

    // ── egui app ──
    let app_state = AppState {
        hb_state,
        latest_event,
        _last_event_time: last_event_time,
        connected,
    };

    let native_options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default()
            .with_inner_size([900.0, 700.0])
            .with_title(format!("serial_bridge — {}", args.port)),
        ..Default::default()
    };

    eframe::run_native(
        "serial_bridge",
        native_options,
        Box::new(|_cc| Ok(Box::new(HoverboardDashboard::new(app_state)))),
    )
    .map_err(|e| anyhow::anyhow!("eframe error: {e}"))?;

    running.store(false, Ordering::Relaxed);
    Ok(())
}

// ── egui app ───────────────────────────────────────────────────────────────

struct HoverboardDashboard {
    state: AppState,
}

impl HoverboardDashboard {
    fn new(state: AppState) -> Self {
        Self { state }
    }
}

impl eframe::App for HoverboardDashboard {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        // Check connection health
        let connected = {
            let guard = self.state.connected.lock().unwrap();
            *guard
        };

        // Read latest event
        let event = {
            let guard = self.state.latest_event.lock().unwrap();
            guard.clone()
        };

        // Read current hb_state for sliders
        let (mut speed_val, mut steer_val) = {
            let guard = self.state.hb_state.lock().unwrap();
            *guard
        };

        // ── Top bar: connection status + sliders ──
        egui::TopBottomPanel::top("top_panel").show(ctx, |ui| {
            ui.horizontal(|ui| {
                // Connection indicator
                if connected {
                    ui.label(egui::RichText::new("● Connected").color(egui::Color32::GREEN));
                } else {
                    ui.label(egui::RichText::new("● Disconnected").color(egui::Color32::RED));
                }
                ui.separator();

                ui.add(
                    egui::Slider::new(&mut speed_val, -800..=800)
                        .text("Speed")
                        .step_by(1.0),
                );
                ui.add(
                    egui::Slider::new(&mut steer_val, -800..=800)
                        .text("Steer")
                        .step_by(1.0),
                );
                if ui.button("⏹ Zero").clicked() {
                    speed_val = 0;
                    steer_val = 0;
                }
            });

            // Write back slider values
            {
                let mut guard = self.state.hb_state.lock().unwrap();
                guard.0 = speed_val;
                guard.1 = steer_val;
            }

            ui.label(format!(
                "Sending HoverboardRequest at 2 Hz → speed={}  steer={}",
                speed_val, steer_val,
            ));
        });

        // ── Central area: dashboard ──
        egui::CentralPanel::default().show(ctx, |ui| {
            if let Some(ref ev) = event {
                egui::ScrollArea::vertical().show(ui, |ui| {
                    ui.heading("Dashboard");
                    ui.separator();

                    // ── Motor ──
                    ui.collapsing("⚙ Motor", |ui| {
                        ui.horizontal(|ui| {
                            ui.vertical(|ui| labeled_value(ui, "Cmd Left", ev.cmdl, "RPM"));
                            ui.vertical(|ui| labeled_value(ui, "Cmd Right", ev.cmdr, "RPM"));
                            ui.vertical(|ui| labeled_value(ui, "Speed Left", ev.spdl, "RPM"));
                            ui.vertical(|ui| labeled_value(ui, "Speed Right", ev.spdr, "RPM"));
                            ui.vertical(|ui| labeled_value(ui, "Speed Avg", ev.spd_avg, "RPM"));
                        });
                        ui.separator();
                        ui.horizontal(|ui| {
                            ui.vertical(|ui| labeled_value(ui, "RPM Max", ev.rpm_mot_max, "RPM"));
                            ui.vertical(|ui| labeled_value(ui, "Current Max", ev.cur_mot_max, "A"));
                            ui.vertical(|ui| labeled_value(ui, "Ctrl Mode", ev.ctrl_mod, ""));
                            ui.vertical(|ui| labeled_value(ui, "Ctrl Type", ev.ctrl_typ, ""));
                        });
                    });

                    // ── Power ──
                    ui.collapsing("⚡ Power & Temperature", |ui| {
                        ui.horizontal(|ui| {
                            ui.vertical(|ui| {
                                labeled_value_scaled(ui, "Battery", ev.batv, 100.0, "V");
                            });
                            ui.vertical(|ui| {
                                labeled_value_scaled(ui, "Temperature", ev.temp, 10.0, "°C");
                            });
                            ui.vertical(|ui| {
                                labeled_value_scaled(ui, "DC Current", ev.dc_curr, 100.0, "A");
                            });
                            ui.vertical(|ui| {
                                labeled_value_scaled(ui, "Left DC I", ev.ldc_curr, 100.0, "A");
                            });
                            ui.vertical(|ui| {
                                labeled_value_scaled(ui, "Right DC I", ev.rdc_curr, 100.0, "A");
                            });
                        });
                    });

                    // ── Field Weakening ──
                    ui.collapsing("🧲 Field Weakening", |ui| {
                        ui.horizontal(|ui| {
                            ui.vertical(|ui| labeled_value(ui, "FI Enable", ev.fi_weak_ena, ""));
                            ui.vertical(|ui| labeled_value(ui, "FI High RPM", ev.fi_weak_hi, "RPM"));
                            ui.vertical(|ui| labeled_value(ui, "FI Low RPM", ev.fi_weak_lo, "RPM"));
                            ui.vertical(|ui| labeled_value(ui, "FI Max A", ev.fi_weak_max, "A"));
                            ui.vertical(|ui| labeled_value(ui, "Phase Adv Max", ev.phase_adv_max_deg, "°"));
                        });
                    });

                    // ── Coefficients ──
                    ui.collapsing("📐 Coefficients", |ui| {
                        ui.horizontal(|ui| {
                            ui.vertical(|ui| {
                                labeled_value_scaled(ui, "Speed Coef", ev.spd_coef, 10.0, "");
                            });
                            ui.vertical(|ui| {
                                labeled_value_scaled(ui, "Steer Coef", ev.str_coef, 10.0, "");
                            });
                            ui.vertical(|ui| {
                                labeled_value_scaled(ui, "Filter Rate", ev.filter_rate, 10.0, "");
                            });
                        });
                    });

                    // ── Inputs ──
                    ui.collapsing("🎮 Inputs", |ui| {
                        ui.horizontal(|ui| {
                            ui.vertical(|ui| {
                                ui.label("Input 1");
                                labeled_value(ui, "  Raw", ev.input1_raw, "");
                                labeled_value(ui, "  Cmd", ev.input1_cmd, "");
                                labeled_value(ui, "  Type", ev.input1_typ, "");
                                labeled_value(ui, "  Min", ev.input1_min, "");
                                labeled_value(ui, "  Mid", ev.input1_mid, "");
                                labeled_value(ui, "  Max", ev.input1_max, "");
                            });
                            ui.separator();
                            ui.vertical(|ui| {
                                ui.label("Input 2");
                                labeled_value(ui, "  Raw", ev.input2_raw, "");
                                labeled_value(ui, "  Cmd", ev.input2_cmd, "");
                                labeled_value(ui, "  Type", ev.input2_typ, "");
                                labeled_value(ui, "  Min", ev.input2_min, "");
                                labeled_value(ui, "  Mid", ev.input2_mid, "");
                                labeled_value(ui, "  Max", ev.input2_max, "");
                            });
                            ui.separator();
                            ui.vertical(|ui| {
                                ui.label("Aux Input 1");
                                labeled_value(ui, "  Raw", ev.aux_input1_raw, "");
                                labeled_value(ui, "  Cmd", ev.aux_input1_cmd, "");
                                labeled_value(ui, "  Type", ev.aux_input1_typ, "");
                                labeled_value(ui, "  Min", ev.aux_input1_min, "");
                                labeled_value(ui, "  Mid", ev.aux_input1_mid, "");
                                labeled_value(ui, "  Max", ev.aux_input1_max, "");
                            });
                            ui.separator();
                            ui.vertical(|ui| {
                                ui.label("Aux Input 2");
                                labeled_value(ui, "  Raw", ev.aux_input2_raw, "");
                                labeled_value(ui, "  Cmd", ev.aux_input2_cmd, "");
                                labeled_value(ui, "  Type", ev.aux_input2_typ, "");
                                labeled_value(ui, "  Min", ev.aux_input2_min, "");
                                labeled_value(ui, "  Mid", ev.aux_input2_mid, "");
                                labeled_value(ui, "  Max", ev.aux_input2_max, "");
                            });
                        });
                    });
                });
            } else {
                ui.vertical_centered(|ui| {
                    ui.add_space(100.0);
                    ui.heading("Waiting for HoverboardEvent...");
                    ui.label("No data received yet from the serial port.");
                });
            }
        });

        // Request repaint at ~30 fps for smooth sliders
        ctx.request_repaint_after(Duration::from_millis(33));
    }
}

// ── Helpers ────────────────────────────────────────────────────────────────

fn labeled_value(ui: &mut egui::Ui, label: &str, value: Option<i32>, unit: &str) {
    match value {
        Some(v) => {
            ui.label(format!("{}: {}{}", label, v, unit));
        }
        None => {
            ui.label(format!("{}: —", label));
        }
    }
}

fn labeled_value_scaled(ui: &mut egui::Ui, label: &str, value: Option<i32>, scale: f64, unit: &str) {
    match value {
        Some(v) => {
            ui.label(format!("{}: {:.1}{}", label, v as f64 / scale, unit));
        }
        None => {
            ui.label(format!("{}: —", label));
        }
    }
}

fn common_hash(s: &str) -> u32 {
    let mut hash: u32 = 0x811c9dc5;
    for b in s.bytes() {
        hash ^= b as u32;
        hash = hash.wrapping_mul(0x01000193);
    }
    if hash == 0 {
        hash = 1;
    }
    hash
}

fn send_message<T: Msg>(
    port: &mut Box<dyn serialport::SerialPort>,
    msg: &T,
    src_id: u32,
) -> anyhow::Result<()> {
    let payload = msg.to_bytes()?;
    let envelope = Envelope {
        src: Some(src_id),
        dst: None,
        msg_type: Some(T::id()),
        request_id: None,
        instance_id: None,
        payload: Some(payload),
    };
    let frame = codec::encode_frame(&envelope)?;
    port.write_all(&frame)?;
    port.flush()?;
    Ok(())
}

fn handle_envelope(
    envelope: &Envelope,
    latest_event: &Arc<Mutex<Option<HoverboardEvent>>>,
    last_event_time: &Arc<Mutex<Option<Instant>>>,
    connected: &Arc<Mutex<bool>>,
) {
    let msg_type = envelope.msg_type.unwrap_or(0);

    if msg_type == HoverboardEvent::id() {
        let payload = match &envelope.payload {
            Some(p) => p,
            None => return,
        };
        match HoverboardEvent::from_bytes(payload) {
            Ok(ev) => {
                {
                    let mut guard = latest_event.lock().unwrap();
                    *guard = Some(ev);
                }
                {
                    let mut guard = last_event_time.lock().unwrap();
                    *guard = Some(Instant::now());
                }
                {
                    let mut guard = connected.lock().unwrap();
                    *guard = true;
                }
            }
            Err(e) => log::warn!("Failed to decode HoverboardEvent: {e}"),
        }
    }

    // Check connection timeout
    {
        let mut guard = connected.lock().unwrap();
        if let Some(t) = *last_event_time.lock().unwrap() {
            if t.elapsed() > Duration::from_secs(5) {
                *guard = false;
            }
        }
    }
}
