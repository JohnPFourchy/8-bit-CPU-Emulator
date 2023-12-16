#include "..\src\bus.h"
#include "..\src\cpu.h"
#include <assert.h>
#include "..\utils\utils.h"

CPU cpu;
void runOtherTests();
void runTestSuites();
void runALUTests();

int main(void) {

    runTestSuites();

    return 0;
}


void runTestSuites() {
    printf("Running testsuite: ALU\n");
    runALUTests();
    printf("Testsuite passed: ALU\n");
    clearFlags();
    runOtherTests();
    printf("Testsuite passed: Other\n");
}

void runALUTests() {
    // Test ADD
    cpu.R0 = 1;
    cpu.R1 = 255;
    executeInstruction(0b10000001);
    assert(cpu.R1 == 0);
    printf("ADD 1, 255  :     => OK\n");

    cpu.R2 = 17;
    cpu.R3 = 100;
    executeInstruction(0b10001011);
    assert(cpu.R3 == 117);
    printf("ADD 17, 100 :     => OK\n");
    
    printf("\n");

    // Test SHL
    cpu.R0 = 128;
    executeInstruction(0b10010000);
    assert(cpu.R0 == 1);
    printf("SHL 1:  =>  OK\n");

    cpu.R0 = 30;
    executeInstruction(0b10010000);
    assert(cpu.R0 == 0b00111100);
    printf("SHL 30:  =>  OK\n");

    printf("\n");

    // Test SHR
    cpu.R0 = 2;
    executeInstruction(0b10100000);
    assert(cpu.R0 == 1);
    printf("SHR 2:  =>  OK\n");

    cpu.R0 = 1;
    executeInstruction(0b10100000);
    assert(cpu.R0 == 128);
    printf("SHR 1:  =>  OK\n");

    printf("\n");

    // Test NOT
    cpu.R3 = 255;
    executeInstruction(0b10111111);
    assert(cpu.R3 == 0);
    printf("NOT 255:    =>  OK\n");

    cpu.R3 = 1;
    executeInstruction(0b10111111);
    assert(cpu.R3 == 254);
    printf("NOT 1:    =>  OK\n");

    printf("\n");

    // Test AND
    cpu.R2 = 5;
    cpu.R1 = 4;
    executeInstruction(0b11000110);
    assert(cpu.R2 == 4);
    printf("AND 5, 4    =>  OK\n");

    cpu.R2 = 255;
    cpu.R1 = 0;
    executeInstruction(0b11000110);
    assert(cpu.R2 == 0);
    printf("AND 255, 0    =>  OK\n");

    printf("\n");

    // Test OR
    cpu.R2 = 255;
    cpu.R1 = 0;
    executeInstruction(0b11010110);
    assert(cpu.R2 == 255);
    printf("OR 255, 0    =>  OK\n");

    cpu.R2 = 2;
    cpu.R1 = 1;
    executeInstruction(0b11010110);
    assert(cpu.R2 == 3);
    printf("OR 2, 1    =>  OK\n");

    printf("\n");

    // Test XOR
    cpu.R2 = 2;
    cpu.R1 = 2;
    executeInstruction(0b11100110);
    assert(cpu.R2 == 0);
    printf("XOR 2, 2    =>  OK\n");

    cpu.R2 = 1;
    cpu.R1 = 128;
    executeInstruction(0b11100110);
    assert(cpu.R2 == 129);
    printf("XOR 1, 128    =>  OK\n");

    printf("\n");

    // Test CMP
    cpu.R0 = 3;
    cpu.R1 = 3;
    executeInstruction(0b11110001);
    assert(cpu.EQUAL == 1);
    printf("CMP 3, 3    =>  OK\n");

    cpu.R0 = 7;
    cpu.R1 = 3;
    executeInstruction(0b11110001);
    assert(cpu.ALARGER == 1);
    printf("CMP 7, 3    =>  OK\n");
}

void runOtherTests() {
    // Test LB
    writeBus(5, 17);
    cpu.R0 = 5;
    executeInstruction(0b00000001);
    assert(cpu.R1 == 17);
    printf("LB 5, R0    =>  OK\n");

    writeBus(55, 99);
    cpu.R0 = 55;
    executeInstruction(0b00000001);
    assert(cpu.R1 == 99);
    printf("LB 55, R0    =>  OK\n");

    printf("\n");

    // Test SB
    cpu.R2 = 47;
    cpu.R0 = 8;
    executeInstruction(0b00010010);
    assert(readBus(8) == 47);
    printf("ST 8, 47    =>  OK\n");

    cpu.R2 = 194;
    cpu.R0 = 39;
    executeInstruction(0b00010010);
    assert(readBus(39) == 194);
    printf("ST 39, 194    =>  OK\n");

    printf("\n");

    // Test DATA
    writeBus(1, 37);
    executeInstruction(0b00100000);
    assert(cpu.R0 == 37);
    printf("DATA R0, 37     =>  OK\n");

    writeBus(3, 65);
    executeInstruction(0b00100000);
    assert(cpu.R0 == 65);
    printf("DATA R0, 65     =>  OK\n");

    printf("\n");

    // Test JMPR
    cpu.R0 = 8;
    executeInstruction(0b00110000);
    assert(cpu.PC == 8);
    printf("JMPR 8  =>  OK\n");

    printf("\n");

    // Test JMP
    writeBus(9, 88);
    executeInstruction(0b01000000);
    assert(cpu.PC == 90);
    printf("JMP 88  =>  OK\n");
}
