#include "../include/lsb1.h"
#include <byte-utils.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp-headers.h"
#include "../include/bmp-utils.h"
#include "../include/constants/bmp.h"
#include "../include/file-utils.h"

void lsb1(uint8_t* data, int total_pixels, const uint8_t* payload, size_t payload_length) {
    if (payload_length * BITS_PER_BYTE > total_pixels * BITS_PER_PIXEL) {
        printf("Error embedding payload: payload too long.\n");
        exit(1);
    }

    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = payload[payload_index];
    for (int byte_index = 0; payload_index < payload_length; byte_index++) {
        int bit_to_embed = get_i_bit(current_char, BITS_PER_BYTE - 1 - payload_bit_index);
        data[byte_index] = (data[byte_index] & 0xFE) | bit_to_embed;
        payload_bit_index++;

        if (payload_bit_index == BITS_PER_BYTE) {
            payload_index++;
            if (payload_index >= payload_length) {
                return;
            }
            current_char = payload[payload_index];
            payload_bit_index = 0;
        }
    }
}

void lsb1_extract(uint8_t* data, int total_pixels, uint8_t* extracted_payload, size_t payload_length) {
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;
    for (uint32_t data_index = 0; payload_index < payload_length; data_index++) {
        uint8_t bit = get_i_bit(data[data_index], 0);

        current_char <<= 1;
        current_char |= bit;
        payload_bit_index++;

        if (payload_bit_index % BITS_PER_BYTE == 0) {
            extracted_payload[payload_index++] = current_char;
            payload_bit_index = 0;
            current_char = 0;
        }
    }
}

void lsb1_extract_extension(uint8_t* data, int total_pixels, uint8_t* extracted_payload) {
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;
    for (uint32_t data_index = 0;; data_index++) {
        uint8_t bit = get_i_bit(data[data_index], 0);

        current_char <<= 1;
        current_char |= bit;
        payload_bit_index++;

        if (payload_bit_index % BITS_PER_BYTE == 0) {
            extracted_payload[payload_index++] = current_char;

            if (current_char == '\0') {
                return;
            }

            payload_bit_index = 0;
            current_char = 0;
        }
    }
}