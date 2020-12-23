# Game plan for the emulator

- create the workflow
- think of the classes
- think of there connections
- think about the connections with screen

## Flow of the Chip8

### Initialise all the values present in the emulator

1. create the intialise memory.
1. create and initialise the registers
1. set the Index register to zero
1. set the PC
1. clear the screen
1. clean the stack

### Load the file into the memory

 1. Fetch the instruction from the memory
 2. Decode use a switch.
 3. Execute write in the switch case

start reading the instr from the memory.

## Class Descriptions

### cpu8 class

This class should be the main power house of the emulator.

#### Variables

- PC
- Registers
- Delay Timer
- Index Register

For each of the below we can have a class to make instance of each of them.

- Memory
- Display
- PC -- point the current instr in memory
- Index Register for memory -- 16 bit register
- Stack -- 16-bit addresses
- Delay Timer -- 8-bit decremented at 60Hz until reaches zero
- Sound Timer -- not an aim intially
- General Variable Registers -- 16( 8-bit ) registers numbered 0-15 in decimal called `V0` to `VF`

### Implementation Details

use chrono to check the cycle time once the time is done we can update it(Our PC speed will be quite high)
Instruction speed of 700 per second is considered good.
