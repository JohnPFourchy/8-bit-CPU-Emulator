#include "..\utils\utils.h"
#include "bus.h"
#include "cpu.h"

// create a cpu object
CPU cpu;

// TODO: Eventually take in command line arguments to a file of machine code
// Main loop which loads a program from a file into RAM and executes in a loop
int main(int argc, char *argv[]) {

    if(argc != 2) {
        fprintf(stderr, "Usage: ./cpu.out {path/to/program name.txt}\n");
        exit(1);
    }

    char *name = argv[1];

    FILE *inputfile;
    inputfile = fopen(name, "r");

    if(inputfile == NULL) {
        fprintf(stderr, "Can't open file: %s\n", name);
        exit(1);
    }

    writeInstructionsToRam(inputfile);

    // Now the program is in RAM, we can execute the instructions until END PROGRAM instruction is seen in RAM
    while(readBus(cpu.PC) != 0b01100000) {
        executeInstruction(readBus(cpu.PC));
        cpu.PC += 1;
    }

    // print out the contents of each register in the cpu
    registerDump();
    return 0;
}

void writeInstructionsToRam(FILE *inputfile) {
    // This loop builds up each instruction from the file and writes it to RAM
    Byte instruction = 0;
    Byte a = 0;
    while(!feof(inputfile)) {
        int i = 7;
        while(i >= 0) {
            int r = getc(inputfile);

            // if we reached the end of the file exit out of both loops
            if(r == -1) {
                goto OUT;
            }

            // there is a newline so skip it
            if(r == 10) {
                i += 1;
            }

            // there is a 1 so add to the instruction number
            if(r == 49) {
                instruction += pow(2, i);
            }
            i -= 1;
        }
        writeBus(a, instruction);
        a += 1;
        instruction = 0;
    }
    OUT:fclose(inputfile);
}

// Takes an instruction and determines if it is an ALU instruction or not
void executeInstruction(Byte instruction) {
    Byte topBit = getBit(instruction, 7);
    switch(topBit) {
        case 1:
            executeALU(instruction);
            break;
        case 0:
            execute(instruction);
            break;
    }
}

// Executes an ALU instruction
void executeALU(Byte instruction) {
    Byte opCode = getBits(instruction, 6, 4);
    Byte RA = getBits(instruction, 3, 2);
    Byte RB = getBits(instruction, 1, 0);
    // clear cpu flags before operation in case they are already set
    clearFlags();

    switch(opCode) {
        case 0: // ADD RA, RB   -   Store result into RB
            setRegister(RB, bitwiseAdd(getRegister(RA), getRegister(RB)));
            // Set the cpu zero bit if the operation results in a zero
            if(getRegister(RB) == 0) {
                cpu.ZERO = 1;
            }
            break;
        case 1: // SHL RA, RB   -   Shift the value in RA left once. Store result in RB, the rightmost bit wraps around
            setRegister(RB, shiftLeft(getRegister(RA)));
            break;
        case 2: // SHR RA, RB   -   Shift the value in RB right once. Store result in RB, the leftmost bit wraps around
            setRegister(RB, shiftRight(getRegister(RA)));
            break;
        case 3: // NOT RA, RB   -   Compliment the bits in RA and store in RB
            setRegister(RB, ~getRegister(RA));
            // Set the cpu zero bit if the operation results in a zero
            if(getRegister(RB) == 0) {
                cpu.ZERO = 1;
            }
            break;
        case 4: // AND RA, RB   -   Set RB to the result of bitwise and RA and RB
            setRegister(RB, getRegister(RA) & getRegister(RB));
            // Set the cpu zero bit if the operation results in a zero
            if(getRegister(RB) == 0) {
                cpu.ZERO = 1;
            }
            break;
        case 5: // OR RA, RB    -   Set RB to the result of bitwise or RA and RB
            setRegister(RB, getRegister(RA) | getRegister(RB));
            // Set the cpu zero bit if the operation results in a zero
            if(getRegister(RB) == 0) {
                cpu.ZERO = 1;
            }
            break;
        case 6: // XOR RA, RB   -   Set RB to the result of bitwise XOR
            setRegister(RB, getRegister(RA) ^ getRegister(RB));
            // Set the cpu zero bit if the operation results in a zero
            if(getRegister(RB) == 0) {
                cpu.ZERO = 1;
            }
            break;
        case 7: // CMP RA, RB   -   Compare values. If equal: set cpu EQUAL byte. If RA larger set cpu ALARGER byte.
            if(getRegister(RA) == getRegister(RB)) {
                cpu.EQUAL = 1;
            }

            if(getRegister(RA) > getRegister(RB)) {
                cpu.ALARGER = 1;
            }
            break;
        default:
            fprintf(stderr, "Incorrect opcode: terminating");
            exit(1);
    }

}

// executes a non ALU instruction
void execute(Byte instruction) {
    Byte opCode = getBits(instruction, 6, 4);
    Byte RA = getBits(instruction, 3, 2);
    Byte RB = getBits(instruction, 1, 0);
    Byte nextInstruction = cpu.PC + 1;

    if(opCode == 0 || opCode == 1) { // LB or SB
        lbsb(opCode, RA, RB);
    } else if(opCode == 2) { // DATA Addr
        // Set RB to the next byte in RAM
        setRegister(RB, readBus(nextInstruction));
        // skip the next instruction
        cpu.PC = cpu.PC + 1;
    } else if(opCode == 3) { // JMPR
        cpu.PC = getRegister(RB);
    } else if(opCode == 4) { // JMP Addr
        cpu.PC = readBus(nextInstruction);
        // skip the next instruction
        cpu.PC = cpu.PC + 2;
    } else if(opCode == 5) {
        jumpWithFlags(nextInstruction);
        // skip the next instruction
        cpu.PC = cpu.PC + 2;
        // After a jump with flags clear them
        clearFlags();
    }
} 

void jumpWithFlags(Byte nextInstruction) {
    // JC - Jump if Carry
    if(cpu.CARRYOUT == 1) {
        cpu.PC = readBus(nextInstruction);
    // JA - Jump if A is larger than B
    } else if(cpu.ALARGER == 1) {
        cpu.PC = readBus(nextInstruction);
    // JE - Jump if A and B are equal
    } else if(cpu.EQUAL == 1) {
        cpu.PC = readBus(nextInstruction);
    } else if(cpu.ZERO == 1) {
        cpu.PC = readBus(nextInstruction);
    }
}

void lbsb(Byte opCode, Byte RA, Byte RB) {
    switch(opCode) {
        // Test these
        case 0:
            setRegister(RB, readBus(getRegister(RA)));
            break;
        case 1:
            writeBus(getRegister(RA), getRegister(RB));
            break;
    }
}

Byte shiftRight(Byte RA) {
    Byte b = (RA & 1);

    RA = RA >> 1;
    // wrap the first bit around to the last
    if(b == 1) {
        RA = RA | 1 << 7;
        cpu.CARRYOUT = 1;
    }
    return RA;
}

Byte shiftLeft(Byte RA) {
    // get the seventh bit in RA value
    Byte b = ((RA >> 7) & 1);

    RA = RA << 1;
    // Add the seventh bit to the first if it is a 1
    if(b == 1) {
        RA = RA | 1;
        cpu.CARRYOUT = 1;
    }
    return RA;
}

// Perform a bitwise add on two values in registers
// allows overflow to occur if the addition results in a value greater than 255 -> the result rolls back from 0
Byte bitwiseAdd(Byte RA, Byte RB) {
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
    // set the carryout if overflow is found
    if(sum < RA || sum < RB) {
        cpu.CARRYOUT = 1;
    }
    return sum;
}

// clears the flags in the cpu
void clearFlags() {
    cpu.CARRYOUT = 0;
    cpu.ALARGER = 0;
    cpu.EQUAL = 0;
    cpu.ZERO = 0;
}

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

// Dump out registers and there values in decimal and binary
void registerDump() {
    printf("REGISTER DUMP:\n");
    printf("R0: %d\t", cpu.R0);
    printByte(getRegister(0));

    printf("R1: %d\t", cpu.R1);
    printByte(getRegister(1));

    printf("R2: %d\t", cpu.R2);
    printByte(getRegister(2));

    printf("R3: %d\t", cpu.R3);
    printByte(getRegister(3));
}