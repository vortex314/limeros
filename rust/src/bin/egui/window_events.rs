use std::{collections::HashSet, sync::Arc};

use anyhow::Result;
use dashmap::DashMap;
use eframe::egui;
use limeros::{Endpoint, UdpNode};

use crate::{
    Record,
    my_window::{EndpointWindowContext, FieldWindowContext, MyWindow, WindowRegistrationKind},
    window_registry,
};

#[derive(PartialEq, Clone, Copy)]
enum SortColumn {
    Time,
    Source,
    Type,
    Field,
}

pub struct WindowEvents {
    node: Arc<UdpNode>,
    cache: Arc<DashMap<String, Record>>,
    endpoints: Arc<DashMap<String, Endpoint>>,
    sort_col: SortColumn,
    sort_desc: bool,
    selected_fields: HashSet<String>,
    window_others: Arc<DashMap<String, Box<dyn MyWindow>>>,
    graph_data: Arc<DashMap<String, crate::MetricData>>,
}

impl WindowEvents {
    pub fn new(
        node: Arc<UdpNode>,
        cache: Arc<DashMap<String, Record>>,
        endpoints: Arc<DashMap<String, Endpoint>>,
        window_others: Arc<DashMap<String, Box<dyn MyWindow>>>,
        graph_data: Arc<DashMap<String, crate::MetricData>>,
    ) -> Self {
        Self {
            node,
            cache,
            endpoints,
            sort_col: SortColumn::Field,
            sort_desc: true,
            selected_fields: HashSet::new(),
            window_others,
            graph_data,
        }
    }

    fn toggle_sort(&mut self, col: SortColumn) {
        if self.sort_col == col {
            self.sort_desc = !self.sort_desc;
        } else {
            self.sort_col = col;
            self.sort_desc = false;
        }
    }

    fn selected_keys(&self, fallback: &str) -> Vec<String> {
        let mut keys = if self.selected_fields.is_empty() {
            vec![fallback.to_owned()]
        } else {
            self.selected_fields.iter().cloned().collect()
        };
        keys.sort();
        keys
    }

    fn render_attach_menu(&self, ui: &mut egui::Ui, key: &str, record: &Record) {
        let selected_keys = self.selected_keys(key);
        let field_windows = window_registry::field_window_registrations()
            .into_iter()
            .filter(|registration| registration.supports_field(record))
            .collect::<Vec<_>>();

        let endpoint = self.endpoints.get(&record.src).map(|entry| entry.value().clone());
        let endpoint_windows = endpoint
            .as_ref()
            .map(|endpoint| {
                window_registry::endpoint_window_registrations()
                    .into_iter()
                    .filter(|registration| {
                        registration.supports_endpoint(endpoint)
                            && (registration.supports)().supports_record(record)
                    })
                    .collect::<Vec<_>>()
            })
            .unwrap_or_default();

        if field_windows.is_empty() && endpoint_windows.is_empty() {
            ui.label("-");
            return;
        }

        egui::ComboBox::from_id_salt(format!("attach_event_{key}"))
            .selected_text("Attach…")
            .width(150.0)
            .show_ui(ui, |ui| {
                if !field_windows.is_empty() {
                    ui.label("Field windows");
                    for registration in &field_windows {
                        let window_key = registration.build_field_key(key);
                        let already_open = self.window_others.contains_key(&window_key);
                        if ui
                            .add_enabled(!already_open, egui::Button::new(registration.label))
                            .clicked()
                        {
                            if let WindowRegistrationKind::Field(factory) = registration.kind {
                                self.window_others.insert(
                                    window_key,
                                    factory(FieldWindowContext {
                                        key: key.to_owned(),
                                        record: record.clone(),
                                        selected_keys: selected_keys.clone(),
                                        cache: self.cache.clone(),
                                        graph_data: self.graph_data.clone(),
                                    }),
                                );
                            }
                        }
                    }
                }

                if !endpoint_windows.is_empty() {
                    if !field_windows.is_empty() {
                        ui.separator();
                    }
                    ui.label("Message windows");
                    for registration in &endpoint_windows {
                        let window_key = registration.build_endpoint_key(&record.src);
                        let already_open = self.window_others.contains_key(&window_key);
                        if ui
                            .add_enabled(!already_open, egui::Button::new(registration.label))
                            .clicked()
                        {
                            if let (Some(endpoint), WindowRegistrationKind::Endpoint(factory)) =
                                (endpoint.clone(), registration.kind)
                            {
                                self.window_others.insert(
                                    window_key,
                                    factory(EndpointWindowContext {
                                        node: self.node.clone(),
                                        source: record.src.clone(),
                                        endpoint,
                                    }),
                                );
                            }
                        }
                    }
                }
            });
    }

    fn render_events(&mut self, ui: &mut egui::Ui) {
        egui::ScrollArea::vertical().show(ui, |ui| {
            use egui_extras::{Column, TableBuilder};

            let mut records: Vec<Record> = self.cache.iter().map(|e| e.value().clone()).collect();

            records.sort_by(|a, b| {
                let ord = match self.sort_col {
                    SortColumn::Time => a.timestamp.cmp(&b.timestamp),
                    SortColumn::Source => a.src.cmp(&b.src),
                    SortColumn::Type => a.typ.cmp(&b.typ),
                    SortColumn::Field => a.field_name.cmp(&b.field_name),
                };
                if self.sort_desc {
                    ord.reverse()
                } else {
                    ord
                }
            });

            TableBuilder::new(ui)
                .striped(true)
                .column(Column::initial(80.0))
                .column(Column::initial(100.0))
                .column(Column::initial(120.0))
                .column(Column::initial(120.0))
                .column(Column::initial(170.0))
                .column(Column::initial(60.0))
                .column(Column::remainder())
                .header(20.0, |mut header| {
                    header.col(|ui| {
                        ui.label("Time");
                    });
                    header.col(|ui| {
                        if ui.label("Source").clicked() {
                            self.toggle_sort(SortColumn::Source);
                        }
                    });
                    header.col(|ui| {
                        if ui.label("Type").clicked() {
                            self.toggle_sort(SortColumn::Type);
                        }
                    });
                    header.col(|ui| {
                        if ui.label("Field").clicked() {
                            self.toggle_sort(SortColumn::Field);
                        }
                    });
                    header.col(|ui| {
                        ui.label("Attach");
                    });
                    header.col(|ui| {
                        ui.label("Count");
                    });
                    header.col(|ui| {
                        ui.label("Value");
                    });
                })
                .body(|body| {
                    body.rows(20.0, records.len(), |mut row| {
                        let r = &records[row.index()];
                        let time_str = chrono::DateTime::<chrono::Local>::from(r.timestamp)
                            .format("%H:%M:%S")
                            .to_string();
                        let key = format!("{}:{}:{}", r.src, r.typ, r.field_name);

                        row.col(|ui| {
                            ui.label(time_str);
                        });
                        row.col(|ui| {
                            ui.label(&r.src);
                        });
                        row.col(|ui| {
                            ui.label(&r.typ);
                        });
                        row.col(|ui| {
                            ui.label(&r.field_name);
                        });
                        row.col(|ui| {
                            ui.horizontal(|ui| {
                                let mut selected = self.selected_fields.contains(key.as_str());
                                if ui.checkbox(&mut selected, "").changed() {
                                    if selected {
                                        self.selected_fields.insert(key.clone());
                                    } else {
                                        self.selected_fields.remove(&key);
                                    }
                                }
                                self.render_attach_menu(ui, &key, r);
                            });
                        });
                        row.col(|ui| {
                            ui.label(r.counter.to_string());
                        });
                        row.col(|ui| {
                            ui.label(&r.value);
                        });
                    });
                });
        });
    }
}

impl MyWindow for WindowEvents {
    fn name(&self) -> &'static str {
        "Events"
    }

    fn show(&mut self, ui: &mut egui::Ui) -> Result<()> {
        egui::Window::new("Events")
            .open(&mut true)
            .resizable([true, true])
            .constrain_to(ui.available_rect_before_wrap())
            .show(ui.ctx(), |ui| {
                self.render_events(ui);
            });
        Ok(())
    }

    fn is_closed(&self) -> bool {
        false
    }
}
