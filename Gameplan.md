
# Structure of CHIP-8 implementation

The codebase have two major classes which handles the functionality of the Emulator which are explained below:

## chip8

This class aims to behave like the simulator for chip-8. All the behavior and attributes of CHIP-8 should be only in this class.

## UI

This class aims to handle the interaction of the chip-8 with the screen, audio, and any other I/O service the chip8 class needs. This class uses SDL2 for the implementing the functionality.

### Driver Program

`main.cpp` is the driver file where these classes interact. Remember we aim to simplify the code of `main` as much as we can.

To run the code do:\
    ```make``` \
    ```./chip8.out <chip-8 program file> ```

A simple IBM Logo.ch8 is already present for testing the source code.
