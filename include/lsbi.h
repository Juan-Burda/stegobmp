#ifndef LSBI_H
#define LSBI_H

#include <color-utils.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsbi(uint8_t* data, int width, int height, int bit_count, const uint8_t* payload, size_t payload_length);

void lsbi_extract(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, size_t payload_length);

void lsbi_invert(uint8_t* data, int width, int height, int bit_count);

void _lsbi_extract_extension(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, const char num_channels, Color start_color);

#endif  // LSBI_H