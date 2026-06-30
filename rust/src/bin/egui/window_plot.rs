use std::sync::Arc;

use anyhow::Result;
use dashmap::DashMap;
use eframe::egui;
use egui_plot::{Line, Plot, PlotPoints};

use crate::{
    MetricData,
    my_window::{FieldWindowContext, MyWindow, Supports, WindowRegistration},
};

pub(crate) struct WindowPlot {
    keys: Vec<String>,
    graph_data: Arc<DashMap<String, MetricData>>,
    window_name: String,
    open: bool,
}

impl WindowPlot {
    pub fn new(keys: Vec<String>, graph_data: Arc<DashMap<String, MetricData>>) -> Self {
        let window_name = keys.join(", ");
        Self {
            keys,
            graph_data,
            window_name,
            open: true,
        }
    }

    pub fn supports_config() -> Supports {
        Supports {
            accepts_numeric_series: true,
            ..Default::default()
        }
    }

    pub fn registration() -> WindowRegistration {
        WindowRegistration::field(
            "plot",
            "Plot series",
            Self::supports_config,
            |ctx: FieldWindowContext| {
                let keys = if ctx.selected_keys.is_empty() {
                    vec![ctx.key.clone()]
                } else {
                    ctx.selected_keys.clone()
                };
                Box::new(Self::new(keys, ctx.graph_data.clone()))
            },
        )
    }
}

impl MyWindow for WindowPlot {
    fn name(&self) -> &'static str {
        "Plot Window"
    }

    fn is_closed(&self) -> bool {
        !self.open
    }

    fn show(&mut self, ui: &mut egui::Ui) -> Result<()> {
        let mut lines = Vec::new();
        for key in &self.keys {
            let metric_data = self.graph_data.get(key);
            if metric_data.is_none() {
                ui.label(format!("No data available for metric key: {}", key));
                return Ok(());
            }
            let points = &metric_data.unwrap().points;
            let plot_points: PlotPoints<'_> = PlotPoints::from(points.clone());
            let line = Line::new(plot_points).name(key.clone());
            lines.push(line);
        }

        egui::Window::new(self.window_name.clone())
            .open(&mut self.open)
            .resizable([true, true])
            .default_size([600.0, 400.0])
            .show(ui.ctx(), |ui| {
                Plot::new(&self.window_name)
                    .view_aspect(2.0)
                    .show(ui, |plot_ui| {
                        for line in lines {
                            plot_ui.line(line);
                        }
                    });
            });
        Ok(())
    }

    fn supports(&self) -> Option<Supports> {
        Some(Self::supports_config())
    }
}
