#ifndef LSB4_H
#define LSB4_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void lsb4(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength);

void lsb4_extract(uint8_t* data, int width, int height, int bitCount, char* extractedPayload, size_t payloadLength);

#endif // LSB4_H