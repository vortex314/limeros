//! HoverboardActor — sends periodic HoverboardRequest commands via UdpEndpoint.

use std::sync::Arc;

use anyhow::Result;
use common::fnv;
use generated::generated::{Envelope, HoverboardEvent, HoverboardReply, HoverboardRequest};
use kameo::prelude::*;
use log::{info, warn};

use crate::{
    brain::{BrainActor, BrainCmd, EnvelopeHandlerEvent, EnvelopeHandlerRequest},
    router::{Register, RouterActor, RouterMessage},
};

// ── Messages ───────────────────────────────────────────────────────────────

/// Timer tick to send a HoverboardRequest.
struct TickHoverboard;

// ── HoverboardActor ────────────────────────────────────────────────────────
// digital twin of Hoverboard drive system, sends periodic HoverboardRequest commands via UdpEndpoint.

pub struct HoverboardCmd {
    pub speed: i32,
    pub steer: i32,
}

pub struct HoverboardProxy {
    brain: ActorRef<BrainActor>,
    serial: Option<Recipient<EnvelopeHandlerRequest>>,
    speed: i32,
    steer: i32,
    temperature: f32,
    voltage: f32,
    last_event_time: Option<std::time::Instant>,
}

impl HoverboardProxy {
    pub fn new(router: ActorRef<RouterActor>, brain: ActorRef<BrainActor>) -> Self {
        HoverboardProxy {
            serial: None,
            brain,
            speed: 0,
            steer: 0,
            temperature: 0.0,
            voltage: 0.0,
            last_event_time: None,
        }
    }
    pub fn set_speed(&mut self, speed: i32) {
        self.speed = speed;
    }
    pub fn set_steer(&mut self, steer: i32) {
        self.steer = steer;
    }

    pub fn device_timed_out(&self) -> bool {
        if let Some(last_time) = self.last_event_time {
            last_time.elapsed().as_secs() > 5
        } else {
            true
        }
    }
    pub async fn handle_hoverboard_reply(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        let reply = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("HoverboardReply envelope missing payload"))?;

        let reply = HoverboardReply::from_bytes(reply)
            .map_err(|e| anyhow::anyhow!("Failed to decode HoverboardReply: {e}"))?;

        if let Some(speed) = reply.speed {
            self.speed = speed;
        }
        if let Some(steer) = reply.steer {
            self.steer = steer;
        }

        Ok(())
    }

    pub async fn handle_hoverboard_event(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        let event = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("HoverboardEvent envelope missing payload"))?;

        let event = HoverboardEvent::from_bytes(event)
            .map_err(|e| anyhow::anyhow!("Failed to decode HoverboardEvent: {e}"))?;

        event.temp.map(|t| self.temperature = t as f32 / 100.0);
        event.batv.map(|v| self.voltage = v as f32 / 100.0);
        self.last_event_time = Some(std::time::Instant::now());

        Ok(())
    }

    pub async fn handle_timer_tick(&mut self) {
        let request = HoverboardRequest {
            speed: Some(self.speed),
            steer: Some(self.steer),
        };
        let payload = match request.to_bytes() {
            Ok(p) => p,
            Err(e) => {
                warn!("Failed to encode HoverboardRequest: {}", e);
                return;
            }
        };
        let envelope = Envelope {
            dst: Some(fnv::fnv1a_32("hoverboard")),
            msg_type: Some(HoverboardRequest::MSG_ID),
            payload: Some(payload),
            ..Default::default()
        };
        //      display_envelope(&envelope, "HoverboardActor");
        let serial = self.serial.clone();
        if let Some(serial) = serial {
            info!("HoverboardProxy sending HoverboardRequest: speed={}, steer={}", self.speed, self.steer);
            let _ = serial
                .tell(EnvelopeHandlerRequest::SendEnvelope {
                    endpoint: fnv::fnv1a_32("hoverboard"),
                    envelope: Arc::new(envelope),
                })
                .await;
        } else {
            warn!("HoverboardProxy serial recipient not set");
        }
    }
}

impl Actor for HoverboardProxy {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        info!("HoverboardActor started (2Hz command loop)");
        // 2Hz timer
        let tick_ref = actor_ref.clone();
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(std::time::Duration::from_millis(500));
            loop {
                interval.tick().await;
                let _ = tick_ref.tell(TickHoverboard).await;
            }
        });

        Ok(state)
    }
}

// ── Handle TickHoverboard ─────────────────────────────────────────────────

impl Message<TickHoverboard> for HoverboardProxy {
    type Reply = ();

    async fn handle(&mut self, _msg: TickHoverboard, ctx: &mut Context<Self, Self::Reply>) {
        self.handle_timer_tick().await;
    }
}


impl Message<EnvelopeHandlerEvent> for HoverboardProxy {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: EnvelopeHandlerEvent,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        match msg {
            EnvelopeHandlerEvent::ReceivedEnvelope {
                recipient: sender,
                envelope,
            } => {
                let _ = self.handle_hoverboard_reply(envelope.clone()).await;
                if self.serial.is_none() || self.serial.as_ref() != Some(&sender) {
                    self.serial = Some(sender);
                }
            }
        }
    }
}

impl Message<BrainCmd> for HoverboardProxy {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: BrainCmd,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        match msg {
            BrainCmd::SetSpeed(speed) => self.set_speed(speed),
            BrainCmd::SetSteer(steer) => self.set_steer(steer),
            _ => {
                warn!("HoverboardActor received unexpected BrainCmd: {:?}", msg);
            }
        }
    }
}
