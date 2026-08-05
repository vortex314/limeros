use generated::generated::{Envelope, id_to_string, opt_id_to_string};
use kameo::prelude::*;
use log::{info, warn};
use std::{collections::HashMap, sync::Arc};

use crate::brain::ResultLog;

pub struct ToDevice {
    pub envelope: Arc<Envelope>,
    pub id: u32,
}

#[derive(Clone)]
pub struct FromDevice {
    pub envelope: Arc<Envelope>,
    pub id: u32,
}

pub struct RegisterDevice {
    pub recipient: Recipient<ToDevice>,
    pub id: u32,
}

pub struct UnRegisterDevice {
    pub recipient: Recipient<ToDevice>,
    pub id: u32,
}

pub struct RegisterTwin {
    pub recipient: Recipient<FromDevice>,
    pub id: u32,
}

pub struct UnRegisterTwin {
    pub recipient: Recipient<FromDevice>,
    pub id: u32,
}

#[derive(Actor)]
pub struct RouterActor {
    to_device_list: HashMap<u32, Recipient<ToDevice>>,
    from_device_list: HashMap<u32, Vec<Recipient<FromDevice>>>,

}

impl RouterActor {
    pub fn new() -> Self {
        RouterActor {
            to_device_list: HashMap::new(),
            from_device_list: HashMap::new(),
        }
    }
}

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
                "RouterActor: No recipient found for ToDevice with id {} {}",
                id_to_string(msg.id),
                msg.id
            );
        }
    }
}

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
                    .await.log_error("Failed to send FromDevice message to recipient");
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
        info!("Registered device with id {} ({})", id_to_string(msg.id), msg.id);
    }
}

impl Message<UnRegisterDevice> for RouterActor {
    type Reply = ();

    async fn handle(
        &mut self,
        msg: UnRegisterDevice,
        _ctx: &mut Context<Self, Self::Reply>,
    ) -> Self::Reply {
        self.to_device_list.remove(&msg.id);
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
