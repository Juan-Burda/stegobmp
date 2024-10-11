#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ssl-utils.h>
#include "../../include/subcommands.h"
#include "../../include/constants/cli-arguments.h"
#include "../include/bmp-utils.h"
#include "../include/lsb1.h"
#include "../include/lsb4.h"
#include "../include/lsbi.h"
#include "../include/bmp-utils.h"
#include "../include/file-utils.h"

typedef void (*SubcommandFunction)(ArgParser *parser);

typedef struct {
    const char *name;
    SubcommandFunction function;
} SubcommandMapping;

void embed_subcommand(ArgParser *parser);
void extract_subcommand(ArgParser *parser);

SubcommandMapping subcommand_map[] = {
    {CMD_EMBED, embed_subcommand},
    {CMD_EXTRACT, extract_subcommand},
};


void run_subcommand(ArgParser *parser) {
    if (!parser->current_subcommand) {
        fprintf(stderr, "Error: No se especificó un subcomando válido.\n");
        return;
    }

    const char *subcommand_name = parser->current_subcommand->name;
    
    for (size_t i = 0; i < sizeof(subcommand_map) / sizeof(SubcommandMapping); i++) {
        if (strcmp(subcommand_map[i].name, subcommand_name) == 0) {
            subcommand_map[i].function(parser);
            return;
        }
    }

    fprintf(stderr, "Error: Subcomando desconocido '%s'.\n", subcommand_name);
}

void embed_subcommand(ArgParser *parser) {
    // Get file arguments
    Argument* carrier_filepath_arg = find_argument(parser->current_subcommand, ARG_CARRIER);
    Argument* payload_data_filepath_arg = find_argument(parser->current_subcommand, ARG_PAYLOAD);
    Argument* output_filepath_arg = find_argument(parser->current_subcommand, ARG_OUTPUT);

    const unsigned char *carrier_filepath =  (unsigned char*)carrier_filepath_arg->value;
    const unsigned char *payload_data_filepath = (unsigned char*)payload_data_filepath_arg->value;
    const unsigned char *output_filepath = (unsigned char*)output_filepath_arg->value;

    // Get encryption arguments
    Argument* encryption_method_arg = find_argument(parser->current_subcommand, ARG_ENCRYPTION);
    Argument* chaining_mode_arg = find_argument(parser->current_subcommand, ARG_MODE);
    Argument* encryption_password_arg = find_argument(parser->current_subcommand, ARG_PASSWORD);

    CipherParams *cipher_params = init_cipher_params(encryption_password_arg->value, encryption_method_arg->value, chaining_mode_arg->value);

    // Get steganography method argument
    Argument* stego_method_arg = find_argument(parser->current_subcommand, ARG_STEGANOGRAPHY);
    const unsigned char* stego_method = (unsigned char*) stego_method_arg->value;

    // Get carrier data (carrier must be a BMP file)
    BMPFileHeader carrier_file_header;
    BMPInfoHeader carrier_info_header;

    uint8_t* carrier_data = read_bmp_data(carrier_filepath, &carrier_file_header, &carrier_info_header);
    if (!carrier_data)
        exit(1);
    
    // Get payload data
    uint8_t *payload_data = NULL;
    const size_t payload_data_length = fmt_data(payload_data_filepath, &payload_data);
   
    // Encrypt data
    uint8_t *encrypted_data = NULL;
    const size_t encrypted_data_length = encrypt(cipher_params, payload_data, payload_data_length, &encrypted_data);

    // Format encrypted data
    uint8_t *payload_encrypted_data = NULL;
    const size_t payload_encrypted_data_length = fmt_encrypted_data(encrypted_data, encrypted_data_length, &payload_encrypted_data);

    int biWidth = carrier_info_header.biWidth;
    int biHeight = carrier_info_header.biHeight;
    int biBitCount = carrier_info_header.biBitCount;

    if (strcmp(stego_method, "lsb1") == 0) {
        lsb1(carrier_data, biWidth, biHeight, biBitCount, payload_encrypted_data, payload_encrypted_data_length);
    } else if (strcmp(stego_method, "lsb4") == 0) {
        lsb4(carrier_data, biWidth, biHeight, biBitCount, payload_encrypted_data, payload_encrypted_data_length);
    } else if (strcmp(stego_method, "lsbi") == 0) {
        lsbi(carrier_data,  biWidth, biHeight, biBitCount, payload_encrypted_data, payload_encrypted_data_length);
    } else {
        fprintf(stderr, "Error: El método de esteganografía debe ser 'lsb1' o 'lsb4'.\n");
        exit(1);
    }

    write_bmp(output_filepath, &carrier_file_header, &carrier_info_header, carrier_data);

    free(payload_encrypted_data);
    free(encrypted_data);
    free(payload_data);
    free(carrier_data);

    free_cipher_params(cipher_params);
}

void extract_subcommand(ArgParser *parser) {
    
}