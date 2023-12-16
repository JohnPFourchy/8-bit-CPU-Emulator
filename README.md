# 8-Bit CPU Emulator

Directory contents:
src/cpu.c - the 8-bit cpu
src/bus.c - contains devices connected to the cpu
utils/utils.c - utility functions used for testing purposes
test/testcases.c - tests for the cpu instructions
programs/{program-name.txt} - example programs that can be executed on the cpu
information.txt - contains the cpu instruction set and information about each type of instruction

## Usage
from the src directory run:
gcc cpu.c bus.c ../utils/utils.c
./cpu.out ../program/{program name.txt}

## Future plans
Create an assembler so programs can be written in an assembly language instead of machine code
Add more devices to the bus to interface with the cpu
