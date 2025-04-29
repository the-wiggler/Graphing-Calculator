#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "graphing.hpp"


int SDL_main(int argc, char* argv[]) {
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Graphing Calculator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Font* asana = TTF_OpenFont("asana.ttf", 24);
    if (!asana) {
        std::cout << "FONT LOAD ERROR: " << TTF_GetError() << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LOOP FOR THE WINDOW PROGRAM                                                                           //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    windowMain windowMain;
    uiMain uiMain(renderer);

    while (windowMain.running) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // creates an instance of graphMain -- the rendering solution to display the graph
        graphMain graphMain(renderer);
        // renders the axes
        graphMain.axesRender();
        // renders the funcion points
        graphMain.functionRender();
        // renders the command window
        uiMain.commandWindow();

        SDL_RenderPresent(renderer);

        // keeps the window open while waiting for a new command
        windowMain.keepWindowOpen();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

return 0;
}