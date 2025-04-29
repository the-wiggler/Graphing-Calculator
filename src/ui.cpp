#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "graphing.hpp"

void uiMain::commandWindow() {
    // draw onto the texture
    SDL_Texture* commandTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        300,
        120);
    SDL_SetTextureBlendMode(commandTexture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, commandTexture);

    // Create a rectangle that covers the entire texture
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 80);
    SDL_Rect cmdBox = { 5, 5, 300, 120 };
    SDL_RenderFillRect(renderer, &cmdBox);
    SDL_Rect cmdInput = { 5, 100, 300, 20 };
    SDL_RenderFillRect(renderer, &cmdInput);

    // load font
    TTF_Font* asana = TTF_OpenFont("asana.ttf", 12);

    // command input text box
    SDL_Color white = { 255, 255, 255, 180 };
    SDL_Surface* cmdInputSurface = TTF_RenderText_Solid(asana, "<Input Command Here>", white);
    SDL_Texture* cmdInpuitTexture = SDL_CreateTextureFromSurface(renderer, cmdInputSurface);
    SDL_FreeSurface(cmdInputSurface);

    SDL_Rect rect = { 10,105,0,0 };
    SDL_QueryTexture(cmdInpuitTexture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderCopy(renderer, cmdInpuitTexture, nullptr, &rect);


    // sets back to default and renders
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, commandTexture, nullptr, &cmdBox);
    //SDL_DestroyTexture(commandTexture);
}