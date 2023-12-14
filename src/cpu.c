#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// both bytes and registers hold 8 bits exactly each
#define Byte uint8_t
#define Register Byte

typedef struct CPU {
    // Program counter starts at 0
    Byte PC;
    // 8 bit CPU will have 256 Ram locations that store one byte each
    Byte RAM[256];

    // 8 bit CPU has four general purpose registers
    Register R0;
    Register R1;
    Register R2;
    Register R3;

} CPU;

// create a cpu object
CPU cpu;

// Function definitions
void printByte(Byte);
Byte getBit(Byte, int);
Byte getBits(Byte, int, int);
void executeInstruction(Byte);
void executeALU(Byte);
void execute(Byte);
void setRegister(Register, Byte);
Byte getRegister(Register);
Byte bitwiseAdd(Register, Register);

// TODO: Eventually take in command line arguments to a file of machine code
// TODO: Eventually add ability to display to screen
// Main loop which loads a program from a file into RAM, gets an instruction, and executes in a loop
int main(void) {

    return EXIT_SUCCESS;
}

// Takes an instruction and determines if it is an ALU instruction or not
void executeInstruction(Byte instruction) {
    Byte topBit = getBit(instruction, 7);
    switch(topBit) {
        case 1:
            executeALU(instruction);
            break;
        case 0:
            //execute();
            break;
    }
}

// Executes an ALU instruction
void executeALU(Byte instruction) {
    Byte opCode = getBits(instruction, 6, 4);
    Byte RA = getBits(instruction, 3, 2);
    Byte RB = getBits(instruction, 1, 0);

    switch(opCode) {
        case 0: // ADD RA, RB   -   Store result into RB
            setRegister(RB, bitwiseAdd(getRegister(RA), getRegister(RB)));
            break;
        case 1:
            // SHL
            break;
        case 2:
            // SHR
            break;
        case 3:
            // NOT
            break;
        case 4:
            // AND
            break;
        case 5:
            // OR
            break;
        case 6:
            // XOR
            break;
        case 7:
            // CMP
            break;
        default:
            fprintf(stderr, "Incorrect opcode: terminating");
            exit(1);
    }

}

//void execute() {}

// Sets a registers value
void setRegister(Register reg, Byte value) {
    switch(reg) {
        case 0:
            cpu.R0 = value;
            break;
        case 1:
            cpu.R1 = value;
            break;
        case 2:
            cpu.R2 = value;
            break;
        case 3:
            cpu.R3 = value;
            break;
        default:
            fprintf(stderr, "Register not valid");
            exit(1);
    }
}

// Gets the value at a specific register
Byte getRegister(Register reg) {
    Register ret;
    switch(reg) {
        case 0:
            ret = cpu.R0;
            break;
        case 1:
            ret = cpu.R1;
            break;
        case 2:
            ret = cpu.R2;
            break;
        case 3:
            ret = cpu.R3;
            break;
        default:
            fprintf(stderr, "Register not valid");
            exit(1);
    }
    return ret;
}

// Perform a bitwise add on two values in registers
// allows overflow to occur if the addition results in a value greater than 255 -> the result rolls back from 0
Byte bitwiseAdd(Register RA, Register RB) {
    Byte sum = 0;
    Byte carry = 0;
    for(int i = 0; i < 8; i++) {
        // if both bits are 1 set a carry
        if(getBit(RA, i) == 1 && getBit(RB, i) == 1) {
            if(carry == 1) {
                sum += pow(2, i);
            }
            carry = 1;
        } else if(getBit(RA, i) == 0 && getBit(RB, i) == 0) {
            if(carry == 1) {
                sum += pow(2, i);
            }
            carry = 0;
        } else {
            if(carry != 1) {
                sum += pow(2, i);
            }
        }
    }
    return sum;
}

// TODO: Move these to a utils file
// helper function for testing
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
