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