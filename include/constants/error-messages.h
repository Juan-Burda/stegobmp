#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H

#include <stdio.h>

#define LOG_ERROR_MSG(fmt, ...) fprintf(stderr, "stegobmp: " fmt, ##__VA_ARGS__)

#define UNKNOWN_SUBCOMMAND "unknown subcommand: '%s'.\n"
#define NO_SUBCOMMAND "no subcommand provided\n."
#define UNKNOWN_ARGUMENT "unknown argument: '%s'\n."
#define TOO_MANY_SUBCOMMANDS "maximum number of subcommands (%d) reached\n."
#define MISSING_REQUIRED_ARGUMENT "missing required argument: '%s'.\n"

#define INVALID_STEG_METHOD "-steg: unknown value '%s'. Must be 'lsb1', 'lsb4' or 'lsbi'.\n"
#define INVALID_ENC_METHOD "-a: unknown value '%s'. Must be 'aes128', 'aes192', 'aes256' or '3des'.\n"
#define INVALID_MODE_METHOD "-m: unknown value '%s'. Must be 'ecb', 'cfb', 'ofb' or 'cbc'.\n"

#endif // ERROR_MESSAGES_H