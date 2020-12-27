#include<bits/stdc++.h>
#include <SDL2/SDL.h>

using namespace std;

#define scale 10
const int START_ADDRESS = 0x200;
const int window_width = 64;
const int window_height = 32;
const int fps = 60;


class chip8
{
private:
    /* data */
public:
    uint8_t reg[16];
    stack<uint16_t> stack_memory;
    uint8_t memory[4096];
    
    uint16_t PC;
    uint16_t index_register;
    uint16_t current_opcode;

    uint8_t delay_timer; // clock speed of 60Hz
    uint8_t sound_timer; // clock speed of 60Hz

    uint32_t display[window_width*window_height]; //Boolean can also work here 0 and 1 state

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

    //Funtions of the processor are here
    chip8();
    bool ReadFile(const string &filename);
    void cycle();
    void DecodeAndExecute(uint16_t opcode);
};

chip8::chip8()
{
    for(int i=0;i<4096;i++){
        memory[i] = 0;
    }
    
    for(int i=0;i<80;i++){
        memory[i] = font[0x050 + i];
    }
    
    PC = START_ADDRESS;
    index_register = 0;


}

bool chip8:: ReadFile(const string &filename){

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
     
    if(!file) return false;

    auto end = file.tellg();
    file.seekg(0,ios::beg);
    auto begin = file.tellg();
    auto const size = end-begin;

    char* buffer = new char[size];

    file.read(buffer,size);
    file.close();

    for(uint16_t i=0;i<size;i++) memory[START_ADDRESS + i] = buffer[i];

    delete[] buffer;

    return true;
}

void chip8::cycle(){
    current_opcode = memory[PC];
    current_opcode <<= 8;
    current_opcode = current_opcode | memory[PC+1];

    PC +=2;

    DecodeAndExecute(current_opcode);
}

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
    case 0x0:
        memset(display,0,sizeof(display));
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
        uint8_t x_pos = reg[nibble[1]] % window_width;
        uint8_t y_pos = reg[nibble[2]] % window_height;
        uint8_t height = nibble[3];
        reg[0xf] = 0;

        for(int row = 0; row < height; row++){
            
            uint8_t sprite_byte = memory[index_register + row];

            for(int col = 0; col< 8;col++){
                
                uint8_t spritepixel = (sprite_byte >> (7-col)) & 1;
                // for avoiding the wrap of sprite
                if((x_pos + col > 63) || (y_pos + row) > 31)
                    break;
                
                uint32_t* screenpixel = &display[(y_pos + row)*window_width + (x_pos + col)];

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
    default:
        cout<<" there is no such opcode implemented right now"<<endl;
        break;
    }

}

void maintainclockspeed(uint32_t startTime)
{
    uint32_t currentTime = SDL_GetTicks();
    if( (1000/fps) > (currentTime - startTime)){
            SDL_Delay((1000/fps) - (currentTime-startTime));
        }
}

int main(int argc, char* argv[]){

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    SDL_Event event;
    bool running = true;

    window = SDL_CreateWindow(" Chip-8 emulator",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                scale*window_width,
                                scale*window_height,
                                SDL_WINDOW_RESIZABLE);

    if(window == NULL){
        cout<<" Window can't be created some error occured "<<SDL_GetError()<<endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, 
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                window_width,
                                window_height);
    chip8 CHIP8;
    CHIP8.ReadFile(argv[1]);
    
    while(running){

        uint32_t startTime = SDL_GetTicks();
        CHIP8.cycle();
        while(SDL_PollEvent(&event)){
            
            switch (event.type)
            {
            case SDL_QUIT:
                    running = false;
                break;
            }
        }

        SDL_UpdateTexture(texture, nullptr, (void const*) CHIP8.display,sizeof(uint32_t) * window_width);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        
        maintainclockspeed(startTime);
        
    }
    

}
