#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define LOG_ERROR_MSG(fmt) fprintf(stderr, "stegobmp: " fmt "\n")
#define LOG_ERROR_MSG_ARGS(fmt, ...) fprintf(stderr, "stegobmp: " fmt "\n", ##__VA_ARGS__)

#define LOG_ERROR_MSG_WITH_ERRNO(fmt)                     \
    do {                                                  \
        fprintf(stderr, "stegobmp: " fmt);                \
        fprintf(stderr, "%s\n", strerror(errno));         \
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

#define INVALID_BMP_FILE "not a valid BMP file"
#define INVALID_BMP_BIT_FILE "not a 24-bit MBP file"

#define MEMORY_ERROR "memory allocation failed!"

#define MEMORY_PAYLOAD "failed to allocate memory for payload"
#define DMFT_PAYLOAD "cannot deformat payload"
#define DECRYPT_PAYLOAD "cannot decrypt payload"

#define CIPHER_CONTEXT_ERROR "error creating cipher context"

#define ENCRYPTION_INITIALIZE_ERROR "error initializing encryption"
#define CIPHERTEXT_ALLOCATION_ERROR "error allocating memory for ciphertext"
#define ENCRYPT_DATA_ERROR "error encrypting data"
#define ENCRYPTION_FINALIZE_ERROR "error finalizing encryption"

#define DECRYPTION_INITIALIZE_ERROR "error initializing decryption"
#define PLAINTEXT_ALLOCATION_ERROR "error allocating memory for plaintext"
#define DECRYPT_DATA_ERROR "error decrypting data"
#define DECRYPTION_FINALIZE_ERROR "error finalizing decryption"

#endif  // ERROR_MESSAGES_H