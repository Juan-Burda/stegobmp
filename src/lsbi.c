#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <bmp.h>
#include <bmp-utils.h>
#include <stdlib.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))

void lsbi(uint8_t* data, int width, int height, int bitCount, const char* payload, size_t payloadLength) {
    if (bitCount != BITS_PER_PIXEL) return;
    
    int totalPixels = width * height;
    size_t messageLength = MIN(strlen(payload), payloadLength);
    size_t bitsToHide = messageLength * BITS_PER_BYTE;

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
    size_t bitIndex = 0;
    for (int i = 0; i < totalPixels && bitIndex < bitsToHide; i++) {
        for (int j = 0; j < BYTES_PER_PIXEL - 1 && bitIndex < bitsToHide; j++) { // Only use Green and Blue channels
            int pixelOffset = i * BYTES_PER_PIXEL + j;
            
            // TODO: Check this line
            uint8_t messageBit = (payload[bitIndex / BITS_PER_BYTE] >> (7 - (bitIndex % BITS_PER_BYTE))) & 1;
            data[pixelOffset] = (data[pixelOffset] & 0xFE) | messageBit;
            bitIndex++;
        }
    }
    
    // Step 2: Bit Inversion
    int patternCount[4] = {0}; // Count of 00, 01, 10, 11 patterns
    int patternChanged[4] = {0}; // Count of changed pixels for each pattern
    
    for (int i = 0; i < totalPixels; i++) {
        for (int j = 0; j < BYTES_PER_PIXEL - 1; j++) { // Only Green and Blue channels
            int pixelOffset = i * BYTES_PER_PIXEL + j;
            int pattern = (data[pixelOffset] >> 1) & 3; // Get 2nd and 3rd LSB
            patternCount[pattern]++;
            if ((data[pixelOffset] & 1) != (originalLSBs[pixelOffset] & 1)) {
                patternChanged[pattern]++;
            }
        }
    }
    
    // Determine which patterns to invert
    bool invertPattern[4] = {false};
    for (int i = 0; i < 4; i++) {
        if (patternChanged[i] > patternCount[i]) {
            invertPattern[i] = true;
        }
    }
    
    // Apply inversion
    for (int i = 0; i < totalPixels; i++) {
        for (int j = 0; j < BYTES_PER_PIXEL - 1; j++) { // Only Green and Blue channels
            int pixelOffset = i * BYTES_PER_PIXEL + j;
            int pattern = (data[pixelOffset] >> 1) & 3;
            if (invertPattern[pattern]) {
                data[pixelOffset] ^= 1; // Invert LSB  
            }
        }
    }
    
    // Store inversion pattern (using the last 4 bits of the last pixel)
    int lastPixelOffset = (totalPixels - 1) * BYTES_PER_PIXEL;
    uint8_t inversionInfo = 0;
    for (int i = 0; i < 4; i++) {
        if (invertPattern[i]) {
            inversionInfo |= (1 << i);
        }
    }

    // Store information in the Blue channel
    data[lastPixelOffset] = (data[lastPixelOffset] & 0xF0) | (inversionInfo & 0x0F);

    free(originalLSBs);
}

void lsbi_extract(uint8_t* data, int width, int height, int bitCount, char* extractedMessage, size_t maxMessageLength) {
    if (bitCount != 24 && bitCount != 32) return; // Only support 24-bit and 32-bit images
    
    int bytesPerPixel = bitCount / 8;
    int totalPixels = width * height;
    
    // First, extract the inversion pattern from the last pixel
    int lastPixelOffset = (totalPixels - 1) * bytesPerPixel;
    uint8_t inversionInfo = data[lastPixelOffset + 2] & 0x0F;
    bool invertPattern[4] = {false};
    for (int i = 0; i < 4; i++) {
        invertPattern[i] = (inversionInfo & (1 << i)) != 0;
    }
    
    // Initialize extracted message
    memset(extractedMessage, 0, maxMessageLength);
    
    size_t bitIndex = 0;
    uint8_t currentByte = 0;
    
    for (int i = 0; i < totalPixels && bitIndex / 8 < maxMessageLength; i++) {
        for (int j = 1; j < 3 && bitIndex / 8 < maxMessageLength; j++) { // Only use Green and Blue channels
            int pixelOffset = i * bytesPerPixel + j;
            int pattern = (data[pixelOffset] >> 1) & 3; // Get 2nd and 3rd LSB
            uint8_t lsb = data[pixelOffset] & 1;
            
            // Apply inversion if necessary
            if (invertPattern[pattern]) {
                lsb ^= 1;
            }
            
            // Add this bit to our current byte
            currentByte = (currentByte << 1) | lsb;
            bitIndex++;
            
            // If we've collected 8 bits, add the byte to our message
            if (bitIndex % 8 == 0) {
                extractedMessage[bitIndex / 8 - 1] = currentByte;
                currentByte = 0;
            }
        }
    }
    
    // Ensure null-termination
    extractedMessage[maxMessageLength - 1] = '\0';
}