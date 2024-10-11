#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

const char *get_file_extension(const char *filename);

size_t get_file_size(FILE *file);

size_t fmt_encrypted_data(const uint8_t* encrypted_data, size_t encrypted_data_length, uint8_t **payload_encrypted_data);

size_t fmt_data(const char *filename, uint8_t** payload_data);

unsigned char* dfmt_encrypted_payload(const char* encryptedPayload, size_t *encryptedPayloadLength);

size_t dfmt_payload(unsigned char *payload, unsigned char *content, unsigned char *extension);

void create_file(const char *filename);

void write_file(const char *filename, unsigned char *content, size_t content_length);

#endif  // FILE_UTILS_H
