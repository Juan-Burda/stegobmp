#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp-headers.h"
#include "../include/bmp-utils.h"
#include "../include/constants/bmp.h"
#include "../include/file-utils.h"
#include "../include/lsb1.h"

void lsb1(uint8_t* data, int width, int height, int bitCount, const char* message) {
    if (bitCount != BITS_PER_PIXEL) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    int rowSize = CALCULATE_ROW_SIZE(width);
    int messageIndex = 0;
    int messageBitIndex = 0;
    uint8_t currentChar = message[messageIndex];

    for (int y = 0; y < height && message[messageIndex] != '\0'; y++) {
        for (int x = 0; x < width && message[messageIndex] != '\0'; x++) {
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

    if (message[messageIndex] == '\0') {
        printf("Message embedded successfully!\n");
    } else {
        printf("Warning: Image is too small to embed the full message.\n");
    }
}

void lsb1_extract(uint8_t* data, int width, int height, int bitCount, char* extractedMessage, int maxMessageLength) {
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
                    if (currentChar == '\0') {
                        extractedMessage[messageIndex] = '\0';
                        printf("Extracted message: %s\n", extractedMessage);
                        return;
                    }
                    extractedMessage[messageIndex++] = currentChar;
                    messageBitIndex = 0;
                    currentChar = 0;
                }
            }
        }
    }

    extractedMessage[messageIndex] = '\0';
    printf("Extracted message: %s\n", extractedMessage);
}