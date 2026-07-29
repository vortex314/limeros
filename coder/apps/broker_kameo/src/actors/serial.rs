//! Serial port actor — receives COBS/CRC-framed Envelopes and sends them.

use std::io;
use std::sync::Arc;
use std::time::Duration;

use kameo::prelude::*;
use generated::generated::{BROKER_ID, EndpointAnnounce, EndpointAnnounceReply, Envelope};
use log::{debug, error, info, warn};
use tokio::sync::Mutex;

use crate::actors::router::{EndpointAddress, IncomingEnvelope};
use crate::codec::{decode_frame, encode_frame};

use tokio::sync::mpsc;

// ── Messages ───────────────────────────────────────────────────────────────

/// Instruct the serial actor to send a raw frame (already COBS-encoded).
pub struct SerialSend {
    pub frame: Arc<Vec<u8>>,
}

/// Received a decoded envelope from the serial line.
struct SerialReceivedFrame {
    raw: Arc<Vec<u8>>,
}

// ── Serial actor ───────────────────────────────────────────────────────────

pub struct SerialActor {
    port_path: String,
    router: ActorRef<crate::actors::router::Router>,
    ep_found: Arc<Mutex<Option<u32>>>,
    /// Shared port handle — set by the retry loop, used by SerialSend.
    serial_port: Arc<Mutex<Option<Box<dyn serialport::SerialPort>>>>,
}

impl SerialActor {
    pub fn new(port_path: String, router: ActorRef<crate::actors::router::Router>) -> Self {
        SerialActor {
            port_path,
            router,
            ep_found: Arc::new(Mutex::new(None)),
            serial_port: Arc::new(Mutex::new(None)),
        }
    }
}

/// Lifecycle hook: open serial port on start with retry loop.
impl Actor for SerialActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(mut state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        let port_path = state.port_path.clone();
        let own_ref = actor_ref.clone();
        let ep_found = state.ep_found.clone();
        let serial_port = state.serial_port.clone();

        // Channel: blocking reader → async forwarder → actor mailbox
        let (tx, mut rx) = mpsc::unbounded_channel::<Vec<u8>>();

        // Forwarder: receives decoded frames from the blocking reader
        // and sends them to the actor via tell() — runs on tokio
        let fwd_ref = own_ref.clone();
        tokio::spawn(async move {
            while let Some(raw) = rx.recv().await {
                let _ = fwd_ref.tell(SerialReceivedFrame { raw: Arc::new(raw) }).await;
            }
        });

        // Connection/read retry loop on a BLOCKING thread (serial I/O blocks)
        tokio::task::spawn_blocking(move || {
            let retry_delay = Duration::from_secs(5);
            let rt = tokio::runtime::Handle::current();

            loop {
                info!("Serial: attempting to open {} ...", port_path);

                match serialport::new(&port_path, 115200)
                    .timeout(Duration::from_millis(50))
                    .open()
                {
                    Ok(mut port) => {
                        info!("Serial: connected on {}", port_path);

                        // Store the port handle for SerialSend to use
                        rt.block_on(async {
                            let mut guard = serial_port.lock().await;
                            *guard = Some(port.try_clone().expect("Failed to clone serial port"));
                        });

                        // Reset endpoint discovery for a fresh session
                        rt.block_on(async {
                            let mut known = ep_found.lock().await;
                            *known = None;
                        });

                        let mut buf: Vec<u8> = Vec::with_capacity(1024);
                        let mut tmp = [0u8; 256];

                        // Reader loop — break on errors to retry
                        'reader: loop {
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
                                                    let _ = tx.send(raw);
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
                                    warn!(
                                        "Serial: connection lost, retrying in {}s ...",
                                        retry_delay.as_secs()
                                    );
                                    break 'reader;
                                }
                            }
                        }

                        // Clear the shared port so SerialSend knows it's gone
                        rt.block_on(async {
                            let mut guard = serial_port.lock().await;
                            *guard = None;
                        });
                        drop(port);
                    }
                    Err(e) => {
                        error!(
                            "Serial: failed to open {}: {} — retrying in {}s ...",
                            port_path,
                            e,
                            retry_delay.as_secs()
                        );
                    }
                }

                std::thread::sleep(retry_delay);
            }
        });

        // Return state immediately — actor is "started"; the retry loop runs detached
        Ok(state)
    }
}

// ── Handle SerialReceivedFrame ────────────────────────────────────────────

impl Message<SerialReceivedFrame> for SerialActor {
    type Reply = ();

    async fn handle(&mut self, msg: SerialReceivedFrame, _ctx: &mut Context<Self, Self::Reply>) {
        debug!("Serial: received frame of {} bytes", msg.raw.len());
        let packet = &msg.raw;
        let envelope = match Envelope::from_bytes(packet) {
            Ok(e) => Arc::new(e),
            Err(e) => {
                warn!("Failed to decode Envelope on {}: {}", self.port_path, e);
                return;
            }
        };

        // show envelope details for debugging
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

        let raw = Arc::new(packet.to_vec());

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
                        envelope: (*envelope).clone(),
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
                    raw: raw.clone(),
                })
                .await;
        }

        // Send EndpointAnnounceReply
         let _ep_announce_reply = EndpointAnnounceReply { utc: None };
       /*  let reply_env = Envelope {
            src: Some(BROKER_ID),
            dst: Some(src),
            msg_type: Some(EndpointAnnounceReply::id()),
            payload: Some(ep_announce_reply.to_bytes().unwrap_or_default()),
            request_id: None,
            instance_id: None,
        };
        let reply_raw = reply_env.to_bytes().unwrap_or_default();
        let _ = self
            .router
            .tell(IncomingEnvelope {
                envelope: Arc::new(reply_env),
                raw: Arc::new(reply_raw),
            })
            .await;*/
    }
}

// ── Handle SerialSend ─────────────────────────────────────────────────────

impl Message<SerialSend> for SerialActor {
    type Reply = ();

    async fn handle(&mut self, msg: SerialSend, _ctx: &mut Context<Self, Self::Reply>) {
        info!("Serial {}: sending frame of {} bytes", self.port_path, msg.frame.len());
        let mut guard = self.serial_port.lock().await;
        if let Some(port) = guard.as_mut() {
            if let Ok(buffer) = encode_frame(&msg.frame) {
                if let Err(e) = port.write_all(&buffer) {
                    warn!("Failed to write to serial port {}: {}", self.port_path, e);
                } else if let Err(e) = port.flush() {
                    warn!("Failed to flush serial port {}: {}", self.port_path, e);
                }
            } else {
                warn!("Failed to encode frame for serial port {}", self.port_path);
            }
        } else {
            warn!("Serial port {} is not open; cannot send", self.port_path);
        }
    }
}
