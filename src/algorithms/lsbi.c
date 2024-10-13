#include <bmp-utils.h>
#include <bmp.h>
#include <lsb1.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_QTY 4

void lsbi(uint8_t* data, int width, int height, int bit_count, const char* payload, size_t payload_length) {
    if (bit_count != BITS_PER_PIXEL) return;

    int total_pixels = width * height;

    // Before embedding, store original LSBs
    int row_size = CALCULATE_ROW_SIZE(width);
    uint8_t* original_lsbs = (uint8_t*)malloc(width * height * BYTES_PER_PIXEL);  // 2 channels per pixel
    int lsb_index = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel_index = (y * row_size) + (x * BYTES_PER_PIXEL);
            for (int color = 0; color < BYTES_PER_PIXEL; color++) {
                original_lsbs[lsb_index++] = data[pixel_index + color] & 1;
            }
        }
    }

    // Apply standard LSB
    _lsb1(data + sizeof(uint32_t), width, height, bit_count, payload, payload_length, BYTES_PER_PIXEL - 1);

    // Bit Inversion
    int pattern_count[PATTERN_QTY] = {0};    // Count of 00, 01, 10, 11 patterns
    int pattern_changed[PATTERN_QTY] = {0};  // Count of changed pixels for each pattern

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel_index = (y * row_size) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL - 1; color++) {  // Only Green and Blue channels
                int byte_index = pixel_index + color;
                int pattern = (data[byte_index] >> 1) & 3;  // Get 2nd and 3rd LSB

                pattern_count[pattern]++;
                if ((data[byte_index] & 1) != (original_lsbs[byte_index] & 1)) {
                    pattern_changed[pattern]++;
                }
            }
        }
    }

    // Determine which patterns to invert
    bool invert_pattern[PATTERN_QTY] = {false};
    for (int i = 0; i < PATTERN_QTY; i++) {
        if (2 * pattern_changed[i] > pattern_count[i]) {
            invert_pattern[i] = true;
        }
    }

    // Apply inversion
    for (int i = 0; i < total_pixels; i++) {
        for (int j = 0; j < BYTES_PER_PIXEL - 1; j++) {  // Only Green and Blue channels
            int pixel_offset = i * BYTES_PER_PIXEL + j;
            int pattern = (data[pixel_offset] >> 1) & 3;
            if (invert_pattern[pattern]) {
                data[pixel_offset] ^= 1;  // Invert LSB
            }
        }
    }

    // Store inversion pattern
    uint8_t inversion_info = 0;
    for (int i = 0; i < PATTERN_QTY; i++) {
        if (invert_pattern[i]) {
            inversion_info |= (1 << i);
        }
    }

    // store inversion info in the last bit of the first 4 bytes
    for (int i = 0; i < PATTERN_QTY; i++) {
        data[i] = (data[i] & 0xF0) | ((inversion_info >> i) & 1);
    }

    free(original_lsbs);
}

void lsbi_invert(uint8_t* data, int width, int height, int bit_count, size_t payload_length, int offset) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    // Extract the inversion pattern from the first 4 bytes
    bool invert_pattern[PATTERN_QTY] = {false};
    for (int i = 0; i < PATTERN_QTY; i++) {
        invert_pattern[i] = (data[i] & 1) != false;
    }

    int row_size = CALCULATE_ROW_SIZE(width);
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;
    for (int y = 0; y < height && payload_index < payload_length; y++) {
        for (int x = 0; x < width && payload_index < payload_length; x++) {
            int pixel_index = (y * row_size) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL - 1; color++) {
                int byte_index = pixel_index + color + sizeof(uint32_t) + offset;  // Skip the first (4 + offset) bytes

                int pattern = (data[byte_index] >> 1) & 3;  // Get 2nd and 3rd LSB
                int lsb = data[byte_index] & 1;

                // Apply inversion if necessary
                if (invert_pattern[pattern]) {
                    lsb ^= 1;
                }

                data[byte_index] = (data[byte_index] & 0xFE) | lsb;
            }
        }
    }
}

void lsbi_extract(uint8_t* data, int width, int height, int bit_count, char* extracted_payload, size_t payload_length) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    // Extract the inversion pattern from the first 4 bytes
    bool invert_pattern[PATTERN_QTY] = {false};
    for (int i = 0; i < PATTERN_QTY; i++) {
        invert_pattern[i] = (data[i] & 1) != false;
    }

    int row_size = CALCULATE_ROW_SIZE(width);
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;

    for (int y = 0; y < height && payload_index < payload_length - 1; y++) {
        for (int x = 0; x < width && payload_index < payload_length - 1; x++) {
            int pixel_index = (y * row_size) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL - 1; color++) {
                int pattern = (data[pixel_index + color] >> 1) & 3;  // Get 2nd and 3rd LSB
                int lsb = data[pixel_index + color] & 1;

                // Apply inversion if necessary
                if (invert_pattern[pattern]) {
                    lsb ^= 1;
                }

                current_char |= (lsb << payload_bit_index);
                payload_bit_index++;

                if (payload_bit_index == BITS_PER_BYTE) {
                    extracted_payload[payload_index++] = current_char;

                    payload_bit_index = 0;
                    current_char = 0;

                    if (payload_index >= payload_length - 1) {
                        break;
                    }
                }
            }
        }
    }
}