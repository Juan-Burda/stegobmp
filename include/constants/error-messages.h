#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define LOG_ERROR_MSG(fmt, ...) fprintf(stderr, "stegobmp: " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR_MSG_WITH_ERRNO(fmt, ...) \
    do { \
        fprintf(stderr, "stegobmp: " fmt, ##__VA_ARGS__); \
        fprintf(stderr, "%s\n", strerror(errno)); \
    } while (0)


#define UNKNOWN_SUBCOMMAND "unknown subcommand: '%s'."
#define NO_SUBCOMMAND "no subcommand provided."
#define UNKNOWN_ARGUMENT "unknown argument: '%s'."
#define TOO_MANY_SUBCOMMANDS "maximum number of subcommands (%d) reached."
#define MISSING_REQUIRED_ARGUMENT "missing required argument: '%s'."

#define INVALID_STEG_METHOD "-steg: unknown value '%s'. Must be 'LSB1', 'LSB4' or 'LSBI'."
#define INVALID_ENC_METHOD "-a: unknown value '%s'. Must be 'aes128', 'aes192', 'aes256' or '3des'."
#define INVALID_MODE_METHOD "-m: unknown value '%s'. Must be 'ecb', 'cfb', 'ofb' or 'cbc'."

#define FILE_OPEN_ERROR "error opening file: "
#define FILE_CREATE_ERROR "error creating file:"

#define INVALID_MBP_FILE "not a valid BMP file"
#define INVALID_MBP_BIT_FILE "not a 24-bit MBP file"

#define MEMORY_ERROR "memory allocation failed!"

#endif // ERROR_MESSAGES_H