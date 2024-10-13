#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 200809L

#include <string-utils.h>
#include <string.h>

/**
 * @brief Duplicate a string up to n characters
 * 
 * @param s The source string
 * @param n The maximum number of characters to copy
 * 
 * @return char* A pointer to the newly allocated string
 */
char *strndup(const char *s, const size_t n) {
    size_t len = strnlen(s, n);
    char *dup = (char *)calloc(len + 1, sizeof(char));
    if (dup) {
        strncpy(dup, s, len);
        dup[len] = '\0';
    }

    return dup;
}