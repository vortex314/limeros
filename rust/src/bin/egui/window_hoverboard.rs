use std::{sync::Arc, time::Instant};

use anyhow::Result;
use eframe::egui::{self, Slider};
use ehmi::{Bar, Gauge};
use limeros::{
    Msg, TypedMessage, UdpMessage, UdpNode,
    msgs::{HoverboardEvent, HoverboardRequest},
};
use log::info;
use tokio::sync::mpsc::{Receiver, Sender};

use crate::{
    my_window::{EndpointWindowContext, MyWindow, Supports, WindowRegistration},
    widget_alive::WidgetAlive,
};

const MAX_UPDATE_INTERVAL_MS: u128 = 10_000;

pub struct HoverboardWindow {
    node: Arc<UdpNode>,
    source: String,
    open: bool,
    enabled: bool,
    speed_slider: f32,
    steer_slider: f32,
    speed_previous: f32,
    steer_previous: f32,
    last_update: Instant,
    speed_left: f32,
    speed_right: f32,
    temperature: f32,
    voltage: f32,
    current: f32,
    _sender: Sender<UdpMessage>,
    receiver: Receiver<UdpMessage>,
}

impl HoverboardWindow {
    pub fn new(node: Arc<UdpNode>, source: impl Into<String>) -> Self {
        let source = source.into();
        let (sender, receiver) = tokio::sync::mpsc::channel(100);
        node.add_sender(sender.clone());
        Self {
            node,
            source,
            open: true,
            enabled: true,
            speed_slider: 0.0,
            steer_slider: 0.0,
            speed_previous: 0.0,
            steer_previous: 0.0,
            last_update: Instant::now(),
            speed_left: 0.0,
            speed_right: 0.0,
            temperature: 0.0,
            voltage: 0.0,
            current: 0.0,
            _sender: sender,
            receiver,
        }
    }

    pub fn supports_config() -> Supports {
        Supports {
            publishes: vec![HoverboardRequest::NAME.to_string()],
            subscribes: vec![HoverboardEvent::NAME.to_string()],
            ..Default::default()
        }
    }

    pub fn registration() -> WindowRegistration {
        WindowRegistration::endpoint(
            "hoverboard",
            "Hoverboard Control",
            Self::supports_config,
            |ctx: EndpointWindowContext| Box::new(Self::new(ctx.node, ctx.source)),
        )
    }

    fn handle_event(&mut self) -> Result<()> {
        while let Ok(udp_msg) = self.receiver.try_recv() {
            if udp_msg.src.as_deref() != Some(&self.source)
                || udp_msg.typ.as_deref() != Some(HoverboardEvent::MSG_TYPE)
            {
                continue;
            }

            if let Some(payload) = udp_msg.msg.as_ref() {
                let event = HoverboardEvent::from_value(payload.clone())?;
                self.last_update = Instant::now();
                event.spdl.map(|v| self.speed_left = v as f32);
                event.spdr.map(|v| self.speed_right = v as f32);
                event.temp.map(|v| self.temperature = v as f32);
                event.batv.map(|v| self.voltage = v as f32);
                event.dc_curr.map(|v| self.current = v as f32);
            }
        }
        Ok(())
    }

    fn send_cmd(&mut self) {
        if self.speed_slider == self.speed_previous && self.steer_slider == self.steer_previous {
            return;
        }

        let req = HoverboardRequest {
            speed: Some(self.speed_slider as i32),
            steer: Some(self.steer_slider as i32),
            ..Default::default()
        };
        let payload = req.to_value().unwrap();
        let _ = self.node.sender().try_send(UdpMessage {
            dst: Some(self.source.clone()),
            src: None,
            typ: Some(HoverboardRequest::MSG_TYPE.to_string()),
            msg: Some(payload),
        });
        info!(
            "Sent HoverboardRequest: speed={} steer={} to {}",
            self.speed_slider, self.steer_slider, self.source
        );
        self.speed_previous = self.speed_slider;
        self.steer_previous = self.steer_slider;
    }

    fn ui(&mut self, ui: &mut egui::Ui) {
        self.enabled = Instant::now().duration_since(self.last_update).as_millis()
            < MAX_UPDATE_INTERVAL_MS;
        ui.add(WidgetAlive::new(self.enabled));

        ui.horizontal(|ui| {
            ui.spacing_mut().slider_width = 400.0;
            ui.add(
                Slider::new(&mut self.speed_slider, -400.0..=400.0)
                    .suffix(" Speed")
                    .step_by(1.0),
            );
        });
        ui.horizontal(|ui| {
            ui.spacing_mut().slider_width = 400.0;
            ui.add(
                Slider::new(&mut self.steer_slider, -100.0..=100.0)
                    .suffix(" Steer")
                    .step_by(1.0),
            );
        });
        ui.vertical_centered(|ui| {
            ui.horizontal(|ui| {
                let text = format!("Left Wheel: \n{:.1} RPM", self.speed_left);
                ui.add(
                    Gauge::new(self.speed_left)
                        .size(200.0)
                        .text(&text)
                        .range(-400f64..=400f64)
                        .stroke_width(0.0)
                        .angle_range(-45i16..=225i16),
                );
                ui.add(egui::Label::new("    "));
                let text = format!("Right Wheel: \n{:.1} RPM", self.speed_right);
                ui.add(
                    Gauge::new(self.speed_right)
                        .size(200.0)
                        .text(&text)
                        .range(-400f64..=400f64)
                        .stroke_width(0.0)
                        .angle_range(-45i16..=225i16),
                );
            });
        });

        ui.vertical(|ui| {
            let text = format!("Temperature: \n{:.1} °C", self.temperature);
            ui.add(
                Bar::new(self.temperature)
                    .range(-20.0..=100.0)
                    .bar_size(5.0)
                    .text(&text),
            );
            let text = format!("Battery: \n{:.1} V", self.voltage);
            ui.add(
                Bar::new(self.voltage)
                    .range(-20.0..=100.0)
                    .bar_size(5.0)
                    .text(&text),
            );
            let text = format!("Current: \n{:.1} A", self.current);
            ui.add(
                Bar::new(self.current)
                    .range(-20.0..=100.0)
                    .bar_size(5.0)
                    .text(&text),
            );
        });
        self.send_cmd();
    }
}

impl MyWindow for HoverboardWindow {
    fn name(&self) -> &'static str {
        "Hoverboard Control"
    }

    fn show(&mut self, ui: &mut egui::Ui) -> Result<()> {
        let _ = self.handle_event();
        let mut open = self.open;
        egui::Window::new(format!("{} [{}]", self.name(), self.source))
            .open(&mut open)
            .resizable([true, true])
            .default_size([200.0, 200.0])
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

    fn supports(&self) -> Option<Supports> {
        Some(Self::supports_config())
    }
}
