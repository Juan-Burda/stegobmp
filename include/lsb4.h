#ifndef LSB4_H
#define LSB4_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void lsb4(uint8_t* data, int width, int height, int bitCount, const char* message, size_t maxMessageLength);

void lsb4_extract(uint8_t* data, int width, int height, int bitCount, char* extractedMessage, size_t maxMessageLength);

#endif // LSB4_H