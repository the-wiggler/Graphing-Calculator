#include <SDL2/SDL.h>
#include <GL/glew.h> 
#include <SDL2/SDL_opengl.h>
#include "graphing.hpp"


int SDL_main(int argc, char* argv[]) {
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Graphing Calculator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LOOP FOR THE WINDOW PROGRAM                                                                           //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    windowMain windowMain;

    while (windowMain.running) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // creates an instance of graphMain -- the rendering solution to display the graph
        graphMain graphMain(renderer);
        // renders the axes
        graphMain.axesRender();
        // renders the funcion points
        graphMain.functionRender();

        SDL_RenderPresent(renderer);

        // keeps the window open while waiting for a new command
        windowMain.keepWindowOpen();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

    }

return 0;
}