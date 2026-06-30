//! ping_sender — sends batches of PingRequest and measures E2E latency.
//!
//! Requires the broker and ping_replyer to be running first.
//!
//!   cargo run --bin ping_sender [-- count N]
//!
//! Sends `N` PingRequest messages (default 5000) and prints latency
//! statistics every 1000 samples.

use std::collections::HashMap;
use std::sync::Arc;
use std::time::{Duration, Instant};

use anyhow::Result;
use log::info;
use tokio::sync::Mutex;
use tokio::time::timeout;

use limeros::msgs::{Msg, PingReply, PingRequest, TypedMessage, UdpMessage};
use limeros::{logger, UdpNode};

const MULTICAST_ADDR: &str = "239.0.0.1:50000";

/// Accumulated latency stats.
#[derive(Debug, Default)]
struct LatencyStats {
    count: u64,
    sum_ns: u128,
    min_ns: u128,
    max_ns: u128,
}

impl LatencyStats {
    fn record(&mut self, latency_ns: u128) {
        self.count += 1;
        self.sum_ns += latency_ns;
        if self.count == 1 || latency_ns < self.min_ns {
            self.min_ns = latency_ns;
        }
        if latency_ns > self.max_ns {
            self.max_ns = latency_ns;
        }
    }

    fn avg_us(&self) -> f64 {
        if self.count == 0 {
            return 0.0;
        }
        (self.sum_ns as f64) / (self.count as f64) / 1000.0
    }

    fn print(&self, label: &str) {
        println!(
            "{}  count={}  min={:.1}µs  avg={:.1}µs  max={:.1}µs",
            label,
            self.count,
            self.min_ns as f64 / 1000.0,
            self.avg_us(),
            self.max_ns as f64 / 1000.0
        );
    }
}

#[tokio::main]
async fn main() -> Result<()> {
    logger::init();

    let total_count: u64 = std::env::args()
        .nth(1)
        .and_then(|s| s.parse().ok())
        .unwrap_or(5000);

    info!("Starting PING SENDER — will send {} pings", total_count);

    let node = UdpNode::new("ping_sender", MULTICAST_ADDR).await?;
    let sender = node.sender();

    // Shared state: pendings maps seq → send Instant, stats accumulates results.
    let pendings: Arc<Mutex<HashMap<u64, Instant>>> = Arc::new(Mutex::new(HashMap::new()));
    let stats: Arc<Mutex<LatencyStats>> = Arc::new(Mutex::new(LatencyStats::default()));
    let done: Arc<Mutex<bool>> = Arc::new(Mutex::new(false));
    let received_count: Arc<Mutex<u64>> = Arc::new(Mutex::new(0));

    // Subscribe to PingReply to measure RTT.
    let p = pendings.clone();
    let s = stats.clone();
    let d = done.clone();
    let rc = received_count.clone();
    node.on::<PingReply, _, _>(move |_src, reply: PingReply| {
        let pendings = p.clone();
        let stats = s.clone();
        let done = d.clone();
        let rc = rc.clone();
        async move {
            let mut pend_guard = pendings.lock().await;
            if let Some(send_instant) = pend_guard.remove(&reply.seq) {
                let rtt = send_instant.elapsed().as_nanos();
                drop(pend_guard);

                let mut stats_guard = stats.lock().await;
                stats_guard.record(rtt);
                let count = stats_guard.count;
                drop(stats_guard);

                if count % 1000 == 0 && count > 0 {
                    let sg = stats.lock().await;
                    sg.print(&format!("[sample {}]", count));
                }

                // Check if we're done
                let current_count = {
                    let mut rc_guard = rc.lock().await;
                    *rc_guard += 1;
                    *rc_guard
                };
                if current_count >= total_count {
                    let sg = stats.lock().await;
                    sg.print("FINAL");
                    let pending = pendings.lock().await.len();
                    let pct = pending as f64 / total_count as f64 * 100.0;
                    println!(
                        "Sent: {}  Received: {}  Lost: {} ({:.1}%)",
                        total_count,
                        sg.count,
                        pending,
                        pct
                    );
                    let mut done_guard = done.lock().await;
                    *done_guard = true;
                }
            }
        }
    });

    // Small delay to let discovery settle.
    tokio::time::sleep(Duration::from_millis(500)).await;

    // Send all PingRequest messages as fast as possible.
    let start = Instant::now();
    for seq in 0..total_count {
        let send_instant = Instant::now();

        pendings.lock().await.insert(seq, send_instant);

        let req = PingRequest {
            seq,
            send_time_ns: 0, // not used for RTT; we measure with Instant
        };
        let payload = req.to_value().unwrap();
        let msg = UdpMessage {
            src: Some("ping_sender".to_string()),
            dst: Some("ping_replyer".to_string()),
            typ: Some(PingRequest::MSG_TYPE.to_string()),
            msg: Some(payload),
        };

        if let Err(e) = sender.send(msg).await {
            log::error!("Failed to send ping {}: {}", seq, e);
        }
    }

    let send_duration = start.elapsed();
    info!(
        "All {} pings sent in {:.1}ms ({:.0} pings/s)",
        total_count,
        send_duration.as_secs_f64() * 1000.0,
        total_count as f64 / send_duration.as_secs_f64()
    );

    // Wait for all replies or timeout.
    let wait_result = timeout(Duration::from_secs(30), async {
        loop {
            if *done.lock().await {
                break;
            }
            tokio::time::sleep(Duration::from_millis(100)).await;
        }
    })
    .await;

    if wait_result.is_err() {
        let stats = stats.lock().await;
        let pending = pendings.lock().await.len();
        stats.print("TIMEOUT");
        println!(
            "Sent: {}  Received: {}  Pending: {} ({:.1}% loss)",
            total_count,
            stats.count,
            pending,
            pending as f64 / total_count as f64 * 100.0
        );
    }

    info!("Ping sender done");
    tokio::signal::ctrl_c().await?;
    Ok(())
}
