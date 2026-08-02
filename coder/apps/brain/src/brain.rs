//! BrainActor — central coordinator owning all digital twins.

use std::sync::Arc;

use anyhow::Result;
use generated::generated::Envelope;
use kameo::prelude::*;
use log::info;
use statig::blocking::StateMachine;
use statig::prelude::*;

use crate::compass::CompassActor;
use crate::cutter::CutterActor;
use crate::hoverboard_proxy::HoverboardProxy;
use crate::imu::ImuActor;
use crate::router::RouterActor;

// ── Messages ───────────────────────────────────────────────────────────────

#[derive(Debug)]
pub enum EnvelopeHandlerRequest {
    SendEnvelope{endpoint:u32,envelope: Arc<Envelope>},
    SetListener{endpoint: u32, recipient: Recipient<EnvelopeHandlerEvent>},
}

#[derive(Debug) ]
pub enum EnvelopeHandlerEvent {
    ReceivedEnvelope { recipient: Recipient<EnvelopeHandlerRequest>, envelope: Arc<Envelope> },
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
    machine: StateMachine<BrainData>,
  //  hoverboard_ref: Option<ActorRef<HoverboardProxy>>,
    cutter_ref: Option<ActorRef<CutterActor>>,
    compass_ref: Option<ActorRef<CompassActor>>,
    imu_ref: Option<ActorRef<ImuActor>>,
    //   ps4_ref: Option<ActorRef<Ps4Proxy>>,
}

impl BrainActor {
    pub fn new(hb_ref: ActorRef<HoverboardProxy>) -> Self {
        let x = BrainData.state_machine();
        BrainActor {
            machine: x,
            hoverboard_ref: Some(hb_ref),
            cutter_ref: None,
            compass_ref: None,
            imu_ref: None,
            //          ps4_ref: None,
        }
    }
    async fn dispatch(&mut self, event: &BrainEvent) {
        self.machine.handle(event);
    }

    async fn on_link_died(
        &mut self,
        _ctx: &mut Context<Self, ()>,
        id: ActorId,
        reason: ActorStopReason,
    ) -> Result<()> {
        // e.g. force the FSM to a safe state if the hoverboard twin dies mid-run
        self.dispatch(&BrainEvent::Failure(Failure::TwinDied(id, reason))).await;
        Ok(())
    }

    // FIX #2: Same for init — explicit Send future
    async fn init(&mut self, actor_ref: ActorRef<BrainActor>) {
        let router = self.router.clone();

   //     let hoverboard_ref =
   //         HoverboardProxy::spawn(HoverboardProxy::new(router.clone(), actor_ref.clone()));
        let cutter_ref = CutterActor::spawn(CutterActor::new(router.clone(), actor_ref.clone()));
        let compass_ref = CompassActor::spawn(CompassActor::new(router.clone(), actor_ref.clone()));
        let imu_ref = ImuActor::spawn(ImuActor::new(router.clone(), actor_ref.clone()));
        //         let ps4_ref = Ps4Proxy::spawn(Ps4Proxy::new(actor_ref.recipient()));

        /* actor_ref.link(&hoverboard_ref).await;
        actor_ref.link(&cutter_ref).await;
        actor_ref.link(&compass_ref).await;
        actor_ref.link(&imu_ref).await;
        actor_ref.link(&ps4_ref).await;*/

   //     self.hoverboard_ref = Some(hoverboard_ref);
        self.cutter_ref = Some(cutter_ref);
        self.compass_ref = Some(compass_ref);
        self.imu_ref = Some(imu_ref);
        //          self.ps4_ref = Some(ps4_ref);
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
        ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        info!("BrainActor: handle BrainEvent: {:?}", msg);
        self.dispatch(&msg).await;
    }
}

impl Message<BrainRequest> for BrainActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: BrainRequest,
        ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        info!("BrainActor: handle BrainRequest: {:?}", msg);
        match msg {
            BrainRequest::SetSpeed(speed) => {
                if let Some(ref hoverboard_ref) = self.hoverboard_ref {
                    let _ = hoverboard_ref.tell(BrainCmd::SetSpeed(speed)).await;
                }
            }
            BrainRequest::SetSteer(steer) => {
                if let Some(ref hoverboard_ref) = self.hoverboard_ref {
                    let _ = hoverboard_ref.tell(BrainCmd::SetSteer(steer)).await;
                }
            }
            BrainRequest::SetCutter(enabled) => {
                if let Some(ref cutter_ref) = self.cutter_ref {
                    let _ = cutter_ref.tell(BrainCmd::SetCutter(enabled)).await;
                }
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
