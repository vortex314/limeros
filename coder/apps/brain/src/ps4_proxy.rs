//! Ps4Actor — digital twin of the PS4 controller, receives Ps4Event telemetry
//! from the broker (published by the ps4_bridge binary).

use anyhow::Result;
use generated::generated::{Ps4Event};
use kameo::prelude::*;
use log::{debug, info};

use crate::brain::{ BrainRequest, StateChange};

// ── Ps4Actor ───────────────────────────────────────────────────────────────
// digital twin of the PS4 controller, tracks button and axis state.
#[derive(Actor)]
pub struct Ps4Proxy {
    listener: Recipient<BrainRequest>,
    drive_speed: i32,
    driver_steer: i32,

}

impl Ps4Proxy {
    pub fn new(listener: Recipient<BrainRequest>) -> Self {
        Ps4Proxy {
            listener,
            drive_speed: 0,
            driver_steer: 0,
        }
    }
}

impl Message<Ps4Event> for Ps4Proxy {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: Ps4Event,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        if msg.button_cross == Some(true) {
            debug!("Ps4Proxy: button_cross pressed, sending BrainRequest::SetState(StateChange::StateIdle)");
            let _ = self.listener.tell(BrainRequest::SetState(StateChange::StateIdle)).await;
        };
        if msg.button_square == Some(true) {
            debug!("Ps4Proxy: button_square pressed, sending BrainRequest::SetState(StateChange::StateRemote)");
            let _ = self.listener.tell(BrainRequest::SetState(StateChange::StateRemote)).await;
        }
        if msg.button_circle == Some(true) {
            debug!("Ps4Proxy: button_circle pressed, sending BrainRequest::SetState(StateChange::StateAutomatic)");
            let _ = self.listener.tell(BrainRequest::SetState(StateChange::StateAutomatic)).await;
        }
        if msg.button_left_trigger == Some(true) {
            debug!("Ps4Proxy: button_left_trigger pressed, sending BrainRequest::SetCutter(true)");
            let _ = self.listener.tell(BrainRequest::SetCutter(true)).await;
        }
        if msg.button_left_trigger == Some(false) {
            debug!("Ps4Proxy: button_left_trigger released, sending BrainRequest::SetCutter(false)");
            let _ = self.listener.tell(BrainRequest::SetCutter(false)).await;
        }
        if msg.axis_lx.is_some() {
            self.driver_steer = (msg.axis_lx.unwrap_or(0) * 400) / 128;
            debug!("Ps4Proxy: axis_lx changed, sending BrainRequest::SetSteer({})", self.driver_steer);
            let _ = self.listener.tell(BrainRequest::SetSteer(self.driver_steer)).await;
        }
        if msg.axis_ly.is_some() {
            self.drive_speed = (msg.axis_ly.unwrap_or(0) * 400) / 128;
            debug!("Ps4Proxy: axis_ly changed, sending BrainRequest::SetSpeed({})", self.drive_speed);
            let _ = self.listener.tell(BrainRequest::SetSpeed(self.drive_speed)).await;
        }
    }
}
