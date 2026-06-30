//! ping_replyer — subscribes to PingRequest and replies with PingReply.
//!
//! Run alongside the broker, then start ping_sender.
//!
//!   cargo run --bin ping_replyer

use std::sync::Arc;

use anyhow::Result;
use log::info;
use tokio::sync::Mutex;

use limeros::msgs::{Msg, PingReply, PingRequest, TypedMessage, UdpMessage};
use limeros::{logger, UdpNode};

const MULTICAST_ADDR: &str = "239.0.0.1:50000";

#[tokio::main]
async fn main() -> Result<()> {
    logger::init();
    info!("Starting PING REPLYER...");

    let node = UdpNode::new("ping_replyer", MULTICAST_ADDR).await?;
    let sender = node.sender();
    let my_id = "ping_replyer".to_string();
    let reply_count = Arc::new(Mutex::new(0u64));

    // Typed handler: on every PingRequest, echo back a PingReply.
    let rc = reply_count.clone();
    node.on::<PingRequest, _, _>(move |_src, req: PingRequest| {
        let s = sender.clone();
        let mid = my_id.clone();
        let rc = rc.clone();
        async move {
            let reply = PingReply {
                seq: req.seq,
                orig_send_time_ns: req.send_time_ns,
            };
            let payload = reply.to_value().unwrap();
            let msg = UdpMessage {
                src: Some(mid),
                dst: Some("ping_sender".to_string()),
                typ: Some(PingReply::MSG_TYPE.to_string()),
                msg: Some(payload),
            };
            let _ = s.send(msg).await;

            let count = {
                let mut c = rc.lock().await;
                *c += 1;
                *c
            };
            if count % 1000 == 0 {
                info!("Replied to {} pings", count);
            }
        }
    });

    info!("Ping replyer running — waiting for PingRequest messages...");
    tokio::signal::ctrl_c().await?;
    info!("Replyer shutting down");
    Ok(())
}
