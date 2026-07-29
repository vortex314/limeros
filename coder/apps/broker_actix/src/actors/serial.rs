//! Serial port actor — receives COBS/CRC-framed Envelopes and sends them.

use std::io;
use std::sync::Arc;
use std::time::Duration;

use crate::actors::router::EndpointUpdate;
use actix::prelude::*;
use anyhow::Context as AnyhowContext;
use dashmap::DashMap;
use generated::generated::{BROKER_ID, EndpointAnnounce, EndpointAnnounceReply, Envelope};
use log::{error, info, warn};
use tokio::sync::Mutex;

use crate::actors::router::{EndpointAddress, IncomingEnvelope};
use crate::codec::decode_frame;
use crate::codec::encode_frame;
// ── Messages ───────────────────────────────────────────────────────────────

/// Internal: instruct the serial actor to send a raw frame (already COBS-encoded).
#[derive(Message, Debug)]
#[rtype(result = "()")]
#[allow(dead_code)]
pub struct SerialSend {
    pub frame: Vec<u8>,
}

/// Internal: received a decoded envelope from the serial line.
#[derive(Message, Debug)]
#[rtype(result = "()")]
struct SerialReceivedFrame {
    raw: Vec<u8>,
}

// ── Serial actor ───────────────────────────────────────────────────────────

pub struct SerialActor {
    port_path: String,
    /// We keep the serial port handle in the actor state.
    /// Reads happen in a background thread that sends SerialReceived messages.
    router: Addr<crate::actors::router::Router>,
    ep_found: Arc<Mutex<Option<u32>>>,
    serial_port: Option<Box<dyn serialport::SerialPort>>,
}

impl SerialActor {
    pub fn new(port_path: String, router: Addr<crate::actors::router::Router>) -> Self {
        SerialActor {
            port_path,
            router,
            ep_found: Arc::new(Mutex::new(None)),
            serial_port: None,
        }
    }

    fn handle_packet(&mut self, ctx: &mut Context<Self>, packet: &[u8]) -> anyhow::Result<()> {
        let envelope = Envelope::from_bytes(packet)?;
        let src = envelope
            .src
            .ok_or_else(|| anyhow::anyhow!("Envelope missing src"))?;
        let bytes = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("Envelope missing payload"))?;
        let msg_type = envelope
            .msg_type
            .ok_or_else(|| anyhow::anyhow!("Envelope missing msg_type"))?;
        if msg_type == generated::generated::EndpointAnnounce::id() {
            let ep_announce = EndpointAnnounce::from_bytes(&bytes)
                .context("Failed to decode EndpointAnnounce")?;
            let mut known_endpoint = self
                .ep_found
                .try_lock()
                .expect("Failed to lock known_endpoint");
            if known_endpoint.is_none() {
                info!(
                    "Serial endpoint '{}' announced src={} name='{}'",
                    self.port_path,
                    src,
                    &ep_announce.name.as_ref().unwrap_or(&"".to_string())
                );
                *known_endpoint = Some(src);
                self.router.do_send(EndpointUpdate {
                    envelope,
                    ep_addr: EndpointAddress::SerialEndpoint(ctx.address(), self.port_path.clone()),
                    ep_announce,
                });
            } else if known_endpoint.unwrap() != src {
                warn!(
                    "Serial endpoint '{}' announced a different src={} (was {})",
                    self.port_path,
                    src,
                    known_endpoint.unwrap()
                );
                self.router.do_send(EndpointUpdate {
                    envelope,
                    ep_addr: EndpointAddress::SerialEndpoint(ctx.address(), self.port_path.clone()),
                    ep_announce,
                });
            }
            *known_endpoint = Some(src);
        } else {
            self.router.do_send(IncomingEnvelope {
                envelope,
                raw: packet.to_vec(),
            });
        }
        let ep_announce_reply = EndpointAnnounceReply { utc: None };
        let env = Envelope {
            src: Some(BROKER_ID),
            dst: Some(src),
            msg_type: Some(EndpointAnnounceReply::id()),
            payload: Some(ep_announce_reply.to_bytes()?),
            request_id: None,
            instance_id: None,
        };
        let raw = env.to_bytes()?;
        self.router.do_send(IncomingEnvelope { envelope: env, raw });
        Result::Ok(())
    }
}

impl Actor for SerialActor {
    type Context = Context<Self>;

    fn started(&mut self, ctx: &mut Context<Self>) {
        let port_path = self.port_path.clone();
        let addr = ctx.address();

        // Open serial port and spawn reader thread
        match serialport::new(&port_path, 115200)
            .timeout(Duration::from_millis(50))
            .open()
        {
            Ok(mut port) => {
                self.serial_port = Some(port.try_clone().expect("Failed to clone serial port"));
                debug!("Serial actor started on {}", port_path);
                let mut writer = port
                    .try_clone()
                    .expect("Failed to clone serial port for writer");

                // Spawn reader thread
                std::thread::spawn(move || {
                    let mut buf: Vec<u8> = Vec::with_capacity(1024);
                    let mut tmp = [0u8; 256];
                    loop {
                        match port.read(&mut tmp) {
                            Ok(0) => continue,
                            Ok(n) => {
                                for &b in &tmp[..n] {
                                    if b == 0x00 {
                                        if buf.is_empty() {
                                            continue;
                                        }
                                        match decode_frame(&buf) {
                                            Ok(raw) => {
                                                addr.do_send(SerialReceivedFrame { raw });
                                            }
                                            Err(e) => warn!(
                                                "Serial frame decode error on {}: {}",
                                                port_path, e
                                            ),
                                        }
                                        buf.clear();
                                    } else {
                                        buf.push(b);
                                        if buf.len() > 4096 {
                                            warn!("Oversized frame on {}, discarding", port_path);
                                            buf.clear();
                                        }
                                    }
                                }
                            }
                            Err(ref e) if e.kind() == io::ErrorKind::TimedOut => continue,
                            Err(e) => {
                                error!("Serial read error on {}: {}", port_path, e);
                                break;
                            }
                        }
                    }
                });
            }
            Err(e) => {
                error!("Failed to open serial port {}: {}", port_path, e);
            }
        }
    }
}

// ── Handle SerialReceived ─────────────────────────────────────────────────

impl Handler<SerialReceivedFrame> for SerialActor {
    type Result = ();

    fn handle(&mut self, msg: SerialReceivedFrame, _ctx: &mut Context<Self>) {
        if self.handle_packet(_ctx, &msg.raw).is_err() {
            warn!(
                "Failed to handle received serial frame on {}: {:?}",
                self.port_path, msg.raw
            );
        }
    }
}

// ── Handle SerialSend ─────────────────────────────────────────────────────

impl Handler<SerialSend> for SerialActor {
    type Result = ();

    fn handle(&mut self, _msg: SerialSend, _ctx: &mut Context<Self>) {
        if let Some(ref mut port) = self.serial_port {
            let frame = _msg.frame;
            // encode cobs and crc
            if let Ok(buffer) = encode_frame(&frame) {
                if let Err(e) = port.write_all(&buffer) {
                    warn!("Failed to write to serial port {}: {}", self.port_path, e);
                }
            } else {
                warn!("Failed to encode frame for serial port {}", self.port_path);
            }
        } else {
            warn!("Serial port {} is not open; cannot send", self.port_path);
        }
    }
}
