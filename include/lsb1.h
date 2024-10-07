#ifndef LSB1_H
#define LSB1_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void lsb1(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength);

void lsb1_extract(uint8_t* data, int width, int height, int bitCount, char* extractedPayload, size_t payloadLength);

#endif // LSB1_H