#ifndef _COLOR_UTILS_H
#define _COLOR_UTILS_H

typedef enum Color {
    BLUE,
    GREEN,
    RED
} Color;

/**
 * Get the next color in the sequence.
 * 
 * @param color The current color.
 * @return The next color in the sequence.
 * 
 * @note The sequence is: BLUE -> GREEN -> RED -> BLUE
 */
Color get_next_color(Color color);

Color get_color_from_byte_index(int byte_index);

#endif