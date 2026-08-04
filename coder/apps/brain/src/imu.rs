//! ImuActor — digital twin of the IMU sensor, receives ImuEvent telemetry.

use std::sync::Arc;

use anyhow::Result;
use generated::generated::{Envelope, ImuEvent};
use kameo::prelude::*;
use log::{info, warn};

use crate::{
    brain::{BrainActor, EnvelopeHandlerEvent, EnvelopeHandlerRequest}, router::{FromDevice, RouterActor},
};

// ── ImuActor ───────────────────────────────────────────────────────────────
// digital twin of the IMU sensor, tracks gyroscope and accelerometer data.
#[derive(Actor)]
pub struct ImuActor {
    _brain: ActorRef<BrainActor>,
    router: ActorRef<RouterActor>,
    pub gyro_x: f32,
    pub gyro_y: f32,
    pub gyro_z: f32,
    pub accel_x: f32,
    pub accel_y: f32,
    pub accel_z: f32,
    last_event_time: Option<std::time::Instant>,
}

impl ImuActor {
    pub fn new(brain: ActorRef<BrainActor>, router: ActorRef<RouterActor>) -> Self {
        ImuActor {
            router,
            _brain: brain,
            gyro_x: 0.0,
            gyro_y: 0.0,
            gyro_z: 0.0,
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

    pub async fn handle_imu_event(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        let event = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("ImuEvent envelope missing payload"))?;

        let event = ImuEvent::from_bytes(event)
            .map_err(|e| anyhow::anyhow!("Failed to decode ImuEvent: {e}"))?;

        event.gyro_x.map(|v| self.gyro_x = v);
        event.gyro_y.map(|v| self.gyro_y = v);
        event.gyro_z.map(|v| self.gyro_z = v);
        event.accel_x.map(|v| self.accel_x = v);
        event.accel_y.map(|v| self.accel_y = v);
        event.accel_z.map(|v| self.accel_z = v);
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

impl Message<FromDevice> for ImuActor {
    type Reply = ();

    async fn handle(&mut self, msg: FromDevice, _ctx: &mut Context<Self, Self::Reply>) {
        if let Err(e) = self.check_envelope(&msg.envelope) {
            warn!("Invalid envelope received by ImuActor: {}", e);
            return;
        }

        if let Err(e) = self.handle_imu_event(msg.envelope).await {
            warn!("Failed to handle ImuEvent: {}", e);
        }
    }
}