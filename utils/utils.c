#include "utils.h"

void printByte(Byte byte) {
    for(int i = 7; i >= 0; i--) {
        printf("%o", (byte >> i) & 1);
    }
    printf("\n");
}

// returns if a bit at a given location is 1 or 0
Byte getBit(Byte byte, int location) {
    return (byte >> location) & 1;
}

// returns a byte which represents bits in a range of a given byte
Byte getBits(Byte byte, int high, int low) {
    Byte rsum = 0;
    for(int i = 0; i <= high - low; i++) {
        if(((byte >> (i + low)) & 1) == 1) {
            rsum += pow(2, i);
        }
    }
    return rsum;
}