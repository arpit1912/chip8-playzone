#include "CHIP8.h"
#include "UI.h"

int main(int argc, char* argv[]){

    bool running = true;

    UI Screen;
    chip8 CHIP8;

    Screen.UI_Init();
    CHIP8.ReadFile(argv[1]);
    
    while(running){

        uint32_t startTime = SDL_GetTicks();
        CHIP8.Cycle();
        Screen.UI_HandleEvent(running);
        Screen.UI_UpdateScreen(CHIP8.display);
        Screen.UI_MaintainClockSpeed(startTime);
        
    }
    
    Screen.~UI();
}
