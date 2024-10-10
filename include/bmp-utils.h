#ifndef BMP_UTILS_H
#define BMP_UTILS_H

#include <stdint.h>

#define CALCULATE_ROW_SIZE(width) (((width)*3 + 3) & ~3)

#define GET_BLUE_CHANNEL(pixel) (pixel + 0)
#define GET_GREEN_CHANNEL(pixel) (pixel + 1)
#define GET_RED_CHANNEL(pixel) (pixel + 2)

#pragma pack(push, 1)
// BMP File Header (14 bytes)
typedef struct {
    unsigned short bfType;       // BMP signature (should be 'BM')
    unsigned int bfSize;         // File size in bytes
    unsigned short bfReserved1;  // Reserved, should be 0
    unsigned short bfReserved2;  // Reserved, should be 0
    unsigned int bfOffBits;      // Offset to image data (pixel array)
} BMPFileHeader;
#pragma pack(pop)

#pragma pack(push, 1)
// BMP Info Header (40 bytes)
typedef struct {
    unsigned int biSize;          // Size of the info header (40 bytes)
    int biWidth;                  // Width of the image
    int biHeight;                 // Height of the image
    unsigned short biPlanes;      // Number of color planes (must be 1)
    unsigned short biBitCount;    // Number of bits per pixel
    unsigned int biCompression;   // Compression type (0 = uncompressed)
    unsigned int biSizeImage;     // Image size in bytes (may be 0 for uncompressed images)
    int biXPelsPerMeter;          // Horizontal resolution
    int biYPelsPerMeter;          // Vertical resolution
    unsigned int biClrUsed;       // Number of colors in the color palette
    unsigned int biClrImportant;  // Important colors (0 = all colors are important)
} BMPInfoHeader;
#pragma pack(pop)

void read_bmp_headers(const char* fileName);

uint8_t* read_bmp_data(const char* fileName, BMPFileHeader* fileHeader, BMPInfoHeader* infoHeader);

void write_bmp(const char* fileName, BMPFileHeader* fileHeader, BMPInfoHeader* infoHeader, uint8_t* data);

void print_bmp_data(uint8_t* data, int width, int height, int bitCount);

#endif  // BMP_UTILS_H