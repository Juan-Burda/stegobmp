#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/file-utils.h"
#include "../../include/constants/bmp.h"

const char * get_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    
    if (!dot || dot == filename) {
        return "";
    }
    
    return dot;
}

size_t get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    return size;
}

unsigned char* fmt_payload(const char *filename, size_t *payload_length) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    size_t file_size = get_file_size(file);
    const char *extension = get_file_extension(filename);
    size_t ext_size = strlen(extension);

    size_t total_size = sizeof(size_t) + file_size + ext_size + 1;
    unsigned char *payload = (unsigned char*)malloc(total_size);
    if (payload == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    memcpy(payload, &file_size, sizeof(size_t));
    fread(payload + sizeof(size_t), 1, file_size, file);
    memcpy(payload + sizeof(size_t) + file_size, extension, ext_size);

    payload[sizeof(size_t) + file_size + ext_size] = '\0';

    fclose(file);
    
    *payload_length = total_size;

    return payload;
}

size_t dfmt_payload(unsigned char* payload, unsigned char* content, unsigned char* extension) {
    size_t file_size = 0;

    memcpy(&file_size, payload, sizeof(size_t));

    memcpy(content, payload + sizeof(size_t), file_size);

    unsigned char* extension_pos = payload + sizeof(size_t) + file_size;
    memcpy(extension, extension_pos, strlen(extension_pos) + 1);

    return file_size;
}

void create_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(1);
    }
    fclose(file);
}

void write_file(const char *filename, unsigned char *content, size_t content_length) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        exit(1);
    }
    fwrite(content, 1, content_length, file);
    fclose(file);
}