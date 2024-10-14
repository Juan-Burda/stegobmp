#include <bmp-headers.h>
#include <bmp-utils.h>
#include <constants/bmp.h>
#include <file-utils.h>
#include <lsb1.h>
#include <stdio.h>
#include <stdlib.h>

void lsb1(uint8_t* data, int width, int height, int bit_count, const char* payload, size_t payload_length) {
    _lsb1(data, width, height, bit_count, payload, payload_length, BYTES_PER_PIXEL);
}

void _lsb1(uint8_t* data, int width, int height, int bit_count, const char* payload, size_t payload_length, const char num_channels) {
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

    for (int y = 0; y < height && payload_index < payload_length; y++) {
        for (int x = 0; x < width && payload_index < payload_length; x++) {
            int pixel_index = (y * row_size) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < num_channels; color++) {
                if (payload_bit_index == BITS_PER_BYTE) {
                    payload_bit_index = 0;
                    payload_index++;

                    if (payload_index < payload_length) {
                        current_char = payload[payload_index];
                    } else {
                        return;
                    }
                }

                int bit_to_embed = (current_char >> payload_bit_index) & 1;

                data[pixel_index + color] = (data[pixel_index + color] & 0xFE) | bit_to_embed;

                payload_bit_index++;
            }
        }
    }
}

void lsb1_extract(uint8_t* data, int width, int height, int bit_count, char* extracted_payload, size_t payload_length) {
    _lsb1_extract(data, width, height, bit_count, extracted_payload, payload_length, BYTES_PER_PIXEL);
}

void _lsb1_extract(uint8_t* data, int width, int height, int bit_count, char* extracted_payload, size_t payload_length, const char num_channels) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int row_size = CALCULATE_ROW_SIZE(width);
    int payload_index = 0;
    int payload_bit_index = 0;
    uint8_t current_char = 0;

    for (int y = 0; y < height && payload_index < payload_length; y++) {
        for (int x = 0; x < width && payload_index < payload_length; x++) {
            int pixel_index = (y * row_size) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < num_channels; color++) {
                uint8_t pixelValue = data[pixel_index + color];
                int bit = pixelValue & 1;

                current_char |= (bit << payload_bit_index);
                payload_bit_index++;

                if (payload_bit_index == BITS_PER_BYTE) {
                    extracted_payload[payload_index++] = current_char;
                    payload_bit_index = 0;
                    current_char = 0;

                    if (payload_index >= payload_length) {
                        break;
                    }
                }
            }
        }
    }
}