#ifndef FILE_UTILS_H
#define FILE_UTILS_H

const char * get_file_extension(const char *filename);

DWORD get_file_size(FILE *file);

unsigned char* create_bit_string(const char *filename);

#endif // FILE_UTILS_H