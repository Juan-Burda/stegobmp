#include <byte-utils.h>

uint8_t get_i_bit(uint8_t byte, int i) {
    return (byte >> i) & 1;
}