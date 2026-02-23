#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void) {
    char input[32];
    scanf("%31s", input);
    
    uint32_t x = strtoul(input, NULL, 0);
    
    uint8_t b3 = (x >> 24) & 0xFF;
    uint8_t b2 = (x >> 16) & 0xFF;
    uint8_t b1 = (x >> 8) & 0xFF;
    uint8_t b0 = x & 0xFF;
    
    printf("Big-endian bytes: %02X %02X %02X %02X\n", b3, b2, b1, b0);
    printf("Little-endian bytes: %02X %02X %02X %02X\n", b0, b1, b2, b3);
    
    uint32_t swapped = ((x & 0x000000FFu) << 24) |
                       ((x & 0x0000FF00u) << 8) |
                       ((x & 0x00FF0000u) >> 8) |
                       ((x & 0xFF000000u) >> 24);


    printf("Swapped: 0x%08X\n", swapped);
    
    return 0;
}