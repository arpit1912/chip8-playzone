# Chip 8 important parts

- Memory -- 4KB of RAM(direct access)
- Display -- 64 * 32 pixels monochrome
- PC -- point the current instr in memory
- Index Register for memory -- 16 bit register
- Stack -- 16-bit addresses
- Delay Timer -- 8-bit decremented at 60Hz until reaches zero
- Sound Timer -- not an aim intially
- General Variable Registers -- 16( 8-bit ) registers numbered 0-15 in decimal called `V0` to `VF`

## Memory 
Chip-8 index registers and program counter can only address 12 bits, which is 4096 addresses.
The first chip8 interpreter was located in RAM, from address `000` to `1FF`. It would expect a chip8 program to be loaded into memory after the interpreter this means that we should the program should be loaded from `200` address and should be read from `200` only.


## Font
the emulator should have a build-in font, with sprite data representing the hexadecimal numbers from 0 - F. Eah font should be 4pixels wide by 5 pixels tall. These font sprites are drawn just like regular sprites.

You should store the font data in memory, because games will draw these characters like regular sprites: They set the index register I to the character’s memory location and then draw it. There’s a special instruction for setting I to a character’s address, so you can choose where to put it. Anywhere in the first 512 bytes (000–1FF) is fine. For some reason, it’s become popular to put it at 050–09F, so you can follow that convention if you want.

## Display 
Each pixel can be on or off. in short each boolean value is a bit.
Original inerpreters update the display at 60Hz. It might be good idea to redraw the screen when the emulator executes an instr that modifies the display data, to run faster.
Generally we switch off all the bits and redraw the screen thus creating a flickering effect.

## Stack
It follows the typical fifo method for data in and out. Each element is of size 16bit(12-bit generally). The stack size is generally 12 or 16 two-byte entries. Unless you encounter a unfinite loop we generally won't face a stack overflow condition.

## Timers

There are two timer registers in the emulator. The delay and the sound timer. Both have same underlining principle. They are one byte in size and should be decremented by one  60times per second. This is independent of the speed of the fetch/decode/execute loop. The sound timer make the computer beep till it is above 0.

## Keypad

The emulator has a hexa decimal keypad. These had 16keys, labelled 0 through F and were arranged in 4 * 4 grid.\
`1 2 3 C` \
`4 5 6 D` \
`7 8 9 E` \
`A 0 B F`

## Fetch/Decode/Execute Loop

The emulator runs a infinite loop, and does these three tasks in succession:

- Fetch the instruction from main memory at the current PC.
- Decode the instruction to find out what the emulator should do.
- Execute the instruction and do what it tells you.

## Timing

In practice, a standard speed of around 700 instructions per second fits well enough for most programs.

## Fetch

Read the instr which the PC is currently pointing at from the memory. An instruction in 2 bytes, so we need to read two consecutive bytes from the memory and combine them into one 16-bit instr.

## Decode

instr are divided into broad nibble, whic is the first hexa decimal number. So, you basically just want to do a huge if/elseif statement here, doing different things depending on what the first number is.

Although every instruction will have a first nibble that tells you what kind of instruction it is, the rest of the nibbles will have different meanings. To differentiate these meanings, we usually call them different things, but all of them can be any hexadecimal number from 0 to F:

X: The second nibble. Used to look up one of the 16 registers (VX) from V0 through VF.
Y: The third nibble. Also used to look up one of the 16 registers (VY) from V0 through VF.
N: The fourth nibble. A 4-bit number.
NN: The second byte (third and fourth nibbles). An 8-bit immediate number.
NNN: The second, third and fourth nibbles. A 12-bit immediate memory address.
To avoid code duplication again, I suggest you extract these values from the opcode before decoding, instead of doing it inside each instruction. If you do it wrong just one place, you’ll have a hard time tracking that down.

## Execute

For CHIP-8, if you went with the switch approach (or similar), this won’t really be a separate stage. Just directly do what the instruction should do inside each case.

In emulators for other systems, you might have a whole bunch of instructions of the same type – say, to add two numbers together – where the operands can be registers, memory locations, immediate values, etc. (these are called addressing modes). But for CHIP-8, that doesn’t matter.

## Imp instructions that are there in the emulator right now

- 00E0 (clear sreen) --> This is pretty simple: It should clear the display, turning all pixels off to 0.
- 1NNN (jump) --> The instr should simply set PC to NNN, causing the prog to jump to that memor loc. Don't increment the PC afterwards,it jumps directly there.
- 6XNN (set register VX) --> Simply set the register VX to the value NN.
- 7XNN (add value to register Vx) --> Add the value NN to VX
- ANNN (set index register I) --> This sets the index register I to the value NNN.
- DXYN (display/draw) --> Print the font stored at index address memory location at the location stored in reg X and reg Y for N number of rows.
