//! Multicast discovery actor — listens for multicast UDP packets and
//! forwards decoded EndpointAnnounce messages to the Router.

use std::net::{Ipv4Addr, SocketAddr};
use std::sync::Arc;

use dashmap::DashMap;
use generated::generated::{
    BROKER_ID, EndpointAnnounce, EndpointAnnounceReply, Envelope, opt_id_to_string,
};
use kameo::prelude::*;
use log::{info, warn};
use tokio::net::UdpSocket;

use crate::router::RouterActor;
use crate::udp::{AddUdpTarget, UdpActor};

// ── Messages ───────────────────────────────────────────────────────────────

/// Start listening on the given multicast group:port.
pub struct StartMulticast {
    pub group: Ipv4Addr,
    pub port: u16,
    pub bind_addr: Ipv4Addr,
}

/// Received a raw multicast UDP packet.
pub struct McastReceive {
    pub raw: Vec<u8>,
    pub addr: SocketAddr,
}

// ── Multicast actor ────────────────────────────────────────────────────────

#[derive(Actor)]
pub struct MulticastActor {
    udp_actor: ActorRef<UdpActor>,
    router: ActorRef<crate::router::RouterActor>,
    known_udp_endpoints: DashMap<u32, SocketAddr>,
}

impl MulticastActor {
    pub fn new(
        udp_actor: ActorRef<UdpActor>,
        router: ActorRef<crate::router::RouterActor>,
    ) -> Self {
        MulticastActor {
            udp_actor,
            router,
            known_udp_endpoints: DashMap::new(),
        }
    }

    async fn handle_packet(
        &mut self,
        packet: &[u8],
        addr: SocketAddr,
    ) -> anyhow::Result<()> {
        let env = Envelope::from_bytes(packet)?;
        let msg_type = env
            .msg_type
            .ok_or_else(|| anyhow::anyhow!("Envelope missing msg_type"))?;

        // Build Arc once — Router forwards it by bumping the refcount
        let envelope = Arc::new(env);

        self.udp_actor
            .tell(McastReceive {
                raw: packet.to_vec(),
                addr,
            })
            .await?;

        if msg_type == EndpointAnnounce::id() {
            self.handle_endpoint_announce(&*envelope, addr).await?;
        }
        Ok(())
    }

    async fn handle_endpoint_announce(
        &mut self,
        envelope: &Envelope,
        addr: SocketAddr,
    ) -> anyhow::Result<()> {
        let bytes = envelope
            .payload
            .as_ref()
            .ok_or_else(|| anyhow::anyhow!("Envelope missing payload"))?;
        let src = envelope
            .src
            .ok_or_else(|| anyhow::anyhow!("Envelope missing src"))?;
        let ep_announce = EndpointAnnounce::from_bytes(bytes)?;
        if let Some(new_addr) = self.known_udp_endpoints.insert(src, addr) {
            if new_addr != addr {
                info!(
                    "UDP endpoint {:?} changed address from {} to {} via multicast",
                    ep_announce.name, new_addr, addr
                );
                self.udp_actor
                    .tell(AddUdpTarget {
                        ep_info: ep_announce,
                        addr,
                    })
                    .await?;
            }
            // else ignore, same address as before
        } else {
            info!(
                "Discovered new UDP endpoint '{}' == {:?} at {} via multicast",
                opt_id_to_string(envelope.src),
                ep_announce.name,
                addr
            );
            self.udp_actor
                .tell(AddUdpTarget {
                    ep_info: ep_announce.clone(),
                    addr,
                })
                .await?;
        };
        let ep_announce_reply = EndpointAnnounceReply { utc: None };
        let env = Envelope {
            src: Some(BROKER_ID),
            dst: Some(src),
            msg_type: Some(EndpointAnnounceReply::id()),
            payload: Some(ep_announce_reply.to_bytes()?),
            request_id: None,
            instance_id: None,
        };
        self.udp_actor
            .tell(McastReceive {
                raw: env.to_bytes()?,
                addr,
            })
            .await?;
        Ok(())
    }
}


impl Message<StartMulticast> for MulticastActor {
    type Reply = ();

    async fn handle(&mut self, msg: StartMulticast, ctx: &mut Context<Self, Self::Reply>) {
        let bind = SocketAddr::new(msg.bind_addr.into(), msg.port);
        let group = SocketAddr::new(msg.group.into(), msg.port);
        let actor_ref = ctx.actor_ref().clone();

        tokio::spawn(async move {
            let socket = match UdpSocket::bind(bind).await {
                Ok(s) => s,
                Err(e) => {
                    warn!("Failed to bind multicast socket on {}: {}", bind, e);
                    return;
                }
            };

            // Join multicast group
            let std_socket = socket2::Socket::from(socket.into_std().unwrap());
            match std_socket.join_multicast_v4(&msg.group, &msg.bind_addr) {
                Ok(()) => {}
                Err(e) => {
                    warn!("Failed to join multicast group {}: {}", msg.group, e);
                    return;
                }
            }
            let socket = UdpSocket::from_std(std_socket.into()).unwrap();

            info!("Multicast actor listening on {} (group {})", bind, group);

            let mut buf = vec![0u8; 64 * 1024];
            loop {
                match socket.recv_from(&mut buf).await {
                    Ok((len, addr)) => {
                        let packet = buf[..len].to_vec();
                        let _ = actor_ref.tell(McastReceive { raw: packet, addr }).await;
                    }
                    Err(e) => {
                        warn!("Multicast recv error: {}", e);
                        break;
                    }
                }
            }
        });
    }
}

impl Message<McastReceive> for MulticastActor {
    type Reply = ();

    async fn handle(&mut self, msg: McastReceive, ctx: &mut Context<Self, Self::Reply>) {
        if let Err(e) = self.handle_packet(&msg.raw, msg.addr).await {
            warn!("Failed to handle multicast packet: {}", e);
        }
    }
}
