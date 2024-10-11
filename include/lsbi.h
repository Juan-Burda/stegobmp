#ifndef LSBI_H
#define LSBI_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsbi(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength);

void lsbi_extract(uint8_t* data, int width, int height, int bitCount, char* extractedPayload, size_t payloadLength);

void lsbi_invert(uint8_t* data, int width, int height, int bitCount, size_t payloadLength, int offset);

#endif  // LSBI_H