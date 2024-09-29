#ifndef BMP_HEADERS_H
#define BMP_HEADERS_H

#pragma pack(push, 1)
// BMP File Header (14 bytes)
typedef struct {
    unsigned short bfType;      // BMP signature (should be 'BM')
    unsigned int bfSize;        // File size in bytes
    unsigned short bfReserved1; // Reserved, should be 0
    unsigned short bfReserved2; // Reserved, should be 0
    unsigned int bfOffBits;     // Offset to image data (pixel array)
} BPM_file_header_t;
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
} BPM_info_header_t;
#pragma pack(pop)

#endif // BMP_HEADERS_H

