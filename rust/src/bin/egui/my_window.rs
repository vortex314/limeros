use std::sync::Arc;

use anyhow::Result;
use dashmap::DashMap;
use eframe::egui;
use limeros::{Endpoint, UdpNode};

use crate::{MetricData, Record};

#[derive(Clone, Debug, Default)]
pub struct Supports {
    pub services: Vec<String>,
    pub publishes: Vec<String>,
    pub subscribes: Vec<String>,
    pub fields: Vec<String>,
    pub accepts_numeric_series: bool,
    pub accepts_any_message: bool,
}

impl Supports {
    pub fn supports_endpoint(&self, endpoint: &Endpoint) -> bool {
        intersects(&self.services, &endpoint.services)
            || intersects(&self.publishes, &endpoint.subscribe)
            || intersects(&self.publishes, &endpoint.services)
            || intersects(&self.subscribes, &endpoint.publish)
    }

    pub fn supports_record(&self, record: &Record) -> bool {
        self.accepts_any_message
            || (self.accepts_numeric_series && record.value.parse::<f64>().is_ok())
            || self.fields.iter().any(|field| field == &record.field_name)
            || self.services.iter().any(|typ| typ == &record.typ)
            || self.publishes.iter().any(|typ| typ == &record.typ)
            || self.subscribes.iter().any(|typ| typ == &record.typ)
    }
}

fn intersects(lhs: &[String], rhs: &[String]) -> bool {
    lhs.iter().any(|item| rhs.iter().any(|candidate| candidate == item))
}

#[derive(Clone)]
pub struct EndpointWindowContext {
    pub node: Arc<UdpNode>,
    pub source: String,
    pub endpoint: Endpoint,
}

#[derive(Clone)]
pub struct FieldWindowContext {
    pub key: String,
    pub record: Record,
    pub selected_keys: Vec<String>,
    pub cache: Arc<DashMap<String, Record>>,
    pub graph_data: Arc<DashMap<String, MetricData>>,
}

pub type EndpointWindowFactory = fn(EndpointWindowContext) -> Box<dyn MyWindow>;
pub type FieldWindowFactory = fn(FieldWindowContext) -> Box<dyn MyWindow>;

#[derive(Clone, Copy)]
pub enum WindowRegistrationKind {
    Endpoint(EndpointWindowFactory),
    Field(FieldWindowFactory),
}

#[derive(Clone, Copy)]
pub struct WindowRegistration {
    pub id: &'static str,
    pub label: &'static str,
    pub kind: WindowRegistrationKind,
    pub supports: fn() -> Supports,
}

impl WindowRegistration {
    pub const fn endpoint(
        id: &'static str,
        label: &'static str,
        supports: fn() -> Supports,
        factory: EndpointWindowFactory,
    ) -> Self {
        Self {
            id,
            label,
            kind: WindowRegistrationKind::Endpoint(factory),
            supports,
        }
    }

    pub const fn field(
        id: &'static str,
        label: &'static str,
        supports: fn() -> Supports,
        factory: FieldWindowFactory,
    ) -> Self {
        Self {
            id,
            label,
            kind: WindowRegistrationKind::Field(factory),
            supports,
        }
    }

    pub fn supports_endpoint(&self, endpoint: &Endpoint) -> bool {
        matches!(self.kind, WindowRegistrationKind::Endpoint(_))
            && (self.supports)().supports_endpoint(endpoint)
    }

    pub fn supports_field(&self, record: &Record) -> bool {
        matches!(self.kind, WindowRegistrationKind::Field(_))
            && (self.supports)().supports_record(record)
    }

    pub fn build_endpoint_key(&self, source: &str) -> String {
        format!("{}::endpoint::{}", self.id, source)
    }

    pub fn build_field_key(&self, key: &str) -> String {
        format!("{}::field::{}", self.id, key)
    }
}

pub trait MyWindow {
    fn name(&self) -> &'static str;
    fn show(&mut self, ui: &mut egui::Ui) -> Result<()>;
    fn is_closed(&self) -> bool;
    fn supports(&self) -> Option<Supports> {
        None
    }
}