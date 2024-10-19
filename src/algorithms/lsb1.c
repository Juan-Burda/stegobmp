#include "../include/lsb1.h"
#include <byte-utils.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp-headers.h"
#include "../include/bmp-utils.h"
#include "../include/constants/bmp.h"
#include "../include/file-utils.h"

void lsb1(uint8_t* data, int width, int height, int bit_count, const uint8_t* payload, size_t payload_length) {
    _lsb1(data, width, height, bit_count, payload, payload_length, BYTES_PER_PIXEL);
}

void _lsb1(uint8_t* data, int width, int height, int bit_count, const uint8_t* payload, size_t payload_length, const char num_channels) {
    if (bit_count != BITS_PER_PIXEL) {
        printf("This function only supports 24-bit BMP files.\n");
        exit(1);
    }

    if (payload_length * BITS_PER_BYTE > width * height * BITS_PER_PIXEL) {
        printf("Error embedding payload: payload too long.\n");
        exit(1);
    }

    int row_size = CALCULATE_ROW_SIZE(width);
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = payload[payload_index];

    for (int byte_index = 0; byte_index < (payload_length * BITS_PER_BYTE); byte_index++) {
        int bit_to_embed = (current_char >> payload_bit_index) & 1;
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

void lsb1_extract(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, size_t payload_length) {
    _lsb1_extract(data, width, height, bit_count, extracted_payload, payload_length, BYTES_PER_PIXEL);
}

void _lsb1_extract(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, size_t payload_length, const char num_channels) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

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

void _lsb1_extract_extension(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, const char num_channels) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int row_size = CALCULATE_ROW_SIZE(width);
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;

    for (int byte_index = 0;; byte_index++) {
        uint8_t byte_value = data[byte_index];
        int bit = byte_value & 1;

        current_char |= (bit << payload_bit_index);
        payload_bit_index++;

        if (payload_bit_index == BITS_PER_BYTE) {
            extracted_payload[payload_index++] = current_char;
            if (current_char == '\0') {
                return;
            }
            payload_bit_index = 0;
            current_char = 0;
        }
    }
}