#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H

#include <stdio.h>

#define LOG_ERROR_MSG(fmt, ...) fprintf(stderr, "Error: " fmt, ##__VA_ARGS__)

#define UNKNOWN_SUBCOMMAND "Unknown subcommand: %s\n"
#define NO_SUBCOMMAND "No subcommand provided\n"
#define UNKNOWN_ARGUMENT "Unknown argument: %s\n"
#define TOO_MANY_SUBCOMMANDS "Maximum number of subcommands (%d) reached\n"

#define INVALID_STEG_METHOD "The steganography method must be 'lsb1', 'lsb4' or 'lsbi'\n"


#endif // ERROR_MESSAGES