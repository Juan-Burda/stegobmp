#include <bmp-utils.h>
#include <bmp.h>
#include <byte-utils.h>
#include <color-utils.h>
#include <lsb1.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_QTY 4
#define GET_PATTERN(byte) ((byte >> 1) & 3)

void lsbi(uint8_t* data, int width, int height, int bit_count, const uint8_t* payload, size_t payload_length) {
    if (bit_count != BITS_PER_PIXEL) return;

    int total_pixels = width * height;

    // Before embedding, store original LSBs
    int row_size = CALCULATE_ROW_SIZE(width);
    uint8_t* original_lsbs = (uint8_t*)malloc(width * height * BYTES_PER_PIXEL);
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

    // Count patterns and changed pixels for each pattern (00, 01, 10, 11)
    int pattern_count[PATTERN_QTY] = {0};    // Count of 00, 01, 10, 11 patterns
    int pattern_changed[PATTERN_QTY] = {0};  // Count of changed pixels for each pattern
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel_index = (y * row_size) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL - 1; color++) {  // Only Green and Blue channels
                int byte_index = pixel_index + color;
                int pattern = GET_PATTERN(data[byte_index]);

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
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel_index = (y * row_size) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL - 1; color++) {  // Only Green and Blue channels
                int byte_index = pixel_index + color;
                if (byte_index < sizeof(uint32_t)) continue;  // Skip the first 4 bytes

                int pattern = GET_PATTERN(data[byte_index]);

                if (invert_pattern[pattern]) {
                    data[byte_index] ^= 1;  // Invert LSB
                }
            }
        }
    }

    // Store inversion pattern
    for (int i = 0; i < PATTERN_QTY; i++) {
        data[i] = (data[i] & 0xFE) | invert_pattern[i];
    }

    free(original_lsbs);
}

void lsbi_invert(uint8_t* data, int width, int height, int bit_count) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    // Extract the inversion pattern from the first 4 bytes
    bool invert_pattern[PATTERN_QTY] = {false};
    for (int i = 0; i < PATTERN_QTY; i++) {
        invert_pattern[i] = (data[i] & 1) != false;
    }

    int total_bytes = width * height * BYTES_PER_PIXEL;

    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;
    for (int byte_index = PATTERN_QTY - 1; byte_index < total_bytes; byte_index++) {
        int pattern = GET_PATTERN(data[byte_index]);
        int bit = get_i_bit(data[byte_index], 0);

        // Apply inversion if necessary
        if (invert_pattern[pattern]) {
            bit ^= 1;
        }

        data[byte_index] = (data[byte_index] & 0xFE) | bit;
    }
}

void lsbi_extract(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, size_t payload_length) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;
    Color current_color = GREEN;
    for (uint32_t data_index = 0; payload_index < payload_length; data_index++) {
        Color prev_color = current_color;
        current_color = get_next_color(current_color);
        if (prev_color == RED) {
            continue;
        }

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

void _lsbi_extract_extension(uint8_t* data, int width, int height, int bit_count, uint8_t* extracted_payload, const char num_channels, Color start_color) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;
    Color current_color = start_color;
    for (uint32_t data_index = 0;;data_index++) {
        Color prev_color = current_color;
        current_color = get_next_color(current_color);
        if (prev_color == RED) {
            continue;
        }
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