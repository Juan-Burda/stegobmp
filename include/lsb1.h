#ifndef LSB1_H
#define LSB1_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsb1(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength);

void _lsb1(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength, const char numChannels);

void lsb1_extract(uint8_t* data, int width, int height, int bitCount, char* extractedPayload, size_t payloadLength);

void _lsb1_extract(uint8_t* data, int width, int height, int bitCount, char* extractedPayload, size_t payloadLength, const char numChannels);

#endif  // LSB1_H