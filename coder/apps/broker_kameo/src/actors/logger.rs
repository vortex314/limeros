use anyhow::Result;
use generated::generated::*;
use kameo::Actor;
use kameo::actor::Spawn;
use kameo::message::{Context, Message};
use log::info;
use log::{error, warn};
use reqwest::Client;
use serde_json::{Value, json};
use std::pin::Pin;
use std::time::Duration;
use tokio::time::{Instant, interval};

use crate::actors::router::IncomingEnvelope;

/// Configuration for OpenObserve
#[derive(Clone)]
pub struct OpenObserveConfig {
    pub endpoint: String, // e.g. "http://localhost:5080" or "https://api.openobserve.ai"
    pub organization: String, // e.g. "default"
    pub stream: String,   // e.g. "kameo_app"
    pub email: String,
    pub password: String,
}

impl OpenObserveConfig {
    fn ingestion_url(&self) -> String {
        format!(
            "{}/api/{}/{}/_json",
            self.endpoint.trim_end_matches('/'),
            self.organization,
            self.stream
        )
    }

    fn auth_header(&self) -> String {
        let credentials = format!("{}:{}", self.email, self.password);
        format!("Basic {}", base64::encode(credentials))
    }
}

/// A single log record
#[derive(Debug, Clone)]
pub struct LogRecord {
    pub level: String,
    pub message: String,
    pub fields: Value, // extra structured fields
}

/// Messages the LoggerActor accepts
pub struct Log(pub LogRecord);

pub struct Flush; // force flush the buffer

/// The actor itself
#[derive(Actor)]
pub struct LoggerActor {
    client: Client,
    config: OpenObserveConfig,
    buffer: Vec<Value>,
    max_batch_size: usize,
    flush_interval: Duration,
    last_flush: Instant,
}

impl LoggerActor {
    pub fn new(config: OpenObserveConfig) -> Self {
        Self {
            client: Client::new(),
            config,
            buffer: Vec::with_capacity(128),
            max_batch_size: 100, // flush when we reach this many records
            flush_interval: Duration::from_secs(2), // or every 2 seconds
            last_flush: Instant::now(),
        }
    }

    async fn flush(&mut self) {
        if self.buffer.is_empty() {
            return;
        }

        info!("Flushing {} log records to OpenObserve {}", self.buffer.len(), self.config.ingestion_url());

        let records = std::mem::take(&mut self.buffer);
        let url = self.config.ingestion_url();
        let auth = self.config.auth_header();

        match self
            .client
            .post(&url)
            .header("Authorization", &auth)
            .header("Content-Type", "application/json")
            .json(&records)
            .send()
            .await
        {
            Ok(resp) if resp.status().is_success() => {
                // success – optionally log metrics here
                info!(" Flushed {} log records to OpenObserve", records.len());
            }
            Ok(resp) => {
                warn!(
                    " OpenObserve returned {}: {:?}",
                    resp.status(),
                    resp.text().await.ok()
                );
                // In production you may want to re-queue or drop
            }
            Err(e) => {
                error!(" Failed to send logs: {e}");
                // Consider a dead-letter buffer or retry logic
            }
        }

        self.last_flush = Instant::now();
    }
}

// ── Message handlers ──────────────────────────────────────────────

impl Message<Log> for LoggerActor {
    type Reply = anyhow::Result<()>;

    async fn handle(&mut self, msg: Log, _ctx: &mut Context<Self, Self::Reply>) -> Self::Reply {
        info!(" {}: {}", msg.0.level, msg.0.message);
        let mut record = json!({
            "level": msg.0.level,
            "message": msg.0.message,
            // OpenObserve will add _timestamp automatically if missing
        });

        // Merge extra fields
        if let Value::Object(extra) = msg.0.fields {
            if let Value::Object(ref mut map) = record {
                map.extend(extra);
            }
        }

        self.buffer.push(record);

        // Flush on size or time
        if self.buffer.len() >= self.max_batch_size
            || self.last_flush.elapsed() >= self.flush_interval
        {
            self.flush().await;
        }
        Ok(())
    }
}

impl Message<Flush> for LoggerActor {
    type Reply = ();

    async fn handle(&mut self, _msg: Flush, _ctx: &mut Context<Self, Self::Reply>) -> Self::Reply {
        self.flush().await;
    }
}

impl Message<IncomingEnvelope> for LoggerActor {
    type Reply = anyhow::Result<()>;

    async fn handle(
        &mut self,
        msg: IncomingEnvelope,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let envelope = msg.envelope;
        let raw = envelope
            .payload
            .as_ref()
            .ok_or(anyhow::anyhow!("Missing payload"))?;
        let msg_type = envelope
            .msg_type
            .ok_or(anyhow::anyhow!("Missing msg_type"))?;
        let mut value = msg_to_value(msg_type, &raw)?;
        value["src"] = json!(&opt_id_to_string(envelope.src));
        value["dst"] = json!(&opt_id_to_string(envelope.dst));
        value["msg_type"] = json!(&opt_id_to_string(envelope.msg_type));
        value["request_id"] = json!(&envelope.request_id);
        value["instance_id"] = json!(&envelope.instance_id);
        value["time"] = json!(chrono::Utc::now().to_rfc3339());
   //    info!("value: {}", value);


        let r = self.buffer.push(value);

        // Flush on size or time
        if self.buffer.len() >= self.max_batch_size
            || self.last_flush.elapsed() >= self.flush_interval
        {
            self.flush().await;
        }
        Ok(())
    }
}

pub fn msg_to_value(msg_type: u32, buffer: &[u8]) -> anyhow::Result<Value> {
    // Implementation goes here
    match msg_type {
        PingRequest::MSG_ID => Ok(serde_json::to_value(&PingRequest::from_bytes(buffer)?)?),
        PingReply::MSG_ID => Ok(serde_json::to_value(&PingReply::from_bytes(buffer)?)?),
        HoverboardEvent::MSG_ID => Ok(serde_json::to_value(&HoverboardEvent::from_bytes(buffer)?)?),
        HoverboardRequest::MSG_ID => Ok(serde_json::to_value(&HoverboardRequest::from_bytes(
            buffer,
        )?)?),
        SysEvent::MSG_ID => Ok(serde_json::to_value(&SysEvent::from_bytes(buffer)?)?),
        SysRequest::MSG_ID => Ok(serde_json::to_value(&SysRequest::from_bytes(buffer)?)?),
        EndpointAnnounce::MSG_ID => Ok(serde_json::to_value(&EndpointAnnounce::from_bytes(
            buffer,
        )?)?),
        EndpointAnnounceReply::MSG_ID => Ok(serde_json::to_value(
            &EndpointAnnounceReply::from_bytes(buffer)?,
        )?),

        _ => {
            // For unknown message types, just return the raw bytes as base64
            let value = json!({
                "msg_type": msg_type,
                "raw_payload": base64::encode(buffer),
            });
            Ok(value)
        }
    }
}
