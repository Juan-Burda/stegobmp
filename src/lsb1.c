#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp-headers.h"
#include "../include/file-utils.h"

void lsb1(const char * filename, const char *bitmap_file){
    // Abrimos archivo a ocultar
    unsigned char * bit_string = create_bit_string(const char *filename, dword *total_size);

    // Abrimos archivo portador
    FILE *bitmap_file = fopen(bitmap_file, "rb");
    if (!bitmap_file) {
        perror("Error opening bitmap bitmap_file");
        return;
    }

    BPM_file_header_t file_header;
    fread(&file_header, sizeof(BPM_file_header_t), 1, bitmap_file);

    BPM_info_header_t info_header;
    fread(&info_header, sizeof(BPM_info_header_t), 1, bitmap_file);

    fseek(bitmap_file, fileHeader.bfOffBits, SEEK_SET);

    int row_size = (infoHeader.biBitCount * infoHeader.biWidth + 31) / 32 * 4;
    int data_size = row_size * abs(infoHeader.biHeight);
    unsigned char *data = (unsigned char *) malloc(data_size);

    fread(data, data_size, 1, bitmap_file);
    
    for (int i = 0; i < 10; i++) {
        printf("Byte %d: %02x\n", i, data[i]);
        data[i] ^= 0x80;
    }
}

