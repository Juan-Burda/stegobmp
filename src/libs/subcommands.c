#include <bmp-utils.h>
#include <bmp.h>
#include <constants/error-messages.h>
#include <file-utils.h>
#include <fmt-utils.h>
#include <lsb1.h>
#include <lsb4.h>
#include <lsbi.h>
#include <ssl-utils.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <subcommands.h>
#include "../../include/constants/cli-arguments.h"

typedef void (*SubcommandFunction)(ArgParser *parser);

typedef struct {
    const char *name;
    SubcommandFunction function;
} SubcommandMapping;

void embed_subcommand(ArgParser *parser);
void extract_subcommand(ArgParser *parser);

void extract_encrypted_payload(const unsigned char * carrier_filepath, const unsigned char * output_filename, 
    const unsigned char * stego_method, const unsigned char * encryption_password, const unsigned char * encryption_method, 
    const unsigned char * chaining_mode);
void extract_unencrypted_payload(const unsigned char * carrier_filepath, const unsigned char * output_filename, const unsigned char * stego_method);

SubcommandMapping subcommand_map[] = {
    {CMD_EMBED, embed_subcommand},
    {CMD_EXTRACT, extract_subcommand},
};

void run_subcommand(ArgParser *parser) {
    const char *subcommand_name = parser->current_subcommand->name;

    for (size_t i = 0; i < sizeof(subcommand_map) / sizeof(SubcommandMapping); i++) {
        if (strcmp(subcommand_map[i].name, subcommand_name) == 0) {
            subcommand_map[i].function(parser);
            return;
        }
    }
}

void embed_subcommand(ArgParser *parser) {
    // Get file arguments
    Argument *carrier_filepath_arg = find_argument(parser->current_subcommand, ARG_CARRIER);
    Argument *payload_data_filepath_arg = find_argument(parser->current_subcommand, ARG_PAYLOAD);
    Argument *output_filepath_arg = find_argument(parser->current_subcommand, ARG_OUTPUT);

    const unsigned char *carrier_filepath = (unsigned char *)carrier_filepath_arg->value;
    const unsigned char *payload_data_filepath = (unsigned char *)payload_data_filepath_arg->value;
    const unsigned char *output_filepath = (unsigned char *)output_filepath_arg->value;

    // Get encryption arguments
    Argument *encryption_method_arg = find_argument(parser->current_subcommand, ARG_ENCRYPTION);
    void *encryption_method_arg_value = encryption_method_arg->default_value;
    if (encryption_method_arg->is_set) {
        encryption_method_arg_value = encryption_method_arg->value;
    }
    Argument *chaining_mode_arg = find_argument(parser->current_subcommand, ARG_MODE);
    void *chaining_mode_arg_value = chaining_mode_arg->default_value;
    if (chaining_mode_arg->is_set) {
        chaining_mode_arg_value = chaining_mode_arg->value;
    }
    Argument *encryption_password_arg = find_argument(parser->current_subcommand, ARG_PASSWORD);

    CipherParams *cipher_params = NULL;
    if (encryption_password_arg->value){
        cipher_params = init_cipher_params(encryption_password_arg->value, encryption_method_arg_value, chaining_mode_arg_value);   
    }

    // Get steganography method argument
    Argument *stego_method_arg = find_argument(parser->current_subcommand, ARG_STEGANOGRAPHY);
    const unsigned char *stego_method = (unsigned char *)stego_method_arg->value;

    // Get carrier data (carrier must be a BMP file)
    BMPFileHeader carrier_file_header;
    BMPInfoHeader carrier_info_header;

    uint8_t *carrier_data = read_bmp_data(carrier_filepath, &carrier_file_header, &carrier_info_header);
    if (!carrier_data)
        exit(1);

    // Get payload data
    uint8_t *payload_data = NULL;
    size_t payload_data_length = fmt_data(payload_data_filepath, &payload_data);

    if (encryption_password_arg->value){
        // Encrypt data
        uint8_t *encrypted_data = NULL;
        const size_t encrypted_data_length = encrypt(cipher_params, payload_data, payload_data_length, &encrypted_data);

        // Format encrypted data
        uint8_t *payload_encrypted_data = NULL;
        const size_t payload_encrypted_data_length = fmt_encrypted_data(encrypted_data, encrypted_data_length, &payload_encrypted_data);
        free(encrypted_data);
        free(payload_data);
        payload_data = payload_encrypted_data;
        payload_data_length = payload_encrypted_data_length;
    }

    int bi_width = carrier_info_header.bi_width;
    int bi_height = carrier_info_header.bi_height;
    int bi_bit_count = carrier_info_header.bi_bit_count;

    if (strcmp(stego_method, "lsb1") == 0) {
        lsb1(carrier_data, bi_width, bi_height, bi_bit_count, payload_data, payload_data_length);
    } else if (strcmp(stego_method, "lsb4") == 0) {
        lsb4(carrier_data, bi_width, bi_height, bi_bit_count, payload_data, payload_data_length);
    } else if (strcmp(stego_method, "lsbi") == 0) {
        lsbi(carrier_data, bi_width, bi_height, bi_bit_count, payload_data, payload_data_length);
    } else {
        LOG_ERROR_MSG(INVALID_STEG_METHOD);
        exit(1);
    }

    write_bmp(output_filepath, &carrier_file_header, &carrier_info_header, carrier_data);

    free(payload_data);
    free(carrier_data);

    if (encryption_password_arg->value){
        free_cipher_params(cipher_params);
    }
}

void extract_subcommand(ArgParser *parser) {
    // Get file arguments
    Argument *carrier_filepath_arg = find_argument(parser->current_subcommand, ARG_CARRIER);
    const unsigned char *carrier_filepath = (unsigned char *)carrier_filepath_arg->value;

    Argument *output_filename_arg = find_argument(parser->current_subcommand, ARG_OUTPUT);
    const unsigned char *output_filename = (unsigned char *)output_filename_arg->value;

    // Get steganography method argument
    Argument *stego_method_arg = find_argument(parser->current_subcommand, ARG_STEGANOGRAPHY);
    const unsigned char *stego_method = (unsigned char *)stego_method_arg->value;

    // Get password argument
    Argument *encryption_password_arg = find_argument(parser->current_subcommand, ARG_PASSWORD);
    const unsigned char * encryption_password = (unsigned char *)encryption_password_arg->value;

    if (encryption_password){
        // Get encryption arguments
        Argument *encryption_method_arg = find_argument(parser->current_subcommand, ARG_ENCRYPTION);
        const unsigned char *encryption_method= (unsigned char *) encryption_method_arg->default_value;
        if (encryption_method_arg->is_set) {
            encryption_method = (unsigned char *) encryption_method_arg->value;
        }
        Argument *chaining_mode_arg = find_argument(parser->current_subcommand, ARG_MODE);
        const unsigned char  *chaining_mode = (unsigned char *) chaining_mode_arg->default_value;
        if (chaining_mode_arg->is_set) {
            chaining_mode = (unsigned char *) chaining_mode_arg->value;
        }

        extract_encrypted_payload(carrier_filepath, output_filename, stego_method, encryption_password, encryption_method, chaining_mode);
    }
    else {
        extract_unencrypted_payload(carrier_filepath, output_filename, stego_method);
    }
}

void extract_encrypted_payload(const unsigned char * carrier_filepath, const unsigned char * output_filename, 
    const unsigned char * stego_method, const unsigned char * encryption_password, const unsigned char * encryption_method, 
    const unsigned char * chaining_mode){ 

    // Get carrier data (carrier must be a BMP file)
    BMPFileHeader carrier_file_header;
    BMPInfoHeader carrier_info_header;

    uint8_t *carrier_data = read_bmp_data(carrier_filepath, &carrier_file_header, &carrier_info_header);
    if (!carrier_data)
        exit(1);

    int bi_width = carrier_info_header.bi_width;
    int bi_height = carrier_info_header.bi_height;
    int bi_bit_count = carrier_info_header.bi_bit_count;

    // Extract the payload encrypted data length
    uint8_t payload_encrypted_data_length_buffer[sizeof(size_t)];

    if (strcmp(stego_method, "lsb1") == 0) {
        lsb1_extract(carrier_data, bi_width, bi_height, bi_bit_count, payload_encrypted_data_length_buffer, sizeof(size_t));
    } else if (strcmp(stego_method, "lsb4") == 0) {
        lsb4_extract(carrier_data, bi_width, bi_height, bi_bit_count, payload_encrypted_data_length_buffer, sizeof(size_t));
    } else if (strcmp(stego_method, "lsbi") == 0) {
        lsbi_invert(carrier_data, bi_width, bi_height, bi_bit_count);
        lsbi_extract(carrier_data + sizeof(uint32_t), bi_width, bi_height, bi_bit_count, payload_encrypted_data_length_buffer, sizeof(size_t));
    } else {
        LOG_ERROR_MSG(INVALID_STEG_METHOD);
        free(carrier_data);
        exit(1);
    }

    // Convert the extracted bytes to size_t
    size_t encrypted_data_length;
    memcpy(&encrypted_data_length, payload_encrypted_data_length_buffer, sizeof(size_t));

    CipherParams *cipher_params = init_cipher_params(encryption_password, encryption_method, chaining_mode);  

    // Allocate memory for the full payload
    uint8_t *encrypted_data = (uint8_t *)malloc(encrypted_data_length);
    if (!encrypted_data) {
        fprintf(stderr, "Failed to allocate memory for payload\n");
        free(carrier_data);
        free_cipher_params(cipher_params);
        exit(1);
    }

    // Extract the full payload
    if (strcmp(stego_method, "lsb1") == 0) {
        lsb1_extract(carrier_data + sizeof(size_t) * BITS_PER_BYTE, bi_width, bi_height, bi_bit_count, encrypted_data, encrypted_data_length);
    } else if (strcmp(stego_method, "lsb4") == 0) {
        lsb4_extract(carrier_data + sizeof(size_t) * (BITS_PER_BYTE / 4), bi_width, bi_height, bi_bit_count, encrypted_data, encrypted_data_length);
    } else if (strcmp(stego_method, "lsbi") == 0) {
        lsbi_extract(carrier_data + sizeof(uint32_t) + sizeof(size_t) * (BITS_PER_BYTE + 4), bi_width, bi_height, bi_bit_count, encrypted_data, encrypted_data_length);
    }

    // Decrypt the payload
    uint8_t *payload_data = NULL;
    const long payload_data_length = decrypt(cipher_params, encrypted_data, encrypted_data_length, &payload_data);
    if (payload_data_length < 0) {
        fprintf(stderr, "Error: No se pudo descifrar el payload.\n");
        free(encrypted_data);
        free(carrier_data);
        free_cipher_params(cipher_params);
        exit(1);
    }

    // Deformat the payload
    unsigned char *data = NULL;
    unsigned char *extension = NULL;
    const size_t data_length = dfmt_data(payload_data, &data, &extension);
    if (data_length < 0) {
        fprintf(stderr, "Error: No se pudo desformatear el payload.\n");
        free(encrypted_data);
        free(carrier_data);
        free_cipher_params(cipher_params);
        exit(1);
    }

    // Write the extracted data to a file
    create_file(output_filename);
    write_file(output_filename, data, data_length);

    // Free memory
    free(encrypted_data);
    free(carrier_data);
    free(payload_data);
    free(data);
    free(extension);

    free_cipher_params(cipher_params);

}

void extract_unencrypted_payload(const unsigned char * carrier_filepath, const unsigned char * output_filename, const unsigned char * stego_method){
    // Get carrier data (carrier must be a BMP file)
    BMPFileHeader carrier_file_header;
    BMPInfoHeader carrier_info_header;

    uint8_t *carrier_data = read_bmp_data(carrier_filepath, &carrier_file_header, &carrier_info_header);
    if (!carrier_data)
        exit(1);

    int bi_width = carrier_info_header.bi_width;
    int bi_height = carrier_info_header.bi_height;
    int bi_bit_count = carrier_info_header.bi_bit_count;

    // Extract the payload data length
    uint8_t payload_data_length_buffer[sizeof(size_t)];

    if (strcmp(stego_method, "lsb1") == 0) {
        lsb1_extract(carrier_data, bi_width, bi_height, bi_bit_count, payload_data_length_buffer, sizeof(size_t));
    } else if (strcmp(stego_method, "lsb4") == 0) {
        lsb4_extract(carrier_data, bi_width, bi_height, bi_bit_count, payload_data_length_buffer, sizeof(size_t));
    } else if (strcmp(stego_method, "lsbi") == 0) {
        lsbi_invert(carrier_data, bi_width, bi_height, bi_bit_count);
        lsbi_extract(carrier_data + sizeof(uint32_t), bi_width, bi_height, bi_bit_count, payload_data_length_buffer, sizeof(size_t));
    }

    // Convert the extracted bytes to size_t
    size_t data_length;
    memcpy(&data_length, payload_data_length_buffer, sizeof(size_t));

    // Allocate memory for the full payload
    uint8_t *payload_data= (uint8_t *)malloc(sizeof(size_t) + data_length + 20);
    if (!payload_data) {
        fprintf(stderr, "Failed to allocate memory for payload\n");
        free(carrier_data);
        exit(1);
    }

    memcpy(payload_data, &data_length, sizeof(size_t));
    
    // Extract the full payload
    if (strcmp(stego_method, "lsb1") == 0) {
        lsb1_extract(carrier_data + sizeof(size_t) * BITS_PER_BYTE, bi_width, bi_height, bi_bit_count, payload_data + sizeof(size_t), data_length);
    } else if (strcmp(stego_method, "lsb4") == 0) {
        lsb4_extract(carrier_data + sizeof(size_t) * (BITS_PER_BYTE / 4), bi_width, bi_height, bi_bit_count, payload_data + sizeof(size_t), data_length);
    } else if (strcmp(stego_method, "lsbi") == 0) {
        lsbi_extract(carrier_data + sizeof(uint32_t) + sizeof(size_t) * (BITS_PER_BYTE + 4), bi_width, bi_height, bi_bit_count, payload_data + sizeof(size_t), data_length);
    }

    _lsb1_extract_extension(carrier_data + (sizeof(size_t) + data_length)* BITS_PER_BYTE, 
            bi_width, bi_height, bi_bit_count, payload_data + sizeof(size_t) + data_length, BYTES_PER_PIXEL);

    // Deformat the payload
    unsigned char *data = NULL;
    unsigned char *extension = NULL;
    const size_t content_length = dfmt_data(payload_data, &data, &extension);
    if (content_length < 0) {
        fprintf(stderr, "Error: No se pudo desformatear el payload.\n");
        free(payload_data);
        free(carrier_data);
        exit(1);
    }

    // Write the extracted data to a file
    create_file(output_filename);
    write_file(output_filename, data, content_length);

    // Free memory
    free(carrier_data);
    free(payload_data);
    free(data);
    free(extension);
}