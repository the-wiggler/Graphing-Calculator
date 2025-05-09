﻿#include <iostream>
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
    std::string argument;

    if (!(str >> cmd)) return;

    // commands that take a single numeric argument
    if (cmd == "xmin" || cmd == "xmax" || cmd == "ymin" || cmd == "ymax") {
        double value;
        if (!(str >> value)) {
            std::cout << "ERROR: Expected a numeric argument for '" << cmd << "'\n";
            return;
        }
        if (cmd == "xmin" && !(value >= DOMAIN_MAX))        DOMAIN_MIN = value;
        else if (cmd == "xmax" && !(value <= DOMAIN_MIN))   DOMAIN_MAX = value;
        else if (cmd == "ymin" && !(value >= RANGE_MAX))    RANGE_MIN = value;
        else if (cmd == "ymax" && !(value <= RANGE_MIN))    RANGE_MAX = value;
        std::cout << cmd << " set to:" << value << "\n";
        return; 
    }

    // one-word commands
    if (cmd == "q" || cmd == "quit" || cmd == "exit") { running = false; return; }
    if (cmd == "help") { std::cout << "Here's a list of all commands:\n" << "q ~ quit\nhelp ~ get list of commands\nxmin ~ set domain minimum\nxmax ~ set domain maximum\nymin ~ set domain minimum\nymax ~ set domain maximum\n"; return; }

    // function selection
    if (cmd == "=" || cmd == "y=" || cmd == "y") {
        // assumes every character after cmd as the input function
        std::string new_func;
        
        str >> std::ws;
        std::getline(str, new_func);

        if (new_func.length() > 0) {
            // now we send the value of new_func to the main function string in the header file
            // This way it can be referenced by other parts of the program
            std::cout << "COMMAND: Function Changed from " << ff << " to ";
            ff = new_func;
            std::cout << ff << std::endl;
        }
        else {
            std::cerr << "ERROR: Invalid Function Input\n";
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////////
// collects text input
////////////////////////////////////////////////////////////////////////////////////////////
void uiMain::textInput() {
    TTF_Font* asana = TTF_OpenFont("asana.ttf", 18);

    SDL_StartTextInput();
    bool quit = false;
    SDL_Event e;

    // the text area
    SDL_Rect textRect = { 10, 20, 300, 25 };
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
                    ////////////////////////////////////////////////////////////////////////
                    // runs the command window that allows for text input
                       commands();
                    ////////////////////////////////////////////////////////////////////////

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