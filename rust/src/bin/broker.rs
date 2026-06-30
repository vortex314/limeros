//! UDP message broker — routes messages based on subscriptions,
//! maintains an FNV32↔string dictionary, and emits metrics.
//!
//! Usage: cargo run --bin broker [--dict dictionary.csv]

use anyhow::Result;
use log::info;
use tokio::signal;

use limeros::broker::{BrokerCore, BrokerHandler, fnv1a_32};
use limeros::{Msg, UdpNode, logger};

/// Default multicast address used by all Limeros devices.
const MULTICAST_ADDR: &str = "239.0.0.1:50000";
/// How often the dictionary is persisted to disk (seconds).
const DICT_SAVE_INTERVAL_SECS: u64 = 30;
/// How often BrokerEvent metrics are emitted (seconds).
const METRICS_INTERVAL_SECS: u64 = 10;

#[tokio::main]
async fn main() -> Result<()> {
    logger::init();
    info!("Starting UDP BROKER...");

    // ── Determine dictionary path ──────────────────────────────────
    let dict_path = std::env::args()
        .nth(1)
        .unwrap_or_else(|| "dictionary.csv".to_string());
    info!("Dictionary file: {}", dict_path);

    // ── Create the UdpNode (provides unicast + multicast transport) ─
    let node = UdpNode::new("broker", MULTICAST_ADDR).await?;
    let node_sender = node.sender();
    let my_id = "broker".to_string();

    // ── Create BrokerCore (dictionary + subscription engine) ───────
    let core = BrokerCore::new(&dict_path)?;
    let core_clone = core.clone();
    let dict_clone = core.dict.clone();

    // ── Register the broker handler ────────────────────────────────
    let handler = BrokerHandler {
        core: core.clone(),
        node_sender,
        my_id: fnv::FnvHash::default().hash(&my_id),
        my_name: my_id.clone(),
    };
    node.add_generic_handler(handler).await;
    info!("Broker handler registered");

    // ── Background: periodically save dictionary ───────────────────
    let dict_save_handle = tokio::spawn(async move {
        let mut interval = tokio::time::interval(std::time::Duration::from_secs(DICT_SAVE_INTERVAL_SECS));
        loop {
            interval.tick().await;
            if let Err(e) = dict_clone.save() {
                log::error!("Failed to save dictionary: {}", e);
            }
        }
    });

    // ── Background: periodically emit BrokerEvent metrics ──────────
    let core_metrics = core.clone();
    let node_metrics = node.clone();
    let metrics_handle = tokio::spawn(async move {
        let mut interval = tokio::time::interval(std::time::Duration::from_secs(METRICS_INTERVAL_SECS));
        loop {
            interval.tick().await;
            let endpoint_count = node_metrics.get_endpoints().await.len() as u64;
            let mut event = core_metrics.build_event(endpoint_count);
            event.subscription_count = Some(core_metrics.subscription_count().await as u32);

            info!(
                "BrokerEvent: routed={} dropped={} subs={} dict={} eps={} uptime={}s",
                event.messages_routed,
                event.messages_dropped,
                event.subscription_count,
                event.dictionary_size,
                event.endpoint_count,
                event.uptime_secs
            );

            // Emit as a UdpMessage for any interested subscribers
            let payload = event.to_bytes().map_err(|e| anyhow::anyhow!("Failed to serialize BrokerEvent: {}", e))?; 
            let msg = UdpMessage {
                src: Some(fnv1a_32(input)),
                dst: None, // broadcast
                msg_typ: Some(BrokerEvent::ID),
                payload: Some(payload),
            };
            if let Err(e) = node_metrics.sender().send(msg).await {
                log::error!("Failed to emit BrokerEvent: {}", e);
            }
        }
    });

    // ── Status printer ─────────────────────────────────────────────
    let node_status = node.clone();
    let core_status = core.clone();
    let status_handle = tokio::spawn(async move {
        let mut interval = tokio::time::interval(std::time::Duration::from_secs(15));
        loop {
            interval.tick().await;
            let eps = node_status.get_endpoints().await;
            info!(
                "Status: {} endpoints, {} subscriptions, {} dict entries",
                eps.len(),
                core_status.subscription_count().await,
                core_status.dict.len(),
            );
            for entry in eps.iter() {
                info!(
                    "  endpoint: {} @ {} (age: {}ms)",
                    entry.key(),
                    entry.value().addr,
                    entry.value().last_seen.elapsed().as_millis()
                );
            }
        }
    });

    // ── Wait for shutdown ──────────────────────────────────────────
    info!("Broker running. Press Ctrl+C to stop.");
    signal::ctrl_c().await?;
    info!("Shutting down...");

    // ── Final save ─────────────────────────────────────────────────
    if let Err(e) = core_clone.dict.save() {
        log::error!("Final dictionary save failed: {}", e);
    }
    info!("Dictionary saved on exit");

    // Clean up background tasks
    dict_save_handle.abort();
    metrics_handle.abort();
    status_handle.abort();

    info!("Broker stopped");
    Ok(())
}
