//! HoverboardActor — sends periodic HoverboardRequest commands via UdpEndpoint.

use std::sync::Arc;

use anyhow::Result;
use generated::generated::{Envelope, HoverboardEvent, HoverboardReply, HoverboardRequest};
use kameo::prelude::*;
use log::{info, warn};

use crate::udp_endpoint::{Subscribe, UdpEndpoint};

// ── Messages ───────────────────────────────────────────────────────────────

/// Timer tick to send a HoverboardRequest.
struct TickHoverboard;

// ── HoverboardActor ────────────────────────────────────────────────────────
// digital twin of Hoverboard drive system, sends periodic HoverboardRequest commands via UdpEndpoint.

pub struct HoverboardActor {
    endpoint_id: u32,
    gateway: ActorRef<UdpEndpoint>,
    speed: i32,
    steer: i32,
    temperature: f32,
    voltage: f32,
    last_event_time: Option<std::time::Instant>,
}

impl HoverboardActor {
    pub fn new(endpoint_id: u32, gateway: ActorRef<UdpEndpoint>) -> Self {
        HoverboardActor {
            endpoint_id,
            gateway,
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

    pub fn check_envelope(&self, envelope: &Arc<Envelope>) -> Result<()> {
        if envelope.src.is_none() || envelope.msg_type.is_none() {
            return Err(anyhow::anyhow!("Envelope missing required fields"));
        }
        Ok(())
    }
}

impl Actor for HoverboardActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        info!("HoverboardActor started (2Hz command loop)");
        let ar_ref = actor_ref.clone();
        state
            .gateway
            .tell(Subscribe {
                msg_types: vec![HoverboardReply::MSG_ID, HoverboardEvent::MSG_ID],
                recipient: actor_ref.recipient(),
            })
            .await?;

        // 2Hz timer
        let tick_ref = ar_ref;
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

impl Message<TickHoverboard> for HoverboardActor {
    type Reply = ();

    async fn handle(&mut self, _msg: TickHoverboard, _ctx: &mut Context<Self, Self::Reply>) {
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
            src: Some(self.endpoint_id),
            dst: None,
            msg_type: Some(HoverboardRequest::MSG_ID),
            request_id: None,
            instance_id: None,
            payload: Some(payload),
        };
        let _ = self.gateway.tell(envelope).await;
    }
}

impl Message<Arc<Envelope>> for HoverboardActor {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        msg: Arc<Envelope>,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.check_envelope(&msg)?;
        match msg.msg_type {
            Some(HoverboardReply::MSG_ID) => self.handle_hoverboard_reply(msg).await,
            Some(HoverboardEvent::MSG_ID) => self.handle_hoverboard_event(msg).await,
            _ => {
                warn!("Received unexpected message type: {:?}", msg.msg_type);
                return Err(anyhow::anyhow!(
                    "Received unexpected message type: {:?}",
                    msg.msg_type
                ));
            }
        }
    }
}
