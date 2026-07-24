//! Multicast discovery actor — listens for multicast UDP packets and
//! forwards decoded EndpointAnnounce messages to the Router.

use std::net::{Ipv4Addr, SocketAddr};

use actix::prelude::*;
use dashmap::DashMap;
use generated::generated::{
    BROKER_ID, EndpointAnnounce, EndpointAnnounceReply, Envelope, opt_id_to_string,
};
use log::{info, warn};

use tokio::net::UdpSocket;

use crate::actors::{
    router::{EndpointAddress, IncomingEnvelope},
    udp,
};

// ── Messages ───────────────────────────────────────────────────────────────

/// Start listening on the given multicast group:port.
#[derive(Message, Debug)]
#[rtype(result = "anyhow::Result<()>")]
pub struct StartMulticast {
    pub group: Ipv4Addr,
    pub port: u16,
    pub bind_addr: Ipv4Addr,
}

/// Start listening on the given multicast group:port.
#[derive(Message, Debug)]
#[rtype(result = "anyhow::Result<()>")]
pub struct UdpReceive {
    pub raw: Vec<u8>,
    pub addr: SocketAddr,
}

// ── Multicast actor ────────────────────────────────────────────────────────

pub struct MulticastActor {
    router: Addr<crate::actors::router::Router>,
    udp_actor: Addr<crate::actors::udp::UdpActor>,
    known_udp_endpoints: DashMap<u32, SocketAddr>, // src -> addr
}

impl MulticastActor {
    pub fn new(
        router: Addr<crate::actors::router::Router>,
        udp_actor: Addr<crate::actors::udp::UdpActor>,
    ) -> Self {
        MulticastActor {
            router,
            udp_actor,
            known_udp_endpoints: DashMap::new(),
        }
    }

    fn handle_packet(&mut self, packet: &[u8], addr: SocketAddr) -> anyhow::Result<()> {
        let envelope = Envelope::from_bytes(packet)?;
        let src = envelope
            .src
            .ok_or_else(|| anyhow::anyhow!("Envelope missing src"))?;
        let bytes = envelope
            .payload
            .as_deref()
            .ok_or_else(|| anyhow::anyhow!("Envelope missing payload"))?;
        let ep_announce = EndpointAnnounce::from_bytes(bytes)?;
        if let Some(new_addr) = self.known_udp_endpoints.insert(src, addr) {
            if new_addr != addr {
                info!(
                    "UDP endpoint {:?} changed address from {} to {} via multicast",
                    ep_announce.name, new_addr, addr
                );
                let ep_update = crate::actors::router::EndpointUpdate {
                    envelope,
                    ep_addr: EndpointAddress::UdpEndpoint(self.udp_actor.clone(), addr),
                    ep_announce,
                };

                self.router.do_send(ep_update);
            }
        } else {
            info!(
                "Discovered new UDP endpoint '{}' == {:?} at {} via multicast",
                opt_id_to_string(envelope.src),
                ep_announce.name,
                addr
            );
            let ep_update = crate::actors::router::EndpointUpdate {
                envelope,
                ep_addr: EndpointAddress::UdpEndpoint(self.udp_actor.clone(), addr),
                ep_announce,
            };
            self.router.do_send(ep_update);
        }
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
        self.router.do_send(IncomingEnvelope { envelope: env, raw });
        Result::Ok(())
    }
}

impl Actor for MulticastActor {
    type Context = Context<Self>;
}

impl Handler<StartMulticast> for MulticastActor {
    type Result = anyhow::Result<()>;

    fn handle(&mut self, msg: StartMulticast, _ctx: &mut Context<Self>) -> Self::Result {
        let bind = SocketAddr::new(msg.bind_addr.into(), msg.port);
        let group = SocketAddr::new(msg.group.into(), msg.port);
        let my_addr = _ctx.address();

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
                        let packet = &buf[..len];
                        my_addr.do_send(UdpReceive {
                            raw: packet.to_vec(),
                            addr,
                        });
                    }
                    Err(e) => {
                        warn!("Multicast recv error: {}", e);
                    }
                }
            }
        });

        Ok(())
    }
}

impl Handler<UdpReceive> for MulticastActor {
    type Result = anyhow::Result<()>;

    fn handle(&mut self, msg: UdpReceive, _ctx: &mut Context<Self>) -> Self::Result {
        self.handle_packet(&msg.raw, msg.addr)
    }
}
