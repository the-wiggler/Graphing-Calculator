#include <SDL2/SDL.h>
#include "graphing.hpp"

void uiMain::commandWindow() {
    // draw onto the texture
    SDL_Texture* commandTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        300,
        120);
    SDL_SetTextureBlendMode(commandTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, commandTexture);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 80);


    // Create a rectangle that covers the entire texture
    SDL_Rect cmdBox = { 5, 5, 300, 120 };
    SDL_RenderFillRect(renderer, &cmdBox);
    SDL_Rect cmdInput = { 5, 100, 300, 20 };
    SDL_RenderFillRect(renderer, &cmdInput);

    // sets back to default and renders
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, commandTexture, nullptr, &cmdBox);
    //SDL_DestroyTexture(commandTexture);
}