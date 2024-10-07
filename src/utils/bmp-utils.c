#include <stdio.h>
#include <stdlib.h>
#include "../../include/bmp-utils.h"

void read_bmp_headers(const char *fileName) {
    FILE *file = fopen(fileName, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    fread(&fileHeader, sizeof(BMPFileHeader), 1, file);

    if (fileHeader.bfType != 0x4D42) {
        printf("Not a valid BMP file.\n");
        fclose(file);
        return;
    }

    fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    printf("File Size: %u bytes\n", fileHeader.bfSize);
    printf("Image Width: %d pixels\n", infoHeader.biWidth);
    printf("Image Height: %d pixels\n", infoHeader.biHeight);
    printf("Bit Depth: %u bits per pixel\n", infoHeader.biBitCount);
    printf("Compression: %s\n", infoHeader.biCompression == 0? "No": "Yes");
    printf("Image Size: %u bytes\n", infoHeader.biSizeImage);

    fclose(file);
}

uint8_t* read_bmp_data(const char* fileName, BMPFileHeader* fileHeader, BMPInfoHeader* infoHeader) {
    FILE *file = fopen(fileName, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    fread(fileHeader, sizeof(BMPFileHeader), 1, file);

    if (fileHeader->bfType != 0x4D42) {
        printf("Not a valid BMP file.\n");
        fclose(file);
        return NULL;
    }

    fread(infoHeader, sizeof(BMPInfoHeader), 1, file);

    fseek(file, fileHeader->bfOffBits, SEEK_SET);

    uint8_t* pixelData = (uint8_t*)malloc(fileHeader->bfSize);
    if (!pixelData) {
        printf("Error: Memory allocation failed!\n");
        fclose(file);
        return NULL;
    }

    fread(pixelData, infoHeader->biSizeImage, 1, file);

    fclose(file);

    return pixelData;
}

void print_bmp_data(uint8_t* data, int width, int height, int bitCount) {
    if (bitCount != 24) {
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

void write_bmp(const char *fileName, BMPFileHeader *fileHeader, BMPInfoHeader *infoHeader, uint8_t *data) {
    FILE *file = fopen(fileName, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(fileHeader, sizeof(BMPFileHeader), 1, file);

    fwrite(infoHeader, sizeof(BMPInfoHeader), 1, file);

    int rowSize = ((infoHeader->biWidth * 3) + 3) & ~3;

    for (int y = 0; y < infoHeader->biHeight; y++) {
        fwrite(&data[y * rowSize], 1, rowSize, file);
    }

    fclose(file);
    printf("BMP saved successfully: %s\n", fileName);
}

