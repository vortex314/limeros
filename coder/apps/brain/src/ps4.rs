//! Ps4Actor — digital twin of the PS4 controller, receives Ps4Event telemetry
//! from the broker (published by the ps4_bridge binary).

use std::sync::Arc;

use anyhow::Result;
use generated::generated::{Envelope, Ps4Event};
use kameo::prelude::*;
use log::{info, warn};

use crate::udp_endpoint::{Subscribe, UdpEndpoint};

// ── Ps4Actor ───────────────────────────────────────────────────────────────
// digital twin of the PS4 controller, tracks button and axis state.

pub struct Ps4Actor {
    endpoint_id: u32,
    _gateway: ActorRef<UdpEndpoint>,
    pub button_left: Option<bool>,
    pub button_right: Option<bool>,
    pub button_up: Option<bool>,
    pub button_down: Option<bool>,
    pub button_square: Option<bool>,
    pub button_cross: Option<bool>,
    pub button_circle: Option<bool>,
    pub button_triangle: Option<bool>,
    pub button_left_shoulder: Option<bool>,
    pub button_right_shoulder: Option<bool>,
    pub button_left_trigger: Option<bool>,
    pub button_right_trigger: Option<bool>,
    pub button_left_joystick: Option<bool>,
    pub button_right_joystick: Option<bool>,
    pub button_share: Option<bool>,
    pub button_options: Option<bool>,
    pub button_touchpad: Option<bool>,
    pub button_ps: Option<bool>,
    pub axis_lx: Option<i32>,
    pub axis_ly: Option<i32>,
    pub axis_rx: Option<i32>,
    pub axis_ry: Option<i32>,
    pub gyro_x: Option<i32>,
    pub gyro_y: Option<i32>,
    pub gyro_z: Option<i32>,
    pub accel_x: Option<i32>,
    pub accel_y: Option<i32>,
    pub accel_z: Option<i32>,
    pub connected: Option<bool>,
    pub battery_level: Option<i32>,
    pub bluetooth: Option<bool>,
    pub debug: Option<String>,
    pub temp: Option<i32>,
    last_event_time: Option<std::time::Instant>,
}

impl Ps4Actor {
    pub fn new(endpoint_id: u32, gateway: ActorRef<UdpEndpoint>) -> Self {
        Ps4Actor {
            endpoint_id,
            _gateway: gateway,
            button_left: None,
            button_right: None,
            button_up: None,
            button_down: None,
            button_square: None,
            button_cross: None,
            button_circle: None,
            button_triangle: None,
            button_left_shoulder: None,
            button_right_shoulder: None,
            button_left_trigger: None,
            button_right_trigger: None,
            button_left_joystick: None,
            button_right_joystick: None,
            button_share: None,
            button_options: None,
            button_touchpad: None,
            button_ps: None,
            axis_lx: None,
            axis_ly: None,
            axis_rx: None,
            axis_ry: None,
            gyro_x: None,
            gyro_y: None,
            gyro_z: None,
            accel_x: None,
            accel_y: None,
            accel_z: None,
            connected: None,
            battery_level: None,
            bluetooth: None,
            debug: None,
            temp: None,
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

    pub async fn handle_ps4_event(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        info!("Ps4Actor: handle_ps4_event: envelope={:?}", envelope);
        let event = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("Ps4Event envelope missing payload"))?;

        let event = Ps4Event::from_bytes(event)
            .map_err(|e| anyhow::anyhow!("Failed to decode Ps4Event: {e}"))?;

        self.button_left = event.button_left;
        self.button_right = event.button_right;
        self.button_up = event.button_up;
        self.button_down = event.button_down;
        self.button_square = event.button_square;
        self.button_cross = event.button_cross;
        self.button_circle = event.button_circle;
        self.button_triangle = event.button_triangle;
        self.button_left_shoulder = event.button_left_shoulder;
        self.button_right_shoulder = event.button_right_shoulder;
        self.button_left_trigger = event.button_left_trigger;
        self.button_right_trigger = event.button_right_trigger;
        self.button_left_joystick = event.button_left_joystick;
        self.button_right_joystick = event.button_right_joystick;
        self.button_share = event.button_share;
        self.button_options = event.button_options;
        self.button_touchpad = event.button_touchpad;
        self.button_ps = event.button_ps;
        self.axis_lx = event.axis_lx;
        self.axis_ly = event.axis_ly;
        self.axis_rx = event.axis_rx;
        self.axis_ry = event.axis_ry;
        self.gyro_x = event.gyro_x;
        self.gyro_y = event.gyro_y;
        self.gyro_z = event.gyro_z;
        self.accel_x = event.accel_x;
        self.accel_y = event.accel_y;
        self.accel_z = event.accel_z;
        self.connected = event.connected;
        self.battery_level = event.battery_level;
        self.bluetooth = event.bluetooth;
        self.debug = event.debug;
        self.temp = event.temp;
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

impl Actor for Ps4Actor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        info!("Ps4Actor started");
        state
            ._gateway
            .tell(Subscribe {
                msg_types: vec![Ps4Event::MSG_ID],
                recipient: actor_ref.recipient(),
            })
            .await?;

        Ok(state)
    }
}

impl Message<Arc<Envelope>> for Ps4Actor {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        msg: Arc<Envelope>,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.check_envelope(&msg)?;
        match msg.msg_type {
            Some(Ps4Event::MSG_ID) => self.handle_ps4_event(msg).await,
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

