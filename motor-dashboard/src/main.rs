mod msgs;

use std::net::SocketAddr;
use std::sync::Arc;

use anyhow::{Context as _, Result};
use clap::Parser;
use eframe::egui::{self, Color32, Pos2, Sense, Stroke, Vec2};
use kameo::actor::Spawn;
use kameo::message::{Context, Message};
use kameo::Actor;
use tokio::net::UdpSocket;
use tokio::sync::{watch, RwLock};

const DASHBOARD_ID: u32 = fnv1a("motor-dashboard");

#[derive(Parser)]
#[command(about = "UDP motor RPM dashboard")]
struct Args {
    #[arg(long, default_value = "0.0.0.0:50001")]
    bind: SocketAddr,
    #[arg(long)]
    peer: Option<SocketAddr>,
}

#[derive(Clone, Debug, Default)]
struct Telemetry {
    measured_rpm: f32,
    target_rpm: f32,
    pwm_percent: f32,
    peer: Option<SocketAddr>,
}

#[derive(Actor)]
struct UdpActor {
    socket: Arc<UdpSocket>,
    peer: Arc<RwLock<Option<SocketAddr>>>,
}

struct SetRpm(i32);

impl Message<SetRpm> for UdpActor {
    type Reply = Result<(), String>;

    async fn handle(&mut self, message: SetRpm, _ctx: &mut Context<Self, Self::Reply>) -> Self::Reply {
        let peer = self.peer.read().await.ok_or("No ESP32 peer discovered; wait for telemetry or pass --peer")?;
        let payload = msgs::MotorRequest { enabled: None, rpm_target: Some(message.0) }
            .to_bytes()
            .map_err(|error| error.to_string())?;
        let envelope = msgs::Envelope {
            src: Some(DASHBOARD_ID),
            dst: Some(msgs::MOTOR_ID),
            msg_type: Some(msgs::MotorRequest::id()),
            sequence: None,
            instance_id: None,
            payload: Some(payload),
        };
        let frame = envelope.to_bytes().map_err(|error| error.to_string())?;
        self.socket.send_to(&frame, peer).await.map_err(|error| error.to_string())?;
        Ok(())
    }
}

struct DashboardApp {
    telemetry: watch::Receiver<Telemetry>,
    udp: kameo::actor::ActorRef<UdpActor>,
    requested_rpm: i32,
    last_sent_rpm: i32,
}

impl DashboardApp {
    fn send_rpm(&mut self) {
        if self.requested_rpm == self.last_sent_rpm {
            return;
        }
        self.last_sent_rpm = self.requested_rpm;
        let udp = self.udp.clone();
        let rpm = self.requested_rpm;
        tokio::spawn(async move {
            let _ = udp.ask(SetRpm(rpm)).await;
        });
    }
}

impl eframe::App for DashboardApp {
    fn update(&mut self, context: &egui::Context, _frame: &mut eframe::Frame) {
        let telemetry = self.telemetry.borrow_and_update().clone();
        egui::CentralPanel::default().show(context, |ui| {
            ui.heading("Motor Control");
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
            let changed = ui.add(egui::Slider::new(&mut self.requested_rpm, 0..=2500).text("RPM command")).changed();
            ui.label(format!("PWM output: {:.1}%", telemetry.pwm_percent));
            if changed || ui.button("Send RPM").clicked() {
                self.send_rpm();
            }
        });
        context.request_repaint_after(std::time::Duration::from_millis(100));
    }
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

#[tokio::main]
async fn main() -> Result<()> {
    let args = Args::parse();
    let socket = Arc::new(UdpSocket::bind(args.bind).await.context("binding UDP socket")?);
    let peer = Arc::new(RwLock::new(args.peer));
    let (telemetry_tx, telemetry_rx) = watch::channel(Telemetry::default());
    let receiver_socket = socket.clone();
    let receiver_peer = peer.clone();
    tokio::spawn(async move {
        let mut buffer = [0_u8; 1024];
        while let Ok((size, source)) = receiver_socket.recv_from(&mut buffer).await {
            let Ok(envelope) = msgs::Envelope::from_bytes(&buffer[..size]) else { continue; };
            if envelope.msg_type != Some(msgs::MotorEvent::id()) { continue; }
            let Some(payload) = envelope.payload else { continue; };
            let Ok(event) = msgs::MotorEvent::from_bytes(&payload) else { continue; };
            *receiver_peer.write().await = Some(source);
            let _ = telemetry_tx.send(Telemetry {
                measured_rpm: event.rpm_actual.unwrap_or_default() as f32,
                target_rpm: event.rpm_target.unwrap_or_default() as f32,
                pwm_percent: event.pwm.unwrap_or_default(),
                peer: Some(source),
            });
        }
    });
    let udp = UdpActor::spawn(UdpActor { socket, peer });
    let app = DashboardApp { telemetry: telemetry_rx, udp, requested_rpm: 1000, last_sent_rpm: -1 };
    eframe::run_native("Motor Dashboard", eframe::NativeOptions::default(), Box::new(|_| Ok(Box::new(app))))
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