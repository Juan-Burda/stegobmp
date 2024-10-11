#include <constants/bmp.h>
#include <file-utils.h>
#include <stdlib.h>
#include <string.h>

const char *get_file_extension(const char *filename) {
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

size_t fmt_encrypted_data(const uint8_t* encrypted_data, size_t encrypted_data_length, uint8_t **payload_encrypted_data) {
    size_t totalSize = sizeof(size_t) + encrypted_data_length;
    *payload_encrypted_data = (uint8_t*) malloc(totalSize);
    if (payload_encrypted_data == NULL) {
        perror("Memory allocation failed");
        return -1;
    }
    memcpy(*payload_encrypted_data, &encrypted_data_length, sizeof(size_t));
    memcpy(*payload_encrypted_data + sizeof(size_t), encrypted_data, encrypted_data_length);

    return totalSize;
}

unsigned char* dfmt_encrypted_payload(const char* encryptedPayload, size_t *encryptedPayloadLength) {
    unsigned char* payload = (unsigned char*)malloc(*encryptedPayloadLength);
    if (payload == NULL) {
        perror("Memory allocation failed"); 
        return NULL;
    }

    memcpy(payload, encryptedPayload + sizeof(size_t), *encryptedPayloadLength);    
    return payload;
}

size_t fmt_data(const char *filename, uint8_t** payload_data) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    size_t file_size = get_file_size(file);
    const char *extension = get_file_extension(filename);
    size_t ext_size = strlen(extension);

    size_t total_size = sizeof(size_t) + file_size + ext_size + 1;
    
    *payload_data = (uint8_t *)malloc(total_size);
    if (*payload_data == NULL) {
        perror("Memory allocation failed"); 
        return -1;
    }

    memcpy(*payload_data, &file_size, sizeof(size_t));
    fread(*payload_data + sizeof(size_t), 1, file_size, file);
    memcpy(*payload_data + sizeof(size_t) + file_size, extension, ext_size);

    (*payload_data)[sizeof(size_t) + file_size + ext_size] = '\0';

    fclose(file);

    return total_size;
}

size_t dfmt_payload(unsigned char *payload, unsigned char *content, unsigned char *extension) {
    size_t file_size = 0;

    memcpy(&file_size, payload, sizeof(size_t));

    memcpy(content, payload + sizeof(size_t), file_size);

    unsigned char *extension_pos = payload + sizeof(size_t) + file_size;
    memcpy(extension, extension_pos, strlen((char *)extension_pos) + 1);

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