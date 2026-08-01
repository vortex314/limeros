//! Serial port actor — receives COBS/CRC-framed Envelopes and sends them.

use std::io;
use std::sync::Arc;
use std::time::Duration;

use generated::generated::{EndpointAnnounce, Envelope};
use kameo::prelude::*;
use log::{debug, error, info, warn};
use tokio::sync::Mutex;
use anyhow::Result;

use crate::{
    codec::{decode_frame, encode_frame}, router::{RouterActor,Register},
};

use tokio::sync::mpsc;

// ── Messages ───────────────────────────────────────────────────────────────

pub enum SerialEvent {
    ReceivedFrame(Arc<Vec<u8>>),
}
pub struct SerialSend {
    frame: Vec<u8>,
}

// ── Serial actor ───────────────────────────────────────────────────────────

pub struct SerialActor {
    port_path: String,
    endpoint_announce : Option<EndpointAnnounce>,
    router: ActorRef<RouterActor>,
    serial_port: Arc<Mutex<Option<Box<dyn serialport::SerialPort>>>>,
}

impl SerialActor {
    pub fn new(port_path: &str, router: ActorRef<RouterActor>) -> Self {
        SerialActor {
            port_path: port_path.to_string(),
            endpoint_announce: None,
            router,
            serial_port: Arc::new(Mutex::new(None)),
        }
    }

    pub async fn handle_endpoint_announce(&mut self, envelope: &Envelope) {
        if let Ok(ep_announce) =
            EndpointAnnounce::from_bytes(envelope.payload.as_ref().unwrap_or(&vec![]))
        {
            if let Some(src) = envelope.src {
                if self.endpoint_announce.is_none() || self.endpoint_announce.as_ref().unwrap().id != envelope.src {
                    info!(
                        "Serial endpoint '{}' announced src={} name='{}'",
                        self.port_path,
                        src,
                        &ep_announce.name.as_ref().unwrap_or(&"".to_string())
                    );
                    self.endpoint_announce = Some(ep_announce.clone());
                }
            }

            let _ = self.router.tell(Arc::new(envelope.clone())).await;
        }
    }
}

/// Lifecycle hook: open serial port on start with retry loop.
impl Actor for SerialActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(
        state: Self::Args,
        actor_ref: ActorRef<Self>,
    ) -> Result<Self, Self::Error> {
        state
            .router
            .tell(Register {
                actor_ref: actor_ref.clone().recipient(),
                description: format!("SerialActor {}", state.port_path),
            })
            .await?;
        let port_path = state.port_path.clone();
        let own_ref = actor_ref.clone();
        let serial_port = state.serial_port.clone();

        // Channel: blocking reader → async forwarder → actor mailbox
        let (tx, mut rx) = mpsc::unbounded_channel::<Vec<u8>>();

        // Forwarder: receives decoded frames from the blocking reader
        // and sends them to the actor via tell() — runs on tokio
        let fwd_ref = own_ref.clone();
        tokio::spawn(async move {
            while let Some(raw) = rx.recv().await {
                let _ = fwd_ref
                    .tell(SerialEvent::ReceivedFrame(Arc::new(raw)))
                    .await;
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
                                                warn!(
                                                    "Oversized frame on {}, discarding",
                                                    port_path
                                                );
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

impl Message<SerialEvent> for SerialActor {
    type Reply = ();

    async fn handle(&mut self, event: SerialEvent, _ctx: &mut Context<Self, Self::Reply>) {
        match event {
            SerialEvent::ReceivedFrame(raw) => {
                debug!("Serial: received frame of {} bytes", raw.len());
                let packet = &raw;
                let envelope = match Envelope::from_bytes(packet) {
                    Ok(e) => Arc::new(e),
                    Err(e) => {
                        warn!("Failed to decode Envelope on {}: {}", self.port_path, e);
                        return;
                    }
                };
                if let Some(EndpointAnnounce::MSG_ID) = envelope.msg_type {
                    self.handle_endpoint_announce(&envelope).await;
                }

                let _ = self.router.tell(envelope.clone()).await;
            }
        }
    }
}

// ── Handle SerialSend ─────────────────────────────────────────────────────

impl Message<SerialSend> for SerialActor {
    type Reply = ();

    async fn handle(&mut self, msg: SerialSend, _ctx: &mut Context<Self, Self::Reply>) {
        info!(
            "Serial {}: sending frame of {} bytes",
            self.port_path,
            msg.frame.len()
        );
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


impl Message<Arc<Envelope>> for SerialActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: Arc<Envelope>,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        // The router broadcasts every envelope to all listeners, so only
        // respond when this serial endpoint is the intended destination.
        let Some(dst) = msg.dst else {
            return; // broadcast / not addressed to us — ignore
        };
        let Some(ep_announce) = &self.endpoint_announce else {
            warn!(
                "Serial {}: no EndpointAnnounce yet, dropping dst={}",
                self.port_path, dst
            );
            return;
        };
        if ep_announce.id != Some(dst) {
            return; // destined for another endpoint — ignore
        }
        if let Ok(raw) = msg.to_bytes() {
            let _ = _ctx.actor_ref().tell(SerialSend { frame: raw }).await;
        }
    }
}