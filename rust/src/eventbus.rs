use anyhow::Result;
use log::warn;
use std::{any::{Any, TypeId}, sync::Arc};
use std::future::Future;
use tokio::sync::mpsc::{self, error::{SendError, TrySendError}};

/// Default channel capacity for the eventbus.
const DEFAULT_BUS_CAPACITY: usize = 256;

#[async_trait::async_trait]
pub trait ActorImpl: Send {
    /// Returns the set of message TypeIds this actor is interested in.
    /// Return an empty slice to receive all messages (backward-compatible default).
    fn interests(&self) -> &[TypeId] {
        &[]
    }

    async fn handle(&mut self, msg: &Arc<dyn Any + Send + Sync>);
}

/// Error returned when an eventbus emission fails (channel full or closed).
#[derive(Debug)]
pub enum BusError {
    Full,
    Closed,
}

impl std::fmt::Display for BusError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            BusError::Full => write!(f, "EventBus channel full"),
            BusError::Closed => write!(f, "EventBus channel closed"),
        }
    }
}

impl std::error::Error for BusError {}

impl<T> From<SendError<T>> for BusError {
    fn from(_e: SendError<T>) -> Self {
        BusError::Closed
    }
}

impl<T> From<TrySendError<T>> for BusError {
    fn from(e: TrySendError<T>) -> Self {
        match e {
            TrySendError::Full(_) => BusError::Full,
            TrySendError::Closed(_) => BusError::Closed,
        }
    }
}

#[derive(Clone)]
pub struct Bus {
    sender: mpsc::Sender<Arc<dyn Any + Send + Sync>>,
}

impl Bus {
    pub fn new(sender: mpsc::Sender<Arc<dyn Any + Send + Sync>>) -> Self {
        Bus { sender }
    }

    /// Emit a message onto the bus. Returns an error if the channel is full or closed.
    pub fn emit<M: 'static + Any + Send + Sync>(&self, msg: M) -> Result<(), BusError> {
        let m: Arc<dyn Any + Send + Sync> = Arc::new(msg);
        self.sender.try_send(m).map_err(|e| {
            warn!("EventBus send error: {}", e);
            BusError::from(e)
        })
    }

    /// Blocking emit — awaits space in the channel.
    pub async fn emit_async<M: 'static + Any + Send + Sync>(&self, msg: M) -> Result<(), BusError> {
        let m: Arc<dyn Any + Send + Sync> = Arc::new(msg);
        self.sender.send(m).await.map_err(|e| {
            warn!("EventBus send error: {}", e);
            BusError::from(e)
        })
    }
}

pub struct ActorStart {
    pub bus: Bus,
}
pub struct ActorStop {}
pub struct EventbusStop {}

pub struct ActorTick {}

pub struct Eventbus {
    sender: mpsc::Sender<Arc<dyn Any + Send + Sync>>,
    receiver: mpsc::Receiver<Arc<dyn Any + Send + Sync>>,
    actors: Vec<Box<dyn ActorImpl>>,
}

impl Eventbus {
    pub fn new() -> Self {
        Self::with_capacity(DEFAULT_BUS_CAPACITY)
    }

    pub fn with_capacity(capacity: usize) -> Self {
        let (sender, receiver) = mpsc::channel::<Arc<dyn Any + Send + Sync>>(capacity);
        Eventbus {
            sender,
            receiver,
            actors: Vec::new(),
        }
    }

    pub fn register_actor(&mut self, actor: Box<dyn ActorImpl>) {
        self.actors.push(actor);
    }

    /// Emit a message (non-blocking). Returns an error if the channel is full or closed.
    pub fn emit<M: 'static + Any + Send + Sync>(&self, msg: M) -> Result<(), BusError> {
        self.sender
            .try_send(Arc::new(msg))
            .map_err(|e| {
                warn!("EventBus emit error: {}", e);
                BusError::from(e)
            })
    }

    /// Emit a message (async, awaits space). Returns an error if the channel is closed.
    pub async fn emit_async<M: 'static + Any + Send + Sync>(&self, msg: M) -> Result<(), BusError> {
        self.sender
            .send(Arc::new(msg))
            .await
            .map_err(|e| {
                warn!("EventBus emit_async error: {}", e);
                BusError::from(e)
            })
    }

    pub fn sender(&self) -> mpsc::Sender<Arc<dyn Any + Send + Sync>> {
        self.sender.clone()
    }

    pub fn bus(&self) -> Bus {
        Bus::new(self.sender.clone())
    }

    async fn recv(&mut self) -> Option<Arc<dyn Any + Send + Sync>> {
        self.receiver.recv().await
    }

    async fn start_all_actors(&mut self) -> Result<()> {
        for actor in &mut self.actors {
            let start_msg: Arc<dyn Any + Send + Sync> = Arc::new(ActorStart {
                bus: Bus::new(self.sender.clone()),
            });
            actor.handle(&start_msg).await;
        }
        Ok(())
    }

    async fn stop_all_actors(&mut self) -> Result<()> {
        for actor in &mut self.actors {
            let stop_msg: Arc<dyn Any + Send + Sync> = Arc::new(ActorStop {});
            actor.handle(&stop_msg).await;
        }
        Ok(())
    }

    /// Dispatch a message only to actors that declare interest (or all actors
    /// if the actor returned an empty interest slice).
    async fn handle_all_actors(&mut self, msg: &Arc<dyn Any + Send + Sync>) -> Result<()> {
        let msg_type_id = (*msg).type_id();
        for actor in &mut self.actors {
            let interests = actor.interests();
            if interests.is_empty() || interests.contains(&msg_type_id) {
                actor.handle(msg).await;
            }
        }
        Ok(())
    }

    pub async fn run(&mut self) -> Result<()> {
        self.start_all_actors().await?;
        while let Some(msg) = self.recv().await {
            self.handle_all_actors(&msg).await?;
            if msg.is::<EventbusStop>() {
                break;
            }
        }
        self.stop_all_actors().await?;
        Ok(())
    }
}

/// Call a closure when the message matches type T.
pub async fn on_message<T: 'static, F, Fut>(msg: &Arc<dyn Any + Send + Sync>, mut f: F)
where
    F: FnMut(&T) -> Fut,
    Fut: Future<Output = ()>,
{
    if msg.is::<T>() {
        let t: &T = msg.downcast_ref().unwrap();
        f(t).await;
    }
}

/// Call a closure on an `Option<T>` if present.
pub fn just<T, F>(t: Option<T>, mut f: F)
where
    F: FnMut(T),
{
    if let Some(value) = t {
        f(value);
    }
}

/// Downcast a message to type T if it matches.
pub fn as_message<T: 'static + Any + Send + Sync>(msg: &Arc<dyn Any + Send + Sync>) -> Option<&T> {
    if msg.is::<T>() {
        Some(msg.downcast_ref::<T>().unwrap())
    } else {
        None
    }
}
