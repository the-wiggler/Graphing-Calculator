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
        // initializes values of the function
        numOutputs outputs;
        outputs.executeFunctionCalculation();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // This scales and renders the initial points to visualize the function on the graph
        if (outputs.func_valid) {
            // finds the uniform scale that the points should be transformed to
            float scale_x = WINDOW_SIZE_X / outputs.x_range;
            float scale_y = WINDOW_SIZE_Y / outputs.y_range;
            float scale = std::min(scale_x, scale_y);

            // draw axes
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            int x_origin = static_cast<int>((0 + (outputs.RANGE_MAX) * scale));
            SDL_RenderDrawLine(renderer, 0, x_origin, WINDOW_SIZE_X, x_origin);
            int y_origin = static_cast<int>(outputs.DOMAIN_MIN / (outputs.DOMAIN_MAX - outputs.DOMAIN_MIN) * WINDOW_SIZE_X);
            SDL_RenderDrawLine(renderer, y_origin, 0, y_origin, WINDOW_SIZE_Y);

            // scales and plots the points onto the screen
            SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);
            for (int i = 0; i < outputs.x_arr.size(); i++) {
                int px = static_cast<int>( (outputs.x_arr[i] - outputs.x_min) * scale );
                int py = static_cast<int>( WINDOW_SIZE_Y - (outputs.y_arr[i] - outputs.y_min) * scale);
                SDL_RenderDrawPoint(renderer, px, py);
            }
        }
        else {
            std::cout << "ERROR: FUNCTION NOT WITHIN RANGE" << std::endl;
        }


        //SDL_RenderDrawLine(renderer, )

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