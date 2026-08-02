use generated::generated::Envelope;
use kameo::prelude::*;
use std::sync::Arc;

#[derive(Debug, Clone)]
pub struct  RouterMessage {
    pub envelope: Arc<Envelope>,
    pub sender : Recipient<RouterMessage>,
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct Register {
    pub actor_ref: Recipient<RouterMessage>,
    pub description: String,
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct UnRegister {
    pub actor_ref: Recipient<RouterMessage>,
}

pub struct RouterTarget {
    pub recipient: Recipient<RouterMessage>,
    pub description: String,
}

#[derive(Actor)]
pub struct RouterActor {
    listeners: Vec<RouterTarget>,
}

impl RouterActor {
    pub fn new() -> Self {
        RouterActor {
            listeners: Vec::new(),
        }
    }
}

impl Message<RouterMessage> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: RouterMessage,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        for listener in self.listeners.iter() {
            if listener.recipient != msg.sender {
                let _ = listener.recipient.tell(msg.clone()).await;
            }
        }
    }
}

impl Message<Register> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: Register,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.listeners.push(RouterTarget {
            recipient: msg.actor_ref,
            description: msg.description,
        });
    }
}

impl Message<UnRegister> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: UnRegister,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.listeners.retain(|t| t.recipient != msg.actor_ref);
    }
}
