#include<bits/stdc++.h>
#include <SDL2/SDL.h>

#include "CHIP8.h"

void UI_MaintainClockSpeed(uint32_t startTime)
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
        std::cout<<" Window can't be created some error occured "<<SDL_GetError()<<std::endl;
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
        CHIP8.Cycle();
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
        
        UI_MaintainClockSpeed(startTime);
        
    }
    

}
