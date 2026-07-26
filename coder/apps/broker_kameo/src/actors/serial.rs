//! Serial port actor — receives COBS/CRC-framed Envelopes and sends them.

use std::io;
use std::sync::Arc;
use std::time::Duration;

use kameo::prelude::*;
use generated::generated::{BROKER_ID, EndpointAnnounce, EndpointAnnounceReply, Envelope};
use log::{error, info, warn};
use tokio::sync::Mutex;

use crate::actors::router::{EndpointAddress, IncomingEnvelope};
use crate::codec::{decode_frame, encode_frame};

// ── Messages ───────────────────────────────────────────────────────────────

/// Instruct the serial actor to send a raw frame (already COBS-encoded).
pub struct SerialSend {
    pub frame: Vec<u8>,
}

/// Received a decoded envelope from the serial line.
struct SerialReceivedFrame {
    raw: Vec<u8>,
}

// ── Serial actor ───────────────────────────────────────────────────────────

pub struct SerialActor {
    port_path: String,
    router: ActorRef<crate::actors::router::Router>,
    ep_found: Arc<Mutex<Option<u32>>>,
    serial_port: Option<Box<dyn serialport::SerialPort>>,
}

impl SerialActor {
    pub fn new(port_path: String, router: ActorRef<crate::actors::router::Router>) -> Self {
        SerialActor {
            port_path,
            router,
            ep_found: Arc::new(Mutex::new(None)),
            serial_port: None,
        }
    }

    // Packet handling is done in the message handler with ctx access.
}

/// Lifecycle hook: open serial port on start.
impl Actor for SerialActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(mut state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        let port_path = state.port_path.clone();
        let addr = actor_ref.clone();

        match serialport::new(&port_path, 115200)
            .timeout(Duration::from_millis(50))
            .open()
        {
            Ok(mut port) => {
                state.serial_port = Some(port.try_clone().expect("Failed to clone serial port"));
                info!("Serial actor started on {}", port_path);
                let _writer = port
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
                                                let _ = addr.tell(SerialReceivedFrame { raw });
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

        Ok(state)
    }
}

// ── Handle SerialReceivedFrame ────────────────────────────────────────────

impl Message<SerialReceivedFrame> for SerialActor {
    type Reply = ();

    async fn handle(&mut self, msg: SerialReceivedFrame, _ctx: &mut Context<Self, Self::Reply>) {
        let packet = &msg.raw;
        let envelope = match Envelope::from_bytes(packet) {
            Ok(e) => e,
            Err(e) => {
                warn!("Failed to decode Envelope on {}: {}", self.port_path, e);
                return;
            }
        };

        let src = match envelope.src {
            Some(s) => s,
            None => {
                warn!("Envelope missing src on {}", self.port_path);
                return;
            }
        };

        let msg_type = match envelope.msg_type {
            Some(t) => t,
            None => {
                warn!("Envelope missing msg_type on {}", self.port_path);
                return;
            }
        };

        if msg_type == generated::generated::EndpointAnnounce::id() {
            let bytes = match envelope.payload.as_ref() {
                Some(b) => b,
                None => {
                    warn!("Envelope missing payload on {}", self.port_path);
                    return;
                }
            };
            let ep_announce = match EndpointAnnounce::from_bytes(bytes) {
                Ok(a) => a,
                Err(e) => {
                    warn!("Failed to decode EndpointAnnounce on {}: {}", self.port_path, e);
                    return;
                }
            };

            let mut known = self.ep_found.lock().await;
            if known.is_none() || known.unwrap() != src {
                info!(
                    "Serial endpoint '{}' announced src={} name='{}'",
                    self.port_path,
                    src,
                    &ep_announce.name.as_ref().unwrap_or(&"".to_string())
                );
                *known = Some(src);

                let ep_addr = EndpointAddress::SerialEndpoint(
                    _ctx.actor_ref().clone(),
                    self.port_path.clone(),
                );

                let _ = self
                    .router
                    .tell(crate::actors::router::EndpointUpdate {
                        envelope: envelope.clone(),
                        ep_addr,
                        ep_announce,
                    })
                    .await;
            }
        } else {
            let _ = self
                .router
                .tell(IncomingEnvelope {
                    envelope: envelope.clone(),
                    raw: packet.to_vec(),
                })
                .await;
        }

        // Send EndpointAnnounceReply
        let ep_announce_reply = EndpointAnnounceReply { utc: None };
        let env = Envelope {
            src: Some(BROKER_ID),
            dst: Some(src),
            msg_type: Some(EndpointAnnounceReply::id()),
            payload: Some(ep_announce_reply.to_bytes().unwrap_or_default()),
            request_id: None,
            instance_id: None,
        };
        let raw = env.to_bytes().unwrap_or_default();
        let _ = self
            .router
            .tell(IncomingEnvelope {
                envelope: env,
                raw,
            })
            .await;
    }
}

// ── Handle SerialSend ─────────────────────────────────────────────────────

impl Message<SerialSend> for SerialActor {
    type Reply = ();

    async fn handle(&mut self, msg: SerialSend, _ctx: &mut Context<Self, Self::Reply>) {
        if let Some(ref mut port) = self.serial_port {
            if let Ok(buffer) = encode_frame(&msg.frame) {
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
