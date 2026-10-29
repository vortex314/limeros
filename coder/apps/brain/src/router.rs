use generated::generated::{id_to_string, opt_id_to_string, Envelope};
use kameo::prelude::*;
use log::{info, warn};
use std::{collections::HashMap, sync::Arc};

use crate::brain::ResultLog;

trait EnvelopeMatcher: Send {
    fn matches(&self, envelope: &Envelope) -> bool;
}

#[derive(Debug, Clone, Copy)]
pub struct EnvelopeOrFilter {
    pub dst: Option<u32>,
    pub src: Option<u32>,
    pub msg_type: Option<u32>,
}

#[derive(Debug, Clone, Copy)]
pub struct EnvelopeAndFilter {
    pub dst: Option<u32>,
    pub src: Option<u32>,
    pub msg_type: Option<u32>,
}
impl EnvelopeMatcher for EnvelopeAndFilter {
    fn matches(&self, envelope: &Envelope) -> bool {
        if self.dst.is_some() {
            if envelope.dst != self.dst {
                return false;
            }
        }
        if self.src.is_some() {
            if envelope.src != self.src {
                return false;
            }
        }
        if self.msg_type.is_some() {
            if envelope.msg_type != self.msg_type {
                return false;
            }
        }
        true
    }
}

impl EnvelopeMatcher for EnvelopeOrFilter {
    fn matches(&self, envelope: &Envelope) -> bool {
        let has_filter = self.dst.is_some() || self.src.is_some() || self.msg_type.is_some();
        if !has_filter {
            return true;
        }
        if self.dst.is_some() && envelope.dst == self.dst {
            return true;
        }
        if self.src.is_some() && envelope.src == self.src {
            return true;
        }
        if self.msg_type.is_some() && envelope.msg_type == self.msg_type {
            return true;
        }
        false
    }
}
pub struct EnvelopeFilter {
    pub dst: Option<u32>,
    pub src: Option<u32>,
    pub msg_type: Option<u32>,
}

pub struct RegisterWithRouter {
    pub recipient: Recipient<Envelope>,
    pub dst: u32,
    pub url: String,
}

pub struct AddListener {
    pub dst: u32,
    pub envelope_filter: EnvelopeFilter,
}

pub struct RemoveListener {
    pub dst: u32,
    pub envelope_filter: EnvelopeFilter,
}

pub struct FromRouter {
    pub envelope: Arc<Envelope>,
    pub id: u32,
}

pub struct ToDevice {
    pub envelope: Arc<Envelope>,
    pub id: u32,
}

#[derive(Clone)]
pub struct FromDevice {
    pub envelope: Arc<Envelope>,
    pub id: u32,
}

struct RegisterEntry {
    pub url: String,
    pub recipients: Vec<Recipient<Envelope>>,
}

struct StatEntry {
    registered: u32,
    recvs: u32,
    sends: u32,
}

#[derive(Actor)]
pub struct RouterActor {
    registered_endpoints: HashMap<u32, RegisterEntry>,
    listeners: HashMap<u32, Vec<EnvelopeFilter>>,
    stats: HashMap<u32, StatEntry>,
}

impl RouterActor {
    pub fn new() -> Self {
        RouterActor {
            registered_endpoints: HashMap::new(),
            listeners: HashMap::new(),
            stats: HashMap::new(),
        }
    }
}

impl Message<RegisterWithRouter> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: RegisterWithRouter,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.registered_endpoints
            .entry(msg.dst)
            .or_insert_with(|| RegisterEntry {
                url: msg.url.clone(),
                recipients: Vec::new(),
            })
            .recipients
            .push(msg.recipient);
        info!(
            "Registered endpoint for dst {} with URL {}",
            id_to_string(msg.dst),
            msg.url
        );
    }
}

impl Message<Envelope> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: Envelope,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        if let Some(register_entry) = self.registered_endpoints.get(&msg.dst.unwrap_or(0)) {
            for recipient in &register_entry.recipients {
                let _ = recipient
                    .tell(msg.clone())
                    .await
                    .log_error("Failed to send Envelope message to recipient");
            }
        }
        msg.src.map(|src| {
            self.stats
                .entry(src)
                .or_insert_with(|| StatEntry {
                    registered: 0,
                    recvs: 0,
                    sends: 0,
                })
                .sends
        });
        msg.dst.map(|dst| {
            self.stats
                .entry(dst)
                .or_insert_with(|| StatEntry {
                    registered: 0,
                    recvs: 0,
                    sends: 0,
                })
                .recvs += 1
        });
    }
}

impl Message<AddListener> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: AddListener,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.listeners
            .entry(msg.dst)
            .or_insert_with(Vec::new)
            .push(msg.envelope_filter);
        info!(
            "Added listeners for dst {} {:?}",
            msg.dst,
            id_to_string(msg.dst)
        );
    }
}
/*
impl Message<ToDevice> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: ToDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        if let Some(recipient) = self.to_device_list.get(&msg.id) {
            let _ = recipient
                .tell(ToDevice {
                    envelope: msg.envelope.clone(),
                    id: msg.id,
                })
                .await;
        } else {
            warn!(
                "RouterActor: No recipient found for ToDevice with id '{}'",
                id_to_string(msg.id)
            );
        }
    }
}
*/
/*
impl Message<FromDevice> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: FromDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        if let Some(recipients) = self.from_device_list.get(&msg.id) {
            for recipient in recipients {
                let _ = recipient
                    .tell(msg.clone())
                    .await
                    .log_error("Failed to send FromDevice message to recipient");
            }
        } else {
            warn!(
                "RouterActor: No recipients found for FromDevice with id {} ({}) from {} ({:?})",
                id_to_string(msg.id),
                msg.id,
                opt_id_to_string(msg.envelope.src),
                msg.envelope.src
            );
        }

        // Forward to locally registered endpoints whose ears are on this dst.
        if let Some(dst) = msg.envelope.dst {
            let accepted = match self.listeners.get(&dst) {
                Some(filters) if !filters.is_empty() => {
                    filters.iter().any(|f| f.matches(&msg.envelope))
                }
                _ => true,
            };
            if accepted {
                if let Some(recipients) = self.registered_endpoints.get(&dst) {
                    for recipient in recipients {
                        let _ = recipient.tell((*msg.envelope).clone()).await;
                    }
                }
            }
        }
    }
}

impl Message<RegisterDevice> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: RegisterDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.to_device_list.insert(msg.id, msg.recipient);
        info!(
            "Registered device with id {} ({})",
            id_to_string(msg.id),
            msg.id
        );
    }
}

impl Message<UnRegisterDevice> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: UnRegisterDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        // Only remove the registration if it still belongs to the caller.
        if let Some(recipient) = self.to_device_list.get(&msg.id) {
            if recipient == &msg.recipient {
                self.to_device_list.remove(&msg.id);
            }
        }
    }
}

impl Message<RegisterTwin> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: RegisterTwin,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.from_device_list
            .entry(msg.id)
            .or_insert_with(Vec::new)
            .push(msg.recipient);
    }
}

impl Message<UnRegisterTwin> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: UnRegisterTwin,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        if let Some(recipients) = self.from_device_list.get_mut(&msg.id) {
            recipients.retain(|r| r != &msg.recipient);
            if recipients.is_empty() {
                self.from_device_list.remove(&msg.id);
            }
        }
    }
}
*/
