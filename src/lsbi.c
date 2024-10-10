#include <bmp-utils.h>
#include <bmp.h>
#include <lsb1.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_QTY 4

void lsbi(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength) {
    if (bitCount != BITS_PER_PIXEL) return;

    int totalPixels = width * height;

    // Before embedding, store original LSBs
    int rowSize = CALCULATE_ROW_SIZE(width);
    uint8_t* originalLSBs = (uint8_t*)malloc(width * height * BYTES_PER_PIXEL);  // 2 channels per pixel
    int lsbIndex = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixelIndex = (y * rowSize) + (x * BYTES_PER_PIXEL);
            for (int color = 0; color < BYTES_PER_PIXEL; color++) {
                originalLSBs[lsbIndex++] = data[pixelIndex + color] & 1;
            }
        }
    }

    // Step 1: Apply standard LSB
    _lsb1(data, width, height, bitCount, payload, payloadLength, BYTES_PER_PIXEL - 1);

    // Step 2: Bit Inversion
    int patternCount[PATTERN_QTY] = {0};    // Count of 00, 01, 10, 11 patterns
    int patternChanged[PATTERN_QTY] = {0};  // Count of changed pixels for each pattern

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixelIndex = (y * rowSize) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL - 1; color++) {  // Only Green and Blue channels
                int byteIndex = pixelIndex + color;
                int pattern = (data[byteIndex] >> 1) & 3;  // Get 2nd and 3rd LSB

                patternCount[pattern]++;
                if ((data[byteIndex] & 1) != (originalLSBs[byteIndex] & 1)) {
                    patternChanged[pattern]++;
                }
            }
        }
    }

    // Determine which patterns to invert
    bool invertPattern[PATTERN_QTY] = {false};
    for (int i = 0; i < PATTERN_QTY; i++) {
        if (2 * patternChanged[i] > patternCount[i]) {
            invertPattern[i] = true;
        }
    }

    // Apply inversion
    for (int i = 0; i < totalPixels; i++) {
        for (int j = 0; j < BYTES_PER_PIXEL - 1; j++) {  // Only Green and Blue channels
            int pixelOffset = i * BYTES_PER_PIXEL + j;
            int pattern = (data[pixelOffset] >> 1) & 3;
            if (invertPattern[pattern]) {
                data[pixelOffset] ^= 1;  // Invert LSB
            }
        }
    }

    // Store inversion pattern (using the last 4 bits of the last pixel)
    int lastPixelOffset = (totalPixels - 1) * BYTES_PER_PIXEL;
    uint8_t inversionInfo = 0;
    for (int i = 0; i < PATTERN_QTY; i++) {
        if (invertPattern[i]) {
            inversionInfo |= (1 << i);
        }
    }

    // Store information in the Blue channel
    data[GET_BLUE_CHANNEL(lastPixelOffset)] = (data[GET_BLUE_CHANNEL(lastPixelOffset)] & 0xF0) | (inversionInfo & 0x0F);

    free(originalLSBs);
}

/*
void lsbi_extract(uint8_t* data, int width, int height, int bitCount, char* extractedMessage, size_t maxMessageLength) {
    if (bitCount != BITS_PER_PIXEL) return;

    int totalPixels = width * height;

    // First, extract the inversion pattern from the last pixel
    int lastPixelOffset = (totalPixels - 1) * BYTES_PER_PIXEL;
    uint8_t inversionInfo = data[GET_BLUE_CHANNEL(lastPixelOffset)] & 0x0F;
    bool invertPattern[PATTERN_QTY] = {false};
    for (int i = 0; i < PATTERN_QTY; i++) {
        invertPattern[i] = (inversionInfo & (1 << i)) != false;
    }

    // Initialize extracted message
    memset(extractedMessage, 0, maxMessageLength);

    size_t bitIndex = 0;
    uint8_t currentByte = 0;

    for (int i = 0; i < totalPixels && bitIndex / BITS_PER_BYTE < maxMessageLength; i++) {
        for (int j = 0; j < (BYTES_PER_PIXEL - 1) && bitIndex / BITS_PER_BYTE < maxMessageLength; j++) {  // Only use Green and Blue channels
            int pixelOffset = i * BYTES_PER_PIXEL + j;
            int pattern = (data[pixelOffset] >> 1) & 3;  // Get 2nd and 3rd LSB
            uint8_t lsb = data[pixelOffset] & 1;

            // Apply inversion if necessary
            if (invertPattern[pattern]) {
                lsb ^= 1;
            }

            // Add this bit to our current byte
            currentByte = (currentByte << 1) | lsb;
            bitIndex++;

            // If we've collected 8 bits, add the byte to our message
            if (bitIndex % BITS_PER_BYTE == 0) {
                extractedMessage[bitIndex / BITS_PER_BYTE - 1] = currentByte;
                currentByte = 0;
            }
        }
    }

    // Ensure null-termination
    extractedMessage[maxMessageLength - 1] = '\0';
}
*/

void lsbi_extract(uint8_t* data, int width, int height, int bitCount, char* extractedPayload, size_t payloadLength) {
    if (bitCount != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    // First, extract the inversion pattern from the last pixel
    int totalPixels = width * height;
    int lastPixelOffset = (totalPixels - 1) * BYTES_PER_PIXEL;
    uint8_t inversionInfo = data[GET_BLUE_CHANNEL(lastPixelOffset)] & 0x0F;
    bool invertPattern[PATTERN_QTY] = {false};
    for (int i = 0; i < PATTERN_QTY; i++) {
        invertPattern[i] = (inversionInfo & (1 << i)) != false;
    }

    int rowSize = CALCULATE_ROW_SIZE(width);
    int payloadIndex = 0;
    int payloadBitIndex = 0;
    uint8_t currentChar = 0;

    for (int y = 0; y < height && payloadIndex < payloadLength - 1; y++) {
        for (int x = 0; x < width && payloadIndex < payloadLength - 1; x++) {
            int pixelIndex = (y * rowSize) + (x * BYTES_PER_PIXEL);

            for (int color = 0; color < BYTES_PER_PIXEL - 1; color++) {
                int pattern = (data[pixelIndex + color] >> 1) & 3;  // Get 2nd and 3rd LSB
                int lsb = data[pixelIndex + color] & 1;

                // Apply inversion if necessary
                if (invertPattern[pattern]) {
                    lsb ^= 1;
                }

                currentChar |= (lsb << payloadBitIndex);
                payloadBitIndex++;

                if (payloadBitIndex == BITS_PER_BYTE) {
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