#include <byte-utils.h>

uint8_t get_i_bit(uint8_t byte, int i) {
    return (byte >> i) & 1;
}

void to_big_endian(uint8_t *data, size_t size) {
    for (size_t i = 0; i < size / 2; i++) {
        uint8_t temp = data[i];
        data[i] = data[size - i - 1];
        data[size - i - 1] = temp;
    }
}