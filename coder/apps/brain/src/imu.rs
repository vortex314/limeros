//! ImuActor — digital twin of the IMU sensor, receives ImuEvent telemetry.

use std::sync::Arc;

use anyhow::Result;
use generated::generated::{Envelope, ImuEvent};
use kameo::prelude::*;
use log::{info, warn};

use crate::udp_endpoint::{Subscribe, UdpEndpoint};

// ── ImuActor ───────────────────────────────────────────────────────────────
// digital twin of the IMU sensor, tracks gyroscope and accelerometer data.

pub struct ImuActor {
    endpoint_id: u32,
    _gateway: ActorRef<UdpEndpoint>,
    pub gyro_x: f32,
    pub gyro_y: f32,
    pub gyro_z: f32,
    pub accel_x: f32,
    pub accel_y: f32,
    pub accel_z: f32,
    last_event_time: Option<std::time::Instant>,
}

impl ImuActor {
    pub fn new(endpoint_id: u32, gateway: ActorRef<UdpEndpoint>) -> Self {
        ImuActor {
            endpoint_id,
            _gateway: gateway,
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

impl Actor for ImuActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        info!("ImuActor started");
        state
            ._gateway
            .tell(Subscribe {
                msg_types: vec![ImuEvent::MSG_ID],
                recipient: actor_ref.recipient(),
            })
            .await?;

        Ok(state)
    }
}

impl Message<Arc<Envelope>> for ImuActor {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        msg: Arc<Envelope>,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.check_envelope(&msg)?;
        match msg.msg_type {
            Some(ImuEvent::MSG_ID) => self.handle_imu_event(msg).await,
            _ => {
                warn!("Received unexpected message type: {:?}", msg.msg_type);
                Err(anyhow::anyhow!(
                    "Received unexpected message type: {:?}",
                    msg.msg_type
                ))
            }
        }
    }
}
