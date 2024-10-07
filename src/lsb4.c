#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp-headers.h"
#include "../include/bmp-utils.h"
#include "../include/constants/bmp.h"
#include "../include/file-utils.h"
#include "../include/lsb4.h"

void lsb4(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength) {
    if (bitCount != BITS_PER_PIXEL) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

     if (payloadLength * 4 > width * height * BITS_PER_PIXEL) {
        printf("Error embedding payload: payload too long.\n");  
        exit(1);
        return;
    }

    int rowSize = CALCULATE_ROW_SIZE(width);
    int payloadIndex = 0;
    int payloadBitIndex = 0;
    uint8_t currentChar = payload[payloadIndex];

    for (int y = 0; y < height && payloadIndex < payloadLength - 1; y++) {
        for (int x = 0; x < width && payloadIndex < payloadLength - 1; x++) {
            int pixelIndex = (y * rowSize) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL; color++) {
                int bitsToEmbed = (currentChar >> payloadBitIndex) & 0xF;

                data[pixelIndex + color] = (data[pixelIndex + color] & 0xF0) | bitsToEmbed;

                payloadBitIndex += 4;
                if (payloadBitIndex >= BITS_PER_BYTE) {
                    payloadBitIndex = 0;
                    payloadIndex++;
                    currentChar = payload[payloadIndex];
                }
            }
        }
    }
}

void lsb4_extract(uint8_t* data, int width, int height, int bitCount, char* extractedPayload, size_t payloadLength) {
    if (bitCount != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int rowSize = CALCULATE_ROW_SIZE(width);
    int payloadIndex = 0;
    int payloadBitIndex = 0;
    uint8_t currentChar = 0;

    for (int y = 0; y < height && payloadIndex < payloadLength - 1; y++) {
        for (int x = 0; x < width && payloadIndex < payloadLength - 1; x++) {
            int pixelIndex = (y * rowSize) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL; color++) {
                int bits = data[pixelIndex + color] & 0xF;

                currentChar |= (bits << payloadBitIndex);
                payloadBitIndex += 4;

                if (payloadBitIndex >= BITS_PER_BYTE) {
                    extractedPayload[payloadIndex++] = currentChar;

                    payloadBitIndex = 0;
                    currentChar = 0;

                    if (payloadIndex >= payloadLength - 1) {
                        break;
                    }
                }
            }
        }
    }
}
