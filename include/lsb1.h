#ifndef LSB1_H
#define LSB1_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsb1(uint8_t* data, int width, int height, int bit_count, const char* payload, size_t payload_length);

void _lsb1(uint8_t* data, int width, int height, int bit_count, const char* payload, size_t payload_length, const char num_channels);

void lsb1_extract(uint8_t* data, int width, int height, int bit_count, char* extractedPayload, size_t payload_length);

void _lsb1_extract(uint8_t* data, int width, int height, int bit_count, char* extractedPayload, size_t payload_length, const char num_channels);

#endif  // LSB1_H