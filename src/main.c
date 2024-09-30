#include <stdio.h>
#include "../include/bmp-utils.h"
#include "../include/file-utils.h"
#include "../include/argument-parser.h"
#include "../include/init-parser.h"
#include "../include/lsb1.h"
#include "../include/constants/cli-arguments.h"

int main(int argc, char *argv[]) {
    ArgParser *parser = init_steganography_parser();

    parse_arguments(parser, argc, argv);

    Argument* bmpFilePathArg = find_argument(parser->current_subcommand, ARG_CARRIER);
    Argument* payloadFilePathArg = find_argument(parser->current_subcommand, ARG_PAYLOAD);

    Argument* bmpFilePathOutputArg = find_argument(parser->current_subcommand, ARG_OUTPUT);

    const unsigned char *bmpFilePath =  (unsigned char*)bmpFilePathArg->value;
    const unsigned char *payloadFilePath = (unsigned char*)payloadFilePathArg->value;
    const unsigned char *bmpFilePathOutput = (unsigned char*)bmpFilePathOutputArg->value;
    const unsigned char *outputFileName = "resources/output";

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // Embed data
    uint8_t* bmpData = read_bmp_data(bmpFilePath, &fileHeader, &infoHeader);
    if (!bmpData)
        exit(1);

    const size_t payloadLength;
    const char* payload = fmt_payload(payloadFilePath, &payloadLength);

    lsb1(bmpData, infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount, payload, payloadLength);

    write_bmp(bmpFilePathOutput, &fileHeader, &infoHeader, bmpData);

    free(bmpData);
    free(payload);

    // Extract data
    uint8_t* bmpSecretData = read_bmp_data(bmpFilePathOutput, &fileHeader, &infoHeader);
    if (!bmpSecretData) {
        exit(1);
    }

    char *extractedMessage = calloc(payloadLength, sizeof(char));
    lsb1_extract(bmpSecretData, infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount, extractedMessage, payloadLength);

    char content[payloadLength];
    char extension[payloadLength];
    size_t extractedMessageLength = dfmt_payload(extractedMessage, content, extension);

    char *outputFilePathWithExtension = (char*)malloc(strlen((char*)outputFileName) + strlen((char*)extension) + 1);
    sprintf(outputFilePathWithExtension, "%s%s", (char*)outputFileName, (char*)extension);
    
    create_file(outputFilePathWithExtension);
    write_file(outputFilePathWithExtension, content, extractedMessageLength);
    
    free(outputFilePathWithExtension);
    free(bmpSecretData);
    free(extractedMessage);

    free_parser(parser);

    return 0;
}