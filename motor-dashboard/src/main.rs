mod msgs;

use std::collections::VecDeque;
use std::fs::File;
use std::io::{BufWriter, Write};
use std::net::SocketAddr;
use std::path::{Path, PathBuf};
use std::sync::{Arc, Mutex};
use std::time::Instant;

use anyhow::{Context as _, Result};
use clap::Parser;
use eframe::egui::{self, Color32, Pos2, Sense, Stroke, Vec2};
use egui_plot::{Legend, Line, Plot, PlotPoints};
use kameo::actor::Spawn;
use kameo::message::{Context, Message};
use kameo::Actor;
use tokio::net::UdpSocket;
use tokio::sync::{mpsc, watch, RwLock};

const DASHBOARD_ID: u32 = fnv1a("motor-dashboard");
const UDP_ACTIVE_WINDOW: std::time::Duration = std::time::Duration::from_secs(1);
const HISTORY_WINDOW_SECONDS: f64 = 120.0;

#[derive(Parser)]
#[command(about = "UDP motor RPM dashboard")]
struct Args {
    #[arg(long, default_value = "0.0.0.0:50001")]
    bind: SocketAddr,
    #[arg(long)]
    peer: Option<SocketAddr>,
    #[arg(long, default_value = "motor-log.csv", value_name = "PATH")]
    log: PathBuf,
    #[arg(long, help = "Do not write a CSV log")]
    no_log: bool,
    #[arg(long, help = "Run a scripted step-response test and exit (no GUI)")]
    step_test: bool,
    #[arg(long, value_delimiter = ',', default_value = "500,1000,1500", help = "RPM steps for --step-test")]
    step_rpms: Vec<i32>,
    #[arg(long, default_value_t = 10, help = "Seconds to hold each --step-test target")]
    step_seconds: u64,
    #[arg(long, help = "Send these PID gains before running --step-test")]
    kp: Option<f32>,
    #[arg(long)]
    ki: Option<f32>,
    #[arg(long)]
    kd: Option<f32>,
}

struct CsvLog {
    writer: BufWriter<File>,
}

impl CsvLog {
    fn create(path: &Path) -> Result<Self> {
        let file = File::create(path).with_context(|| format!("creating log file {}", path.display()))?;
        let mut writer = BufWriter::new(file);
        writeln!(
            writer,
            "epoch_ms,src,rpm_measured,rpm_target,pwm_percent,current_a,voltage_v,temperature_c,kp,ki,kd,enabled"
        )
        .context("writing log header")?;
        writer.flush().context("flushing log header")?;
        Ok(Self { writer })
    }

    fn write_event(&mut self, source: SocketAddr, event: &msgs::MotorEvent) {
        let _ = writeln!(
            self.writer,
            "{},{},{},{},{},{},{},{},{},{},{},{}",
            epoch_millis(),
            source,
            event.rpm_measured.map_or(f32::NAN, |value| value as f32),
            event.rpm_target.map_or(f32::NAN, |value| value as f32),
            event.pwm.unwrap_or(f32::NAN),
            event.current.unwrap_or(f32::NAN),
            event.voltage.unwrap_or(f32::NAN),
            event.temperature.unwrap_or(f32::NAN),
            event.Kp.unwrap_or(f32::NAN),
            event.Ki.unwrap_or(f32::NAN),
            event.Kd.unwrap_or(f32::NAN),
            event.enabled.map_or("", |enabled| if enabled { "1" } else { "0" }),
        );
        // Flush so a crash does not lose the run being analysed.
        let _ = self.writer.flush();
    }
}

fn epoch_millis() -> u128 {
    std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_or(0, |elapsed| elapsed.as_millis())
}

#[derive(Clone, Debug, Default)]
struct Telemetry {
    measured_rpm: f32,
    target_rpm: f32,
    pwm_percent: f32,
    current: f32,
    voltage: f32,
    pid: Pid,
    peer: Option<SocketAddr>,
    last_received: Option<Instant>,
}

#[derive(Clone, Copy, Debug, Default, PartialEq)]
struct Pid {
    kp: f32,
    ki: f32,
    kd: f32,
}

#[derive(Actor)]
struct UdpActor {
    socket: Arc<UdpSocket>,
    peer: Arc<RwLock<Option<SocketAddr>>>,
}

#[derive(Clone, Copy, Debug, Default, PartialEq)]
struct MotorCommand {
    rpm_target: Option<i32>,
    kp: Option<f32>,
    ki: Option<f32>,
    kd: Option<f32>,
}

impl MotorCommand {
    fn merge(&mut self, newer: MotorCommand) {
        if newer.rpm_target.is_some() {
            self.rpm_target = newer.rpm_target;
        }
        if newer.kp.is_some() {
            self.kp = newer.kp;
        }
        if newer.ki.is_some() {
            self.ki = newer.ki;
        }
        if newer.kd.is_some() {
            self.kd = newer.kd;
        }
    }
}

impl Message<MotorCommand> for UdpActor {
    type Reply = Result<(), String>;

    async fn handle(&mut self, message: MotorCommand, _ctx: &mut Context<Self, Self::Reply>) -> Self::Reply {
        let peer = self.peer.read().await.ok_or("No ESP32 peer discovered; wait for telemetry or pass --peer")?;
        send_motor_request(&self.socket, peer, message).await
    }
}

async fn send_motor_request(socket: &UdpSocket, peer: SocketAddr, command: MotorCommand) -> Result<(), String> {
    let request = msgs::MotorRequest {
        enabled: None,
        rpm_target: command.rpm_target,
        Kp: command.kp,
        Ki: command.ki,
        Kd: command.kd,
    };
    let payload = request.to_bytes().map_err(|error| error.to_string())?;
    let envelope = msgs::Envelope {
        src: Some(DASHBOARD_ID),
        dst: Some(msgs::MOTOR_ID),
        msg_type: Some(msgs::MotorRequest::id()),
        sequence: None,
        instance_id: None,
        payload: Some(payload),
    };
    let frame = envelope.to_bytes().map_err(|error| error.to_string())?;
    socket
        .send_to(&frame, peer)
        .await
        .map_err(|error| error.to_string())?;
    Ok(())
}

struct DashboardApp {
    telemetry: watch::Receiver<Telemetry>,
    command_tx: mpsc::UnboundedSender<MotorCommand>,
    requested_rpm: i32,
    pid: Pid,
    pid_initialized: bool,
    log_status: Option<String>,
    started: Instant,
    last_sample: Option<Instant>,
    measured_history: VecDeque<[f64; 2]>,
    target_history: VecDeque<[f64; 2]>,
    pwm_history: VecDeque<[f64; 2]>,
}

impl DashboardApp {
    fn send_rpm(&self) {
        let _ = self.command_tx.send(MotorCommand {
            rpm_target: Some(self.requested_rpm),
            ..MotorCommand::default()
        });
    }

    fn send_pid(&self) {
        let _ = self.command_tx.send(MotorCommand {
            kp: Some(self.pid.kp),
            ki: Some(self.pid.ki),
            kd: Some(self.pid.kd),
            ..MotorCommand::default()
        });
    }

    fn record_sample(&mut self, telemetry: &Telemetry) {
        if telemetry.last_received.is_none() || telemetry.last_received == self.last_sample {
            return;
        }
        self.last_sample = telemetry.last_received;
        let seconds = self.started.elapsed().as_secs_f64();
        push_sample(&mut self.measured_history, seconds, telemetry.measured_rpm);
        push_sample(&mut self.target_history, seconds, telemetry.target_rpm);
        push_sample(&mut self.pwm_history, seconds, telemetry.pwm_percent);
    }
}

fn push_sample(history: &mut VecDeque<[f64; 2]>, seconds: f64, rpm: f32) {
    history.push_back([seconds, rpm as f64]);
    while history.front().is_some_and(|point| point[0] < seconds - HISTORY_WINDOW_SECONDS) {
        history.pop_front();
    }
}

async fn send_motor_commands(mut commands: mpsc::UnboundedReceiver<MotorCommand>, udp: kameo::actor::ActorRef<UdpActor>) {
    const MIN_INTERVAL: std::time::Duration = std::time::Duration::from_millis(20);
    let mut last_sent: Option<MotorCommand> = None;
    while let Some(mut command) = commands.recv().await {
        tokio::time::sleep(MIN_INTERVAL).await;
        while let Ok(newer) = commands.try_recv() {
            command.merge(newer);
        }
        if last_sent == Some(command) {
            continue;
        }
        if udp.ask(command).await.is_ok() {
            last_sent = Some(command);
        }
    }
}

impl eframe::App for DashboardApp {
    fn update(&mut self, context: &egui::Context, _frame: &mut eframe::Frame) {
        let telemetry = self.telemetry.borrow_and_update().clone();
        self.record_sample(&telemetry);
        if !self.pid_initialized && telemetry.last_received.is_some() {
            self.pid = telemetry.pid;
            self.pid_initialized = true;
        }
        let udp_active = telemetry
            .last_received
            .is_some_and(|received| received.elapsed() < UDP_ACTIVE_WINDOW);
        egui::CentralPanel::default().show(context, |ui| {
            ui.horizontal(|ui| {
                ui.heading("Motor Control");
                ui.with_layout(egui::Layout::right_to_left(egui::Align::Center), |ui| {
                    udp_led(ui, udp_active);
                });
            });
            ui.label(match telemetry.peer {
                Some(peer) => format!("Connected to {peer}"),
                None => "Waiting for motor telemetry...".to_owned(),
            });
            ui.add_space(16.0);
            ui.columns(2, |columns| {
                rpm_gauge(&mut columns[0], "Measured RPM", telemetry.measured_rpm, 2500.0, Color32::from_rgb(54, 168, 111));
                rpm_gauge(&mut columns[1], "Target RPM", telemetry.target_rpm, 2500.0, Color32::from_rgb(52, 123, 205));
            });
            ui.add_space(16.0);
            let response = ui.add(egui::Slider::new(&mut self.requested_rpm, 0..=2500).text("RPM command"));
            if response.changed() || response.drag_stopped() {
                self.send_rpm();
            }
            ui.label(format!("PWM output: {:.1}%", telemetry.pwm_percent));
            ui.horizontal(|ui| {
                ui.label(format!("Current: {:.2} A", telemetry.current));
                ui.separator();
                ui.label(format!("Voltage: {:.2} V", telemetry.voltage));
            });
            ui.add_space(12.0);
            ui.label("PID gains")
                .on_hover_text("Current = reported by the motor; Target = sent in a MotorRequest when edited");
            let mut pid_edited = false;
            egui::Grid::new("pid_grid")
                .num_columns(3)
                .spacing([16.0, 4.0])
                .show(ui, |ui| {
                    ui.label(egui::RichText::new("Gain").weak());
                    ui.label(egui::RichText::new("Current").weak());
                    ui.label(egui::RichText::new("Target").weak());
                    ui.end_row();

                    ui.label("Kp");
                    ui.monospace(format!("{:.4}", telemetry.pid.kp));
                    pid_edited |= ui
                        .add(egui::DragValue::new(&mut self.pid.kp).speed(0.01).fixed_decimals(4))
                        .changed();
                    ui.end_row();

                    ui.label("Ki");
                    ui.monospace(format!("{:.4}", telemetry.pid.ki));
                    pid_edited |= ui
                        .add(egui::DragValue::new(&mut self.pid.ki).speed(0.001).fixed_decimals(4))
                        .changed();
                    ui.end_row();

                    ui.label("Kd");
                    ui.monospace(format!("{:.4}", telemetry.pid.kd));
                    pid_edited |= ui
                        .add(egui::DragValue::new(&mut self.pid.kd).speed(0.001).fixed_decimals(4))
                        .changed();
                    ui.end_row();
                });
            if pid_edited {
                self.send_pid();
            }
            ui.add_space(16.0);
            ui.label("RPM history");
            let measured: Vec<[f64; 2]> = self.measured_history.iter().copied().collect();
            let target: Vec<[f64; 2]> = self.target_history.iter().copied().collect();
            Plot::new("rpm_history")
                .legend(Legend::default())
                .height(200.0)
                .include_y(0.0)
                .x_axis_label("seconds")
                .y_axis_label("RPM")
                .show(ui, |plot_ui| {
                    plot_ui.line(
                        Line::new("measured", PlotPoints::from(measured))
                            .color(Color32::from_rgb(54, 168, 111))
                            .width(2.0_f32),
                    );
                    plot_ui.line(
                        Line::new("target", PlotPoints::from(target))
                            .color(Color32::from_rgb(52, 123, 205))
                            .width(2.0_f32),
                    );
                });
            ui.add_space(12.0);
            ui.label("PWM output history");
            let pwm: Vec<[f64; 2]> = self.pwm_history.iter().copied().collect();
            Plot::new("pwm_history")
                .legend(Legend::default())
                .height(170.0)
                .include_y(0.0)
                .include_y(100.0)
                .x_axis_label("seconds")
                .y_axis_label("percent")
                .show(ui, |plot_ui| {
                    plot_ui.line(
                        Line::new("PWM", PlotPoints::from(pwm))
                            .color(Color32::from_rgb(230, 160, 60))
                            .width(2.0_f32),
                    );
                });
            if let Some(status) = &self.log_status {
                ui.add_space(8.0);
                ui.label(egui::RichText::new(status).weak().small());
            }
        });
        context.request_repaint_after(std::time::Duration::from_millis(100));
    }
}

fn udp_led(ui: &mut egui::Ui, active: bool) {
    let (response, painter) = ui.allocate_painter(Vec2::new(18.0, 18.0), Sense::hover());
    let center = response.rect.center();
    let glow = Color32::from_rgba_unmultiplied(54, 200, 92, 70);
    let on = Color32::from_rgb(54, 200, 92);
    let off = Color32::from_gray(70);
    if active {
        painter.circle_filled(center, 8.0, glow);
    }
    painter.circle_filled(center, 5.0, if active { on } else { off });
    response.on_hover_text(if active {
        "Receiving UDP telemetry"
    } else {
        "No UDP telemetry received recently"
    });
}

fn rpm_gauge(ui: &mut egui::Ui, label: &str, rpm: f32, max_rpm: f32, color: Color32) {
    let (response, painter) = ui.allocate_painter(Vec2::splat(220.0), Sense::hover());
    let center = response.rect.center();
    let radius = 82.0;
    let start = std::f32::consts::PI * 0.75_f32;
    let end = std::f32::consts::PI * 2.25_f32;
    painter.circle_stroke(center, radius, Stroke::new(12.0_f32, Color32::from_gray(55)));
    let fraction = (rpm / max_rpm).clamp(0.0, 1.0);
    let points: Vec<Pos2> = (0..=64)
        .map(|index| center + Vec2::angled(start + (end - start) * fraction * index as f32 / 64.0) * radius)
        .collect();
    painter.add(egui::Shape::line(points, Stroke::new(12.0_f32, color)));
    painter.text(center + Vec2::new(0.0, -14.0), egui::Align2::CENTER_CENTER, format!("{rpm:.0}"), egui::FontId::proportional(34.0), Color32::WHITE);
    painter.text(center + Vec2::new(0.0, 22.0), egui::Align2::CENTER_CENTER, "RPM", egui::FontId::proportional(14.0), Color32::LIGHT_GRAY);
    painter.text(response.rect.center_bottom() - Vec2::new(0.0, 8.0), egui::Align2::CENTER_BOTTOM, label, egui::FontId::proportional(16.0), Color32::WHITE);
}

fn open_log(args: &Args) -> Option<Arc<Mutex<CsvLog>>> {
    if args.no_log {
        return None;
    }
    match CsvLog::create(&args.log) {
        Ok(log) => {
            println!("Logging MotorEvent to {}", args.log.display());
            Some(Arc::new(Mutex::new(log)))
        }
        Err(error) => {
            eprintln!("Warning: CSV logging disabled: {error:#}");
            None
        }
    }
}

fn spawn_receiver(
    socket: Arc<UdpSocket>,
    peer: Arc<RwLock<Option<SocketAddr>>>,
    log: Option<Arc<Mutex<CsvLog>>>,
    telemetry_tx: watch::Sender<Telemetry>,
) {
    tokio::spawn(async move {
        let mut buffer = [0_u8; 1024];
        while let Ok((size, source)) = socket.recv_from(&mut buffer).await {
            let Ok(envelope) = msgs::Envelope::from_bytes(&buffer[..size]) else { continue; };
            if envelope.msg_type != Some(msgs::MotorEvent::id()) { continue; }
            let Some(payload) = envelope.payload else { continue; };
            let Ok(event) = msgs::MotorEvent::from_bytes(&payload) else { continue; };
            if let Some(log) = &log {
                if let Ok(mut log) = log.lock() {
                    log.write_event(source, &event);
                }
            }
            *peer.write().await = Some(source);
            let _ = telemetry_tx.send(Telemetry {
                measured_rpm: event.rpm_measured.unwrap_or_default() as f32,
                target_rpm: event.rpm_target.unwrap_or_default() as f32,
                pwm_percent: event.pwm.unwrap_or_default(),
                current: event.current.unwrap_or_default(),
                voltage: event.voltage.unwrap_or_default(),
                pid: Pid {
                    kp: event.Kp.unwrap_or_default(),
                    ki: event.Ki.unwrap_or_default(),
                    kd: event.Kd.unwrap_or_default(),
                },
                peer: Some(source),
                last_received: Some(Instant::now()),
            });
        }
    });
}

async fn run_step_test(args: &Args, socket: Arc<UdpSocket>, peer: Arc<RwLock<Option<SocketAddr>>>) -> Result<()> {
    let deadline = Instant::now() + std::time::Duration::from_secs(10);
    let peer_address = loop {
        if let Some(address) = *peer.read().await {
            break address;
        }
        if Instant::now() >= deadline {
            anyhow::bail!("no telemetry from the motor; is it powered and on the network?");
        }
        tokio::time::sleep(std::time::Duration::from_millis(100)).await;
    };
    println!("Motor peer {peer_address}");

    if args.kp.is_some() || args.ki.is_some() || args.kd.is_some() {
        send_motor_request(
            &socket,
            peer_address,
            MotorCommand {
                kp: args.kp,
                ki: args.ki,
                kd: args.kd,
                ..MotorCommand::default()
            },
        )
        .await
        .map_err(|error| anyhow::anyhow!("sending PID gains: {error}"))?;
        println!("sent PID gains kp={:?} ki={:?} kd={:?}", args.kp, args.ki, args.kd);
        tokio::time::sleep(std::time::Duration::from_millis(300)).await;
    }

    let mut schedule: Vec<(i32, u64)> = vec![(0, 2)];
    schedule.extend(args.step_rpms.iter().map(|rpm| (*rpm, args.step_seconds)));
    schedule.push((0, 3));

    for (rpm, seconds) in schedule {
        send_motor_request(
            &socket,
            peer_address,
            MotorCommand {
                rpm_target: Some(rpm),
                ..MotorCommand::default()
            },
        )
        .await
        .map_err(|error| anyhow::anyhow!("sending target {rpm} RPM: {error}"))?;
        println!("target {rpm:>4} RPM for {seconds} s");
        tokio::time::sleep(std::time::Duration::from_secs(seconds)).await;
    }

    println!("step test finished; telemetry saved to {}", args.log.display());
    Ok(())
}

#[tokio::main]
async fn main() -> Result<()> {
    let args = Args::parse();
    let socket = Arc::new(UdpSocket::bind(args.bind).await.context("binding UDP socket")?);
    let peer = Arc::new(RwLock::new(args.peer));
    let log = open_log(&args);
    let log_status = log
        .as_ref()
        .map(|_| format!("Logging MotorEvent to {}", args.log.display()));
    let (telemetry_tx, telemetry_rx) = watch::channel(Telemetry::default());
    spawn_receiver(socket.clone(), peer.clone(), log, telemetry_tx);

    if args.step_test {
        return run_step_test(&args, socket.clone(), peer).await;
    }

    let udp = UdpActor::spawn(UdpActor { socket, peer });
    let (command_tx, command_rx) = mpsc::unbounded_channel();
    tokio::spawn(send_motor_commands(command_rx, udp));
    let app = DashboardApp {
        telemetry: telemetry_rx,
        command_tx,
        requested_rpm: 1000,
        pid: Pid::default(),
        pid_initialized: false,
        log_status,
        started: Instant::now(),
        last_sample: None,
        measured_history: VecDeque::new(),
        target_history: VecDeque::new(),
        pwm_history: VecDeque::new(),
    };
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_inner_size([1000.0, 1000.0]),
        ..Default::default()
    };
    eframe::run_native("Motor Dashboard", options, Box::new(|_| Ok(Box::new(app))))
        .map_err(|error| anyhow::anyhow!("{error:?}"))
}

const fn fnv1a(value: &str) -> u32 {
    let bytes = value.as_bytes();
    let mut index = 0;
    let mut hash = 2_166_136_261_u32;
    while index < bytes.len() {
        hash = (hash ^ bytes[index] as u32).wrapping_mul(16_777_619);
        index += 1;
    }
    hash
}