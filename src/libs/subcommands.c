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
    Argument* bmpFilePathArg = find_argument(parser->current_subcommand, ARG_CARRIER);
    Argument* payloadFilePathArg = find_argument(parser->current_subcommand, ARG_PAYLOAD);

    Argument* bmpFilePathOutputArg = find_argument(parser->current_subcommand, ARG_OUTPUT);

    const unsigned char *bmpFilePath =  (unsigned char*)bmpFilePathArg->value;
    const unsigned char *payloadFilePath = (unsigned char*)payloadFilePathArg->value;
    const unsigned char *bmpFilePathOutput = (unsigned char*)bmpFilePathOutputArg->value;
    const unsigned char *outputFileName = "resources/output";

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    uint8_t* bmpData = read_bmp_data(bmpFilePath, &fileHeader, &infoHeader);
    if (!bmpData)
        exit(1);

    const size_t payloadLength;
    const unsigned char* payload = fmt_payload(payloadFilePath, &payloadLength);

    Argument* encMethodArg = find_argument(parser->current_subcommand, ARG_ENCRYPTION);
    Argument* chainingModeArg = find_argument(parser->current_subcommand, ARG_MODE);
    Argument* passwordArg = find_argument(parser->current_subcommand, ARG_PASSWORD);

    CipherParams *cipherParams = init_cipher_params(passwordArg->value, encMethodArg->value, chainingModeArg->value);

    uint8_t *encryptedPayload = NULL;
    size_t encryptedPayloadLength = encrypt(cipherParams, payload, payloadLength, &encryptedPayload);

    const unsigned char* fmtEncryptedPayload = fmt_encrypted_payload(encryptedPayload, &encryptedPayloadLength);

    Argument* stegMethodArg = find_argument(parser->current_subcommand, ARG_STEGANOGRAPHY);
    if (strcmp(stegMethodArg->value, "lsb4") == 0) {
        lsb4(bmpData, infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount, fmtEncryptedPayload, encryptedPayloadLength);
    } else if (strcmp(stegMethodArg->value, "lsb1") == 0) {
        lsb1(bmpData, infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount, fmtEncryptedPayload, encryptedPayloadLength);
    } else if (strcmp(stegMethodArg->value, "lsbi") == 0) {
        lsbi(bmpData, infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount, fmtEncryptedPayload, encryptedPayloadLength);
    } 
    else {
        fprintf(stderr, "Error: El método de esteganografía debe ser 'lsb1' o 'lsb4'.\n");
        exit(1);
    }

    write_bmp(bmpFilePathOutput, &fileHeader, &infoHeader, bmpData);

    free_cipher_params(cipherParams);

    free(encryptedPayload);
    free(fmtEncryptedPayload);

    free(bmpData);
    free(payload);
}

void extract_subcommand(ArgParser *parser) {

}