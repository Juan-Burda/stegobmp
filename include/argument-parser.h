#ifndef _ARGUMENT_PARSER_H_
#define _ARGUMENT_PARSER_H_

#include <string.h>
#include <stdbool.h>

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
    bool is_required;
    void * default_value;
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
void add_argument(Subcommand *subcommand, const char *name, ArgType type, const char **choices, int choice_count, bool is_required,
    const char * default_value);
int parse_arguments(ArgParser *parser, int argc, char *argv[]);
Argument* find_argument(Subcommand *subcommand, const char *name);

#endif // _ARGUMENT_PARSER_H_