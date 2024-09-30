#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp-headers.h"
#include "../include/bmp-utils.h"
#include "../include/constants/bmp.h"
#include "../include/file-utils.h"
#include "../include/lsb1.h"

void lsb1(uint8_t* data, int width, int height, int bitCount, const char* message, size_t maxMessageLength) {
    if (bitCount != BITS_PER_PIXEL) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int rowSize = CALCULATE_ROW_SIZE(width);
    int messageIndex = 0;
    int messageBitIndex = 0;
    uint8_t currentChar = message[messageIndex];

    for (int y = 0; y < height && messageIndex < maxMessageLength - 1; y++) {
        for (int x = 0; x < width && messageIndex < maxMessageLength - 1; x++) {
            int pixelIndex = (y * rowSize) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL; color++) {
                int bitToEmbed = (currentChar >> messageBitIndex) & 1;

                data[pixelIndex + color] = (data[pixelIndex + color] & 0xFE) | bitToEmbed;

                messageBitIndex++;
                if (messageBitIndex == BITS_PER_BYTE) {
                    messageBitIndex = 0;
                    messageIndex++;
                    currentChar = message[messageIndex];
                }
            }
        }
    }
}

void lsb1_extract(uint8_t* data, int width, int height, int bitCount, char* extractedMessage, size_t maxMessageLength) {
    if (bitCount != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int rowSize = CALCULATE_ROW_SIZE(width);
    int messageIndex = 0;
    int messageBitIndex = 0;
    uint8_t currentChar = 0;

    for (int y = 0; y < height && messageIndex < maxMessageLength - 1; y++) {
        for (int x = 0; x < width && messageIndex < maxMessageLength - 1; x++) {
            int pixelIndex = (y * rowSize) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL; color++) {
                int bit = data[pixelIndex + color] & 1;

                currentChar |= (bit << messageBitIndex);
                messageBitIndex++;

                if (messageBitIndex == BITS_PER_BYTE) {
                    extractedMessage[messageIndex++] = currentChar;

                    messageBitIndex = 0;
                    currentChar = 0;

                    if (messageIndex >= maxMessageLength - 1) {
                        break;
                    }
                }
            }
        }
    }
}