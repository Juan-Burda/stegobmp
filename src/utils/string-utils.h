#include <string.h>
#include "../../include/string-utils.h"

char *strndup(const char *s, const size_t n) {
    size_t len = strnlen(s, n);            // Find the length of the source string, but no more than n
    char *dup = malloc(len + 1);           // Allocate space for the copied string (+1 for the null terminator)
    if(dup) {
        strncpy(dup, s, len);              // Copy the string up to len characters
        dup[len] = '\0';                   // Ensure the string is null-terminated
    }

    return dup;
}