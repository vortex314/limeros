//! MotorProxy — digital twin of a motor controller, sends periodic MotorRequest
//! commands via the router and tracks MotorReply / MotorEvent telemetry.

use std::sync::Arc;
use std::time::{Duration, Instant};

use anyhow::Result;
use common::fnv;
use generated::generated::{
    EndpointAnnounce, Envelope, MotorEvent, MotorReply, MotorRequest,  SysEvent, opt_id_to_string,
};
use kameo::prelude::*;
use log::{info, warn};

use crate::router::RegisterWithRouter;
use crate::{
    brain::{BrainActor, ResultLog},
    router::{FromDevice, RouterActor},
};

/// Name of the motor endpoint as announced on the bus.
const MOTOR_ENDPOINT: &str = "motor";
/// Send a MotorRequest twice per second.
const TICK_INTERVAL: Duration = Duration::from_millis(500);
/// Consider the motor offline after this many seconds without an event.
const EVENT_TIMEOUT_SECS: u64 = 5;

// ── Messages ───────────────────────────────────────────────────────────────

/// Timer tick to send a MotorRequest.
struct TickMotor;

/// Commands accepted by [`MotorProxy`].
#[allow(dead_code)]
pub enum MotorCmd {
    SetEnabled(bool),
    SetRpm(i32),
}

// ── MotorProxy ─────────────────────────────────────────────────────────────

pub struct MotorProxy {
    _brain: ActorRef<BrainActor>,
    router: ActorRef<RouterActor>,
    enabled: bool,
    rpm_target: i32,
    rpm_measured: i32,
    pwm: f32,
    current: f32,
    voltage: f32,
    temperature: f32,
    kp: f32,
    ki: f32,
    kd: f32,
    last_event_time: Option<Instant>,
}

impl MotorProxy {
    pub fn new(brain: ActorRef<BrainActor>, router: ActorRef<RouterActor>) -> Self {
        MotorProxy {
            _brain: brain,
            router,
            enabled: false,
            rpm_target: 0,
            rpm_measured: 0,
            pwm: 0.0,
            current: 0.0,
            voltage: 0.0,
            temperature: 0.0,
            kp: 0.0,
            ki: 0.0,
            kd: 0.0,
            last_event_time: None,
        }
    }

    pub fn set_enabled(&mut self, enabled: bool) {
        self.enabled = enabled;
    }

    pub fn set_rpm(&mut self, rpm: i32) {
        self.rpm_target = rpm;
    }

    #[allow(dead_code)]
    pub fn device_timed_out(&self) -> bool {
        match self.last_event_time {
            Some(last_time) => last_time.elapsed().as_secs() > EVENT_TIMEOUT_SECS,
            None => true,
        }
    }

    pub async fn handle_motor_reply(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        let reply = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("MotorReply envelope missing payload"))?;

        let reply = MotorReply::from_bytes(reply)
            .map_err(|e| anyhow::anyhow!("Failed to decode MotorReply: {e}"))?;

        if let Some(code) = reply.status_code {
            if code != 0 {
                warn!(
                    "MotorReply error {}: {}",
                    code,
                    reply.message.as_deref().unwrap_or("")
                );
            }
        }

        Ok(())
    }

    pub async fn handle_motor_event(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        let event = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("MotorEvent envelope missing payload"))?;

        let event = MotorEvent::from_bytes(event)
            .map_err(|e| anyhow::anyhow!("Failed to decode MotorEvent: {e}"))?;
        info!("Received MotorEvent: {:?}", event);

        event.enabled.map(|e| self.enabled = e);
        event.rpm_measured.map(|r| self.rpm_measured = r);
        event.rpm_target.map(|r| self.rpm_target = r);
        event.pwm.map(|p| self.pwm = p);
        event.current.map(|c| self.current = c);
        event.voltage.map(|v| self.voltage = v);
        event.temperature.map(|t| self.temperature = t);
        event.Kp.map(|v| self.kp = v);
        event.Ki.map(|v| self.ki = v);
        event.Kd.map(|v| self.kd = v);
        self.last_event_time = Some(Instant::now());

        Ok(())
    }

    pub async fn handle_timer_tick(&mut self) {
        let request = MotorRequest {
            enabled: Some(self.enabled),
            rpm_target: Some(self.rpm_target),
        };
        let payload = match request.to_bytes() {
            Ok(p) => p,
            Err(e) => {
                warn!("Failed to encode MotorRequest: {}", e);
                return;
            }
        };
        let envelope = Envelope {
            dst: Some(fnv::fnv1a_32(MOTOR_ENDPOINT)),
            msg_type: Some(MotorRequest::MSG_ID),
            payload: Some(payload),
            ..Default::default()
        };
        self.router
            .tell(envelope)
            .await
            .log_error("Failed to send ToDevice message to motor");
    }
    pub async fn handle_sys_event(&mut self, envelope: Arc<Envelope>) -> Result<()> {
        let event = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("SysEvent envelope missing payload"))?;

        let event = SysEvent::from_bytes(event)
            .map_err(|e| anyhow::anyhow!("Failed to decode SysEvent: {e}"))?;

        // Handle SysEvent fields here if needed
        info!("Received SysEvent: {:?}", event);

        Ok(())
    }
}

impl Actor for MotorProxy {
    type Args = Self;
    type Error = anyhow::Error;

    async fn on_start(state: Self::Args, actor_ref: ActorRef<Self>) -> Result<Self, Self::Error> {
        info!("MotorProxy started (2Hz command loop)");
        state
            .router
            .tell(RegisterWithRouter {
                dst: fnv::fnv1a_32("MotorProxy"),
                recipient: actor_ref.clone().recipient(),
                url: "MotorProxy".to_string(),
            })
            .await
            .log_error("Failed to register MotorProxy with router");

        // 2Hz timer
        let tick_ref = actor_ref.clone();
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(TICK_INTERVAL);
            loop {
                interval.tick().await;
                let _ = tick_ref.tell(TickMotor).await;
            }
        });

        Ok(state)
    }
}

impl Message<Envelope> for MotorProxy {
    type Reply = ();
    async fn handle(
        &mut self,
        _msg: Envelope,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        // Handle Envelope messages if needed
    }
}


// ── Handle TickMotor ──────────────────────────────────────────────────────

impl Message<TickMotor> for MotorProxy {
    type Reply = ();

    async fn handle(&mut self, _msg: TickMotor, _ctx: &mut Context<Self, Self::Reply>) {
        self.handle_timer_tick().await;
    }
}

impl Message<FromDevice> for MotorProxy {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: FromDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        let envelope = msg.envelope;
        match envelope.msg_type {
            Some(MotorReply::MSG_ID) => {
                if let Err(e) = self.handle_motor_reply(envelope).await {
                    warn!("Failed to handle MotorReply: {}", e);
                }
            }
            Some(MotorEvent::MSG_ID) => {
                if let Err(e) = self.handle_motor_event(envelope).await {
                    warn!("Failed to handle MotorEvent: {}", e);
                }
            }
            Some(SysEvent::MSG_ID) => {
                if let Err(e) = self.handle_sys_event(envelope).await {
                    warn!("Failed to handle SysEvent: {}", e);
                }
            }
            Some(EndpointAnnounce::MSG_ID) => {}
            _ => {
                warn!(
                    "MotorProxy received unexpected message type: {} {:?}",
                    opt_id_to_string(envelope.msg_type),
                    envelope.msg_type
                );
            }
        }
    }
}

impl Message<MotorCmd> for MotorProxy {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: MotorCmd,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        match msg {
            MotorCmd::SetEnabled(enabled) => self.set_enabled(enabled),
            MotorCmd::SetRpm(rpm) => self.set_rpm(rpm),
        }
    }
}
