// this file is responsible for the actual function and its axes rendering on the screen
#include <iostream>
#include <SDL3/SDL.h>
#include "graphing.hpp"
#include <cmath>
#include <vector>

static SDL_Texture* cachedAxes = nullptr;
static SDL_Texture* cachedFunc = nullptr;
static numOutputs outputs;

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
    if (!axesBad && cachedAxes) {
        SDL_RenderTexture(renderer, cachedAxes, nullptr, nullptr);
        return;
    }

    if (cachedAxes) SDL_DestroyTexture(cachedAxes);
    cachedAxes = makeTargetTexture(renderer);
    SDL_SetRenderTarget(renderer, cachedAxes);

    // clears the renderer with a transparent color so nothing is obstructed by accident
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    //////////////////////////////////////////////////////////////////////////////////
    // These lines calculate the position where x=0 and y=0 should be on the screen --
    float x_origin = (RANGE_MAX / RANGE_INTERVAL) * WINDOW_SIZE_Y;
    float y_origin = (-DOMAIN_MIN / DOMAIN_INTERVAL) * WINDOW_SIZE_X;

    // Determine grid spacing based on zoom level
    float gridSpacing = 1.0f;  // Default to 1
    // this scales the tick placement based on the zoom level
    if (DOMAIN_INTERVAL > 24) {
        int labelFactor = static_cast<int>(DOMAIN_INTERVAL / 24);
        for (int i = 0; i < labelFactor; i++) {
            gridSpacing += 3;
        }
    }

    float startX = ceil(DOMAIN_MIN / gridSpacing) * gridSpacing; // finds first grid line position
    float startY = ceil(RANGE_MIN / gridSpacing) * gridSpacing;  // (first int value on screen where tick marks start)
    // -------------------------------------------------------------------------------
    //////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////
    // graph reference lines ---------------------------------------------------------
    SDL_SetRenderDrawColor(renderer, 132,121,124,150);
        // Vertical grid lines (for x-axis)
    for (float x = startX; x <= DOMAIN_MAX; x += gridSpacing) {
        // Convert x from domain space to screen space
        float screenX = ((x - DOMAIN_MIN) / DOMAIN_INTERVAL) * WINDOW_SIZE_X;
        SDL_RenderLine(renderer, screenX, 0, screenX, WINDOW_SIZE_Y);
        
    }
        // Horizontal grid lines (for y-axis)
    for (float y = startY; y <= RANGE_MAX; y += gridSpacing) {
        // Convert y from range space to screen space
        float screenY = ((RANGE_MAX - y) / RANGE_INTERVAL) * WINDOW_SIZE_Y;
        SDL_RenderLine(renderer, 0, screenY, WINDOW_SIZE_X, screenY);
    }
    // -------------------------------------------------------------------------------
    //////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////  
    // graph axes tick marks ---------------------------------------------------------
    SDL_SetRenderDrawColor(renderer, 132,121,124,255);
        // domain tick marks
    for (float x = startX; x <= DOMAIN_MAX; x += gridSpacing) {
        float screenX = ((x - DOMAIN_MIN) / DOMAIN_INTERVAL) * WINDOW_SIZE_X;
        SDL_RenderLine(renderer, screenX, x_origin - 5, screenX, x_origin + 5);
    }

        // range tick marks
    for (float y = startY; y <= RANGE_MAX; y += gridSpacing) {
        float screenY = ((RANGE_MAX - y) / RANGE_INTERVAL) * WINDOW_SIZE_Y;
        SDL_RenderLine(renderer, y_origin - 5, screenY, y_origin + 5, screenY);
    }
    // -------------------------------------------------------------------------------
    //////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////
    // creates and renders tick mark labels for the axes -----------------------------
    ////// for x-axis
    for (float x = startX; x <= DOMAIN_MAX; x += gridSpacing) {
        // skip zero to avoid overlap with origin
        if (std::abs(x) < 0.01) continue;

        float screenX = ((x - DOMAIN_MIN) / DOMAIN_INTERVAL) * WINDOW_SIZE_X;

        // creates text for the tick value
        std::string tickText = std::to_string(static_cast<int>(std::round(x)));

        // renders the text
        SDL_Surface* textSurface = TTF_RenderText_Blended(tickFontSmall, tickText.c_str(), tickText.length(), textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            
            // position the text below the x-axis
            SDL_FRect renderQuad = { 
                screenX - static_cast<float>(textSurface->w) / 2.0f, // center horizontally
                x_origin + 8, // below tick mark
                static_cast<float>(textSurface->w), 
                static_cast<float>(textSurface->h) 
            };
            
            SDL_RenderTexture(renderer, textTexture, nullptr, &renderQuad);
            SDL_DestroySurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
    for (float y = startY; y <= RANGE_MAX; y += gridSpacing) {
        // skip zero to avoid overlap with origin
        if (std::abs(y) < 0.01) continue;

        float screenY = ((RANGE_MAX - y) / RANGE_INTERVAL) * WINDOW_SIZE_Y;

        // creates text for the tick value
        std::string tickText = std::to_string(static_cast<int>(std::round(y)));

        // renders the text
        SDL_Surface* textSurface = TTF_RenderText_Blended(tickFontSmall, tickText.c_str(), tickText.length(), textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            
            // position the text to the left of the y-axis
            SDL_FRect renderQuad = { 
                y_origin - static_cast<float>(textSurface->w) - 8, // to the left of tick mark
                screenY - static_cast<float>(textSurface->h) / 2.0f, // center vertically
                static_cast<float>(textSurface->w), 
                static_cast<float>(textSurface->h) 
            };
            
            SDL_RenderTexture(renderer, textTexture, nullptr, &renderQuad);
            SDL_DestroySurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
    // -------------------------------------------------------------------------------
    //////////////////////////////////////////////////////////////////////////////////

    // this draws the axis lines if theyre applicable within the given domain/range
    SDL_RenderLine(renderer, 0, x_origin, WINDOW_SIZE_X, x_origin);
    SDL_RenderLine(renderer, y_origin, 0, y_origin, WINDOW_SIZE_Y);

    // sets back to default and renders
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderTexture(renderer, cachedAxes, nullptr, nullptr);
    axesBad = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// the function responsible for graphing the points onto the window
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void graphMain::functionRender() {
    if (!funcBad && cachedFunc) {
        SDL_RenderTexture(renderer, cachedFunc, nullptr, nullptr);
        return;
    }

    if (cachedFunc) SDL_DestroyTexture(cachedFunc);
    cachedFunc = makeTargetTexture(renderer);
    SDL_SetRenderTarget(renderer, cachedFunc);

    // calculates values of the function
    outputs.executeFunctionCalculation();

    // This scales and renders the initial points to visualize the function on the graph
    if (outputs.func_valid) {

        // scales and plots the points onto the screen
        std::vector<SDL_FPoint> SDL_fpoints;
        SDL_fpoints.reserve(outputs.fpoints.size());

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (size_t i = 0; i < outputs.fpoints.size(); i++) {
            // Transform x from domain space to screen space
            int px = ((outputs.fpoints[i].x - DOMAIN_MIN) / DOMAIN_INTERVAL) * WINDOW_SIZE_X;

            // Transform y from range space to screen space
            int py = ((RANGE_MAX - outputs.fpoints[i].y) / RANGE_INTERVAL) * WINDOW_SIZE_Y;

            SDL_FPoint point;
            point.x = px;
            point.y = py;
            SDL_fpoints.push_back(point);
        }

        // Renders the points on the renderer
        for (size_t i = 0; i < SDL_fpoints.size() - 1; i++) {
            // in order to fix this you can create a new function that runs through all points as they are being calculated
            // in numOutputs. This will detect when a the function is increasing as an asymptote would: think 0.1, 0.01, 0.001, etc
            // when it detects this it flags the "approximate" asymptote value and logs that into a vector array of asymptote values
            // this array will then be used by functionRender here to determine where it should NOT be drawing lines (or somthing
            // like that idrk lol) Like basically it goes through the fpoints array and when it sees a big y jump its like 'oh shit
            // theres an asymptote between those values' and it does all the things.
            if (SDL_fpoints[i].y < 100 && SDL_fpoints[i + 1].y > 1400) {
                continue;
            }
            else {
                SDL_RenderLine(renderer,  
                               SDL_fpoints[i].x, SDL_fpoints[i].y,      // start point
                               SDL_fpoints[i + 1].x, SDL_fpoints[i + 1].y);  // end point
            }
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderTexture(renderer, cachedFunc, nullptr, nullptr);
    funcBad = false;
}
