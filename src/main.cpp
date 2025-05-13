#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "graphing.hpp"


int main(int argc, char* argv[]) {
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Graphing Calculator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Font* asana = TTF_OpenFont("asana.ttf", 24);
    if (!asana) {
        SDL_ShowSimpleMessageBox(0x00000010, "Graphing Calculator", "ERROR: FONT FILE <asana.ttf> NOT FOUND", NULL);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LOOP FOR THE WINDOW PROGRAM                                                                           //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    uiMain uiMain(renderer);

    while (uiMain.running) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // creates an instance of graphMain -- the rendering solution to display the graph
        graphMain graph(renderer);
        // renders the axes
        graph.axesRender();
        // renders the funcion points
        graph.functionRender();

        // renders the command window
        uiMain.textInput();

        SDL_Delay(128); // hardcoded FPS limit (yikes!)
    }

    TTF_CloseFont(asana);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

return 0;
}