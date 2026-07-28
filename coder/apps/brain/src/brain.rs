//! BrainActor — central coordinator owning all digital twins.

use anyhow::Result;
use kameo::prelude::*;
use log::info;

use crate::compass::CompassActor;
use crate::cutter::CutterActor;
use crate::hoverboard::HoverboardActor;
use crate::imu::ImuActor;
use crate::udp_endpoint::UdpEndpoint;

// ── Messages ───────────────────────────────────────────────────────────────

/// Command to set hoverboard speed.
pub struct SetSpeed(pub i32);

/// Command to set hoverboard steer.
pub struct SetSteer(pub i32);

/// Command to enable/disable the cutter (enabled, rpm).
pub struct SetCutter(pub bool, pub i32);

// ── BrainActor ─────────────────────────────────────────────────────────────

pub struct BrainActor {
    pub udp_ref: ActorRef<UdpEndpoint>,
    pub hoverboard_ref: ActorRef<HoverboardActor>,
    pub cutter_ref: ActorRef<CutterActor>,
    pub compass_ref: ActorRef<CompassActor>,
    pub imu_ref: ActorRef<ImuActor>,
}

impl BrainActor {
    pub fn new(
        udp_ref: ActorRef<UdpEndpoint>,
        hoverboard_ref: ActorRef<HoverboardActor>,
        cutter_ref: ActorRef<CutterActor>,
        compass_ref: ActorRef<CompassActor>,
        imu_ref: ActorRef<ImuActor>,
    ) -> Self {
        BrainActor {
            udp_ref,
            hoverboard_ref,
            cutter_ref,
            compass_ref,
            imu_ref,
        }
    }
}

impl Actor for BrainActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, _actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        info!("BrainActor started — all digital twins running");
        Ok(state)
    }
}

// ── Command handlers ───────────────────────────────────────────────────────

impl Message<SetSpeed> for BrainActor {
    type Reply = ();

    async fn handle(&mut self, msg: SetSpeed, _ctx: &mut Context<Self, Self::Reply>) {
        info!("Brain: setting hoverboard speed to {}", msg.0);
    }
}

impl Message<SetSteer> for BrainActor {
    type Reply = ();

    async fn handle(&mut self, msg: SetSteer, _ctx: &mut Context<Self, Self::Reply>) {
        info!("Brain: setting hoverboard steer to {}", msg.0);
    }
}

impl Message<SetCutter> for BrainActor {
    type Reply = ();

    async fn handle(&mut self, msg: SetCutter, _ctx: &mut Context<Self, Self::Reply>) {
        info!("Brain: setting cutter enabled={} rpm={}", msg.0, msg.1);
    }
}

