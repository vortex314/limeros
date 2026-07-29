//! BrainActor — central coordinator owning all digital twins.

use std::sync::Arc;

use anyhow::Result;
use generated::generated::Envelope;
use kameo::prelude::*;
use log::info;
use statig::prelude::*;

use crate::compass::CompassActor;
use crate::cutter::CutterActor;
use crate::hoverboard::HoverboardActor;
use crate::imu::ImuActor;
use crate::ps4::Ps4Actor;
use crate::udp_endpoint::UdpEndpoint;

// ── Messages ───────────────────────────────────────────────────────────────

pub struct SetSpeed(pub i32);
pub struct SetSteer(pub i32);
pub struct SetCutter(pub bool, pub i32);

// ── Unified event enum ─────────────────────────────────────────────────────
//
// Each kameo message handler converts its typed message into this enum,
// then calls `self.dispatch(event)` — the single method that drives
// the state machine. This avoids conflicting with kameo's own `handle`.

#[derive(Debug)]
pub enum BrainEvent {
    RemoteOnButtonPressed,
    IdleButtonPressed,
    AutoModeButtonPressed,
    SetSpeed(i32),
    SetSteer(i32),
    SetCutter(bool, i32),
}

// ── Shared storage for the state machine ───────────────────────────────────
//
// statig's macro operates on THIS type, not on BrainActor. An instance of
// it lives inside the running `StateMachine<BrainData>` that BrainActor
// owns below. Field access from state handlers, and from BrainActor via
// Deref, both go through here.

pub struct BrainData {
    pub udp_ref: ActorRef<UdpEndpoint>,
    pub hoverboard_ref: ActorRef<HoverboardActor>,
    pub cutter_ref: ActorRef<CutterActor>,
    pub compass_ref: ActorRef<CompassActor>,
    pub imu_ref: ActorRef<ImuActor>,
    pub ps4_ref: ActorRef<Ps4Actor>,
}

#[state_machine(initial = "State::idle()", state(derive(Debug)))]
impl BrainData {
    #[state]
    fn idle(&mut self, event: &BrainEvent) -> Outcome<State> {
        match event {
            BrainEvent::RemoteOnButtonPressed => Transition(State::remote_on()),
            _ => Super,
        }
    }

    #[state]
    fn remote_on(&mut self, event: &BrainEvent) -> Outcome<State> {
        match event {
            BrainEvent::IdleButtonPressed => Transition(State::idle()),
            _ => {
                info!("RemoteOn state received event: {:?}", event);
                Super
            }
        }
    }
}

// ── BrainActor — the kameo actor; wraps the running, initialized machine ──

pub struct BrainActor {
    machine: InitializedStateMachine<BrainData>,
}

impl BrainActor {
    pub fn new(
        udp_ref: ActorRef<UdpEndpoint>,
        hoverboard_ref: ActorRef<HoverboardActor>,
        cutter_ref: ActorRef<CutterActor>,
        compass_ref: ActorRef<CompassActor>,
        imu_ref: ActorRef<ImuActor>,
        ps4_ref: ActorRef<Ps4Actor>,
    ) -> Self {
        let data = BrainData {
            udp_ref,
            hoverboard_ref,
            cutter_ref,
            compass_ref,
            imu_ref,
            ps4_ref,
        };
        BrainActor {
            machine: data.uninitialized_state_machine().init(),
        }
    }

    /// Routes any typed message into the shared event and drives the FSM.
    fn dispatch(&mut self, event: BrainEvent) {
        self.machine.handle(&event);
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

impl Message<Arc<Envelope>> for BrainActor {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        _msg: Arc<Envelope>,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        Ok(())
    }
}

impl Message<BrainEvent> for BrainActor {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        msg: BrainEvent,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.dispatch(msg);
        Ok(())
    }
}

impl Message<SetSpeed> for BrainActor {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        msg: SetSpeed,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.dispatch(BrainEvent::SetSpeed(msg.0));
        Ok(())
    }
}

impl Message<SetSteer> for BrainActor {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        msg: SetSteer,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.dispatch(BrainEvent::SetSteer(msg.0));
        Ok(())
    }
}

impl Message<SetCutter> for BrainActor {
    type Reply = Result<()>;

    async fn handle(
        &mut self,
        msg: SetCutter,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.dispatch(BrainEvent::SetCutter(msg.0, msg.1));
        Ok(())
    }
}