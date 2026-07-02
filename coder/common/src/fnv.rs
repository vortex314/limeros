pub fn fnv1a_32(s: &str) -> u32 {
    // 1. Initialize the hash with the 32-bit offset basis
    let mut hash: u32 = 2166136261;
    
    // 2. Process each byte of the string
    for byte in s.as_bytes() {
        // XOR the lower 8 bits of the hash with the byte
        hash ^= *byte as u32;
        // Multiply by the FNV prime (wrapping_mul prevents overflow panics in debug mode)
        hash = hash.wrapping_mul(16777619);
    }
    
    hash
}