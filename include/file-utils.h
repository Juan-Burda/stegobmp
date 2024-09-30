#ifndef FILE_UTILS_H
#define FILE_UTILS_H

const char * get_file_extension(const char *filename);

size_t get_file_size(FILE *file);

unsigned char* fmt_payload(const char *filename, size_t *payload_length);

size_t dfmt_payload(unsigned char* payload, unsigned char* content, unsigned char* extension);

void create_file(const char *filename);

void write_file(const char *filename, unsigned char *content, size_t content_length);

#endif // FILE_UTILS_H
