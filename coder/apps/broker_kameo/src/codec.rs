// COBS / CRC-16 framing for Limeros serial protocol.
//
// Wire format (mirrors codec.cpp):
//   encode:  CBOR bytes → append CRC-16 (CCITT, big-endian) → COBS encode → append 0x00
//   decode:  strip trailing 0x00 → COBS decode → extract & verify last 2 bytes (CRC-16) → return remaining bytes

/// CRC-16-CCITT (polynomial 0x1021, initial value 0xFFFF).
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

/// COBS encode. Appends a 0x00 terminator.
#[allow(dead_code)]
pub fn cobs_encode(input: &[u8]) -> Vec<u8> {
    let n = input.len();
    let mut output = Vec::with_capacity(n + n / 254 + 2);
    let mut code_idx: usize = 0;
    output.push(0);
    let mut code: u8 = 1;

    for &byte in input {
        if byte == 0 {
            output[code_idx] = code;
            code_idx = output.len();
            output.push(0);
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
    output.push(0x00);
    output
}

/// COBS decode. Input does NOT include the trailing 0x00 delimiter.
pub fn cobs_decode(input: &[u8]) -> anyhow::Result<Vec<u8>> {
    let mut output = Vec::with_capacity(input.len());
    let mut idx: usize = 0;

    while idx < input.len() {
        let code = input[idx];
        if idx + code as usize > input.len() && code != 1 {
            anyhow::bail!("COBS decode error: code={code} at offset {idx}, input len={}", input.len());
        }
        idx += 1;
        for _ in 1..code {
            output.push(input[idx]);
            idx += 1;
        }
        if code != 0xFF && idx < input.len() {
            output.push(0);
        }
    }
    Ok(output)
}

/// Encode an Envelope into a framed serial packet.
#[allow(dead_code)]
pub fn encode_frame(buffer: &[u8]) -> anyhow::Result<Vec<u8>> {
    let crc = crc16(buffer);
    let mut with_crc = buffer.to_vec();
    with_crc.push((crc >> 8) as u8);
    with_crc.push((crc & 0xFF) as u8);
    let frame = cobs_encode(&with_crc);
    Ok(frame)
}

/// Decode a framed serial packet into an Envelope.
pub fn decode_frame(raw: &[u8]) -> anyhow::Result<Vec<u8>> {
    let decoded = cobs_decode(raw)?;
    if decoded.len() < 2 {
        anyhow::bail!("Frame too short for CRC: {} byte(s)", decoded.len());
    }
    let (data, crc_bytes) = decoded.split_at(decoded.len() - 2);
    let received_crc = ((crc_bytes[0] as u16) << 8) | (crc_bytes[1] as u16);
    let computed_crc = crc16(data);
    if received_crc != computed_crc {
        anyhow::bail!("CRC mismatch: received 0x{received_crc:04X}, computed 0x{computed_crc:04X}");
    }
    Ok(data.to_vec())
}
