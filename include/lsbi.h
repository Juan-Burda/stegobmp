#ifndef LSBI_H
#define LSBI_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsbi(uint8_t* data, int width, int height, int bit_count, const char* payload, size_t payload_length);

void lsbi_extract(uint8_t* data, int width, int height, int bit_count, char* extracted_payload, size_t payload_length);

void lsbi_invert(uint8_t* data, int width, int height, int bit_count, size_t payload_length, int offset);

#endif  // LSBI_H