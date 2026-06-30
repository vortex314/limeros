use std::{sync::Arc, time::Instant};

use anyhow::Result;
use dashmap::DashMap;
use eframe::egui;
use limeros::{Endpoint, UdpNode};

use crate::{
    my_window::{EndpointWindowContext, MyWindow, WindowRegistrationKind},
    window_registry,
};

pub struct WindowEndpoints {
    node: Arc<UdpNode>,
    endpoints: Arc<DashMap<String, Endpoint>>,
    window_others: Arc<DashMap<String, Box<dyn MyWindow>>>,
}

impl WindowEndpoints {
    pub fn new(
        node: Arc<UdpNode>,
        endpoints: Arc<DashMap<String, Endpoint>>,
        window_others: Arc<DashMap<String, Box<dyn MyWindow>>>,
    ) -> Self {
        Self {
            node,
            endpoints,
            window_others,
        }
    }

    fn render_attach_menu(&self, ui: &mut egui::Ui, source: &str, endpoint: &Endpoint) {
        let supported = window_registry::endpoint_window_registrations()
            .into_iter()
            .filter(|registration| registration.supports_endpoint(endpoint))
            .collect::<Vec<_>>();

        if supported.is_empty() {
            ui.label("-");
            return;
        }

        egui::ComboBox::from_id_salt(format!("attach_endpoint_{source}"))
            .selected_text("Attach…")
            .width(160.0)
            .show_ui(ui, |ui| {
                for registration in supported {
                    let window_key = registration.build_endpoint_key(source);
                    let already_open = self.window_others.contains_key(&window_key);
                    if ui
                        .add_enabled(!already_open, egui::Button::new(registration.label))
                        .clicked()
                    {
                        if let WindowRegistrationKind::Endpoint(factory) = registration.kind {
                            self.window_others.insert(
                                window_key,
                                factory(EndpointWindowContext {
                                    node: self.node.clone(),
                                    source: source.to_owned(),
                                    endpoint: endpoint.clone(),
                                }),
                            );
                        }
                    }
                }
            });
    }
}

impl MyWindow for WindowEndpoints {
    fn name(&self) -> &'static str {
        "Endpoints"
    }

    fn show(&mut self, ui: &mut egui::Ui) -> Result<()> {
        egui::Window::new("Endpoints")
            .open(&mut true)
            .resizable([true, true])
            .constrain_to(ui.available_rect_before_wrap())
            .show(ui.ctx(), |ui| {
                egui::Grid::new("endpoint_grid").striped(true).show(ui, |ui| {
                    ui.label("Peer ID");
                    ui.label("Address");
                    ui.label("Subscribe");
                    ui.label("Publish");
                    ui.label("Services");
                    ui.label("Last Seen");
                    ui.label("Attach");
                    ui.end_row();

                    for entry in self.endpoints.iter() {
                        let source = entry.key().to_string();
                        let ep = entry.value().clone();
                        let secs_ago = Instant::now().duration_since(ep.last_seen).as_secs();

                        ui.label(&source);
                        ui.label(ep.addr.to_string());
                        ui.label(format!("{:?}", ep.subscribe));
                        ui.label(format!("{:?}", ep.publish));
                        ui.label(format!("{:?}", ep.services));
                        ui.label(format!("{} sec ago", secs_ago));
                        self.render_attach_menu(ui, &source, &ep);
                        ui.end_row();
                    }
                });
            });
        Ok(())
    }

    fn is_closed(&self) -> bool {
        false
    }
}

