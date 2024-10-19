#ifndef _FMT_UTILS_H_
#define _FMT_UTILS_H_

#include <stddef.h>
#include <stdint.h>

uint32_t fmt_encrypted_data(const uint8_t *encrypted_data, uint32_t encrypted_data_length, uint8_t **payload_encrypted_data);

uint32_t fmt_data(const char *filename, uint8_t **payload_data);

uint32_t *dfmt_encrypted_payload(const char *encryptedPayload, uint32_t *encryptedPayloadLength);

uint32_t dfmt_data(unsigned char *payload_data, unsigned char **content, unsigned char **extension);

uint32_t dfmt_prev_encrypted_data(unsigned char *payload_data, unsigned char **content, unsigned char **extension);

#endif  // _FMT_UTILS_H_