use std::{sync::Arc, time::Instant};

use anyhow::Result;
use eframe::egui::{self, Slider};
use ehmi::Bar;
use limeros::{
    Msg, UdpMessage, UdpNode, msgs::{HeatingEvent, HeatingRequest, TypedMessage}
};
use tokio::sync::mpsc::{Receiver, Sender};

use crate::{my_window::MyWindow, widget_alive::WidgetAlive};

const MAX_UPDATE_INTERVAL_MS: u128 = 10000;

pub struct WindowHeater {
    node: Arc<UdpNode>,
    open: bool,
    enabled: bool,
    setpoint_c: f32,
    kp: f32,
    ki: f32,
    kd: f32,
    setpoint_previous: f32,
    enabled_previous: bool,
    kp_previous: f32,
    ki_previous: f32,
    kd_previous: f32,
    temperature_c: f32,
    output_pct: f32,
    heater_on: bool,
    fault: bool,
    last_update: Instant,
    sender: Sender<UdpMessage>,
    receiver: Receiver<UdpMessage>,
}

impl WindowHeater {
    pub fn new(node: Arc<UdpNode>) -> Self {
        let (sender, receiver) = tokio::sync::mpsc::channel(100);
        node.add_sender(sender.clone());
        Self {
            node,
            open: true,
            enabled: false,
            setpoint_c: 0.0,
            kp: 1.0,
            ki: 0.0,
            kd: 0.0,
            setpoint_previous: 0.0,
            enabled_previous: false,
            kp_previous: 1.0,
            ki_previous: 0.0,
            kd_previous: 0.0,
            temperature_c: 0.0,
            output_pct: 0.0,
            heater_on: false,
            fault: false,
            last_update: Instant::now(),
            sender,
            receiver,
        }
    }

    fn handle_event(&mut self) {
        while let Ok(udp_msg) = self.receiver.try_recv() {
            if udp_msg.typ.as_deref() != Some(HeatingEvent::MSG_TYPE) {
                continue;
            }
            if let Some(payload) = udp_msg.msg {
                if let Ok(event) = HeatingEvent::from_value(payload) {
                    self.last_update = Instant::now();
                    if let Some(value) = event.temperature_c {
                        self.temperature_c = value;
                    }
                    if let Some(value) = event.setpoint_c {
                        self.setpoint_c = value;
                        self.setpoint_previous = value;
                    }
                    if let Some(value) = event.output_pct {
                        self.output_pct = value;
                    }
                    if let Some(value) = event.enabled {
                        self.enabled = value;
                        self.enabled_previous = value;
                    }
                    if let Some(value) = event.heater_on {
                        self.heater_on = value;
                    }
                    if let Some(value) = event.fault {
                        self.fault = value;
                    }
                }
            }
        }
    }

    fn send_cmd(&mut self) {
        if self.setpoint_c == self.setpoint_previous
            && self.enabled == self.enabled_previous
            && self.kp == self.kp_previous
            && self.ki == self.ki_previous
            && self.kd == self.kd_previous
        {
            return;
        }

        let payload = HeatingRequest {
            setpoint_c: Some(self.setpoint_c),
            enabled: Some(self.enabled),
            kp: Some(self.kp),
            ki: Some(self.ki),
            kd: Some(self.kd),
            ..Default::default()
        };

        let _ = self.node.sender().try_send(UdpMessage {
            dst: Some("heater".to_string()),
            src: Some("egui-monitor".to_string()),
            typ: Some(HeatingRequest::MSG_TYPE.to_string()),
            msg: payload.to_value().ok(),
        });

        self.setpoint_previous = self.setpoint_c;
        self.enabled_previous = self.enabled;
        self.kp_previous = self.kp;
        self.ki_previous = self.ki;
        self.kd_previous = self.kd;
    }

    fn ui(&mut self, ui: &mut egui::Ui) {
        self.handle_event();

        let alive = Instant::now().duration_since(self.last_update).as_millis()
            < MAX_UPDATE_INTERVAL_MS;
        ui.add(WidgetAlive::new(alive));

        ui.horizontal(|ui| {
            ui.checkbox(&mut self.enabled, "Enabled");
            status_indicator(ui, "Heater", self.heater_on);
            status_indicator(ui, "Fault", self.fault);
        });

        ui.horizontal(|ui| {
            ui.spacing_mut().slider_width = 320.0;
            ui.add(
                Slider::new(&mut self.setpoint_c, 0.0..=300.0)
                    .suffix(" Setpoint (C)")
                    .step_by(0.5),
            );
        });

        ui.horizontal(|ui| {
            ui.label("PID");
            ui.add(Slider::new(&mut self.kp, 0.0..=50.0).prefix("Kp ").step_by(0.1));
            ui.add(Slider::new(&mut self.ki, 0.0..=10.0).prefix("Ki ").step_by(0.01));
            ui.add(Slider::new(&mut self.kd, 0.0..=10.0).prefix("Kd ").step_by(0.01));
        });

        ui.separator();
        ui.vertical(|ui| {
            ui.add(
                Bar::new(self.temperature_c)
                    .range(0.0..=350.0)
                    .bar_size(6.0)
                    .text(&format!("Temperature: {:.2} C", self.temperature_c)),
            );
            ui.add(
                Bar::new(self.output_pct)
                    .range(0.0..=100.0)
                    .bar_size(6.0)
                    .text(&format!("Output: {:.1}%", self.output_pct)),
            );
            ui.label(format!("Setpoint: {:.2} C", self.setpoint_c));
        });

        self.send_cmd();
    }
}

fn status_indicator(ui: &mut egui::Ui, label: &str, active: bool) {
    let color = if active {
        egui::Color32::GREEN
    } else {
        egui::Color32::DARK_GRAY
    };
    ui.horizontal(|ui| {
        ui.label(label);
        let rect_size = egui::Vec2::splat(14.0);
        let (rect, _) = ui.allocate_exact_size(rect_size, egui::Sense::hover());
        ui.painter().rect_filled(rect, 2.0, color);
    });
}

impl MyWindow for WindowHeater {
    fn name(&self) -> &'static str {
        "Heater"
    }

    fn show(&mut self, ui: &mut egui::Ui) -> Result<()> {
        let mut open = self.open;
        egui::Window::new(self.name())
            .open(&mut open)
            .resizable([true, true])
            .default_size([500.0, 240.0])
            .show(ui.ctx(), |ui| {
                self.ui(ui);
                ui.allocate_space(ui.available_size());
            });
        self.open = open;
        Ok(())
    }

    fn is_closed(&self) -> bool {
        !self.open
    }
}
