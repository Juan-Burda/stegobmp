#ifndef LSBI_H
#define LSBI_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsbi(uint8_t* data, int width, int height, int bit_count, const uint8_t* payload, size_t payload_length);

void lsbi_extract(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, size_t payload_length);

void lsbi_invert(uint8_t* data, int width, int height, int bit_count);

#endif  // LSBI_H