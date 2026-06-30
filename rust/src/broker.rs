//! UDP message broker with FNV32 dictionary, subscription-based routing,
//! and bidirectional string↔id translation.
//!
//! The broker intercepts every UdpMessage flowing through the node,
//! matches them against registered subscriptions (by src / dst / msg_type,
//! either as human-readable strings or compact FNV32 hashes), and forwards
//! matching messages to subscribers — in binary JSON format for id-based
//! subscriptions or in human-readable string format for string-based ones.
//!
//! A persistent FNV32↔string dictionary (CSV file) enables logging,
//! metrics, and string-format forwarding.

use std::fs;
use std::io::{BufRead, BufReader, Write};
use std::path::Path;
use std::sync::atomic::{AtomicU64, Ordering};
use std::sync::Arc;
use std::time::Instant;

use anyhow::Result;
use dashmap::DashMap;
use log::{debug, error, info, warn};
use serde::{Deserialize, Serialize};
use tokio::sync::RwLock;

use crate::msgs::MSG_IDS;
use crate::msgs::{
    BrokerEvent, BrokerRequest, BrokerSubscribeRequest, BrokerUnsubscribeRequest, UdpMessage,
};
use crate::{Msg, UdpMessageHandler};

// ── FNV-1a 32-bit hash ──────────────────────────────────────────────

/// FNV-1a 32-bit hash — deterministic, fast, good distribution for short
/// identifiers (device names, message type strings).
pub fn fnv1a_32(input: &str) -> u32 {
    let mut hash: u32 = 0x811C9DC5;
    for &byte in input.as_bytes() {
        hash ^= u32::from(byte);
        hash = hash.wrapping_mul(0x01000193);
    }
    hash
}

// ── FNV32 ↔ String Dictionary ───────────────────────────────────────

/// Bidirectional dictionary mapping FNV32 hashes to human-readable strings.
/// Persisted to a CSV file (`hash,word` per line).
pub struct Dictionary {
    /// FNV32 hash → original string.
    map: DashMap<u32, String>,
    /// Reverse lookup: string → FNV32 hash.
    reverse: DashMap<String, u32>,
    /// File path for persistence.
    file_path: String,
}

impl Dictionary {
    /// Create or load a dictionary from `file_path`.
    /// If the file exists, entries are loaded; otherwise an empty dictionary is created.
    pub fn new(file_path: &str) -> Result<Self> {
        let dict = Self {
            map: DashMap::new(),
            reverse: DashMap::new(),
            file_path: file_path.to_string(),
        };

        for (word, hash) in MSG_IDS {
            dict.reverse.insert(word.to_string().clone(), *hash);
        }
        if Path::new(file_path).exists() {
            dict.load()?;
        }
        Ok(dict)
    }

    /// Load entries from the CSV file.
    fn load(&self) -> Result<()> {
        let f = fs::File::open(&self.file_path)?;
        let reader = BufReader::new(f);
        for line in reader.lines() {
            let line = line?;
            let line = line.trim();
            if line.is_empty() || line.starts_with('#') {
                continue;
            }
            let mut parts = line.splitn(2, ',');
            let hash_str = parts.next().unwrap_or("").trim();
            let word = parts.next().unwrap_or("").trim();
            if hash_str.is_empty() || word.is_empty() {
                continue;
            }
            if let Ok(hash) = u32::from_str_radix(hash_str, 10) {
                self.map.insert(hash, word.to_string());
                self.reverse.insert(word.to_string(), hash);
            }
        }
        info!(
            "Dictionary loaded {} entries from {}",
            self.map.len(),
            self.file_path
        );
        Ok(())
    }

    /// Save all entries to the CSV file (atomic write via temp file).
    pub fn save(&self) -> Result<()> {
        let tmp_path = format!("{}.tmp", self.file_path);
        let mut f = fs::File::create(&tmp_path)?;
        writeln!(f, "# FNV32 hash, word")?;
        for entry in self.map.iter() {
            writeln!(f, "{},{}", entry.key(), entry.value())?;
        }
        f.flush()?;
        fs::rename(&tmp_path, &self.file_path)?;
        debug!(
            "Dictionary saved {} entries to {}",
            self.map.len(),
            self.file_path
        );
        Ok(())
    }

    /// Register a word. Returns its FNV32 hash.
    /// If the word is already known, the existing hash is returned.
    pub fn add(&self, word: &str) -> u32 {
        if let Some(hash) = self.reverse.get(word) {
            return *hash;
        }
        let hash = fnv1a_32(word);
        self.map.insert(hash, word.to_string());
        self.reverse.insert(word.to_string(), hash);
        hash
    }

    /// Look up the string for a given FNV32 hash.
    pub fn get(&self, hash: u32) -> Option<String> {
        self.map.get(&hash).map(|r| r.value().clone())
    }

    /// Resolve a string to its FNV32 hash (computes on-the-fly if not registered).
    pub fn resolve(&self, word: &str) -> u32 {
        if let Some(hash) = self.reverse.get(word) {
            return *hash;
        }
        let hash = fnv1a_32(word);
        self.map.insert(hash, word.to_string());
        self.reverse.insert(word.to_string(), hash);
        hash
    }

    /// Number of entries.
    pub fn len(&self) -> usize {
        self.map.len()
    }
}

// ── Subscription ────────────────────────────────────────────────────

/// A single subscription filter registered by a subscriber.
#[derive(Debug, Clone)]
pub struct BrokerSubscription {
    /// Unique subscription id (for removal).
    pub id: u64,

    /// Pre-computed FNV32 hashes for fast id-based matching.
    pub src: Option<u32>,
    pub dst: Option<u32>,
    pub msg_typ: Option<u32>,
    /// Who subscribed.
    pub subscriber_id: u32,
    /// If true, forward matched messages as human-readable strings.
    /// If false, forward the original binary JSON.
    pub string_format: bool,
}

impl BrokerSubscription {
    /// Check whether a UdpMessage matches this subscription filter.
    pub fn matches(&self, msg: &UdpMessage) -> bool {
        // Id-based filters: hash the incoming fields and compare
        if self.src.is_some() && msg.src != self.src {
            return false;
        }
        if self.dst.is_some() && msg.dst != self.dst {
            return false;
        }
        if self.msg_typ.is_some() && msg.msg_typ != self.msg_typ {
            return false;
        }

        true
    }

    pub fn new(
        id: u64,
        src: Option<u32>,
        dst: Option<u32>,
        msg_typ: Option<u32>,
        subscriber_id: u32,
        string_format: bool,
    ) -> Self {
        Self {
            id,
            src,
            dst,
            msg_typ,
            subscriber_id,
            string_format,
        }
    }
}

// ── Broker Core ─────────────────────────────────────────────────────

/// The broker engine: dictionary, subscription table, routing logic, metrics.
pub struct BrokerCore {
    /// FNV32 ↔ string dictionary.
    pub dict: Arc<Dictionary>,
    /// All active subscriptions (RwLock for read-heavy access).
    subscriptions: RwLock<Vec<BrokerSubscription>>,
    /// Monotonically increasing subscription ID counter.
    next_sub_id: AtomicU64,
    /// Counters.
    messages_routed: AtomicU64,
    messages_dropped: AtomicU64,
    /// When the broker started.
    start_time: Instant,
}

impl BrokerCore {
    pub fn new(dict_path: &str) -> Result<Arc<Self>> {
        let dict = Arc::new(Dictionary::new(dict_path)?);
        Ok(Arc::new(Self {
            dict,
            subscriptions: RwLock::new(Vec::new()),
            next_sub_id: AtomicU64::new(1),
            messages_routed: AtomicU64::new(0),
            messages_dropped: AtomicU64::new(0),
            start_time: Instant::now(),
        }))
    }

    /// Add a subscription. Returns the subscription id.
    pub async fn add_subscription(
        &self,
        src_hash: Option<u32>,
        dst_hash: Option<u32>,
        msg_type_hash: Option<u32>,
        subscriber_id: u32,
        string_format: bool,
    ) -> u64 {
        let id = self.next_sub_id.fetch_add(1, Ordering::Relaxed);
        let sub = BrokerSubscription {
            id,
            src: src_hash,
            dst: dst_hash,
            msg_typ: msg_type_hash,
            subscriber_id,
            string_format,
        };
        self.subscriptions.write().await.push(sub);
        debug!("Added subscription id={}", id);
        id
    }

    /// Remove all subscriptions for a given subscriber.
    pub async fn remove_subscriptions(&self, subscriber: u32) -> usize {
        let mut subs = self.subscriptions.write().await;
        let before = subs.len();
        subs.retain(|s| s.subscriber_id != subscriber);
        let removed = before - subs.len();
        debug!("Removed {} subscriptions for {}", removed, subscriber);
        removed
    }

    /// Process a single incoming UdpMessage: match against subscriptions
    /// and return a list of (subscriber_id, UdpMessage) to forward.
    pub async fn route(&self, msg: &UdpMessage) -> Vec<(u32, UdpMessage)> {
        let subs = self.subscriptions.read().await;
        let mut forwards = Vec::new();

        for sub in subs.iter() {
            if !sub.matches(msg) {
                continue;
            }
            let out_msg = if sub.string_format {
                // Convert to human-readable string format
                UdpMessage {
                    src: msg.src,
                    dst: msg.dst,
                    msg_typ: msg.msg_typ,
                    req_id: msg.req_id,
                    payload: Some(msg.to_readable().into_bytes()),
                }
            } else {
                // Forward the original binary JSON
                msg.clone()
            };

            forwards.push((sub.subscriber_id, out_msg));
        }

        if forwards.is_empty() {
            self.messages_dropped.fetch_add(1, Ordering::Relaxed);
        } else {
            self.messages_routed
                .fetch_add(forwards.len() as u64, Ordering::Relaxed);
        }

        forwards
    }

    /// Number of active subscriptions.
    pub async fn subscription_count(&self) -> usize {
        self.subscriptions.read().await.len()
    }

    /// Build a BrokerEvent snapshot.
    pub fn build_event(&self, endpoint_count: u64) -> BrokerEvent {
        BrokerEvent {
            messages_routed: Some(self.messages_routed.load(Ordering::Relaxed)),
            messages_dropped: Some(self.messages_dropped.load(Ordering::Relaxed)),
            subscription_count: Some(0), // filled by caller if needed
            dictionary_size: Some(self.dict.len() as u32),
            endpoint_count: Some(endpoint_count as u32),
            uptime_secs: Some(self.start_time.elapsed().as_secs()),
        }
    }
}

// ── UdpMessageHandler adapter ───────────────────────────────────────

/// Wraps BrokerCore so it can be registered as a UdpMessageHandler on a UdpNode.
/// Also holds a reference to the UdpNode's sender for forwarding.
pub struct BrokerHandler {
    pub core: Arc<BrokerCore>,
    pub node_sender: tokio::sync::mpsc::Sender<UdpMessage>,
    /// My own id string (e.g. "broker").
    pub my_id: u32,
    pub my_name: String,
}

#[async_trait::async_trait]
impl UdpMessageHandler for BrokerHandler {
    async fn handle(&self, udp_message: &UdpMessage) -> anyhow::Result<()> {
        // Check if this is a BrokerSubscribeRequest addressed to us
        if udp_message.msg_typ == Some(BrokerSubscribeRequest::ID)
            || udp_message.msg_typ == Some(BrokerUnsubscribeRequest::ID)
        {
            self.handle_generic_broker_request(udp_message).await?;
            // Don't route BrokerSubscribeRequests to subscribers
            return Ok(());
        }

        // Don't route our own events to prevent loops
        if udp_message.dst == Some(self.my_id) {
            return Ok(());
        }

        // Route the message to matching subscribers
        let forwards = self.core.route(udp_message).await;
        for (dst, out_msg) in forwards {
            let mut fwd = out_msg;
            fwd.dst = Some(dst);
            if let Err(e) = self.node_sender.send(fwd).await {
                error!("Failed to forward routed message: {}", e);
            }
        }

        Ok(())
    }
}

impl BrokerHandler {
    async fn handle_subscribe_request(&self, udp_message: &UdpMessage) -> Result<()> {
        let payload = udp_message
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("no payload"))?;
        let subscribe_request = BrokerSubscribeRequest::from_bytes(payload)?;
        // Handle the subscription request
        let mut string_format = false;
        let mut src_filter = None;
        if let Some(src_str) = subscribe_request.src_str {
            src_filter = Some(self.core.dict.resolve(&src_str));
            string_format = true;
        } else if let Some(src_hash) = subscribe_request.src_id {
            src_filter = Some(src_hash);
        }
        let mut dst_filter = None;
        if let Some(dst_str) = subscribe_request.dst_str {
            dst_filter = Some(self.core.dict.resolve(&dst_str));
            string_format = true;
        } else if let Some(dst_hash) = subscribe_request.dst_id {
            dst_filter = Some(dst_hash);
        }
        let mut msg_type_filter = None;
        if let Some(msg_type_str) = subscribe_request.msg_typ_str {
            msg_type_filter = Some(self.core.dict.resolve(&msg_type_str));
        } else if let Some(msg_type_hash) = subscribe_request.msg_typ_id {
            msg_type_filter = Some(msg_type_hash);
        }

        let broker_subscription = BrokerSubscription::new(
            self.core.next_sub_id.fetch_add(1, Ordering::Relaxed),
            src_filter,
            dst_filter,
            msg_type_filter,
            udp_message.src.unwrap_or_default(),
            string_format, // string format
        );
        info!("Added subscription: {:?}", broker_subscription);

        self.core
            .subscriptions
            .write()
            .await
            .push(broker_subscription);
        Ok(())
    }

    async fn handle_unsubscribe_request(&self, udp_message: &UdpMessage) -> Result<()> {
        let subscriber_id = udp_message
            .src
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("no source"))?
            .clone();
        let removed_count = self.core.remove_subscriptions(subscriber_id).await;
        info!(
            "Removed {} subscriptions for {}",
            removed_count, subscriber_id
        );
        Ok(())
    }

    async fn handle_broker_request(&self, udp_message: &UdpMessage) -> Result<()> {
        let payload = udp_message.payload.as_ref().ok_or_else(|| anyhow::anyhow!("no payload"))?;
        let broker_request = BrokerRequest::from_bytes(payload)?;
        if broker_request.add_word.is_some() {
            let word = broker_request.add_word.unwrap();
            let hash = self.core.dict.add(&word);
            info!("Dictionary: added '{}' → hash {}", word, hash);
        };
        Ok(())
    }
    /// Handle a BrokerSubscribeRequest message (add word, subscribe, unsubscribe, config).
    async fn handle_generic_broker_request(&self, msg: &UdpMessage) -> Result<()> {
        match msg.msg_typ {
            Some(BrokerSubscribeRequest::ID) => self.handle_subscribe_request(msg).await,
            Some(BrokerUnsubscribeRequest::ID) => self.handle_unsubscribe_request(msg).await,
            Some(BrokerRequest::ID) => self.handle_broker_request(msg).await,
            _ => {
                // forward to subscribers if it's a BrokerEvent or other known type
                warn!("Unknown Broker request type: {:?}", msg.msg_typ);
                Ok(())
            }
        };
        Ok(())
    }
}
