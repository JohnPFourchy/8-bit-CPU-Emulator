#ifndef CPU_H
#define CPU_H

#include "..\utils\defines.h"

typedef struct CPU {
    // Program counter starts at 0
    Byte PC;

    // Flag bytes - EQUAL and ALARGER are handled by CMP
    // These are set by ALU operations and reset with CLF instruction
    // They are set to zero after a jump instruction
    Byte EQUAL;
    Byte ALARGER;
    Byte ZERO;
    Byte CARRYOUT;

    // 8 bit CPU has four general purpose registers
    Register R0;
    Register R1;
    Register R2;
    Register R3;

} CPU;

// Function definitions
void executeInstruction(Byte instruction);
void writeInstructionsToRam(FILE*);
void executeALU(Byte instruction);
void execute(Byte instruction);
void setRegister(Register, Byte);
Byte getRegister(Register);
Byte bitwiseAdd(Byte raValue, Byte rbValue);
Byte shiftLeft(Byte value);
Byte shiftRight(Byte value);
void lbsb(Byte opCode, Byte, Byte);
void clearFlags();
void jumpWithFlags(Byte nextInstruction);
void registerDump();

#endif /*CPU_H*/