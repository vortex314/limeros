#pragma once

#include <stddef.h>
#include <stdint.h>

size_t cobs_encode(const uint8_t *data,
                   size_t len,
                   uint8_t *out,
                   size_t outCap);

int cobs_decode(const uint8_t *frame,
                size_t len,
                uint8_t *out,
                size_t outCap);
