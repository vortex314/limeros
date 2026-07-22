// serial_bridge — Communicate with an ESP32 Limeros eventbus over a serial port.
//
// Wire protocol (mirrors hoverboard_actor.cpp / codec.cpp):
//   SEND:  Envelope → CBOR → CRC-16(CCITT, big-endian) → COBS → 0x00 → serial
//   RECV:  serial → buffer till 0x00 → COBS decode → verify CRC-16 → CBOR decode Envelope → payload

mod codec;

use std::io::{self, BufRead, Write};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use std::thread;
use std::time::Duration;

use log::{info, warn};

use anyhow::Context;
use clap::Parser;
use generated::generated::{
    EndpointAnnounce, EndpointAnnounceReply, Envelope, HoverboardEvent, HoverboardRequest, Msg,
};

// ── CLI ────────────────────────────────────────────────────────────────────

#[derive(Parser, Debug)]
#[command(about = "Serial bridge for Limeros eventbus over UART")]
struct Args {
    /// Serial port device, e.g. /dev/ttyUSB0
    #[arg(short, long)]
    port: String,

    /// Baud rate (default: 115200)
    #[arg(short, long, default_value_t = 115200)]
    baud: u32,
}

// ── main ───────────────────────────────────────────────────────────────────

fn main() -> anyhow::Result<()> {
   // env_logger::init();
    common::logger::init();
    let args = Args::parse();

    let running = Arc::new(AtomicBool::new(true));
    let r = running.clone();

    // Open serial port
    let mut port = serialport::new(&args.port, args.baud)
        .timeout(Duration::from_millis(50))
        .open()
        .with_context(|| format!("Failed to open serial port {}", args.port))?;

    // Clone for reader thread
    let mut reader = port.try_clone().context("Failed to clone serial port")?;

    // ── Reader thread ──
    let reader_handle = thread::spawn(move || {
        let mut buf: Vec<u8> = Vec::with_capacity(1024);
        let mut tmp = [0u8; 256];
        while r.load(Ordering::Relaxed) {
            match reader.read(&mut tmp) {
                Ok(0) => {
                    // timeout / no data
                    continue;
                }
                Ok(n) => {
 //                   info!("Read {} bytes from serial", n);
                    for &b in &tmp[..n] {
                        if b == 0x00 {
                            // COBS frame delimiter
                            if buf.is_empty() {
                                continue;
                            }
                            match codec::decode_frame(&buf) {
                                Ok(envelope) => handle_envelope(&envelope),
                                Err(e) => {
                                    log::warn!("Frame decode error: {e}");
                                }
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
                Err(ref e) if e.kind() == io::ErrorKind::TimedOut => {
                    continue;
                }
                Err(e) => {
                    log::error!("Serial read error: {e}");
                    break;
                }
            }
        }
    });

    // ── Writer (main thread) ──
    println!("serial_bridge listening on {} @ {} baud", args.port, args.baud);
    println!("Commands:");
    println!("  speed <value>          — send HoverboardRequest with speed");
    println!("  steer <value>          — send HoverboardRequest with steer");
    println!("  hb <speed> <steer>     — send HoverboardRequest with both");
    println!("  announce               — send EndpointAnnounce");
    println!("  announce-reply         — send EndpointAnnounceReply");
    println!("  quit / exit            — exit");
    println!();

    let stdin = io::stdin();
    let mut line = String::new();
    let src_id = common_hash("serial_bridge");

    while running.load(Ordering::Relaxed) {
        line.clear();
        if stdin.lock().read_line(&mut line).is_err() {
            break;
        }
        let trimmed = line.trim();
        if trimmed.is_empty() {
            continue;
        }

        let parts: Vec<&str> = trimmed.split_whitespace().collect();
        match parts[0] {
            "quit" | "exit" => {
                running.store(false, Ordering::Relaxed);
                break;
            }
            "speed" => {
                let speed: i32 = parts.get(1).and_then(|s| s.parse().ok()).unwrap_or(0);
                let req = HoverboardRequest {
                    speed: Some(speed),
                    steer: None,
                    req_id: None,
                };
                if let Err(e) = send_message(&mut port, &req, src_id) {
                    log::error!("Send error: {e}");
                } else {
                    println!("Sent HoverboardRequest speed={speed}");
                }
            }
            "steer" => {
                let steer: i32 = parts.get(1).and_then(|s| s.parse().ok()).unwrap_or(0);
                let req = HoverboardRequest {
                    speed: None,
                    steer: Some(steer),
                    req_id: None,
                };
                if let Err(e) = send_message(&mut port, &req, src_id) {
                    log::error!("Send error: {e}");
                } else {
                    println!("Sent HoverboardRequest steer={steer}");
                }
            }
            "hb" => {
                let speed: i32 = parts.get(1).and_then(|s| s.parse().ok()).unwrap_or(0);
                let steer: i32 = parts.get(2).and_then(|s| s.parse().ok()).unwrap_or(0);
                let req = HoverboardRequest {
                    speed: Some(speed),
                    steer: Some(steer),
                    req_id: None,
                };
                if let Err(e) = send_message(&mut port, &req, src_id) {
                    log::error!("Send error: {e}");
                } else {
                    println!("Sent HoverboardRequest speed={speed} steer={steer}");
                }
            }
            "announce" => {
                let announce = EndpointAnnounce {
                    id: Some(src_id),
                    name: Some("serial_bridge".into()),
                    description: Some("Serial bridge CLI".into()),
                    services: None,
                    events: None,
                    replies: None,
                    subscribes: None,
                };
                if let Err(e) = send_message(&mut port, &announce, src_id) {
                    log::error!("Send error: {e}");
                } else {
                    println!("Sent EndpointAnnounce");
                }
            }
            "announce-reply" => {
                let reply = EndpointAnnounceReply {
                    utc: Some(chrono::Utc::now().timestamp_millis() as u64),
                };
                if let Err(e) = send_message(&mut port, &reply, src_id) {
                    log::error!("Send error: {e}");
                } else {
                    println!("Sent EndpointAnnounceReply");
                }
            }
            _ => {
                println!("Unknown command: {trimmed}");
            }
        }
    }

    // Wait briefly for reader thread to finish
    let _ = reader_handle.join();
    println!("Done.");
    Ok(())
}

// ── Helpers ────────────────────────────────────────────────────────────────

fn common_hash(s: &str) -> u32 {
    let mut hash: u32 = 0x811c9dc5;
    for b in s.bytes() {
        hash ^= b as u32;
        hash = hash.wrapping_mul(0x01000193);
    }
    // Clamp to >0 (0 means "unset" in the protocol)
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
    // 1. CBOR-encode the inner message
    let payload = msg.to_bytes()?;

    // 2. Wrap in Envelope
    let envelope = Envelope {
        src: Some(src_id),
        dst: None,
        msg_type: Some(T::id()),
        request_id: None,
        instance_id: None,
        payload: Some(payload),
    };

    // 3. CBOR-encode Envelope → CRC → COBS → 0x00
    let frame = codec::encode_frame(&envelope)?;
    port.write_all(&frame)?;
    port.flush()?;
    Ok(())
}

fn handle_envelope(envelope: &Envelope) {
    let msg_type = envelope.msg_type.unwrap_or(0);
    let src = envelope
        .src
        .map(|id| generated::generated::id_to_string(id))
        .unwrap_or_else(|| "?".into());
    let payload = match &envelope.payload {
        Some(p) => p,
        None => {
            log::debug!("Envelope with no payload from {src}");
            return;
        }
    };

    match msg_type {
        id if id == EndpointAnnounce::id() => match EndpointAnnounce::from_bytes(payload) {
            Ok(a) => {
                let name = a.name.as_deref().unwrap_or("?");
                let desc = a.description.as_deref().unwrap_or("-");
                println!(
                    "⬅ EndpointAnnounce: {} (0x{:08X}) — {}",
                    name,
                    a.id.unwrap_or(0),
                    desc
                );
            }
            Err(e) => log::warn!("Failed to decode EndpointAnnounce: {e}"),
        },

        id if id == HoverboardEvent::id() => match HoverboardEvent::from_bytes(payload) {
            Ok(ev) => {
                print!("⬅ HoverboardEvent");
                if let Some(v) = ev.cmdl {
                    print!("  cmdl={v}");
                }
                if let Some(v) = ev.cmdr {
                    print!("  cmdr={v}");
                }
                if let Some(v) = ev.spd_avg {
                    print!("  rpm_avg={v}");
                }
                if let Some(v) = ev.batv {
                    print!("  batV={:.2}", v as f64 / 100.0);
                }
                if let Some(v) = ev.temp {
                    print!("  temp={:.1}°C", v as f64 / 10.0);
                }
                if let Some(v) = ev.dc_curr {
                    print!("  dc_i={:.2}A", v as f64 / 100.0);
                }
                println!("  (from {src})");
            }
            Err(e) => log::warn!("Failed to decode HoverboardEvent: {e}"),
        },

        _ => {
            let name = generated::generated::id_to_string(msg_type);
            log::debug!("Unknown message type: {name} (0x{msg_type:08X}) from {src}");
        }
    }
}
