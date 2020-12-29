#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include "FLAG.h"

class UI{

    public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        SDL_Event event;

        UI();
        ~UI();
        bool UI_Init();
        void UI_MaintainClockSpeed(uint32_t startTime);
        void UI_HandleEvent(bool &running);
        void UI_UpdateScreen(uint32_t display[]);

};

UI::UI(){

    window = NULL;
    renderer = NULL;
    texture = NULL;
}

UI::~UI(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
bool UI::UI_Init(){

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chip-8 emulator",
                                 SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCALE*WINDOW_WIDTH,
                                SCALE*WINDOW_HEIGHT,
                                SDL_WINDOW_RESIZABLE);

    if(window == NULL){
        std::cout<<" Window can't be created, "<<SDL_GetError()<<"\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if( renderer == NULL)
    {
        std::cout<<" Renderer can't be created, "<<SDL_GetError()<<"\n";
        return false;
    
    }
    texture = SDL_CreateTexture(renderer, 
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                WINDOW_WIDTH,
                                WINDOW_HEIGHT);

    if( texture ==NULL)
    {
        std::cout<<" texture can't be created, "<<SDL_GetError()<<"\n";
        return false;
    
    }

}

void UI::UI_MaintainClockSpeed(uint32_t startTime){

    uint32_t currentTime = SDL_GetTicks();
    if( (1000/FPS) > (currentTime - startTime)){
            SDL_Delay((1000/FPS) - (currentTime-startTime));
        }
}

void UI::UI_HandleEvent(bool &running){

    while(SDL_PollEvent(&event)){  
            switch (event.type)
            {
            case SDL_QUIT:
                    running = false;
                break;
            }
        }
}

void UI::UI_UpdateScreen(uint32_t display[]){

    SDL_UpdateTexture(texture, nullptr, (void const*) display,sizeof(uint32_t) * WINDOW_WIDTH);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

#endif 