#ifndef LSBI_H
#define LSBI_H

#include <color-utils.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsbi(uint8_t* data, int total_pixels, const uint8_t* payload, size_t payload_length);

void lsbi_extract(uint8_t* data, int total_pixels, uint8_t* extracted_payload, size_t payload_length);

void lsbi_invert(uint8_t* data, int total_pixels);

void lsbi_extract_extension(uint8_t* data, int total_pixels, uint8_t* extracted_payload, Color start_color);

#endif  // LSBI_H