#include <iostream>
#include <sstream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "graphing.hpp"

std::string inputText = "";

////////////////////////////////////////////////////////////////////////////////////////////
// available commands
////////////////////////////////////////////////////////////////////////////////////////////
void uiMain::commands()
{
    std::istringstream str(inputText);
    std::string cmd;
    double value;

    if (!(str >> cmd)) return;

    // commands that take a single numeric argument
    if ((cmd == "xmin" || cmd == "xmax" ||
        cmd == "ymin" || cmd == "ymax") && (str >> value))
    {
        if (cmd == "xmin") DOMAIN_MIN = value;
        else if (cmd == "xmax") DOMAIN_MAX = value;
        else if (cmd == "ymin") RANGE_MIN = value;
        else if (cmd == "ymax") RANGE_MAX = value;
        return; 
    }

    // one-word commands
    if (cmd == "q") { running = false; return; }

    // function selection
    if (cmd == "x")     ff = [](double x) { return x; };
    else if (cmd == "x^2")   ff = [](double x) { return x * x; };
    else if (cmd == "x^3")   ff = [](double x) { return x * x * x; };
    else if (cmd == "x^4")   ff = [](double x) { return x * x * x * x; };
    else if (cmd == "sin(x)")ff = [](double x) { return sin(x); };
    else if (cmd == "cos(x)")ff = [](double x) { return cos(x); };
    else if (cmd == "tan(x)")ff = [](double x) { return tan(x); };
    else if (cmd == "log(x)")ff = [](double x) { return log(x); };
    else if (cmd == "abs(x)")ff = [](double x) { return std::abs(x); };
    else return;                         

}

////////////////////////////////////////////////////////////////////////////////////////////
// collects text input
////////////////////////////////////////////////////////////////////////////////////////////
void uiMain::textInput() {
    TTF_Font* asana = TTF_OpenFont("asana.ttf", 12);

    SDL_StartTextInput();
    bool quit = false;
    SDL_Event e;

    // the text area
    SDL_Rect textRect = { 10, 105, 300, 20 };
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                running = false;
            }
            else if (e.type == SDL_TEXTINPUT) inputText += e.text.text;
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && !inputText.empty()) {
                    inputText.pop_back();
                }
                else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                    SDL_SetClipboardText(inputText.c_str());
                }
                else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                    inputText = SDL_GetClipboardText();
                }

                // all of the supported commands that can be entered into the command window
                else if (e.key.keysym.sym == SDLK_RETURN) {
                    std::cout << "COMMAND INPUT: " << inputText << std::endl;

                    commands();

                    inputText.clear();
                    recalculateRange();
                    quit = true;
                }
            }
        }

        // the box that renders over the previous text when updated
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &textRect);

        // only updates the screen if there is something in the string
        if (!inputText.empty()) {
            SDL_Color textColor = { 0, 0, 0, 255 };
            SDL_Surface* textSurface = TTF_RenderText_Solid(asana, inputText.c_str(), textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect renderQuad = { textRect.x, textRect.y, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
        SDL_RenderPresent(renderer);
        // delay to save resources
        SDL_Delay(16);
    }

    SDL_RenderClear(renderer);
    SDL_StopTextInput();
    TTF_CloseFont(asana);
}


////////////////////////////////////////////////////////////////////////////////////////////
// UI feature that holds the background for the command window
////////////////////////////////////////////////////////////////////////////////////////////
void uiMain::commandWindow() {
    // draw onto the texture
    SDL_Texture* commandTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        300,
        120);
    SDL_SetTextureBlendMode(commandTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, commandTexture);

    // draw the background
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 80);
    SDL_Rect cmdBox = { 5, 5, 300, 120 };
    SDL_RenderFillRect(renderer, &cmdBox);

    // sets back to default and renders
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, commandTexture, nullptr, &cmdBox);
    //SDL_DestroyTexture(commandTexture);

    textInput();
}