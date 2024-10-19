#include <bmp-headers.h>
#include <bmp-utils.h>
#include <byte-utils.h>
#include <constants/bmp.h>
#include <file-utils.h>
#include <lsb4.h>
#include <stdio.h>
#include <stdlib.h>

void lsb4(uint8_t* data, int width, int height, int bit_count, const uint8_t* payload, size_t payload_length) {
    if (bit_count != BITS_PER_PIXEL) {
        printf("This function only supports 24-bit BMP files.\n");
        exit(1);
    }

    if (payload_length * (BITS_PER_BYTE / 4) > width * height * BITS_PER_PIXEL) {
        printf("Error embedding payload: payload too long.\n");
        exit(1);
    }

    int payload_index = 0;
    int payload_nibble_index = 0;  // 0 to 1
    uint8_t current_char = payload[payload_index];
    for (int byte_index = 0; payload_index < payload_length; byte_index++) {
        if (payload_nibble_index == NIBBLES_PER_BYTE) {
            payload_nibble_index = 0;
            payload_index++;

            if (payload_index < payload_length) {
                current_char = payload[payload_index];
            } else {
                return;
            }
        }

        uint8_t bits_to_embed = get_i_nibble(current_char, NIBBLES_PER_BYTE - 1 - payload_nibble_index);

        data[byte_index] = (data[byte_index] & 0xF0) | bits_to_embed;
        payload_nibble_index++;
    }
}

void lsb4_extract(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, size_t payload_length) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;
    for (uint32_t data_index = 0; payload_index < payload_length; data_index++) {
        uint8_t bits = data[data_index] & 0xF;

        current_char <<= 4;
        current_char |= bits;
        payload_bit_index += 4;

        if (payload_bit_index % BITS_PER_BYTE == 0) {
            extracted_payload[payload_index++] = current_char;
            payload_bit_index = 0;
            current_char = 0;
        }
    }
}

void _lsb4_extract_extension(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, const char num_channels) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;
    for (uint32_t data_index = 0;; data_index++) {
        uint8_t bits = data[data_index] & 0xF;

        current_char <<= 4;
        current_char |= bits;
        payload_bit_index += 4;

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