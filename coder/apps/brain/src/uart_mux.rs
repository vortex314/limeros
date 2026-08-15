//! [`UartMuxActor`] — owns the USB TTY of the 8-port UART multiplexer.
//!
//! It connects only to the device with a specific USB vendor/product id,
//! sends raw bytes to any UART port, and fans received frames out to actors
//! that registered interest in a specific port.

use std::collections::HashMap;
use std::fmt;
use std::sync::Arc;
use std::time::Duration;

use generated::generated::{EndpointAnnounce, Envelope, opt_id_to_string};
use kameo::actor::{ActorId, ActorRef, Recipient, WeakActorRef};
use kameo::error::ActorStopReason;
use kameo::message::{Context, Message};
use kameo::Actor;
use log::{info, warn};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::sync::mpsc;
use tokio_serial::SerialPortBuilderExt;
use anyhow::{Context as ErrContext, Result};

use crate::brain::ResultLog;
use crate::codec::decode_cobs_frame;
use crate::protocol::{build_frame, FrameParser};
use crate::router::{self, FromDevice, RegisterDevice, RouterActor, ToDevice};

/// Error type for the actors. `Clone` so startup results can be inspected.
#[derive(Debug, Clone)]
pub struct PortError(pub String);

impl fmt::Display for PortError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

#[derive(Debug, Clone)]
pub struct ReceivedFrame {
    /// UART port number (0-7).
    pub uart: u8,
    /// Payload bytes.
    pub data: Vec<u8>,
}

/// Finds the TTY device path for a USB serial device with the given VID/PID.
pub fn find_uart_mux_port(vid: u16, pid: u16) -> Result<String, PortError> {
    let ports = serialport::available_ports()
        .map_err(|e| PortError(format!("failed to enumerate serial ports: {e}")))?;
    for port in &ports {
        if let serialport::SerialPortType::UsbPort(info) = &port.port_type {
            if info.vid == vid && info.pid == pid {
                return Ok(port.port_name.clone());
            }
        }
    }
    Err(PortError(format!(
        "no USB serial port found with VID 0x{:04x} PID 0x{:04x} (lm4f-uartd 8-port UART mux)",
        vid, pid
    )))
}

/// Configuration used to start the [`UartMuxActor`].
#[derive(Debug, Clone)]
pub struct UartMuxConfig {
    /// USB vendor ID to match (e.g. `0xc03e`).
    pub vid: u16,
    /// USB product ID to match (e.g. `0xb007`).
    pub pid: u16,
    /// Baud rate for the USB CDC ACM link.
    pub baud: u32,
    // router ref is passed in the constructor of UartMuxActor, so it doesn't need to be part of the config
}

/// Actor that owns the USB TTY of the 8-port UART multiplexer.
///
/// - On start it finds and opens the device matching [`UartMuxConfig::vid`] and
///   [`UartMuxConfig::pid`], and spawns a reader task that continuously parses
///   incoming frames.
/// - [`SendFrame`] writes raw bytes to a UART port.
/// - [`RegisterInterest`] subscribes an actor (as a [`Recipient`]) to received
///   frames for a specific UART port.
/// - Received frames are dispatched to the interested subscribers as
///   [`ReceivedFrame`] messages.
pub struct UartMuxActor {
    router_ref: ActorRef<router::RouterActor>,

    /// Handle of the background reader task.
    reader_task: tokio::task::JoinHandle<()>,
    /// Configuration kept so the port can be re-opened after an I/O error.
    config: UartMuxConfig,
    /// Set while a reconnection is in flight, to avoid double reconnects.
    reconnecting: bool,
    /// Outgoing frame bytes queued for the TX task.
    tx: Option<mpsc::Sender<Vec<u8>>>,
    /// Task that owns the write half of the port and batches USB writes.
    /// `None` while the port is being re-opened.
    tx_task: Option<tokio::task::JoinHandle<()>>,
    /// Mapping from envelope dst/src ID to UART port number.
    id_to_uart: HashMap<u32, u8>,
    uart_buffers : HashMap<u8, Vec<u8>>,
}

impl Actor for UartMuxActor {
    type Args = Self;
    type Error = PortError;

    async fn on_start(mut state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        warn!(
            "[uart-mux] opening device (VID 0x{:04x} PID 0x{:04x})",
            state.config.vid, state.config.pid
        );
        // The port open is blocking (serialport enumeration + TTY open, and a
        // CDC ACM open can block on a USB control transfer). Run it off the
        // async worker and time-box it so a wedged device can never leave the
        // actor stuck in "starting".
        let (reader, writer) =
            match tokio::time::timeout(Duration::from_secs(10), Self::open_port(&state.config)).await {
                Ok(Ok(split)) => split,
                Ok(Err(e)) => return Err(e),
                Err(_) => {
                    return Err(PortError(
                        "timed out opening the serial port (device not responding)".to_string(),
                    ))
                }
            };
        let reader_task = Self::spawn_reader(reader, &actor_ref);
        let (tx, tx_task) = Self::spawn_tx(writer, &actor_ref);
        info!("[uart-mux] device opened successfully");

        // The router ref and config were already set by `UartMuxActor::new`,
        // so just fill in the runtime fields. `state.router_ref` is available
        // here if you need to register with / talk to the router on start.
        state.reader_task = reader_task;
        state.tx = Some(tx);
        state.tx_task = Some(tx_task);
        Ok(state)
    }

    async fn on_stop(
        &mut self,
        _actor_ref: WeakActorRef<Self>,
        _reason: ActorStopReason,
    ) -> Result<(), Self::Error> {
        self.reader_task.abort();
        if let Some(t) = self.tx_task.take() {
            t.abort();
        }
        Ok(())
    }

}

impl UartMuxActor {
    pub fn new(router_ref: ActorRef<RouterActor>, config: UartMuxConfig) -> Self {
        Self {
            router_ref,
            reader_task: tokio::task::spawn(async {}),
            config,
            reconnecting: false,
            tx: None,
            tx_task: None,
            id_to_uart: HashMap::new(),
            uart_buffers: HashMap::new(),
        }
    }

    async fn handle_to_device(&mut self, msg: ToDevice) -> Result<()> {
        let uart = self.id_to_uart.get(&msg.id).context("dst ID not mapped to a UART port")?;
        let bytes = msg.envelope.to_bytes()?;
        let frame = build_frame(*uart, &bytes);
        self.tx.as_ref().context("port is not open (reconnecting)")?
        .send(frame).await?;
        Ok(())
    }

    async fn handle_recvd_frame(&mut self, msg: ReceivedFrame, ctx: &mut Context<Self, ()>) -> Result<()> {
        let uart_buffer = self.uart_buffers.entry(msg.uart).or_insert_with(Vec::new);
        for b in msg.data {
            if b == 0x0 { // end of frame
                let frame = uart_buffer.clone();
                uart_buffer.clear();
                let v = decode_cobs_frame(&frame)?;
                let envelope =Envelope::from_bytes(&v) ?;
                let src = envelope.src.context("missing src in envelope")?;
                let msg_type = envelope.msg_type.context("missing msg_type in envelope")?;
                if msg_type == EndpointAnnounce::MSG_ID {
                    warn!("endpoint announce from UART {} {} ", msg.uart, opt_id_to_string(envelope.src));
                    self.id_to_uart.insert(src, msg.uart);
                    self.router_ref.tell(RegisterDevice{ recipient: ctx.actor_ref().clone().recipient(), id: src }).try_send().ok();
                }
                // send the envelope to the router
                self.router_ref.tell(FromDevice{ envelope:Arc::new(envelope), id: src }).try_send().ok();
            } else {
                uart_buffer.push(b);
                if uart_buffer.len() > 1024 {
                    warn!("UART {} buffer overflow, clearing buffer", msg.uart);
                    uart_buffer.clear();
                }
            }
        }
        Ok(())
    }
    /// Finds and opens the USB serial port for `config`, returning the read and
    /// write halves.
    async fn open_port(
        config: &UartMuxConfig,
    ) -> Result<
        (
            tokio::io::ReadHalf<tokio_serial::SerialStream>,
            tokio::io::WriteHalf<tokio_serial::SerialStream>,
        ),
        PortError,
    > {
        // serialport enumeration and the TTY open are blocking syscalls (and a
        // CDC ACM open can block on a USB control transfer), so run them on the
        // blocking thread pool instead of stalling the async runtime. Note that
        // `open_native_async` is *not* actually async — it blocks.
        let config = config.clone();
        tokio::task::spawn_blocking(move || {
            let path = find_uart_mux_port(config.vid, config.pid)?;
            warn!(
                "[uart-mux] opening {path} (VID 0x{:04x} PID 0x{:04x})",
                config.vid, config.pid
            );
            let port = tokio_serial::new(&path, config.baud)
                .open_native_async()
                .map_err(|e| PortError(format!("failed to open {path}: {e}")))?;
            Ok::<_, PortError>(tokio::io::split(port))
        })
        .await
        .map_err(|e| PortError(format!("port open task panicked: {e}")))?
    }

    /// Keeps trying to re-open the port until it succeeds, or until the actor
    /// stops (checked via `alive`). Waits between attempts with a capped
    /// exponential backoff so a missing/unplugged device doesn't spin.
    async fn open_port_with_retry(
        config: &UartMuxConfig,
        alive: &WeakActorRef<Self>,
    ) -> Option<(
        tokio::io::ReadHalf<tokio_serial::SerialStream>,
        tokio::io::WriteHalf<tokio_serial::SerialStream>,
    )> {
        let mut delay = Duration::from_millis(500);
        loop {
            if !alive.is_alive() {
                return None;
            }
            match Self::open_port(config).await {
                Ok(split) => return Some(split),
                Err(e) => {
                    warn!(
                        "[uart-mux] reconnect attempt failed: {e}; retrying in {} ms",
                        delay.as_millis()
                    );
                    tokio::time::sleep(delay).await;
                    if delay < Duration::from_secs(5) {
                        delay *= 2;
                    }
                }
            }
        }
    }

    /// Spawns the background task that reads bytes from `reader`, parses frames
    /// and forwards them to the actor for dispatch to subscribers.
    ///
    /// On an I/O error or EOF (port closed / device unplugged) it tells the
    /// actor a [`PortDown`], so the port gets re-opened.
    fn spawn_reader(
        reader: tokio::io::ReadHalf<tokio_serial::SerialStream>,
        actor_ref: &ActorRef<Self>,
    ) -> tokio::task::JoinHandle<()> {
        let weak = actor_ref.downgrade();
        let actor_ref = actor_ref.clone();
        tokio::spawn(async move {
            let mut parser = FrameParser::new();
            let mut reader = reader;
            let mut buf = [0u8; 256];
            loop {
                if !weak.is_alive() {
                    break;
                }
                match reader.read(&mut buf).await {
                    Ok(n) if n > 0 => {
                        for &byte in &buf[..n] {
                            if let Some(frame) = parser.feed(byte) {
                                let _ = actor_ref
                                    .tell(ReceivedFrame {
                                        uart: frame.uart,
                                        data: frame.data,
                                    })
                                    .send()
                                    .await;
                            }
                        }
                    }
                    Ok(_) => {
                        // EOF: the port was closed (e.g. device unplugged).
                        warn!("[uart-mux] read EOF; re-opening port");
                        let _ = actor_ref.tell(PortDown).send().await;
                        break;
                    }
                    Err(e) => {
                        warn!("[uart-mux] read error: {e}; re-opening port");
                        let _ = actor_ref.tell(PortDown).send().await;
                        break;
                    }
                }
            }
        })
    }

    /// Spawns the TX task that owns the write half of the port and batches
    /// outgoing frames into fewer, larger USB writes.
    ///
    /// The buffer is flushed when it reaches `TX_FLUSH_BYTES` or when
    /// `TX_FLUSH_INTERVAL` elapses, whichever comes first. A per-frame
    /// `flush()` (tcdrain on the CDC ACM tty) costs several ms, which is what
    /// limited throughput to ~200 msg/s; batching amortises that cost across
    /// many small frames. Returns the channel sender and the task handle.
    fn spawn_tx(
        writer: tokio::io::WriteHalf<tokio_serial::SerialStream>,
        actor_ref: &ActorRef<Self>,
    ) -> (mpsc::Sender<Vec<u8>>, tokio::task::JoinHandle<()>) {
        // Keep latency low by flushing at least every 2 ms, and throughput high
        // by batching up to 512 bytes per USB transaction.
        const TX_FLUSH_INTERVAL: Duration = Duration::from_millis(2);
        const TX_FLUSH_BYTES: usize = 512;

        let (tx, mut rx) = mpsc::channel::<Vec<u8>>(1024);
        let actor_ref = actor_ref.clone();
        let task = tokio::spawn(async move {
            let mut writer = writer;
            let mut buf = Vec::with_capacity(1024);
            let mut flush_at = tokio::time::Instant::now() + TX_FLUSH_INTERVAL;
            loop {
                tokio::select! {
                    maybe = rx.recv() => match maybe {
                        Some(frame) => {
                            buf.extend_from_slice(&frame);
                            if buf.len() >= TX_FLUSH_BYTES {
                                if let Err(e) = Self::tx_flush(&mut writer, &mut buf).await {
                                    warn!("[uart-mux] tx write failed: {e}; re-opening port");
                                    let _ = actor_ref.tell(PortDown).send().await;
                                    return;
                                }
                                flush_at = tokio::time::Instant::now() + TX_FLUSH_INTERVAL;
                            }
                        }
                        None => {
                            // Sender dropped (actor stopped / reconnecting):
                            // drain whatever is buffered, then drop the writer.
                            let _ = Self::tx_flush(&mut writer, &mut buf).await;
                            return;
                        }
                    },
                    _ = tokio::time::sleep_until(flush_at) => {
                        if !buf.is_empty() {
                            if let Err(e) = Self::tx_flush(&mut writer, &mut buf).await {
                                warn!("[uart-mux] tx flush failed: {e}; re-opening port");
                                let _ = actor_ref.tell(PortDown).send().await;
                                return;
                            }
                        }
                        flush_at = tokio::time::Instant::now() + TX_FLUSH_INTERVAL;
                    }
                }
            }
        });
        (tx, task)
    }

    /// Writes and flushes the accumulated TX buffer, then clears it.
    async fn tx_flush(
        writer: &mut tokio::io::WriteHalf<tokio_serial::SerialStream>,
        buf: &mut Vec<u8>,
    ) -> std::io::Result<()> {
        writer.write_all(buf).await?;
        writer.flush().await?;
        buf.clear();
        Ok(())
    }
}

/// Internal signal that the port has failed (read or write) and should be
/// re-opened.
struct PortDown;

impl Message<PortDown> for UartMuxActor {
    type Reply = ();

    async fn handle(
        &mut self,
        _msg: PortDown,
        ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        if self.reconnecting {
            return;
        }
        self.reconnecting = true;

        // Drop the TX sender (closing the channel to the TX task) and abort the
        // old reader/TX tasks so the old port's fds are released. The TX task
        // owns the write half, so we await its handle in the reconnect task to
        // make sure the old port is fully closed before re-opening it.
        self.tx.take();
        let old_tx = self.tx_task.take();
        if let Some(t) = &old_tx {
            t.abort();
        }
        self.reader_task.abort();

        // Re-open on a detached task so the actor keeps processing messages
        // (e.g. RegisterInterest, SendFrame) while the device is away.
        let actor_ref = ctx.actor_ref().clone();
        let weak = actor_ref.downgrade();
        let config = self.config.clone();
        tokio::spawn(async move {
            if let Some(old) = old_tx {
                let _ = old.await;
            }
            if let Some((reader, writer)) = Self::open_port_with_retry(&config, &weak).await {
                let _ = actor_ref.tell(PortReopened { reader, writer }).send().await;
            }
        });
    }
}

/// Internal message delivering the halves of a freshly re-opened port.
struct PortReopened {
    reader: tokio::io::ReadHalf<tokio_serial::SerialStream>,
    writer: tokio::io::WriteHalf<tokio_serial::SerialStream>,
}

impl Message<PortReopened> for UartMuxActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: PortReopened,
        ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let (tx, tx_task) = Self::spawn_tx(msg.writer, ctx.actor_ref());
        self.tx = Some(tx);
        self.tx_task = Some(tx_task);
        self.reader_task = Self::spawn_reader(msg.reader, ctx.actor_ref());
        self.reconnecting = false;
        warn!("[uart-mux] port re-opened");
    }
}

/// Send raw bytes to a UART port on the device.
pub struct SendFrame {
    /// UART port number (0-7).
    pub uart: u8,
    /// Raw bytes to transmit.
    pub data: Vec<u8>,
}

impl Message<SendFrame> for UartMuxActor {
    type Reply = Result<(), PortError>;

    async fn handle(
        &mut self,
        msg: SendFrame,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        if msg.uart >= 8 {
            return Err(PortError(format!("invalid UART number {}", msg.uart)));
        }
        let frame = build_frame(msg.uart, &msg.data);
        // Enqueue the frame for the TX task; no I/O happens here, so the actor
        // can process SendFrame messages at full speed. The TX task batches
        // the writes and flushes periodically, and the bounded channel gives
        // natural back-pressure if the device is slower than the producers.
        match &self.tx {
            Some(tx) => tx
                .send(frame)
                .await
                .map_err(|_| PortError("port tx task is gone".to_string())),
            None => Err(PortError("port is not open (reconnecting)".to_string())),
        }
    }
}


impl Message<ReceivedFrame> for UartMuxActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: ReceivedFrame,
        ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        // convert frame to uart and payload
        self.handle_recvd_frame(msg,ctx).await.log_error(" Cannot handle received frame ");
    }
}
pub struct StopTask;
impl Message<StopTask> for UartMuxActor {
    type Reply = ();

    async fn handle(
        &mut self,
        _msg: StopTask,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        if let Some(t) = self.tx_task.take() {
            t.abort();
        }
    }
}

impl Message<ToDevice> for UartMuxActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: ToDevice,
        ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.handle_to_device(msg).await.log_error(" Cannot send ");
    }
}

