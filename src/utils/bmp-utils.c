#include <stdio.h>
#include <stdlib.h>
#include "../../include/bmp-utils.h"

#pragma pack(push, 1)
// BMP File Header (14 bytes)
typedef struct {
    unsigned short bfType;      // BMP signature (should be 'BM')
    unsigned int bfSize;        // File size in bytes
    unsigned short bfReserved1; // Reserved, should be 0
    unsigned short bfReserved2; // Reserved, should be 0
    unsigned int bfOffBits;     // Offset to image data (pixel array)
} BMPFileHeader;
#pragma pack(pop)

#pragma pack(push, 1)
// BMP Info Header (40 bytes)
typedef struct {
    unsigned int biSize;         // Size of the info header (40 bytes)
    int biWidth;                 // Width of the image
    int biHeight;                // Height of the image
    unsigned short biPlanes;     // Number of color planes (must be 1)
    unsigned short biBitCount;   // Number of bits per pixel
    unsigned int biCompression;  // Compression type (0 = uncompressed)
    unsigned int biSizeImage;    // Image size in bytes (may be 0 for uncompressed images)
    int biXPelsPerMeter;         // Horizontal resolution
    int biYPelsPerMeter;         // Vertical resolution
    unsigned int biClrUsed;      // Number of colors in the color palette
    unsigned int biClrImportant; // Important colors (0 = all colors are important)
} BMPInfoHeader;
#pragma pack(pop)

void readBMPHeader(const char *fileName) {
    FILE *file = fopen(fileName, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // Read file header
    fread(&fileHeader, sizeof(BMPFileHeader), 1, file);

    // Verify that it's a BMP file by checking the signature
    if (fileHeader.bfType != 0x4D42) {
        printf("Not a valid BMP file.\n");
        fclose(file);
        return;
    }

    // Read info header
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Print BMP metadata
    printf("File Size: %u bytes\n", fileHeader.bfSize);
    printf("Image Width: %d pixels\n", infoHeader.biWidth);
    printf("Image Height: %d pixels\n", infoHeader.biHeight);
    printf("Bit Depth: %u bits per pixel\n", infoHeader.biBitCount);
    printf("Compression: %s\n", infoHeader.biCompression == 0? "No": "Yes");
    printf("Image Size: %u bytes\n", infoHeader.biSizeImage);

    fclose(file);
}