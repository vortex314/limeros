//! Serial port actor — receives COBS/CRC-framed Envelopes and sends them.

use std::collections::HashMap;
use std::sync::Arc;
use std::time::Duration;

use anyhow::Result;
use generated::generated::{EndpointAnnounce, Envelope};
use kameo::prelude::*;
use log::{debug, error, info, warn};
use tokio::{io, sync::Mutex};

use crate::{
    brain::ResultLog,
    codec::{decode_frame, encode_frame},
    router::{FromDevice, RegisterDevice, RouterActor, ToDevice},
};

// ── Messages ───────────────────────────────────────────────────────────────

pub enum SerialInternalEvent {
    ReceivedFrame(Arc<Vec<u8>>),
}
pub struct SerialSend {
    frame: Vec<u8>,
}

// ── Serial actor ───────────────────────────────────────────────────────────

pub struct SerialActor {
    port_path: String,
    serial_port: Arc<Mutex<Option<Box<dyn serialport::SerialPort>>>>,
    router: ActorRef<crate::router::RouterActor>,
    registered_device: Option<u32>,
}

impl SerialActor {
    pub fn new(port_path: &str, router: ActorRef<RouterActor>) -> Self {
        SerialActor {
            port_path: port_path.to_string(),
            serial_port: Arc::new(Mutex::new(None)),
            router,
            registered_device: None,
        }
    }
}

fn spawn_serial_task(
    port_path: String,
    serial_port: Arc<tokio::sync::Mutex<Option<Box<dyn serialport::SerialPort>>>>,
    frame_target: ActorRef<SerialActor>,
) -> tokio::task::JoinHandle<()> {
    tokio::task::spawn_blocking(move || {
        let retry_delay = Duration::from_secs(5);

        loop {
            debug!("Serial: attempting to open {port_path} ...");

            match serialport::new(&port_path, 115200)
                .timeout(Duration::from_millis(50))
                .open()
            {
                Ok(mut port) => {
                    info!("Serial: connected on {port_path}");

                    {
                        // .blocking_lock() — the sync-context counterpart to
                        // .lock().await, made for exactly this situation.
                        let mut guard = serial_port.blocking_lock();
                        *guard = Some(port.try_clone().expect("failed to clone serial port"));
                    }

                    let mut buf: Vec<u8> = Vec::with_capacity(1024);
                    let mut tmp = [0u8; 256];

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
                                                if let Err(e) = frame_target
                                                    .tell(SerialInternalEvent::ReceivedFrame(
                                                        Arc::new(raw),
                                                    ))
                                                    .blocking_send()
                                                {
                                                    warn!(
                                                        "Serial: failed to deliver frame from {port_path}: {e}"
                                                    );
                                                }
                                            }
                                            Err(e) => warn!(
                                                "Serial frame decode error on {port_path}: {e}"
                                            ),
                                        }
                                        buf.clear();
                                    } else {
                                        buf.push(b);
                                        if buf.len() > 4096 {
                                           // info!("Serial {} {}", port_path, String::from_utf8_lossy(&buf).to_string());
                                            // split per newline and send to log, but don't send to router
                                            /*let log_lines: Vec<String> = buf
                                                .split(|&x| x == b'\n')
                                                .map(|line| {
                                                    String::from_utf8_lossy(line).to_string()
                                                })
                                                                              break 'reader;
  .collect();
                                            for line in log_lines {
                                                warn!("Serial log from {port_path}: {}", line);
                                            }*/
                                            buf.clear();
                                        }
                                    }
                                }
                            }
                            Err(ref e) if e.kind() == io::ErrorKind::TimedOut => continue,
                            Err(e) => {
                                error!("Serial read error on {port_path}: {e}");
                                warn!(
                                    "Serial: connection lost, retrying in {}s ...",
                                    retry_delay.as_secs()
                                );
                                break 'reader;
                            }
                        }
                    }

                    {
                        let mut guard = serial_port.blocking_lock();
                        *guard = None;
                    }
                    drop(port);
                }
                Err(e) => {
                    debug!(
                        "Serial: failed to open {port_path}: {e} — retrying in {}s ...",
                        retry_delay.as_secs()
                    );
                }
            }

            std::thread::sleep(retry_delay);
        }
    })
}

/// Lifecycle hook: open serial port on start with retry loop.
impl Actor for SerialActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        let port_path = state.port_path.clone();
        let own_ref = actor_ref.clone();
        let serial_port = state.serial_port.clone();

        // Channel: blocking reader → async forwarder → actor mailbox

        let _serial_handle = spawn_serial_task(port_path, serial_port, own_ref);

        Ok(state)
    }
}

// ── Handle SerialReceivedFrame ────────────────────────────────────────────

impl Message<SerialInternalEvent> for SerialActor {
    type Reply = ();

    async fn handle(&mut self, event: SerialInternalEvent, _ctx: &mut Context<Self, Self::Reply>) {
        match event {
            SerialInternalEvent::ReceivedFrame(raw) => {
                debug!("Serial: received frame of {} bytes", raw.len());
                let packet = &raw;
                let envelope = match Envelope::from_bytes(packet) {
                    Ok(e) => Arc::new(e),
                    Err(e) => {
                        warn!("Failed to decode Envelope on {}: {}", self.port_path, e);
                        return;
                    }
                };
                if let Some(src) = envelope.src {
                    if self.registered_device != Some(src) {
                        self.registered_device = Some(src);
                        self.router
                            .tell(RegisterDevice {
                                id: src,
                                recipient: _ctx.actor_ref().clone().recipient(),
                            })
                            .await
                            .log_error("Failed to register device with router");
                    }
                    self.router
                        .tell(FromDevice {
                            id: src,
                            envelope: envelope.clone(),
                        })
                        .await
                        .log_error("Failed to send FromDevice message to router");
                }
            }
        }
    }
}

// ── Handle SerialSend ─────────────────────────────────────────────────────

impl Message<SerialSend> for SerialActor {
    type Reply = ();

    async fn handle(&mut self, msg: SerialSend, _ctx: &mut Context<Self, Self::Reply>) {
        debug!(
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

impl Message<ToDevice> for SerialActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: ToDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let envelope = msg.envelope;
        let raw = match envelope.to_bytes() {
            Ok(r) => r,
            Err(e) => {
                warn!("Failed to serialize envelope for serial send: {}", e);
                return;
            }
        };
        let _ = self.handle(SerialSend { frame: raw }, _ctx).await;
    }
}
