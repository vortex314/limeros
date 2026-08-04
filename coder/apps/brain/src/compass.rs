//! CompassActor — digital twin of the compass sensor, receives CompassEvent telemetry.

use std::sync::Arc;

use anyhow::Result;
use common::fnv;
use generated::generated::{CompassEvent, Envelope};
use kameo::prelude::*;
use log::{info, warn};

use crate::{
    brain::{BrainActor, EnvelopeHandlerEvent, EnvelopeHandlerRequest, ResultLog}, router::{FromDevice, RegisterTwin, RouterActor, },
};

// ── CompassActor ───────────────────────────────────────────────────────────
// digital twin of the compass sensor, tracks heading/pitch/roll/mag/accel.
pub struct CompassActor {
    brain: ActorRef<BrainActor>,
    router: ActorRef<RouterActor>,
    pub heading: f32,
    pub pitch: f32,
    pub roll: f32,
    pub mag_x: f32,
    pub mag_y: f32,
    pub mag_z: f32,
    pub accel_x: f32,
    pub accel_y: f32,
    pub accel_z: f32,
    last_event_time: Option<std::time::Instant>,
}

impl CompassActor {
    pub fn new(brain: ActorRef<BrainActor>, router: ActorRef<RouterActor>) -> Self {
        CompassActor {
            brain,
            router,
            heading: 0.0,
            pitch: 0.0,
            roll: 0.0,
            mag_x: 0.0,
            mag_y: 0.0,
            mag_z: 0.0,
            accel_x: 0.0,
            accel_y: 0.0,
            accel_z: 0.0,
            last_event_time: None,
        }
    }

    pub fn device_timed_out(&self) -> bool {
        if let Some(last_time) = self.last_event_time {
            last_time.elapsed().as_secs() > 5
        } else {
            true
        }
    }

    pub async fn handle_compass_event(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        let event = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("CompassEvent envelope missing payload"))?;

        let event = CompassEvent::from_bytes(event)
            .map_err(|e| anyhow::anyhow!("Failed to decode CompassEvent: {e}"))?;

        event.heading.map(|v| self.heading = v);
        event.pitch.map(|v| self.pitch = v);
        event.roll.map(|v| self.roll = v);
        event.mag_x.map(|v| self.mag_x = v);
        event.mag_y.map(|v| self.mag_y = v);
        event.mag_z.map(|v| self.mag_z = v);
        event.accel_x.map(|v| self.accel_x = v);
        event.accel_y.map(|v| self.accel_y = v);
        event.accel_z.map(|v| self.accel_z = v);
        self.last_event_time = Some(std::time::Instant::now());

        Ok(())
    }

}

impl Actor for CompassActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(
        mut state: Self::Args,
        actor_ref: ActorRef<Self>,
    ) -> Result<Self, Self::Error> {
        info!("CompassActor started");
        // Register with the router to receive CompassEvent messages
        state.router.tell(RegisterTwin {
            id: fnv::fnv1a_32("compass"),
            recipient: actor_ref.clone().recipient(),
        }).await.log_error("Failed to register CompassActor with router");
        Ok(state)
    }
}

impl Message<FromDevice> for CompassActor {
    type Reply = ();

    async fn handle(&mut self, msg: FromDevice, _ctx: &mut Context<Self, Self::Reply>) {
        if let Err(e) = self.handle_compass_event(msg.envelope).await {
            warn!("Failed to handle compass event: {}", e);
        }
    }
}
