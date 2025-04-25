#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include "numOutputs.hpp"

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 800


int SDL_main(int argc, char* argv[]) {
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Graphing Calculator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LOOP FOR THE WINDOW PROGRAM                                                                           //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (running) {
        numOutputs outputs;

        outputs.executeFunctionCalculation();

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // This scales and renders the initial points to visualize the function on the graph
        for (int i = 0; i < outputs.x_arr.size(); i++) {
            int scaled_x_point = static_cast<int>((outputs.x_arr[i] - outputs.x_min) * WINDOW_SIZE_X / outputs.x_range);
            int scaled_y_point = static_cast<int>(WINDOW_SIZE_Y - (outputs.y_arr[i] - outputs.y_min) * WINDOW_SIZE_Y / outputs.y_range);

            if (i > 0) { SDL_RenderDrawPoint(renderer, scaled_x_point, scaled_y_point); }

        }

        outputs.x = 0;

        SDL_RenderPresent(renderer);

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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

    }


return 0;
}