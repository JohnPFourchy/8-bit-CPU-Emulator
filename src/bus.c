// This file will store connected devices to the bus. In this case, just RAM, cpu and maybe a screen
#include "bus.h"

// The 256 Bytes of RAM
Byte RAM[256];

// write data to an address in RAM
void writeBus(Byte address, Byte data) {
    RAM[address] = data;
}

// read data from an address in RAM
Byte readBus(Byte address) {
    return RAM[address];
}