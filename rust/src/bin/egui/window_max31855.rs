use std::{sync::Arc, time::Instant};

use anyhow::Result;
use eframe::egui;
use ehmi::Bar;
use limeros::{Msg,
    UdpMessage, UdpNode,
    msgs::{Max31855Event, TypedMessage},
};
use limeros::msgs::TypedMessage as _;
use tokio::sync::mpsc::{Receiver, Sender};

use crate::{my_window::MyWindow, widget_alive::WidgetAlive};

const MAX_UPDATE_INTERVAL_MS: u128 = 10000;

pub struct WindowMax31855 {
    node: Arc<UdpNode>,
    open: bool,
    last_update: Instant,
    thermocouple_c: f32,
    internal_c: f32,
    fault: bool,
    open_circuit: bool,
    short_to_gnd: bool,
    short_to_vcc: bool,
    sender: Sender<UdpMessage>,
    receiver: Receiver<UdpMessage>,
}

impl WindowMax31855 {
    pub fn new(node: Arc<UdpNode>) -> Self {
        let (sender, receiver) = tokio::sync::mpsc::channel(100);
        node.add_sender(sender.clone());
        Self {
            node,
            open: true,
            last_update: Instant::now(),
            thermocouple_c: 0.0,
            internal_c: 0.0,
            fault: false,
            open_circuit: false,
            short_to_gnd: false,
            short_to_vcc: false,
            sender,
            receiver,
        }
    }

    fn handle_event(&mut self) {
        while let Ok(udp_msg) = self.receiver.try_recv() {
            if udp_msg.typ.as_deref() != Some(Max31855Event::MSG_TYPE) {
                continue;
            }
            if let Some(payload) = udp_msg.msg {
                if let Ok(event) = Max31855Event::from_value(payload) {
                    self.last_update = Instant::now();
                    event.thermocouple_c.map(|v| self.thermocouple_c = v);
                    event.internal_c.map(|v| self.internal_c = v);
                    event.fault.map(|v| self.fault = v);
                    event.open_circuit.map(|v| self.open_circuit = v);
                    event.short_to_gnd.map(|v| self.short_to_gnd = v);
                    event.short_to_vcc.map(|v| self.short_to_vcc = v);
                }
            }
        }
    }

    fn ui(&mut self, ui: &mut egui::Ui) {
        let alive = Instant::now()
            .duration_since(self.last_update)
            .as_millis()
            < MAX_UPDATE_INTERVAL_MS;
        ui.add(WidgetAlive::new(alive));

        ui.vertical(|ui| {
            let text = format!("Thermocouple: {:.2} °C", self.thermocouple_c);
            ui.add(
                Bar::new(self.thermocouple_c)
                    .range(-50.0..=1000.0)
                    .bar_size(5.0)
                    .text(&text),
            );
            let text = format!("Internal: {:.2} °C", self.internal_c);
            ui.add(
                Bar::new(self.internal_c)
                    .range(-50.0..=100.0)
                    .bar_size(5.0)
                    .text(&text),
            );
        });

        ui.separator();
        ui.label("Fault Status");
        ui.horizontal(|ui| {
            fault_indicator(ui, "Fault", self.fault);
            fault_indicator(ui, "Open Circuit", self.open_circuit);
            fault_indicator(ui, "Short to GND", self.short_to_gnd);
            fault_indicator(ui, "Short to VCC", self.short_to_vcc);
        });
    }
}

fn fault_indicator(ui: &mut egui::Ui, label: &str, active: bool) {
    let color = if active {
        egui::Color32::RED
    } else {
        egui::Color32::GREEN
    };
    ui.vertical(|ui| {
        ui.label(label);
        let rect_size = egui::Vec2::splat(16.0);
        let (rect_id, _) = ui.allocate_exact_size(rect_size, egui::Sense::hover());
        ui.painter().rect_filled(rect_id, 2.0, color);
    });
}

impl MyWindow for WindowMax31855 {
    fn name(&self) -> &'static str {
        "Max31855"
    }

    fn show(&mut self, ui: &mut egui::Ui) -> Result<()> {
        self.handle_event();
        let mut open = self.open;
        egui::Window::new(self.name())
            .open(&mut open)
            .resizable([true, true])
            .default_size([300.0, 200.0])
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
