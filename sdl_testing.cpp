#include <SDL2/SDL.h>
#include <iostream>

#define width 500
#define height 250

using namespace std;

int main(int argc, char* argv[]){

    SDL_Window* window = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(" Chip-8 emulator",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            width,
                            height,
                            SDL_WINDOW_RESIZABLE);

    if(window == NULL){
        cout<<" Window can't ne created some error occurred"<<endl
            <<SDL_GetError()<<endl;
    }  

    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture;
    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,200,100);
    //video width = 100
    // video height = 50
    uint32_t videobuffer[200*100];

    for(int i=0;i<200*100;i++){
            videobuffer[i] = 0xFFFFFFFF;
    }
    SDL_UpdateTexture(texture, nullptr, (void const*) videobuffer,sizeof(videobuffer[0]) * 200);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    
    SDL_Event event;
    bool running = true;
    while(running){

    while( SDL_PollEvent(&event)){
         switch (event.type)
         {
         case SDL_QUIT:
             running = false;
             break;
         }
    }
    
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);                      
    SDL_Quit();

}

