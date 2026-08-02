//! CutterActor — digital twin of the cutter system, sends periodic CutterRequest commands via UdpEndpoint.

use std::sync::Arc;

use anyhow::Result;
use generated::generated::{CutterEvent, CutterReply, CutterRequest, Envelope};
use kameo::prelude::*;
use log::{info, warn};

use crate::{brain::{BrainActor, BrainCmd}, router::{Register, RouterActor, RouterMessage}};

// ── Messages ───────────────────────────────────────────────────────────────

/// Timer tick to send a CutterRequest.
struct TickCutter;

// ── CutterActor ────────────────────────────────────────────────────────────
// digital twin of the cutter system, sends periodic CutterRequest commands via UdpEndpoint.

pub struct CutterActor {
    router: ActorRef<RouterActor>,
    brain: ActorRef<BrainActor>,
    enabled: bool,
    rpm: i32,
    current: f32,
    voltage: f32,
    temperature: f32,
    last_event_time: Option<std::time::Instant>,
}

impl CutterActor {
    pub fn new(router: ActorRef<RouterActor>, brain: ActorRef<BrainActor>) -> Self {
        CutterActor {
            router,
            brain,
            enabled: false,
            rpm: 0,
            current: 0.0,
            voltage: 0.0,
            temperature: 0.0,
            last_event_time: None,
        }
    }

    pub fn set_enabled(&mut self, enabled: bool) {
        self.enabled = enabled;
    }

    pub fn set_rpm(&mut self, rpm: i32) {
        self.rpm = rpm;
    }

    pub fn device_timed_out(&self) -> bool {
        if let Some(last_time) = self.last_event_time {
            last_time.elapsed().as_secs() > 5
        } else {
            true
        }
    }

    pub async fn handle_cutter_reply(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        let reply = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("CutterReply envelope missing payload"))?;

        let _reply = CutterReply::from_bytes(reply)
            .map_err(|e| anyhow::anyhow!("Failed to decode CutterReply: {e}"))?;

        // CutterReply carries error_code/message — log if error
        if let Some(code) = _reply.error_code {
            if code != 0 {
                warn!("CutterReply error {}: {}", code, _reply.message.as_deref().unwrap_or(""));
            }
        }

        Ok(())
    }

    pub async fn handle_cutter_event(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        let event = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("CutterEvent envelope missing payload"))?;

        let event = CutterEvent::from_bytes(event)
            .map_err(|e| anyhow::anyhow!("Failed to decode CutterEvent: {e}"))?;

        event.enabled.map(|e| self.enabled = e);
        event.rpm.map(|r| self.rpm = r);
        event.current.map(|c| self.current = c);
        event.voltage.map(|v| self.voltage = v);
        event.temperature.map(|t| self.temperature = t);
        self.last_event_time = Some(std::time::Instant::now());

        Ok(())
    }

    pub async fn handle_timer_tick(&mut self, sender: Recipient<RouterMessage>) {
        let request = CutterRequest {
            enabled: Some(self.enabled),
            rpm: Some(self.rpm),
        };
        let payload = match request.to_bytes() {
            Ok(p) => p,
            Err(e) => {
                warn!("Failed to encode CutterRequest: {}", e);
                return;
            }
        };
        let envelope = Envelope {
            msg_type: Some(CutterRequest::MSG_ID),
            payload: Some(payload),
            ..Default::default()
        };
        let _ = self
            .router
            .tell(RouterMessage {
                envelope: Arc::new(envelope),
                sender,
            })
            .await;
    }

    pub fn check_envelope(&self, envelope: &Arc<Envelope>) -> Result<()> {
        if envelope.src.is_none() || envelope.msg_type.is_none() {
            return Err(anyhow::anyhow!("Envelope missing required fields"));
        }
        Ok(())
    }
}

impl Actor for CutterActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        info!("CutterActor started (2Hz command loop)");
        let ar_ref = actor_ref.clone();
        state
            .router
            .tell(Register {
                actor_ref: actor_ref.recipient(),
                description: format!("CutterActor"),
            })
            .await?;

        // 2Hz timer
        let tick_ref = ar_ref;
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(std::time::Duration::from_millis(500));
            loop {
                interval.tick().await;
                let _ = tick_ref.tell(TickCutter).await;
            }
        });

        Ok(state)
    }
}

// ── Handle TickCutter ─────────────────────────────────────────────────────

impl Message<TickCutter> for CutterActor {
    type Reply = ();

    async fn handle(&mut self, _msg: TickCutter, ctx: &mut Context<Self, Self::Reply>) {
        let sender = ctx.actor_ref().clone().recipient();
        self.handle_timer_tick(sender).await;
    }
}

impl Message<RouterMessage> for CutterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: RouterMessage,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let envelope = msg.envelope;
        let m = envelope.clone();
        if Some(CutterReply::MSG_ID) == envelope.msg_type {
            let _ = self.handle_cutter_reply(envelope).await;
        }
        if Some(CutterEvent::MSG_ID) == m.msg_type {
            let _ = self.handle_cutter_event(m).await;
        }
    }
}

impl Message<BrainCmd> for CutterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: BrainCmd,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let _ = match msg {
            BrainCmd::SetCutter(enabled) => {
                self.enabled = enabled;
                Ok(())
            }
            _ => Err(anyhow::anyhow!("CutterActor received unexpected BrainCmd")),
        };
    }
}
