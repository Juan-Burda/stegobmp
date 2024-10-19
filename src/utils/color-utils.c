#include <bmp.h>
#include <color-utils.h>

Color get_next_color(Color color) {
    switch (color) {
        case RED:
            return BLUE;
        case GREEN:
            return RED;
        case BLUE:
            return GREEN;
        default:  // Should never happen
            return RED;
    }
}

Color get_color_from_byte_index(int byte_index) {
    switch (byte_index % BYTES_PER_PIXEL) {
        case 0:
            return BLUE;
        case 1:
            return GREEN;
        case 2:
            return RED;
        default:  // Should never happen
            return RED;
    }
}