use anyhow::Result;
use dashmap::DashMap;
use log::{debug, error, info};
use serde::Serialize;
use std::net::SocketAddr;
use std::sync::Arc;
use std::time::Duration;
use tokio::net::UdpSocket;
use tokio::sync::{mpsc, Mutex};
use tokio::task::JoinHandle;

pub mod broker;
pub mod eventbus;
pub mod logger;
pub mod msgs;

use crate::broker::fnv1a_32;
use crate::msgs::DeviceAliveEvent;
pub use crate::msgs::{ Msg, UdpMessage};

pub mod scout;
pub use scout::Endpoint;
pub use scout::Scout;

#[async_trait::async_trait]
pub trait UdpMessageHandler: Send + Sync {
    async fn handle(&self, udp_message: &UdpMessage) -> anyhow::Result<()>;
}

struct HandlerWrapper<T, F, Fut>
where
    T: Msg,
    F: Fn(u32, T) -> Fut + Send + Sync,
    Fut: std::future::Future<Output = ()> + Send,
{
    callback: F,
    _phantom: std::marker::PhantomData<(T, Fut)>,
}

impl<T, F, Fut> HandlerWrapper<T, F, Fut>
where
    T: Msg + for<'de> serde::Deserialize<'de>,
    F: Fn(u32, T) -> Fut + Send + Sync + 'static,
    Fut: std::future::Future<Output = ()> + Send + 'static,
{
    fn new(callback: F) -> Self {
        HandlerWrapper {
            callback,
            _phantom: std::marker::PhantomData,
        }
    }
}

#[async_trait::async_trait]
impl<T, F, Fut> UdpMessageHandler for HandlerWrapper<T, F, Fut>
where
    T: Msg + for<'de> serde::Deserialize<'de> + Send + Sync,
    F: Fn(u32, T) -> Fut + Send + Sync,
    Fut: std::future::Future<Output = ()> + Send + Sync,
{
    async fn handle(&self, udp_message: &UdpMessage) -> anyhow::Result<()> {
        let src = udp_message.src.unwrap_or_default();
        if let Some(ref msg_data) = udp_message.payload {
            if let Ok(msg) = T::from_bytes(msg_data) {
                (self.callback)(src, msg).await;
            }
        }
        Ok(())
    }
}

pub struct UdpNode {
    multicast_addr: SocketAddr,
    multicast_scout: Arc<Scout>,
    unicast_socket: Arc<UdpSocket>,
    my_id: Arc<Mutex<String>>,
    my_subscriptions: Arc<Mutex<Vec<String>>>,
    handlers: Arc<DashMap<u32, Box<dyn UdpMessageHandler>>>,
    /// Maps Peer ID -> (Data Address, Last Seen)
    tx_queue_sender: mpsc::Sender<UdpMessage>,
    tx_queue_receiver: Arc<Mutex<mpsc::Receiver<UdpMessage>>>,
    generic_handlers: Arc<Mutex<Vec<Box<dyn UdpMessageHandler>>>>,
    generic_senders: Arc<Mutex<Vec<mpsc::Sender<UdpMessage>>>>,
    tasks: Mutex<Vec<JoinHandle<()>>>,
}

impl UdpNode {
    pub async fn new(id: &str, multicast_addr: &str) -> Result<Arc<Self>> {
        let multicast_addr = multicast_addr
            .parse::<SocketAddr>()
            .map_err(|e| anyhow::anyhow!("Invalid multicast address: {}", e))?;
        let multicast_scout = Scout::new(multicast_addr.to_string()).await?;
        let unicast_socket = Arc::new(UdpSocket::bind("0.0.0.0:0").await?); // give random port
        info!("Unicast socket bound to {}", unicast_socket.local_addr()?);
        let (tx_queue_sender, tx_queue_receiver) = mpsc::channel::<UdpMessage>(100);

        let node = Arc::new(Self {
            multicast_addr,
            multicast_scout,
            unicast_socket: unicast_socket.clone(),
            my_id: Arc::new(Mutex::new(id.to_string())),
            my_subscriptions: Arc::new(Mutex::new(vec![])),

            handlers: Arc::new(DashMap::new()),
            tx_queue_sender,
            tx_queue_receiver: Arc::new(Mutex::new(tx_queue_receiver)),
            generic_handlers: Arc::new(Mutex::new(Vec::new())),
            generic_senders: Arc::new(Mutex::new(Vec::new())),
            tasks: Mutex::new(Vec::new()),
        });

        *node.tasks.lock().await = UdpNode::start_tasks(node.clone());

        Ok(node)
    }

    fn start_tasks(node: Arc<Self>) -> Vec<JoinHandle<()>> {
        vec![
            UdpNode::start_unicast_sender(node.clone()),
            UdpNode::start_unicast_receiver(node.clone()),
            Scout::start(node.multicast_scout.clone()),
            UdpNode::start_multicast_sender(node.clone()),
        ]
    }

    /*
       send my AliveEvent periodically via multicast
    */
    fn start_multicast_sender(node: Arc<Self>) -> JoinHandle<()> {
        // To be implemented if needed
        let my_id = node.my_id.clone();
        let my_subscriptions = node.my_subscriptions.clone();
        let unicast_socket = node.unicast_socket.clone();
        tokio::spawn(async move {
            let mut interval = tokio::time::interval(Duration::from_secs(1));
            // Target address for multicast

            loop {
                interval.tick().await;

                // 1. Send Heartbeat
                let mut alive = DeviceAliveEvent::default();
                let my_id = my_id.lock().await;
                let my_subscriptions = my_subscriptions.lock().await;
                alive.subscribes = Vec::new(); // For now, we don't send subscriptions in the AliveEvent
        //        alive.src = fnv1a_32(&my_id.clone());

                if let Ok(payload) = alive.to_bytes() {
                    let udp_message = UdpMessage {
                        src: Some(fnv1a_32(&my_id)),
                        dst: None,
                        msg_typ: Some(DeviceAliveEvent::ID),
                        req_id: None,
                        payload: Some(payload),
                    };
                    // We send this via our standard TX queue, aiming at the Multicast Addr
                    debug!("MC Send AliveEvent {:?}", alive);
                    let data = udp_message.to_bytes().unwrap_or_default();
                    let _ = unicast_socket.send_to(&data, &node.multicast_addr).await;
                }
            }
        })
    }

    fn start_unicast_sender(node: Arc<Self>) -> JoinHandle<()> {
        let send_socket = node.unicast_socket.clone();
        let tx_queue_receiver = node.tx_queue_receiver.clone();
        tokio::spawn(async move {
            while let Some(udp_message) = tx_queue_receiver.lock().await.recv().await {
                let target = if let Some(dst_endpoint) = &udp_message.dst {
                    if let Some(addr) = node.multicast_scout.endpoint_to_addr(*dst_endpoint) {
                        debug!(
                            "UC Send {:?} to {:?} @ {:?}",
                            udp_message.msg_typ, dst_endpoint, addr
                        );

                        addr
                    } else {
                        info!("Unknown endpoint: {}", dst_endpoint);
                        continue;
                    }
                } else {
                    info!("No destination endpoint specified");
                    continue;
                };
                if let Ok(data) = udp_message.to_bytes() {
                    if let Err(e) = send_socket.send_to(&data, target).await {
                        error!("Send error: {}", e);
                    }
                }
            }
        })
    }

    fn start_unicast_receiver(node: Arc<Self>) -> JoinHandle<()> {
        let generic_handlers = node.generic_handlers.clone();
        let recv_socket = node.unicast_socket.clone();
        let typed_handlers = node.handlers.clone();
        tokio::spawn(async move {
            let mut buf = [0u8; 65535];
            loop {
                if let Ok((len, _addr)) = recv_socket.recv_from(&mut buf).await {
                    let data: Vec<u8> = buf[..len].to_vec();
                    if let Ok(packet) = UdpMessage::from_bytes(&data) {
                        // ── typed handlers (on::<T>) ─────────────────
                        if let Some(ref typ) = packet.msg_typ {
                            if typed_handlers.contains_key(typ) {
                                let h = typed_handlers.clone();
                                let t = typ.clone();
                                let pkt = packet.clone();
                                tokio::spawn(async move {
                                    if let Some(handler) = h.get(&t) {
                                        if let Err(e) = handler.handle(&pkt).await {
                                            error!("Typed handler error: {}", e);
                                        }
                                    }
                                });
                            }
                        }

                        // ── generic handlers (UdpMessageHandler) ────
                        let gen_handlers = generic_handlers.clone();
                        let handlers_guard = gen_handlers.lock().await;
                        for i in 0..handlers_guard.len() {
                            let packet = packet.clone();
                            let gen_handlers_clone = gen_handlers.clone();
                            tokio::spawn(async move {
                                let handlers = gen_handlers_clone.lock().await;
                                if let Some(handler) = handlers.get(i) {
                                    if let Err(e) = handler.handle(&packet).await {
                                        error!("Generic handler error: {}", e);
                                    }
                                }
                            });
                        }
                        drop(handlers_guard);
                        for sender in node.generic_senders.lock().await.iter() {
                            let p = packet.clone();
                            let _ = sender.send(p).await;
                        }
                    } else {
                        error!("Failed to deserialize UdpMessage from {} bytes", len);
                    }
                }
            }
        })
    }

    pub async fn send_event<T>(&self, event: T)
    where
        T: Msg ,
    {
        let udp_message = UdpMessage {
            src: Some(fnv1a_32(&self.my_id.lock().await.clone())),
            dst: Some(fnv1a_32("broker")),
            msg_typ: Some(T::ID),
            req_id: None,
            payload: Some(event.to_bytes().unwrap()),
        };
        self.tx_queue_sender.send(udp_message).await.unwrap()
    }

    pub async fn send_msg_to<T>(&self, dst: &str, msg: T)
    where
        T: Msg ,
    {
        let udp_message = UdpMessage {
            src: Some(fnv1a_32(&self.my_id.lock().await.clone())),
            dst: Some(fnv1a_32(dst)),
            msg_typ: Some(T::ID),
            req_id: None,
            payload: Some(msg.to_bytes().unwrap()),
        };
        self.tx_queue_sender.send(udp_message).await.unwrap()
    }
    // add a generic U
    pub fn add_handler<H>(&self, typ: u32, handler: H)
    where
        H: UdpMessageHandler + 'static,
    {
        self.handlers.insert(typ, Box::new(handler));
    }

    pub fn add_sender(&self, sender: mpsc::Sender<UdpMessage>) {
        let mut senders = self.generic_senders.try_lock();
        if let Ok(ref mut senders) = senders {
            senders.push(sender);
        }
    }

    pub fn on<T, F, Fut>(&self, callback: F)
    where
        T: Msg + for<'de> serde::Deserialize<'de> + Send + Sync + 'static,
        F: Fn(u32, T) -> Fut + Send + Sync + 'static,
        Fut: std::future::Future<Output = ()> + Send + Sync + 'static,
    {
        self.handlers.insert(
            T::ID,
            Box::new(HandlerWrapper::new(callback)),
        );
    }

    pub async fn send_typed<T:  Msg>(
        &self,
        src: &str,
        dest_id: &str,
        msg: T,
    ) -> anyhow::Result<()> {
        let payload = msg.to_bytes()?;

        let packet = UdpMessage {
            src: Some(fnv1a_32(src)),
            dst: Some(fnv1a_32(dest_id)),
            msg_typ: Some(T::ID),
            req_id: None,
            payload: Some(payload),
        };

        self.tx_queue_sender
            .send(packet)
            .await
            .map_err(|_| anyhow::anyhow!("Queue Closed"))?;
        Ok(())
    }

    pub fn sender(&self) -> mpsc::Sender<UdpMessage> {
        self.tx_queue_sender.clone()
    }

    pub async fn add_generic_handler<H>(&self, handler: H)
    where
        H: UdpMessageHandler + 'static,
    {
        self.generic_handlers.lock().await.push(Box::new(handler));
    }

    pub async fn get_subscriptions(&self) -> Arc<DashMap<String, Vec<scout::Subscription>>> {
        self.multicast_scout.subscriptions.clone()
    }

    pub async fn get_endpoints(&self) -> Arc<DashMap<u32, Endpoint>> {
        self.multicast_scout.endpoints.clone()
    }

    pub async fn add_subscription(&self, subscription: &str) {
        let mut my_subscriptions = self.my_subscriptions.lock().await;
        if !my_subscriptions.contains(&subscription.to_string()) {
            my_subscriptions.push(subscription.to_string());
        }
    }

    pub async fn display_subscriptions(&self) {
        let subs = self.multicast_scout.subscriptions.clone();
        for entry in subs.iter() {
            debug!("Subscription: {} -> {:?}", entry.key(), entry.value());
        }
    }
}
