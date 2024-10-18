#include <bmp-headers.h>
#include <bmp-utils.h>
#include <constants/bmp.h>
#include <file-utils.h>
#include <lsb4.h>
#include <stdio.h>
#include <stdlib.h>

void lsb4(uint8_t* data, int width, int height, int bit_count, const char* payload, size_t payload_length) {
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

    for (int byte_index = 0; byte_index < (payload_length * (BITS_PER_BYTE / 2)); byte_index++) {
        if (payload_bit_index == BITS_PER_BYTE) {
            payload_bit_index = 0;
            payload_index++;

            if (payload_index < payload_length) {
                current_char = payload[payload_index];
            } else {
                return;
            }
        }

        int bits_to_embed = (current_char >> payload_bit_index) & 0xF;

        data[byte_index] = (data[byte_index] & 0xF0) | bits_to_embed;

        payload_bit_index += 4;
    }
}

void lsb4_extract(uint8_t* data, int width, int height, int bit_count, char* extracted_payload, size_t payload_length) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int row_size = CALCULATE_ROW_SIZE(width);
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;

    for (int byte_index = 0; byte_index < (payload_length * (BITS_PER_BYTE / 2)); byte_index++) {
        int bits = data[byte_index] & 0xF;

        current_char |= (bits << payload_bit_index);
        payload_bit_index += 4;

        if (payload_bit_index >= BITS_PER_BYTE) {
            extracted_payload[payload_index++] = current_char;

            payload_bit_index = 0;
            current_char = 0;

            if (payload_index >= payload_length) {
                break;
            }
        }
    }
}
