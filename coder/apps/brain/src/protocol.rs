//! Shared framing protocol for the LM4F120H5QR 8-port UART multiplexer.
//!
//! This module contains the wire protocol used between the host and the
//! firmware over USB CDC ACM. It is shared by the CLI tool (`main.rs`), the
//! actor-based application (`mux_app.rs`), and the actors in [`crate::actors`].
//!
//! Protocol frame (both directions):
//!   [SYNC=0xA5] [LEN_LO] [LEN_HI] [PORT] [PAYLOAD...] [CRC8]
//!
//! PORT byte:
//!   bit 7:    direction (0 = host->uart, 1 = uart->host)
//!   bits 6:3: reserved (0)
//!   bits 2:0: UART number (0-7)
//!
//! CRC-8: Dallas/Maxim 1-Wire, computed over SYNC .. last payload byte.

/// Frame sync byte.
pub const PROTO_SYNC: u8 = 0xA5;

/// Maximum payload size in bytes for a single frame.
pub const MAX_PAYLOAD: usize = 1024;

// CRC-8 Dallas/Maxim 1-Wire (same table as firmware)
const CRC8_TABLE: [u8; 256] = [
    0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83, 0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
    0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E, 0x5F, 0x01, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC,
    0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0, 0xE1, 0xBF, 0x5D, 0x03, 0x80, 0xDE, 0x3C, 0x62,
    0xBE, 0xE0, 0x02, 0x5C, 0xDF, 0x81, 0x63, 0x3D, 0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF,
    0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5, 0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x07,
    0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x06, 0x58, 0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A,
    0x65, 0x3B, 0xD9, 0x87, 0x04, 0x5A, 0xB8, 0xE6, 0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24,
    0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B, 0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x05, 0xE7, 0xB9,
    0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0x0F, 0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD,
    0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92, 0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0x0E, 0x50,
    0xAF, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C, 0x6D, 0x33, 0xD1, 0x8F, 0x0C, 0x52, 0xB0, 0xEE,
    0x32, 0x6C, 0x8E, 0xD0, 0x53, 0x0D, 0xEF, 0xB1, 0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73,
    0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49, 0x08, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B,
    0x57, 0x09, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4, 0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16,
    0xE9, 0xB7, 0x55, 0x0B, 0x88, 0xD6, 0x34, 0x6A, 0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8,
    0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7, 0xB6, 0xE8, 0x0A, 0x54, 0xD7, 0x89, 0x6B, 0x35,
];

/// Compute the CRC-8 Dallas/Maxim 1-Wire checksum over `data`.
pub fn crc8(data: &[u8]) -> u8 {
    let mut crc: u8 = 0;
    for &byte in data {
        crc = CRC8_TABLE[(crc ^ byte) as usize];
    }
    crc
}

/// A decoded frame: raw bytes plus the UART id they belong to.
#[derive(Debug, Clone)]
pub struct Frame {
    /// UART port number (0-7).
    pub uart: u8,
    /// Payload bytes.
    pub data: Vec<u8>,
}

/// Builds a wire frame for sending `data` to `uart`.
///
/// Panics if `uart` is >= 8 or the payload exceeds [`MAX_PAYLOAD`].
pub fn build_frame(uart: u8, data: &[u8]) -> Vec<u8> {
    assert!(uart < 8, "UART number must be 0-7");
    assert!(data.len() <= MAX_PAYLOAD, "Payload too large");

    let len = data.len() as u16;
    let mut frame = Vec::with_capacity(4 + data.len() + 1);

    let port = uart & 0x07; // bit 7 = 0 => host->uart

    frame.push(PROTO_SYNC);
    frame.push((len & 0xFF) as u8);
    frame.push(((len >> 8) & 0xFF) as u8);
    frame.push(port);
    frame.extend_from_slice(data);

    let csum = crc8(&frame);
    frame.push(csum);

    frame
}

/// State machine for parsing framed protocol bytes.
pub struct FrameParser {
    synced: bool,
    len: u16,
    port: u8,
    pos: u16,
    crc: u8,
    buf: [u8; MAX_PAYLOAD],
}

impl FrameParser {
    /// Creates a new, unsynchronised parser.
    pub fn new() -> Self {
        Self {
            synced: false,
            len: 0,
            port: 0,
            pos: 0,
            crc: 0,
            buf: [0u8; MAX_PAYLOAD],
        }
    }

    /// Feed one byte. Returns `Some(Frame)` when a valid frame completes.
    pub fn feed(&mut self, byte: u8) -> Option<Frame> {
        if !self.synced {
            if byte == PROTO_SYNC {
                self.synced = true;
                self.crc = CRC8_TABLE[byte as usize];
                self.pos = 0;
            }
            return None;
        }

        // Accumulate the CRC over the header + payload only (positions
        // 0 .. 3+len-1), never over the trailing CRC byte itself.
        if (self.pos as usize) < 3 + self.len as usize {
            self.crc = CRC8_TABLE[(self.crc ^ byte) as usize];
        }

        match self.pos {
            0 => {
                self.len = byte as u16;
            }
            1 => {
                self.len |= (byte as u16) << 8;
                if self.len as usize > MAX_PAYLOAD {
                    self.synced = false;
                    return None;
                }
            }
            2 => {
                self.port = byte;
                if (byte & 0x07) >= 8 {
                    self.synced = false;
                    return None;
                }
            }
            _ => {
                // Payload occupies positions 3 .. 3+len-1.
                let idx = self.pos as usize - 3;
                if idx < self.len as usize {
                    self.buf[idx] = byte;
                } else {
                    // CRC byte at position 3+len.
                    self.synced = false;
                    if byte == self.crc {
                        let uart = self.port & 0x07;
                        let data = self.buf[..self.len as usize].to_vec();
                        return Some(Frame { uart, data });
                    }
                    return None;
                }
            }
        }
        self.pos += 1;
        None
    }
}

impl Default for FrameParser {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    /// A frame produced by `build_frame` must parse back byte-for-byte. This
    /// regresses the off-by-one where the first payload byte was dropped and
    /// the CRC never matched.
    #[test]
    fn roundtrip_preserves_payload_and_uart() {
        let cases: [&[u8]; 4] = [
            b"HELLO",
            b"",
            &[0u8; 100],
            &[0xA5u8, 0x00, 0xFF, 0x5A],
        ];
        for data in cases {
            let frame = build_frame(3, data);
            let mut parser = FrameParser::new();
            let mut out = None;
            for &byte in &frame {
                if let Some(f) = parser.feed(byte) {
                    out = Some(f);
                }
            }
            let f = out.expect("valid frame should be accepted");
            assert_eq!(f.uart, 3);
            assert_eq!(f.data, data);
        }
    }

    /// A corrupted CRC byte must reject the frame.
    #[test]
    fn rejects_bad_crc() {
        let mut frame = build_frame(1, b"payload");
        *frame.last_mut().unwrap() ^= 0xFF;
        let mut parser = FrameParser::new();
        let mut out = None;
        for &byte in &frame {
            if let Some(f) = parser.feed(byte) {
                out = Some(f);
            }
        }
        assert!(out.is_none(), "corrupt frame must be rejected");
    }

    /// Leading garbage must not stop the parser from resyncing on SYNC.
    #[test]
    fn resyncs_after_leading_garbage() {
        let frame = build_frame(2, b"xyz");
        let mut parser = FrameParser::new();
        let mut out = None;
        for &byte in [0x00u8, 0x11, 0xFF].iter().chain(frame.iter()) {
            if let Some(f) = parser.feed(byte) {
                out = Some(f);
            }
        }
        let f = out.expect("parser should resync after garbage");
        assert_eq!(f.uart, 2);
        assert_eq!(f.data, b"xyz");
    }
}
