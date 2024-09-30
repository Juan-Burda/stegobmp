#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../include/subcommands.h"
#include "../../include/constants/cli-arguments.h"
#include "../include/bmp-utils.h"

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
    // TODO
    printf("Ejecutando subcomando embed");
}

void extract_subcommand(ArgParser *parser) {
    // TODO
    printf("Ejecutando subcomando extract");
}