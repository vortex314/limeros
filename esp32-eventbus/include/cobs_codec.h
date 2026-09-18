#pragma once

#include <stddef.h>
#include <stdint.h>

size_t cobs_encode(const uint8_t *data,
                   size_t len,
                   uint8_t *out,
                   size_t outCap);

uint16_t crc16_ccitt(const uint8_t *data, size_t len);

int cobs_decode(const uint8_t *frame,
                size_t len,
                uint8_t *out,
                size_t outCap);


// ============================================================================
// Combined COBS + CRC Codec - Single buffer, minimal memory
// ============================================================================

class COBSCodec {
public:
    static constexpr size_t MAX_DATA_SIZE = 253;  // 255 - 2 CRC bytes
    static constexpr size_t MAX_ENCODED_SIZE = 255 + 1;  // +1 for final zero
    
    // Encode with CRC in place (uses same buffer)
 //   static bool encode_inplace(uint8_t* buffer, size_t data_len, size_t& encoded_len) {
 
};
