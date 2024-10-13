#ifndef _FMT_UTILS_H_
#define _FMT_UTILS_H_

#include <stddef.h>
#include <stdint.h>

size_t fmt_encrypted_data(const uint8_t *encrypted_data, size_t encrypted_data_length, uint8_t **payload_encrypted_data);

size_t fmt_data(const char *filename, uint8_t **payload_data);

unsigned char *dfmt_encrypted_payload(const char *encryptedPayload, size_t *encryptedPayloadLength);

size_t dfmt_data(unsigned char *payload_data, unsigned char **content, unsigned char **extension);

#endif  // _FMT_UTILS_H_