#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){

    SDL_Window* window = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(" Chip-8 emulator",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            400,
                            200,
                            SDL_WINDOW_RESIZABLE);

    if(window == NULL){
        cout<<" Window can't ne created some error occurred"<<endl
            <<SDL_GetError()<<endl;
    }  

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

    SDL_DestroyWindow(window);                      
    SDL_Quit();

}