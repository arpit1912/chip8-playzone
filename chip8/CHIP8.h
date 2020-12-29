#ifndef CHIP8_H
#define CHIP8_H

#include<fstream>
#include<iostream>
#include<stack>

#include "FLAG.h"

class chip8
{
private:
    uint8_t reg[16];
    std::stack<uint16_t> stack_memory;
    uint16_t PC;
    uint16_t index_register;
    uint16_t current_opcode;

    uint8_t delay_timer; // clock speed of 60Hz
    uint8_t sound_timer; // clock speed of 60Hz
public:
    
    uint8_t memory[4096];
    uint32_t display[WINDOW_WIDTH*WINDOW_HEIGHT];

    uint16_t font[80]={
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    chip8();
    bool ReadFile(const char* filename);
    void Cycle();
    void Fetch();
    void DecodeAndExecute(uint16_t opcode);
};

// Setting the processor up(Kind of boot loading)
chip8::chip8()
{
    for(int i=0;i<4096;i++){
        memory[i] = 0; // clearing up the memory space
    }
    
    for(int i=0;i<80;i++){
        memory[i] = font[0x050 + i];  // setting up the font in the memory from 0x050 
    }
    
    PC = START_ADDRESS; // setting the PC to 0x200
    index_register = 0; // any value can be given doesn't matter but for consistency given it zero


}
// Loading the Process into the memory
bool chip8:: ReadFile(const char* filename){
    // Opening the file from the end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
     
    if(!file) return false;

    //calc the size and setting the pointer to intial location in the file
    auto end = file.tellg();
    file.seekg(0,std::ios::beg);
    auto begin = file.tellg();
    auto const size = end-begin;

    //creating a buffer to make a copy
    char* buffer = new char[size];

    file.read(buffer,size);
    file.close();
    // writing the copy into the memory
    for(uint16_t i=0;i<size;i++) memory[START_ADDRESS + i] = buffer[i];

    // deleting the buffer memory as it's not needed from now on
    delete[] buffer;

    return true;
}


// running a cycle for the instruction opcode
void chip8::Cycle(){  
    
    Fetch();
    PC +=2;
    DecodeAndExecute(current_opcode);

}

// fetching the instr from the two continous blocks
void chip8::Fetch(){
    current_opcode = memory[PC];
    current_opcode <<= 8;
    current_opcode = current_opcode | memory[PC+1];
}

// Decoding and executing the current opcode
void chip8::DecodeAndExecute(uint16_t opcode){
    /// Dxyn
    uint16_t nibble[6]{}; // 6 type of nibbles are there in the opcode
    // decoding the opcode with masking 
    nibble[0] = (opcode & 0xf000) >> 12; // first byte
    nibble[1] = (opcode & 0x0f00) >> 8; // second byte, X
    nibble[2] = (opcode & 0x00f0) >> 4; // third byte, Y
    nibble[3] = opcode & 0x000f; // fourth byte, N
    nibble[4] = opcode & 0x00ff; // NN
    nibble[5] = opcode & 0x0fff; // NNN
    
    // execution of the opcode
    switch (nibble[0])
    {
    case 0x0: // clearing the screen 
        {
            for(int i=0;i< WINDOW_WIDTH*WINDOW_HEIGHT ; i++){
                display[i] = 0;
            }
        }
        break;
    case 0x1:
        PC = nibble[5];
        break;
    case 0x6:
        reg[nibble[1]] = nibble[4];
        break;
    case 0x7:
        reg[nibble[1]] +=nibble[4];
        break;
    case 0xa:
        index_register = nibble[5];
        break;
    case 0xd: //display dxyn
    {
        uint8_t x_pos = reg[nibble[1]] % WINDOW_WIDTH;
        uint8_t y_pos = reg[nibble[2]] % WINDOW_HEIGHT;
        uint8_t height = nibble[3];
        reg[0xf] = 0;

        for(int row = 0; row < height; row++){
            
            uint8_t sprite_byte = memory[index_register + row];

            for(int col = 0; col< 8;col++){
                
                uint8_t spritepixel = (sprite_byte >> (7-col)) & 1;
                // for avoiding the wrap of sprite
                if((x_pos + col > 63) || (y_pos + row) > 31)
                    break;
                
                uint32_t* screenpixel = &display[(y_pos + row)*WINDOW_WIDTH + (x_pos + col)];

                if(spritepixel){
                    if(*screenpixel == 0xFFFFFFFF){
                        reg[0xf] = 1;
                    }   

                    *screenpixel = (*screenpixel) ^ 0xFFFFFFFF;
                }
            }
        }
        break;
    }
    // THere are many OPCODEs which are still left to be implemented!
    default:
        std::cout<<" there is no such opcode implemented right now"<<std::endl;
        break;
    }

}
#endif