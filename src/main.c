#include <stdio.h>
#include "../include/bmp-utils.h"
#include "../include/argument-parser.h"
#include "../include/init-parser.h"
#include "../include/lsb1.h"
#include "../include/constants/cli-arguments.h"

int main(int argc, char *argv[]) {
    ArgParser *parser = init_steganography_parser();

    parse_arguments(parser, argc, argv);

    if (parser->current_subcommand) {
        printf("Subcommand: %s\n", parser->current_subcommand->name);
        for (int i = 0; i < parser->current_subcommand->arg_count; i++) {
            Argument *arg = &parser->current_subcommand->args[i];
            if (arg->is_set) {
                printf("%s: %s\n", arg->name, (char*)arg->value);
            }
        }
    }

    Argument* bmpFilePathArg = find_argument(parser->current_subcommand, ARG_INPUT);
    Argument* bmpFilePathOutputArg = find_argument(parser->current_subcommand, ARG_OUTPUT);

    const char *bmpFilePath =  (char*)bmpFilePathArg->value;
    const char *bmpFilePathOutput = (char*)bmpFilePathOutputArg->value;

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    read_bmp_headers(bmpFilePath);

    uint8_t* bmpData = read_bmp_data(bmpFilePath, &fileHeader, &infoHeader);
    if (bmpData) {
        const char* message = "Secret message!";

        lsb1(bmpData, infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount, message);

        write_bmp(bmpFilePathOutput, &fileHeader, &infoHeader, bmpData);

        free(bmpData);
    }

    uint8_t* bmpSecretData = read_bmp_data(bmpFilePathOutput, &fileHeader, &infoHeader);
    if (bmpSecretData) {
        char extractedMessage[30];
        lsb1_extract(bmpSecretData, infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount, extractedMessage, sizeof(extractedMessage));

        free(bmpSecretData);
    }

    free_parser(parser);

    return 0;
}