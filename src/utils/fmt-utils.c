#define _DEFAULT_SOURCE

#include <constants/bmp.h>
#include <endian.h>
#include <file-utils.h>
#include <stdlib.h>
#include <string.h>
#include <error-messages.h>

uint32_t fmt_encrypted_data(const uint8_t *encrypted_data, uint32_t encrypted_data_length, uint8_t **payload_encrypted_data) {
    uint32_t totalSize = sizeof(uint32_t) + encrypted_data_length;
    *payload_encrypted_data = (uint8_t *)malloc(totalSize);
    if (payload_encrypted_data == NULL) {
        LOG_ERROR_MSG_WITH_ERRNO(MEMORY_ERROR);
        return -1;
    }

    uint32_t be_encrypted_data_length = encrypted_data_length;
    be_encrypted_data_length = htobe32(be_encrypted_data_length);  // !!
    memcpy(*payload_encrypted_data, &be_encrypted_data_length, sizeof(uint32_t));

    memcpy(*payload_encrypted_data + sizeof(uint32_t), encrypted_data, encrypted_data_length);

    return totalSize;
}

unsigned char *dfmt_encrypted_payload(const char *encryptedPayload, uint32_t *encryptedPayloadLength) {
    unsigned char *payload = (unsigned char *)malloc(*encryptedPayloadLength);
    if (payload == NULL) {
        LOG_ERROR_MSG_WITH_ERRNO(MEMORY_ERROR);
        return NULL;
    }

    memcpy(payload, encryptedPayload + sizeof(uint32_t), *encryptedPayloadLength);
    return payload;
}

uint32_t fmt_data(const char *filename, uint8_t **payload_data) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        LOG_ERROR_MSG_WITH_ERRNO(FILE_OPEN_ERROR);
        return -1;
    }

    uint32_t file_size = get_file_size(file);
    const char *extension = get_file_extension(filename);
    uint32_t ext_size = strlen(extension);

    uint32_t total_size = sizeof(uint32_t) + file_size + ext_size + 1;

    *payload_data = (uint8_t *)malloc(total_size);
    if (*payload_data == NULL) {
        LOG_ERROR_MSG_WITH_ERRNO(MEMORY_ERROR);
        return -1;
    }

    uint32_t be_file_size = file_size;
    be_file_size = htobe32(be_file_size);  // !!
    memcpy(*payload_data, &be_file_size, sizeof(uint32_t));
    fread(*payload_data + sizeof(uint32_t), 1, file_size, file);
    memcpy(*payload_data + sizeof(uint32_t) + file_size, extension, ext_size);

    (*payload_data)[sizeof(uint32_t) + file_size + ext_size] = '\0';

    fclose(file);

    return total_size;
}

uint32_t dfmt_data(unsigned char *payload_data, unsigned char **content, unsigned char **extension) {
    uint32_t file_size = 0;

    memcpy(&file_size, payload_data, sizeof(uint32_t));

    *content = (unsigned char *)malloc(file_size);
    if (*content == NULL) {
        LOG_ERROR_MSG_WITH_ERRNO(MEMORY_ERROR);
        return -1;
    }
    memcpy(*content, payload_data + sizeof(uint32_t), file_size);

    unsigned char *extension_pos = payload_data + sizeof(uint32_t) + file_size;
    uint32_t extension_length = strlen((char *)extension_pos) + 1;
    *extension = (unsigned char *)malloc(extension_length);
    if (*extension == NULL) {
        LOG_ERROR_MSG_WITH_ERRNO(MEMORY_ERROR);
        return -1;
    }

    memcpy(*extension, extension_pos, extension_length);

    return file_size;
}

uint32_t dfmt_prev_encrypted_data(unsigned char *payload_data, unsigned char **content, unsigned char **extension) {
    uint32_t file_size = 0;

    memcpy(&file_size, payload_data, sizeof(uint32_t));
    file_size = be32toh(file_size);  // !!

    *content = (unsigned char *)malloc(file_size);
    if (*content == NULL) {
        LOG_ERROR_MSG_WITH_ERRNO(MEMORY_ERROR);
        return -1;
    }
    memcpy(*content, payload_data + sizeof(uint32_t), file_size);

    unsigned char *extension_pos = payload_data + sizeof(uint32_t) + file_size;
    uint32_t extension_length = strlen((char *)extension_pos) + 1;
    *extension = (unsigned char *)malloc(extension_length);
    if (*extension == NULL) {
        LOG_ERROR_MSG_WITH_ERRNO(MEMORY_ERROR);
        return -1;
    }

    memcpy(*extension, extension_pos, extension_length);

    return file_size;
}