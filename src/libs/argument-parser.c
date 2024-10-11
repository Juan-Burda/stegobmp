#include <stdio.h>
#include <stdlib.h>
#include "../../include/string-utils.h"
#include "../../include/argument-parser.h"
#include "../../include/constants/error-messages.h"
#include "../../include/constants/error-codes.h"
#include <cli-arguments.h>

ArgParser* create_parser() {
    ArgParser *parser = malloc(sizeof(ArgParser));
    parser->subcommands = malloc(MAX_SUBCOMMANDS * sizeof(Subcommand));
    parser->subcommand_count = 0;
    parser->subcommand_capacity = MAX_SUBCOMMANDS;
    parser->current_subcommand = NULL;
    return parser;
}

Subcommand* add_subcommand(ArgParser *parser, const char *name) {
    if (parser->subcommand_count >= 2) {
        LOG_ERROR_MSG(TOO_MANY_SUBCOMMANDS, MAX_SUBCOMMANDS);
        return NULL;
    }

    Subcommand *subcommand = &parser->subcommands[parser->subcommand_count++];
    subcommand->name = strndup(name, MAX_ARG_LEN);
    subcommand->args = NULL;
    subcommand->arg_count = 0;
    subcommand->arg_capacity = 0;

    return subcommand;
}


void add_argument(Subcommand *subcommand, const char *name, ArgType type, const char **choices, int choice_count, bool is_required
    , const char * default_value) {
    if (subcommand->arg_count == subcommand->arg_capacity) {
        int new_capacity = subcommand->arg_capacity == 0 ? 1 : subcommand->arg_capacity * 2;
        subcommand->args = realloc(subcommand->args, new_capacity * sizeof(Argument));
        subcommand->arg_capacity = new_capacity;
    }
    Argument *arg = &subcommand->args[subcommand->arg_count++];
    arg->name = strndup(name, MAX_ARG_LEN);
    arg->type = type;
    arg->value = NULL;
    arg->is_set = false;
    arg->is_required = is_required;
    if (choices && choice_count > 0) {
        arg->choices = malloc(choice_count * sizeof(char*));
        for (int i = 0; i < choice_count; i++) {
            arg->choices[i] = strndup(choices[i], MAX_ARG_LEN);
        }
        arg->choice_count = choice_count;
    } else {
        arg->choices = NULL;
        arg->choice_count = 0;
    }
    if (default_value){
         arg->default_value=strndup(default_value, MAX_ARG_LEN);
    }
    else {
        arg->default_value = NULL;
    }
}

Argument* find_argument(Subcommand *subcommand, const char *name) {
    for (int i = 0; i < subcommand->arg_count; i++) {
        if (strcmp(subcommand->args[i].name, name) == 0) {
            return &subcommand->args[i];
        }
    }
    return NULL;
}

int parse_arguments(ArgParser *parser, int argc, char *argv[]) {
    if (argc < 2) {
        LOG_ERROR_MSG(NO_SUBCOMMAND);
        return ERROR;
    }

    for (int i = 0; i < parser->subcommand_count; i++) {
        if (strcmp(parser->subcommands[i].name, argv[1]) == 0) {
            parser->current_subcommand = &parser->subcommands[i];
            break;
        }
    }

    if (!parser->current_subcommand) {
        LOG_ERROR_MSG(UNKNOWN_SUBCOMMAND, argv[1]);
        return ERROR;
    }

    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            Argument *arg = find_argument(parser->current_subcommand, &argv[i][1]);
            if (arg) {
                arg->is_set = true;
                switch (arg->type) {
                    case ARG_FLAG:
                        arg->value = malloc(sizeof(bool));
                        *(bool*)arg->value = true;
                        break;
                    case ARG_STRING:
                    case ARG_CHOICE:
                        if (i + 1 < argc) {
                            arg->value = strndup(argv[++i], MAX_ARG_LEN);
                        }
                        break;
                }
            } else {
                LOG_ERROR_MSG(UNKNOWN_ARGUMENT, argv[i]);
                return ERROR;
            }
        }
    }

    for (int i = 0; i < parser->current_subcommand->arg_count; i++) {
        Argument *arg = &parser->current_subcommand->args[i];
        if (arg->is_required && !arg->is_set) {
            LOG_ERROR_MSG(MISSING_REQUIRED_ARGUMENT, arg->name);
            return ERROR;
        }
    }

    return SUCCESS;
}

void free_parser(ArgParser *parser) {
    for (int i = 0; i < parser->subcommand_count; i++) {
        Subcommand *subcommand = &parser->subcommands[i];
        free(subcommand->name);
        for (int j = 0; j < subcommand->arg_count; j++) {
            Argument *arg = &subcommand->args[j];
            free(arg->name);
            free(arg->value);
            for (int k = 0; k < arg->choice_count; k++) {
                free(arg->choices[k]);
            }
            free(arg->choices);
            if (arg->default_value){
                free(arg->default_value);
            }
        }
        free(subcommand->args);
    }
    free(parser->subcommands);
    free(parser);
}