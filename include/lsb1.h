#ifndef LSB1_H
#define LSB1_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void lsb1(uint8_t* data, int width, int height, int bit_count, const char* payload, size_t payload_length);

/** 
 * @brief Embeds a payload into an image using the LSB1 algorithm, only using the first num_channels channels.
 * 
 * @param data The image data.
 * @param width The width of the image.
 * @param height The height of the image.
 * @param bit_count The number of bits per pixel.
 * @param payload The payload to embed.
 * @param payload_length The length of the payload.
 * @param num_channels The number of channels to use.
 */
void _lsb1(uint8_t* data, int width, int height, int bit_count, const char* payload, size_t payload_length, const char num_channels);

void lsb1_extract(uint8_t* data, int width, int height, int bit_count, char* extracted_payload, size_t payload_length);

/**
 * @brief Extracts a payload from an image using the LSB1 algorithm, only using the first num_channels channels.
 */
void _lsb1_extract(uint8_t* data, int width, int height, int bit_count, char* extracted_payload, size_t payload_length, const char num_channels);

#endif  // LSB1_H