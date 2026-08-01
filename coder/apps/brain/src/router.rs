use anyhow::Result;
use generated::generated::Envelope;
use kameo::prelude::*;
use std::sync::Arc;

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct Register {
    pub actor_ref: Recipient<Arc<Envelope>>,
    pub description: String,
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct UnRegister {
    pub actor_ref: Recipient<Arc<Envelope>>,
}

pub struct RouterTarget {
    pub recipient: Recipient<Arc<Envelope>>,
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

impl Message<Arc<Envelope>> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        envelope: Arc<Envelope>,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        for listener in self.listeners.iter() {
            listener.recipient.tell(envelope.clone()).await;
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
