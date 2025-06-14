﻿#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "graphing.hpp"

TTF_Font* font = nullptr;
TTF_Font* tickFontSmall = nullptr;

int main() {

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Calc", WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // handles font loading
    std::string fontPath = SDL_GetBasePath();
    fontPath += "font.ttf";
    font = TTF_OpenFont(fontPath.c_str(), WINDOW_SIZE_Y/24);
    tickFontSmall = TTF_OpenFont(fontPath.c_str(), WINDOW_SIZE_Y/48);
    if (!font) {
        SDL_ShowSimpleMessageBox(0x00000010, "Graphing Calculator", "ERROR: FONT FILE <font.ttf> NOT FOUND", NULL);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LOOP FOR THE WINDOW PROGRAM                                                                           //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    uiMain uiMain(window, renderer);

    while (uiMain.running) {

        SDL_SetRenderDrawColor(renderer, 44,37,37,255);
        SDL_RenderClear(renderer);

        // creates an instance of graphMain -- the rendering solution to display the graph
        graphMain graph(renderer);
        // renders the axes
        graph.axesRender();
        // renders the funcion points
        graph.functionRender();

        // renders the command window
        uiMain.textInput();

        // ## ms delay (i think it makes 60 fps)
        SDL_Delay(16);

    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

return 0;
}
