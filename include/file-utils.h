#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

const char *get_file_extension(const char *filename);

size_t get_file_size(FILE *file);

void create_file(const char *filename);

void write_file(const char *filename, unsigned char *content, size_t content_length);

#endif  // FILE_UTILS_H
