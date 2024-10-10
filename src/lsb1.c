#include "../include/lsb1.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp-headers.h"
#include "../include/bmp-utils.h"
#include "../include/constants/bmp.h"
#include "../include/file-utils.h"

void lsb1(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength) {
    _lsb1(data, width, height, bitCount, payload, payloadLength, BYTES_PER_PIXEL);
}

void _lsb1(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength, const char numChannels) {
    if (bitCount != BITS_PER_PIXEL) {
        printf("This function only supports 24-bit BMP files.\n");
        exit(1);
    }

    if (payloadLength * BITS_PER_BYTE > width * height * BITS_PER_PIXEL) {
        printf("Error embedding payload: payload too long.\n");
        exit(1);
    }

    int rowSize = CALCULATE_ROW_SIZE(width);
    int payloadIndex = 0;
    int payloadBitIndex = 0;
    uint8_t currentChar = payload[payloadIndex];

    for (int y = 0; y < height && payloadIndex < payloadLength; y++) {
        for (int x = 0; x < width && payloadIndex < payloadLength; x++) {
            int pixelIndex = (y * rowSize) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < numChannels; color++) {
                int bitToEmbed = (currentChar >> payloadBitIndex) & 1;

                data[pixelIndex + color] = (data[pixelIndex + color] & 0xFE) | bitToEmbed;

                payloadBitIndex++;
                if (payloadBitIndex == BITS_PER_BYTE) {
                    payloadBitIndex = 0;
                    currentChar = payload[payloadIndex];
                    payloadIndex++;
                }
            }
        }
    }
}

void lsb1_extract(uint8_t* data, int width, int height, int bitCount, char* extractedPayload, size_t payloadLength) {
    _lsb1_extract(data, width, height, bitCount, extractedPayload, payloadLength, BYTES_PER_PIXEL);
}

void _lsb1_extract(uint8_t* data, int width, int height, int bitCount, char* extractedPayload, size_t payloadLength, const char numChannels) {
    if (bitCount != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int rowSize = CALCULATE_ROW_SIZE(width);
    int payloadIndex = 0;
    int payloadBitIndex = 0;
    uint8_t currentChar = 0;

    for (int y = 0; y < height && payloadIndex < payloadLength; y++) {
        for (int x = 0; x < width && payloadIndex < payloadLength; x++) {
            int pixelIndex = (y * rowSize) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < numChannels; color++) {
                int bit = data[pixelIndex + color] & 1;

                currentChar |= (bit << payloadBitIndex);
                payloadBitIndex++;

                if (payloadBitIndex == BITS_PER_BYTE) {
                    extractedPayload[payloadIndex++] = currentChar;

                    payloadBitIndex = 0;
                    currentChar = 0;

                    if (payloadIndex >= payloadLength) {
                        break;
                    }
                }
            }
        }
    }
}