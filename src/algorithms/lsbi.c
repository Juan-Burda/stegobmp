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

/**
 * @brief Embed a payload in a BMP image using the LSB1 algorithm but without modifying the red channel.
 */
static void _lsb1_no_red(uint8_t* data, int total_pixels, const uint8_t* payload, size_t payload_length, Color start_color);

void lsbi(uint8_t* data, int total_pixels, const uint8_t* payload, size_t payload_length) {
    int total_bytes = total_pixels * BYTES_PER_PIXEL;

    // Before embedding, store original LSBs
    uint8_t* original_lsbs = (uint8_t*)malloc(total_bytes);
    for (int byte_index = 0; byte_index < total_bytes; byte_index++) {
        original_lsbs[byte_index] = get_i_bit(data[byte_index], 0);
    }

    // Apply standard LSB
    Color start_color = get_color_from_byte_index(sizeof(uint32_t));
    _lsb1_no_red(data + sizeof(uint32_t), total_pixels, payload, payload_length, start_color);

    // Count patterns and changed pixels for each pattern (00, 01, 10, 11)
    int pattern_count[PATTERN_QTY] = {0};    // Count of 00, 01, 10, 11 patterns
    int pattern_changed[PATTERN_QTY] = {0};  // Count of changed pixels for each pattern
    for (int byte_index = 0; byte_index < total_bytes; byte_index++) {
        int pattern = GET_PATTERN(data[byte_index]);

        pattern_count[pattern]++;
        if ((data[byte_index] & 1) != (original_lsbs[byte_index] & 1)) {
            pattern_changed[pattern]++;
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
    for (int byte_index = PATTERN_QTY - 1; byte_index < total_bytes; byte_index++) {
        int pattern = GET_PATTERN(data[byte_index]);

        if (invert_pattern[pattern]) {
            data[byte_index] ^= 1;  // Invert LSB
        }
    }

    // Store inversion pattern
    for (int i = 0; i < PATTERN_QTY; i++) {
        data[i] = (data[i] & 0xFE) | invert_pattern[i];
    }

    free(original_lsbs);
}

void lsbi_invert(uint8_t* data, int total_pixels) {
    // Extract the inversion pattern from the first 4 bytes
    bool invert_pattern[PATTERN_QTY] = {false};
    for (int i = 0; i < PATTERN_QTY; i++) {
        invert_pattern[i] = (data[i] & 1) != false;
    }

    int total_bytes = total_pixels * BYTES_PER_PIXEL;

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

void lsbi_extract(uint8_t* data, int total_pixels, uint8_t* extracted_payload, size_t payload_length) {
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

void lsbi_extract_extension(uint8_t* data, int total_pixels, uint8_t* extracted_payload, Color start_color) {
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;
    Color current_color = start_color;
    for (uint32_t data_index = 0;; data_index++) {
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

static void _lsb1_no_red(uint8_t* data, int total_pixels, const uint8_t* payload, size_t payload_length, Color start_color) {
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = payload[payload_index];
    Color current_color = start_color;
    for (int byte_index = 0; payload_index < payload_length; byte_index++) {
        Color prev_color = current_color;
        current_color = get_next_color(current_color);
        if (prev_color == RED) {
            continue;
        }

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