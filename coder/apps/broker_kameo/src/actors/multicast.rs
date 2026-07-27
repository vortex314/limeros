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

use crate::actors::router::{EndpointAddress, IncomingEnvelope};

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
    router: ActorRef<crate::actors::router::Router>,
    udp_actor: ActorRef<crate::actors::udp::UdpActor>,
    known_udp_endpoints: DashMap<u32, SocketAddr>,
}

impl MulticastActor {
    pub fn new(
        router: ActorRef<crate::actors::router::Router>,
        udp_actor: ActorRef<crate::actors::udp::UdpActor>,
    ) -> Self {
        MulticastActor {
            router,
            udp_actor,
            known_udp_endpoints: DashMap::new(),
        }
    }

    async fn handle_packet(&mut self, packet: &[u8], addr: SocketAddr) -> anyhow::Result<()> {
        let env = Envelope::from_bytes(packet)?;
        let msg_type = env
            .msg_type
            .ok_or_else(|| anyhow::anyhow!("Envelope missing msg_type"))?;

        // Build Arc once — Router forwards it by bumping the refcount
        let envelope = Arc::new(env);
        let raw = Arc::new(packet.to_vec());

        self.router
            .tell(IncomingEnvelope {
                envelope: envelope.clone(),
                raw: raw.clone(),
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
                let envelop_copy = envelope.clone();
                let ep_update = crate::actors::router::EndpointUpdate {
                    envelope: envelop_copy,
                    ep_addr: EndpointAddress::UdpEndpoint(self.udp_actor.clone(), addr),
                    ep_announce,
                };
                // Use tell since we're in sync context — spawn to make it async
                self.router.tell(ep_update).await?;
            }
            // else ignore, same address as before
        } else {
            info!(
                "Discovered new UDP endpoint '{}' == {:?} at {} via multicast",
                opt_id_to_string(envelope.src),
                ep_announce.name,
                addr
            );
            let ep_update = crate::actors::router::EndpointUpdate {
                envelope: envelope.clone(),
                ep_addr: EndpointAddress::UdpEndpoint(self.udp_actor.clone(), addr),
                ep_announce,
            };
            self.router.tell(ep_update).await?;
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
        let raw = env.to_bytes()?;
        self.router
            .tell(IncomingEnvelope {
                envelope: Arc::new(env),
                raw: Arc::new(raw),
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

    async fn handle(&mut self, msg: McastReceive, _ctx: &mut Context<Self, Self::Reply>) {
        if let Err(e) = self.handle_packet(&msg.raw, msg.addr).await {
            warn!("Failed to handle multicast packet: {}", e);
        }
    }
}
