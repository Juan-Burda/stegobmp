#ifndef _BYTE_UTILS_H
#define _BYTE_UTILS_H

#include <stddef.h>
#include <stdint.h>

/**
 * Get the i-th bit of a byte, starting from the rightmost bit.
 * 
 * @param byte The byte to extract the bit from.
 * @param i The index of the bit to extract. Must be between 0 and 7.
 * 
 * @return The i-th bit of the byte.
 */
uint8_t get_i_bit(uint8_t byte, int i);

/**
 * Get the i-th nibble of a byte, starting from the rightmost nibble.
 * 
 * @param byte The byte to extract the nibble from.
 * @param i The index of the nibble to extract. Must be between 0 and 1.
 * 
 * @return The i-th nibble of the byte.
 */
uint8_t get_i_nibble(uint8_t byte, uint8_t i);

/**
 * Transform a byte array to big-endian.
 */
void to_big_endian(uint8_t *data, size_t size);

#endif  // _BYTE_UTILS_H