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
        // calculates values of the function
        numOutputs outputs;
        outputs.executeFunctionCalculation();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        // This scales and renders the initial points to visualize the function on the graph
        if (outputs.func_valid) {

            // These lines calculate the position where x=0 and y=0 should be on the screen
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            float x_origin = (outputs.RANGE_MAX / outputs.RANGE_INTERVAL) * WINDOW_SIZE_Y;
            SDL_RenderDrawLine(renderer, 0, x_origin, WINDOW_SIZE_X, x_origin);
            float y_origin = (-outputs.DOMAIN_MIN / outputs.DOMAIN_INTERVAL) * WINDOW_SIZE_X;
            SDL_RenderDrawLine(renderer, y_origin, 0, y_origin, WINDOW_SIZE_Y);
            // domain tick marks
            for (int i = 0; i <= outputs.DOMAIN_INTERVAL; i++) {
                float a = (1 / outputs.DOMAIN_INTERVAL) * WINDOW_SIZE_X;
                SDL_RenderDrawLine(renderer, 0 + i * a, x_origin - 5, 0 + i * a, x_origin + 5);
            }
            // range tick marks
            for (int i = 0; i <= outputs.RANGE_INTERVAL; i++) {
                float a = (1 / outputs.RANGE_INTERVAL) * WINDOW_SIZE_Y;
                SDL_RenderDrawLine(renderer, y_origin - 5, 0 + i * a, y_origin + 5, 0 + i * a);
            }


            // scales and plots the points onto the screen
            std::vector<SDL_Point> SDL_fpoints;
            SDL_fpoints.reserve(outputs.fpoints.size());

            SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);
            for (int i = 0; i < outputs.fpoints.size(); i++) {
                // Transform x from domain space to screen space
                int px = ((outputs.fpoints[i].x - outputs.DOMAIN_MIN) / outputs.DOMAIN_INTERVAL) * WINDOW_SIZE_X;

                // Transform y from range space to screen space
                int py = ((outputs.RANGE_MAX - outputs.fpoints[i].y) / outputs.RANGE_INTERVAL) * WINDOW_SIZE_Y;

                SDL_fpoints.push_back({ px, py });
            }

            // Draw lines between points
            SDL_RenderDrawLines(renderer, SDL_fpoints.data(), SDL_fpoints.size());

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