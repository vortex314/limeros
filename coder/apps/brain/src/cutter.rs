//! CutterActor — digital twin of the cutter system, sends periodic CutterRequest commands via UdpEndpoint.

use std::sync::Arc;

use anyhow::Result;
use common::fnv;
use generated::generated::{CutterEvent, CutterReply, CutterRequest, Envelope};
use kameo::prelude::*;
use log::{info, warn};

use crate::{brain::{BrainActor, BrainCmd, EnvelopeHandlerEvent, EnvelopeHandlerRequest, ResultLog}, router::{FromDevice, RegisterTwin, RouterActor, }};

// ── Messages ───────────────────────────────────────────────────────────────

/// Timer tick to send a CutterRequest.
struct TickCutter;

// ── CutterActor ────────────────────────────────────────────────────────────
// digital twin of the cutter system, sends periodic CutterRequest commands via UdpEndpoint.

pub struct CutterActor {
    brain: ActorRef<BrainActor>,
    router: ActorRef<RouterActor>,
    device : Option<Recipient<EnvelopeHandlerRequest>>,
    enabled: bool,
    rpm: i32,
    current: f32,
    voltage: f32,
    temperature: f32,
    last_event_time: Option<std::time::Instant>,
}

impl CutterActor {
    pub fn new(brain: ActorRef<BrainActor>, router: ActorRef<RouterActor>) -> Self {
        CutterActor {
            brain,
            router,
            device: None,
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

    pub async fn handle_timer_tick(&mut self) {
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
        self.device.as_ref().map(async |device| {
            let _ = device
                .tell(EnvelopeHandlerRequest::SendEnvelope {
                    endpoint : fnv::fnv1a_32("cutter"),
                    envelope: Arc::new(envelope),
                })
                .await;
        });
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
        // 2Hz timer
        let tick_ref = actor_ref.clone();
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(std::time::Duration::from_millis(500));
            loop {
                interval.tick().await;
                let _ = tick_ref.tell(TickCutter).await;
            }
        });

        state.router.tell(RegisterTwin{
            id: fnv::fnv1a_32("cutter"),
            recipient: actor_ref.clone().recipient(),
        }).await.log_error("Failed to register CutterActor with router");

        Ok(state)
    }
}

// ── Handle TickCutter ─────────────────────────────────────────────────────

impl Message<TickCutter> for CutterActor {
    type Reply = ();

    async fn handle(&mut self, _msg: TickCutter, ctx: &mut Context<Self, Self::Reply>) {
        self.handle_timer_tick().await;
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
 

 impl Message<FromDevice> for CutterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: FromDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let envelope = msg.envelope;
        match envelope.msg_type {
            Some(CutterReply::MSG_ID) => {
                if let Err(e) = self.handle_cutter_reply(envelope).await {
                    warn!("Failed to handle CutterReply: {}", e);
                }
            }
            Some(CutterEvent::MSG_ID) => {
                if let Err(e) = self.handle_cutter_event(envelope).await {
                    warn!("Failed to handle CutterEvent: {}", e);
                }
            }
            _ => {
                warn!(
                    "CutterActor received unexpected message type: {:?}",
                    envelope.msg_type
                );
            }
        }
    }
}