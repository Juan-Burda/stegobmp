#include <stdio.h>
#include "../include/bmp-utils.h"
#include "../include/argument-parser.h"
#include "../include/init-parser.h"

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

    free_parser(parser);

    return 0;
}