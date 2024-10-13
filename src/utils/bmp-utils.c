#include <bmp-utils.h>
#include <stdio.h>
#include <stdlib.h>

void read_bmp_headers(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    BMPFileHeader file_header;
    BMPInfoHeader info_header;

    fread(&file_header, sizeof(BMPFileHeader), 1, file);

    if (file_header.bf_type != 0x4D42) {
        printf("Not a valid BMP file.\n");
        fclose(file);
        return;
    }

    fread(&info_header, sizeof(BMPInfoHeader), 1, file);

    printf("File Size: %u bytes\n", file_header.bf_size);
    printf("Image Width: %d pixels\n", info_header.bi_width);
    printf("Image Height: %d pixels\n", info_header.bi_height);
    printf("Bit Depth: %u bits per pixel\n", info_header.bi_bit_count);
    printf("Compression: %s\n", info_header.bi_compression == 0 ? "No" : "Yes");
    printf("Image Size: %u bytes\n", info_header.bi_size_image);

    fclose(file);
}

uint8_t *read_bmp_data(const char *filename, BMPFileHeader *file_header, BMPInfoHeader *info_header) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    fread(file_header, sizeof(BMPFileHeader), 1, file);

    if (file_header->bf_type != 0x4D42) {
        printf("Not a valid BMP file.\n");
        fclose(file);
        return NULL;
    }

    fread(info_header, sizeof(BMPInfoHeader), 1, file);

    fseek(file, file_header->bf_off_bits, SEEK_SET);

    uint8_t *pixelData = (uint8_t *)malloc(file_header->bf_size);
    if (!pixelData) {
        printf("Error: Memory allocation failed!\n");
        fclose(file);
        return NULL;
    }

    fread(pixelData, info_header->bi_size_image, 1, file);

    fclose(file);

    return pixelData;
}

void print_bmp_data(uint8_t *data, int width, int height, int bit_count) {
    if (bit_count != 24) {
        printf("This function only supports 24-bit BMP files.\n");
        return;
    }

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;

            uint8_t blue = data[index];
            uint8_t green = data[index + 1];
            uint8_t red = data[index + 2];

            printf("(%3u, %3u, %3u) ", red, green, blue);
        }
        printf("\n");
    }
}

void write_bmp(const char *filename, BMPFileHeader *file_header, BMPInfoHeader *info_header, uint8_t *data) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(file_header, sizeof(BMPFileHeader), 1, file);

    fwrite(info_header, sizeof(BMPInfoHeader), 1, file);

    int row_size = ((info_header->bi_width * 3) + 3) & ~3;

    for (int y = 0; y < info_header->bi_height; y++) {
        fwrite(&data[y * row_size], 1, row_size, file);
    }

    fclose(file);
    printf("BMP saved successfully: %s\n", filename);
}
