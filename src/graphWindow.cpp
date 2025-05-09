// this file is responsible for the actual function and its axes rendering on the screen
#include <iostream>
#include <SDL2/SDL.h>
#include "graphing.hpp"
#include <cmath>
#include <vector>

// create a target texture the size of the window
static SDL_Texture* makeTargetTexture(SDL_Renderer* r)
{
    SDL_Texture* tex = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_SIZE_X, WINDOW_SIZE_Y);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    return tex;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// the function responsible for creating the axes based on the const data from graphing.hpp
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void graphMain::axesRender() {
    // draw onto the texture
    SDL_Texture* axesTexture = makeTargetTexture(renderer);
    SDL_SetRenderTarget(renderer, axesTexture);

    // clears the renderer with a transparent color so nothing is obstructed by accident
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    //////////////////////////////////////////////////////////////////////////////////
    // These lines calculate the position where x=0 and y=0 should be on the screen
    float x_origin = (RANGE_MAX / RANGE_INTERVAL) * WINDOW_SIZE_Y;
    float y_origin = (-DOMAIN_MIN / DOMAIN_INTERVAL) * WINDOW_SIZE_X;
    //
    //////////////////////////////////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////////////////////////////////
    // graph reference lines
    SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
    for (int i = 0; i <= DOMAIN_INTERVAL; i++) {
        float a = (1 / DOMAIN_INTERVAL) * WINDOW_SIZE_X;
        SDL_RenderDrawLine(renderer, 0 + i * a, 0, 0 + i * a, WINDOW_SIZE_X);
    }
    for (int i = 0; i <= RANGE_INTERVAL; i++) {
        float a = (1 / RANGE_INTERVAL) * WINDOW_SIZE_Y;
        SDL_RenderDrawLine(renderer, 0, 0 + i * a, WINDOW_SIZE_Y, 0 + i * a);
    }
    //
    //////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////
    // domain tick marks
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= DOMAIN_INTERVAL; i++) {
        float a = (1 / DOMAIN_INTERVAL) * WINDOW_SIZE_X;
        SDL_RenderDrawLine(renderer, 0 + i * a, x_origin - 5, 0 + i * a, x_origin + 5);
    }
    // range tick marks
    for (int i = 0; i <= RANGE_INTERVAL; i++) {
        float a = (1 / RANGE_INTERVAL) * WINDOW_SIZE_Y;
        SDL_RenderDrawLine(renderer, y_origin - 5, 0 + i * a, y_origin + 5, 0 + i * a);
    }
    //
    //////////////////////////////////////////////////////////////////////////////////

    // this draws the axis lines if they're applicable within the given domain/range
    SDL_RenderDrawLine(renderer, 0, x_origin, WINDOW_SIZE_X, x_origin);
    SDL_RenderDrawLine(renderer, y_origin, 0, y_origin, WINDOW_SIZE_Y);

    // sets back to default and renders
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, axesTexture, nullptr, nullptr);
    SDL_DestroyTexture(axesTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// the function responsible for graphing the points onto the window
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void graphMain::functionRender() {
    numOutputs outputs;
    // draw onto the texture
    SDL_Texture* funcTexture = makeTargetTexture(renderer);
    SDL_SetRenderTarget(renderer, funcTexture);

    // clears the renderer with a transparent color so nothing is obstructed by accident
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // calculates values of the function
    outputs.executeFunctionCalculation();

    // This scales and renders the initial points to visualize the function on the graph
    if (outputs.func_valid) {

        // scales and plots the points onto the screen
        std::vector<SDL_Point> SDL_fpoints;
        SDL_fpoints.reserve(outputs.fpoints.size());

        SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);
        for (int i = 0; i < outputs.fpoints.size(); i++) {
            // Transform x from domain space to screen space
            int px = ((outputs.fpoints[i].x - DOMAIN_MIN) / DOMAIN_INTERVAL) * WINDOW_SIZE_X;

            // Transform y from range space to screen space
            int py = ((RANGE_MAX - outputs.fpoints[i].y) / RANGE_INTERVAL) * WINDOW_SIZE_Y;

            SDL_fpoints.push_back({ px, py });
        }

        // Renders the points on the renderer
        SDL_RenderDrawPoints(renderer, SDL_fpoints.data(), SDL_fpoints.size());

    }
    else {
        SDL_ShowSimpleMessageBox(0x00000010, "Graphing Calculator", "WARNING: Function not within bounds of range", NULL);
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, funcTexture, nullptr, nullptr);
    SDL_DestroyTexture(funcTexture);
}