#include <stdio.h>
#include "../include/argument-parser.h"
#include "../include/init-parser.h"
#include "../include/subcommands.h"

int main(int argc, char *argv[]) {
    ArgParser *parser = init_steganography_parser();

    parse_arguments(parser, argc, argv);
    
    run_subcommand(parser);

    /*
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

    */
   free_parser(parser);

    return 0;
}