#include <stdio.h>
#include <stdlib.h>
#include "../../include/file-utils.h"

const char * get_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    
    if (!dot || dot == filename) {
        return "";
    }
    
    return dot;
}

DWORD get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    DWORD size = (DWORD)ftell(file);
    rewind(file);
    return size;
}

unsigned char* create_bit_string(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("No se pudo abrir el archivo");
        return NULL;
    }

    dword file_size = get_file_size(file);
    
    const char * extension = get_file_extension(filename);
    size_t ext_size = strlen(extension);

    total_size = sizeof(dword) + file_size + ext_size;

    unsigned char *bit_string = (unsigned char*)malloc(total_size);
    if (bit_string == NULL) {
        perror("No se pudo asignar memoria");
        fclose(file);
        return NULL;
    }

    memcpy(bit_string, &file_size, sizeof(dword));

    fread(bit_string + sizeof(dword), 1, file_size, file);

    memcpy(bit_string + sizeof(dword) + file_size, extension, ext_size);

    fclose(file);

    return bit_string;
}
