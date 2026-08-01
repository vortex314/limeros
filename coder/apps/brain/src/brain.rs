//! BrainActor — central coordinator owning all digital twins.

use anyhow::Result;
use kameo::prelude::*;
use log::info;
use statig::prelude::*;
use statig::blocking::StateMachine;

use crate::compass::CompassActor;
use crate::cutter::CutterActor;
use crate::hoverboard::HoverboardActor;
use crate::imu::ImuActor;
use crate::ps4::Ps4Actor;
use crate::router::RouterActor;

// ── Messages ───────────────────────────────────────────────────────────────

// ── Unified event enum ─────────────────────────────────────────────────────
//
// Each kameo message handler converts its typed message into this enum,
// then calls `self.dispatch(event)` — the single method that drives
// the state machine. This avoids conflicting with kameo's own `handle`.

#[derive(Debug)]
pub enum BrainEvent {
    Initialize,
    RemoteOn,
    Idle,
    Automatic,
    TwinDied(ActorId, ActorStopReason),
}
#[derive(Debug)]
pub enum BrainCmd {
    SetSpeed(i32),
    SetSteer(i32),
    SetCutter(bool, i32),
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
            BrainEvent::RemoteOn => Transition(State::remote_on()),
            BrainEvent::Automatic => Transition(State::automatic()),
            _ => Super,
        }
    }

    #[state]
    fn remote_on(&mut self, event: &BrainEvent) -> Outcome<State> {
        match event {
            BrainEvent::Idle => Transition(State::idle()),
            BrainEvent::Automatic => Transition(State::automatic()),
            _ => {
                info!("RemoteOn state received event: {:?}", event);
                Super
            }
        }
    }

    #[state]
    fn automatic(&mut self, event: &BrainEvent) -> Outcome<State> {
        match event {
            BrainEvent::Idle => Transition(State::idle()),
            BrainEvent::RemoteOn => Transition(State::remote_on()),
            _ => {
                info!("Automatic state received event: {:?}", event);
                Super
            }
        }
    }
    #[action]
    fn stop_all(&mut self) {
        info!("Stopping all twins");
    }
}

// ── BrainActor — drives the FSM via stored data + current state ID ────────
pub struct BrainActor {
    router: ActorRef<RouterActor>,
    machine: StateMachine<BrainData>,
    hoverboard_ref: Option<ActorRef<HoverboardActor>>,
    cutter_ref: Option<ActorRef<CutterActor>>,
    compass_ref: Option<ActorRef<CompassActor>>,
    imu_ref: Option<ActorRef<ImuActor>>,
    ps4_ref: Option<ActorRef<Ps4Actor>>,
}

impl BrainActor {
    pub fn new(router: ActorRef<RouterActor>) -> Self {
        let x = BrainData.state_machine();
        BrainActor {
            router,
            machine: x,
            hoverboard_ref: None,
            cutter_ref: None,
            compass_ref: None,
            imu_ref: None,
            ps4_ref: None,
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
        self.dispatch(&BrainEvent::TwinDied(id, reason)).await;
        Ok(())
    }

    // FIX #2: Same for init — explicit Send future
    async fn init(&mut self, actor_ref: ActorRef<BrainActor>) {
            let router = self.router.clone();

            let hoverboard_ref =
                HoverboardActor::spawn(HoverboardActor::new(router.clone(), actor_ref.clone()));
            let cutter_ref =
                CutterActor::spawn(CutterActor::new(router.clone(), actor_ref.clone()));
            let compass_ref =
                CompassActor::spawn(CompassActor::new(router.clone(), actor_ref.clone()));
            let imu_ref = ImuActor::spawn(ImuActor::new(router.clone(), actor_ref.clone()));
            let ps4_ref = Ps4Actor::spawn(Ps4Actor::new(router.clone(), actor_ref.clone()));

            /* actor_ref.link(&hoverboard_ref).await;
            actor_ref.link(&cutter_ref).await;
            actor_ref.link(&compass_ref).await;
            actor_ref.link(&imu_ref).await;
            actor_ref.link(&ps4_ref).await;*/

            self.hoverboard_ref = Some(hoverboard_ref);
            self.cutter_ref = Some(cutter_ref);
            self.compass_ref = Some(compass_ref);
            self.imu_ref = Some(imu_ref);
            self.ps4_ref = Some(ps4_ref);
    }
}

impl Actor for BrainActor {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(mut state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
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
        let _ = match msg {
            BrainEvent::Initialize => {
                self.init(ctx.actor_ref().clone()).await;
                self.dispatch(&BrainEvent::Idle).await;
            }
            _ => {
                self.dispatch(&msg).await;
            }
        };
    }
}
