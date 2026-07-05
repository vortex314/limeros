use anyhow::{Context, Result};
use log::{debug, info};
use std::net::{Ipv4Addr, SocketAddr, SocketAddrV4};
use tokio::net::UdpSocket;
pub struct UdpNode {
    #[allow(dead_code)]
    id: u32,
    udp_addr: Option<SocketAddr>,
    mc_addr: SocketAddr,
    udp_socket: Option<UdpSocket>,
    mc_socket: Option<UdpSocket>,
}

impl UdpNode {
    pub fn new(id: u32, mc_addr: SocketAddr) -> Self {
        info!(
            "Creating UdpNode with id {} and multicast address {}",
            id, mc_addr
        );
        UdpNode {
            id,
            udp_addr: None,
            mc_addr,
            udp_socket: None,
            mc_socket: None,
        }
    }

    pub async fn bind_unicast(&mut self) -> Result<()> {
        // create UDP socket and bind to any port on all interfaces
        let socket = UdpSocket::bind("0.0.0.0:0")
            .await
            .with_context(|| format!("failed to bind UDP socket to 0.0.0.0:0"))?;

        self.udp_addr = Some(socket.local_addr()?);
        info!("UDP socket bound to {}", self.udp_addr.unwrap());
        self.udp_socket = Some(socket);
        Ok(())
    }

    pub async fn bind_unicast_with_port(&mut self,port:u16) -> Result<()> {
        // create UDP socket and bind to any port on all interfaces
        let socket = UdpSocket::bind(("0.0.0.0", port))
            .await
            .with_context(|| format!("failed to bind UDP socket to 0.0.0.0:{}", port))?;

        self.udp_addr = Some(socket.local_addr()?);
        info!("UDP socket bound to {}", self.udp_addr.unwrap());
        self.udp_socket = Some(socket);
        Ok(())
    }

    pub async fn bind_multicast(&mut self) -> Result<()> {
        // create multicast socket and bind to the specified address
        let listen_addr = SocketAddrV4::new(Ipv4Addr::UNSPECIFIED, self.mc_addr.port());
        let mc_socket = UdpSocket::bind(listen_addr)
            .await
            .with_context(|| format!("failed to bind multicast UDP socket to {}", listen_addr))?;
        mc_socket.join_multicast_v4(
            self.mc_addr.ip().to_string().parse::<Ipv4Addr>()?,
            Ipv4Addr::UNSPECIFIED,
        )?;
        info!(
            "Joined multicast group {} on {}",
            self.mc_addr,
            mc_socket.local_addr()?
        );
        self.mc_socket = Some(mc_socket);
        Ok(())
    }

    pub async fn send_multicast(&self, data: &[u8]) -> Result<()> {
        debug!("Sending multicast message to {}: [{} bytes]", self.mc_addr, data.len());
        if let Some(socket) = &self.udp_socket {
            socket
                .send_to(data, self.mc_addr)
                .await
                .with_context(|| format!("failed to send multicast to {}", self.mc_addr))?;
        }
        Ok(())
    }

    pub async fn send_unicast(&self, data: &[u8], addr: SocketAddr) -> Result<()> {
        debug!("Sending unicast message to {}: [{}]", addr, data.len());
        if let Some(socket) = &self.udp_socket {
            socket
                .send_to(data, addr)
                .await
                .with_context(|| format!("failed to send UDP to {}", addr))?;
        }
        Ok(())
    }

    pub async fn receive_unicast(&self, buf: &mut [u8]) -> Result<(usize, SocketAddr)> {
        if let Some(socket) = &self.udp_socket {
            let (len, addr) = socket
                .recv_from(buf)
                .await
                .with_context(|| "failed to receive UDP packet")?;
            Ok((len, addr))
        } else {
            Err(anyhow::anyhow!("UDP socket is not bound"))
        }
    }

    pub async fn receive_multicast(&self, buf: &mut [u8]) -> Result<(usize, SocketAddr)> {
        if let Some(socket) = &self.mc_socket {
            debug!(
                "Waiting to receive multicast packet on {} {} ....",
                self.mc_addr,
                socket.local_addr()?
            );
            let (len, addr) = socket
                .recv_from(buf)
                .await
                .with_context(|| "failed to receive multicast packet")?;
            debug!("Received multicast packet from {}: [{} bytes]", addr, len);
            Ok((len, addr))
        } else {
            Err(anyhow::anyhow!("Multicast socket is not joined"))
        }
    }

    pub async fn close(&mut self) -> Result<()> {
        if self.udp_socket.is_some() {
            info!("UDP socket closed");
            self.udp_socket = None;
        }
        if self.mc_socket.is_some() {
            info!("Multicast socket closed");
            self.mc_socket = None;
        }
        Ok(())
    }
}
