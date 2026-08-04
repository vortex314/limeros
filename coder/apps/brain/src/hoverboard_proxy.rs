//! HoverboardActor — sends periodic HoverboardRequest commands via UdpEndpoint.

use std::sync::Arc;

use anyhow::Result;
use common::fnv;
use generated::generated::{EndpointAnnounce, Envelope, HoverboardEvent, HoverboardReply, HoverboardRequest, id_to_string, opt_id_to_string};
use kameo::prelude::*;
use kameo_actors::message_bus::Register;
use log::{info, warn};

use crate::{
    brain::{BrainActor, BrainCmd,  ResultLog}, router::{FromDevice, RegisterTwin, RouterActor, ToDevice},
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
    router: ActorRef<RouterActor>,
    speed: i32,
    steer: i32,
    temperature: f32,
    voltage: f32,
    last_event_time: Option<std::time::Instant>,
}

impl HoverboardProxy {
    pub fn new(brain: ActorRef<BrainActor>, router: ActorRef<RouterActor>) -> Self {
        HoverboardProxy {
            router,
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
        self.router.tell(ToDevice {
            id: fnv::fnv1a_32("hoverboard"),
            envelope: Arc::new(envelope),
        }).await.log_error("Failed to send ToDevice message to hoverboard");
    }
}

impl Actor for HoverboardProxy {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        info!("HoverboardActor started (2Hz command loop)");
        state.router.tell(RegisterTwin {
            id: fnv::fnv1a_32("hoverboard"),
            recipient: actor_ref.clone().recipient(),
        }).await.log_error("Failed to register HoverboardProxy with router");
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

impl Message<FromDevice> for HoverboardProxy {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: FromDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let envelope = msg.envelope;
        match envelope.msg_type {
            Some(HoverboardReply::MSG_ID) => {
                if let Err(e) = self.handle_hoverboard_reply(envelope).await {
                    warn!("Failed to handle HoverboardReply: {}", e);
                }
            }
            Some(HoverboardEvent::MSG_ID) => {
                if let Err(e) = self.handle_hoverboard_event(envelope).await {
                    warn!("Failed to handle HoverboardEvent: {}", e);
                }
            }
            Some(EndpointAnnounce::MSG_ID) => {
            }
            _ => {
                warn!(
                    "HoverboardActor received unexpected message type: {} {:?}",
                    opt_id_to_string(envelope.msg_type),
                    envelope.msg_type
                );
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
