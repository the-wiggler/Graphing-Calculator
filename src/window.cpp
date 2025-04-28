// this file is responsbile for managing the window. For example: keeping the window open when waiting
#include <SDL2/SDL.h>
#include "graphing.hpp"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void windowMain::keepWindowOpen() {
    SDL_Event event;
    // stops the rendering loop until a certain command is executed
    bool wait_for_reset = true;
    while (wait_for_reset) {
        if (SDL_WaitEvent(&event)) {
            // quits the program
            if (event.type == SDL_QUIT) {
                running = false;
                wait_for_reset = false;
            }
            // re-renders the function when the letter "r" is pressed
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                wait_for_reset = false;
            }
        }
    }
}