#include <stdio.h>
#include "../include/bmp-utils.h"

int main() {
    printf("Reading BMP file header...\n");

    const char *bmpFilePath = "resources/DON_CANGREJO.bmp";
    readBMPHeader(bmpFilePath);
    return 0;
}