// COBS / CRC-16 framing for Limeros serial protocol.
//
// Wire format (mirrors codec.cpp):
//   encode:  CBOR bytes → append CRC-16 (CCITT, big-endian) → COBS encode → append 0x00
//   decode:  strip trailing 0x00 → COBS decode → extract & verify last 2 bytes (CRC-16) → return remaining bytes

use generated::generated::Envelope;

// ── CRC-16 CCITT ───────────────────────────────────────────────────────────

/// CRC-16-CCITT (polynomial 0x1021, initial value 0xFFFF).
/// Mirrors crc16() in codec.cpp and hoverboard_actor.cpp.
pub fn crc16(data: &[u8]) -> u16 {
    let mut crc: u16 = 0xFFFF;
    for &byte in data {
        crc ^= (byte as u16) << 8;
        for _ in 0..8 {
            if crc & 0x8000 != 0 {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    crc
}

// ── COBS ───────────────────────────────────────────────────────────────────

/// COBS encode. Appends a 0x00 terminator.
/// Mirrors cobs_encode() in codec.cpp.
pub fn cobs_encode(input: &[u8]) -> Vec<u8> {
    let n = input.len();
    // Worst case: every 254 data bytes add 1 overhead byte + 1 for leading code + 1 for terminator
    let mut output = Vec::with_capacity(n + n / 254 + 2);
    let mut code_idx: usize = 0;
    output.push(0); // placeholder for first code byte
    let mut code: u8 = 1;

    for &byte in input {
        if byte == 0 {
            output[code_idx] = code;
            code_idx = output.len();
            output.push(0); // placeholder for next code
            code = 1;
        } else {
            output.push(byte);
            code += 1;
            if code == 0xFF {
                output[code_idx] = code;
                code_idx = output.len();
                output.push(0);
                code = 1;
            }
        }
    }
    output[code_idx] = code;
    output.push(0x00); // COBS terminator
    output
}

/// COBS decode. Input does NOT include the trailing 0x00 delimiter.
/// Mirrors cobs_decode() in codec.cpp and hoverboard_actor.cpp.
pub fn cobs_decode(input: &[u8]) -> anyhow::Result<Vec<u8>> {
    let mut output = Vec::with_capacity(input.len());
    let mut idx: usize = 0;

    while idx < input.len() {
        let code = input[idx];
        // A code of 1 means the next code-byte immediately follows (no zero was replaced)
        // We still advance past it. A code that would read past the buffer is an error.
        if idx + code as usize > input.len() && code != 1 {
            anyhow::bail!("COBS decode error: code={code} at offset {idx}, input len={}", input.len());
        }
        idx += 1;
        for _ in 1..code {
            output.push(input[idx]);
            idx += 1;
        }
        // Insert implicit zero unless this code block is max-length (0xFF),
        // or we've reached the end of input.
        if code != 0xFF && idx < input.len() {
            output.push(0);
        }
    }
    Ok(output)
}

// ── Frame encode / decode ──────────────────────────────────────────────────

/// Encode an Envelope into a framed serial packet.
/// Pipeline: CBOR → CRC-16 (big-endian appended) → COBS → 0x00 terminator.
pub fn encode_frame(envelope: &Envelope) -> anyhow::Result<Vec<u8>> {
    let cbor_bytes = envelope.to_bytes()?;

    // Append CRC-16 big-endian
    let crc = crc16(&cbor_bytes);
    let mut with_crc = cbor_bytes;
    with_crc.push((crc >> 8) as u8);
    with_crc.push((crc & 0xFF) as u8);

    // COBS encode (adds its own 0x00 terminator)
    let frame = cobs_encode(&with_crc);
    Ok(frame)
}

/// Decode a framed serial packet (bytes between two 0x00 delimiters) into an Envelope.
/// Pipeline: COBS decode → verify & strip CRC-16 → CBOR decode Envelope.
pub fn decode_frame(raw: &[u8]) -> anyhow::Result<Envelope> {
    // COBS decode
    let decoded = cobs_decode(raw)?;

    // Split off the last 2 bytes (CRC-16 big-endian)
    if decoded.len() < 2 {
        anyhow::bail!("Frame too short for CRC: {} byte(s)", decoded.len());
    }
    let (data, crc_bytes) = decoded.split_at(decoded.len() - 2);
    let received_crc = ((crc_bytes[0] as u16) << 8) | (crc_bytes[1] as u16);
    let computed_crc = crc16(data);

    if received_crc != computed_crc {
        anyhow::bail!(
            "CRC mismatch: received 0x{received_crc:04X}, computed 0x{computed_crc:04X}"
        );
    }

    // CBOR decode as Envelope
    let envelope = Envelope::from_bytes(data)?;
    Ok(envelope)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_crc16_known_vectors() {
        // CRC-16/CCITT-FALSE (polynomial 0x1021, init 0xFFFF, no reflection, no final XOR)
        // These values match the C++ codec.cpp implementation.
        assert_eq!(crc16(b""), 0xFFFF);
        assert_eq!(crc16(b"hello"), 0xD26E);
        assert_eq!(crc16(b"\x01\x02\x03"), 0xADAD);
        assert_eq!(crc16(b"123456789"), 0x29B1);
    }

    #[test]
    fn test_cobs_roundtrip() {
        let test_vectors: Vec<Vec<u8>> = vec![
            vec![],
            vec![0x01, 0x02, 0x03],
            vec![0x00],
            vec![0x00, 0x00],
            vec![0x01, 0x00, 0x02],
            (0..512).map(|i| (i % 256) as u8).collect(),
        ];

        for data in &test_vectors {
            let encoded = cobs_encode(data);
            // Strip trailing 0x00 for decode
            let without_term = &encoded[..encoded.len() - 1];
            let decoded = cobs_decode(without_term).expect("COBS decode failed");
            assert_eq!(
                &decoded, data,
                "COBS roundtrip failed for len={}",
                data.len()
            );
        }
    }

    #[test]
    fn test_frame_roundtrip() {
        let envelope = Envelope {
            src: Some(0xDEAD),
            dst: None,
            msg_type: Some(0xBEEF),
            request_id: None,
            instance_id: None,
            payload: Some(vec![0x42, 0x43, 0x44]),
        };

        let frame = encode_frame(&envelope).expect("encode");
        // strip trailing 0x00
        let raw = &frame[..frame.len() - 1];
        let decoded = decode_frame(raw).expect("decode");
        assert_eq!(decoded, envelope);
    }
}
