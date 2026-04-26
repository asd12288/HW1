#include <stdio.h>

int main() {
    
    unsigned short X = 65535;  // 0xFFFF
    unsigned short Y = 1;

    unsigned short R_low;
    unsigned short R_high = 0;

    unsigned int temp = (unsigned int)X + Y;

    R_low = temp & 0xFFFF;        // lower 16 bits
    if (temp > 0xFFFF) {
        R_high = 1;               // carry
    }

    printf("R_high = %u\n", R_high);
    printf("R_low  = %u\n", R_low);

    return 0;
}