#ifndef LSB1_H
#define LSB1_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsb1(uint8_t* data, int total_pixels, const uint8_t* payload, size_t payload_length);

void lsb1_extract(uint8_t* data, int total_pixels, uint8_t* extracted_payload, size_t payload_length);

void lsb1_extract_extension(uint8_t* data, int total_pixels, uint8_t* extractedPayload);

#endif  // LSB1_H