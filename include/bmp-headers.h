#ifndef BMP_HEADERS_H
#define BMP_HEADERS_H

#pragma pack(push, 1)
// BMP File Header (14 bytes)
typedef struct {
    unsigned short bf_type;       // BMP signature (should be 'BM')
    unsigned int bf_size;         // File size in bytes
    unsigned short bf_reserved1;  // Reserved, should be 0
    unsigned short bf_reserved2;  // Reserved, should be 0
    unsigned int bf_off_bits;     // Offset to image data (pixel array)
} BPM_file_header_t;
#pragma pack(pop)

#pragma pack(push, 1)
// BMP Info Header (40 bytes)
typedef struct {
    unsigned int bi_size;           // Size of the info header (40 bytes)
    int bi_width;                   // Width of the image
    int bi_height;                  // Height of the image
    unsigned short bi_planes;       // Number of color planes (must be 1)
    unsigned short bi_bit_count;    // Number of bits per pixel
    unsigned int biCompression;     // Compression type (0 = uncompressed)
    unsigned int bi_size_image;     // Image size in bytes (may be 0 for uncompressed images)
    int bi_x_pels_per_meter;        // Horizontal resolution
    int bi_y_pels_per_meter;        // Vertical resolution
    unsigned int bi_clr_used;       // Number of colors in the color palette
    unsigned int bi_clr_important;  // Important colors (0 = all colors are important)
} BPM_info_header_t;
#pragma pack(pop)

#endif  // BMP_HEADERS_H
