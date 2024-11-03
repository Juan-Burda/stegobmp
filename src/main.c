#include <argument-parser.h>
#include <bmp-utils.h>
#include <constants/error-codes.h>
#include <init-parser.h>
#include <lsb1.h>
#include <lsbi.h>
#include <ssl-utils.h>
#include <stdint.h>
#include <stdio.h>
#include <subcommands.h>

int main(int argc, char *argv[]) {
    ArgParser *parser = init_steganography_parser();

    int parse_value = parse_arguments(parser, argc, argv);
    if (parse_value != SUCCESS) {
        free_parser(parser);
        return parse_value;
    }

    run_subcommand(parser);

    free_parser(parser);

    return SUCCESS;
}