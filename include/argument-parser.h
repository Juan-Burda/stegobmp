#ifndef _ARGUMENT_PARSER_H_
#define _ARGUMENT_PARSER_H_

#include <string.h>
#include <stdbool.h>

#define MAX_ARGS 16
#define MAX_ARG_LEN 128
#define MAX_SUBCOMMANDS 2

typedef enum {
    ARG_FLAG,
    ARG_STRING,
    ARG_CHOICE
} ArgType;

typedef struct {
    char *name;
    ArgType type;
    void *value;
    bool is_set;
    char **choices;
    int choice_count;
} Argument;

typedef struct {
    char *name;
    Argument *args;
    int arg_count;
    int arg_capacity;
} Subcommand;

typedef struct {
    Subcommand *subcommands;
    int subcommand_count;
    int subcommand_capacity;
    Subcommand *current_subcommand;
} ArgParser;


ArgParser* create_parser();
void free_parser(ArgParser *parser);
Subcommand* add_subcommand(ArgParser *parser, const char *name);
void add_argument(Subcommand *subcommand, const char *name, ArgType type, const char **choices, int choice_count);
void parse_arguments(ArgParser *parser, int argc, char *argv[]);

#endif // _ARGUMENT_PARSER_H_