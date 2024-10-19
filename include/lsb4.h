#ifndef LSB4_H
#define LSB4_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsb4(uint8_t* data, int width, int height, int bit_count, const uint8_t* payload, size_t payload_length);

void lsb4_extract(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, size_t payload_length);

#endif  // LSB4_H