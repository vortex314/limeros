//! BrainActor — central coordinator owning all digital twins.

use std::sync::Arc;

use anyhow::Result;
use generated::generated::Envelope;
use kameo::prelude::*;
use log::{info, warn};
use statig::blocking::StateMachine;
use statig::prelude::*;

use crate::compass::CompassActor;
use crate::cutter::CutterActor;
use crate::hoverboard_proxy::HoverboardProxy;
use crate::imu::ImuActor;
use crate::ps4_proxy::Ps4Proxy;
use crate::router::RouterActor;

// ── Messages ───────────────────────────────────────────────────────────────

#[derive(Debug)]
pub enum EnvelopeHandlerRequest {
    SendEnvelope{endpoint:u32,envelope: Arc<Envelope>},
    SetListener{endpoint: u32, recipient: Recipient<EnvelopeHandlerEvent>},
}

#[derive(Debug) ]
pub enum EnvelopeHandlerEvent {
    ReceivedEnvelope { sender: Recipient<EnvelopeHandlerRequest>, envelope: Arc<Envelope> },
}

// ── Unified event enum ─────────────────────────────────────────────────────
//
// Each kameo message handler converts its typed message into this enum,
// then calls `self.dispatch(event)` — the single method that drives
// the state machine. This avoids conflicting with kameo's own `handle`.

#[derive(Debug)]
pub enum Failure {
    TwinDied(ActorId, ActorStopReason),
    TemperatureHigh(f32),
    VoltageLow(f32),
    CurrentHigh(f32),
}

#[derive(Debug)]
pub enum BrainEvent {
    Failure(Failure),
    LocationUpdate(f32, f32),
    CompassUpdate(f32, f32, f32, f32, f32, f32, f32, f32, f32),
    ImuUpdate(f32, f32, f32, f32, f32, f32, f32, f32, f32),
    HoverboardUpdate(i32, i32, f32, f32),
    CutterUpdate(bool, i32, f32, f32, f32),
    StateIdle,
    StateRemote,
    StateAutomatic,
}
#[derive(Debug)]
pub enum BrainRequest {
    SetSpeed(i32),
    SetSteer(i32),
    SetCutter(bool),
    SetState(StateChange),
}

#[derive(Debug)]
pub enum StateChange {
    StateAutomatic,
    StateRemote,
    StateIdle,
}
#[derive(Debug)]
pub enum BrainCmd {
    SetSpeed(i32),
    SetSteer(i32),
    SetCutter(bool),
}

// ── Shared storage for the state machine ───────────────────────────────────
//
// statig's macro operates on THIS type, not on BrainActor. An instance of
// it lives inside the running `StateMachine<BrainData>` that BrainActor
// owns below.

pub struct BrainData;

#[state_machine(initial = "State::idle()", state(derive(Debug)))]
impl BrainData {
    #[state(entry_action = "stop_all")]
    fn idle(&mut self, event: &BrainEvent) -> Outcome<State> {
        match event {
            BrainEvent::StateRemote => Transition(State::remote_on()),
            BrainEvent::StateAutomatic => Transition(State::automatic()),
            _ => Super,
        }
    }

    #[state]
    fn remote_on(&mut self, event: &BrainEvent) -> Outcome<State> {
        match event {
            BrainEvent::StateIdle => Transition(State::idle()),
            BrainEvent::StateAutomatic => Transition(State::automatic()),
            _ => {
                info!("RemoteOn state received event: {:?}", event);
                Super
            }
        }
    }

    #[state]
    fn automatic(&mut self, event: &BrainEvent) -> Outcome<State> {
        match event {
            BrainEvent::StateIdle => Transition(State::idle()),
            BrainEvent::StateRemote => Transition(State::remote_on()),
            _ => {
                info!("Automatic state received event: {:?}", event);
                Super
            }
        }
    }
    #[action]
    fn stop_all(&mut self) {}
}

// ── BrainActor — drives the FSM via stored data + current state ID ────────
pub struct BrainActor {
    pub  machine: Option<StateMachine<BrainData>>,
    pub hoverboard_ref: ActorRef<HoverboardProxy>,
    pub cutter_ref: ActorRef<CutterActor>,
    pub compass_ref: ActorRef<CompassActor>,
    pub imu_ref: ActorRef<ImuActor>,
    pub ps4_ref: ActorRef<Ps4Proxy>,
}

impl BrainActor {

    async fn dispatch(&mut self, event: &BrainEvent) {
        if let Some(ref mut machine) = self.machine {
            machine.handle(event);
        }
    }

    async fn on_link_died(
        &mut self,
        _ctx: &mut Context<Self, ()>,
        id: ActorId,
        reason: ActorStopReason,
    ) -> Result<()> {
        // e.g. force the FSM to a safe state if the hoverboard twin dies mid-run
        warn!("Linked actor {:?} died: {:?}", id, reason);
        self.dispatch(&BrainEvent::Failure(Failure::TwinDied(id, reason))).await;
        Ok(())
    }

    // FIX #2: Same for init — explicit Send future
    async fn init(&mut self, actor_ref: ActorRef<BrainActor>) {
        self.machine = Some(BrainData.state_machine());
    }
}

impl Actor for BrainActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(
        mut state: Self::Args,
        actor_ref: ActorRef<Self>,
    ) -> Result<Self, Self::Error> {
        info!("BrainActor started");
        state.init(actor_ref.clone()).await;
        Ok(state)
    }
}

// ── Command handlers ───────────────────────────────────────────────────────

impl Message<BrainEvent> for BrainActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: BrainEvent,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        info!("BrainActor: handle BrainEvent: {:?}", msg);
        self.dispatch(&msg).await;
    }
}

/// Extension trait adding fire-and-forget error handling to `Result`.
pub trait ResultLog<T, E> {
    /// Consumes the `Result`, logging any error with `context`. Returns nothing.
    fn log_error(self, context: &str)
    where
        E: std::fmt::Display;

    /// Consumes the `Result`, calling `f` with the error if there is one. Returns nothing.
    fn on_error(self, f: impl FnOnce(E));
}

impl<T, E> ResultLog<T, E> for Result<T, E> {
    fn log_error(self, context: &str)
    where
        E: std::fmt::Display,
    {
        if let Err(e) = self {
            warn!("{context}: {e}");
        }
    }

    fn on_error(self, f: impl FnOnce(E)) {
        if let Err(e) = self {
            f(e);
        }
    }
}

impl Message<BrainRequest> for BrainActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: BrainRequest,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        info!("BrainActor: handle BrainRequest: {:?}", msg);
        match msg {
            BrainRequest::SetSpeed(speed) => {
                self.hoverboard_ref.tell(BrainCmd::SetSpeed(speed)).await.log_error("Failed to send SetSpeed to HoverboardProxy");
            }
            BrainRequest::SetSteer(steer) => {
                self.hoverboard_ref.tell(BrainCmd::SetSteer(steer)).await.log_error("Failed to send SetSteer to HoverboardProxy");
            }
            BrainRequest::SetCutter(enabled) => {
                self.cutter_ref.tell(BrainCmd::SetCutter(enabled)).await.on_error(|e| log::error!("Failed to send SetCutter to CutterActor: {:?}", e));
            }
            BrainRequest::SetState(state_change) => match state_change {
                StateChange::StateAutomatic => {
                    self.dispatch(&BrainEvent::StateAutomatic).await;
                }
                StateChange::StateRemote => {
                    self.dispatch(&BrainEvent::StateRemote).await;
                }
                StateChange::StateIdle => {
                    self.dispatch(&BrainEvent::StateIdle).await;
                }
            },
        }
    }
}
