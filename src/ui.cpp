#include <iostream>
#include <sstream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
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

        axesBad = true;
        funcBad = true;
        return; 
    }

    // one-word commands
    if (cmd == "q" || cmd == "quit" || cmd == "exit") { running = false; return; }
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

        funcBad = true;
    }

}

////////////////////////////////////////////////////////////////////////////////////////////
// collects text input
////////////////////////////////////////////////////////////////////////////////////////////
void uiMain::textInput() {

    SDL_StartTextInput(window);
    bool quit = false;
    SDL_Event e;

    // the text area
    SDL_FRect textRect = { 10.0, 10.0, WINDOW_SIZE_X/3, WINDOW_SIZE_Y/20 };
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
                running = false;
            }
            else if (e.type == SDL_EVENT_TEXT_INPUT) inputText += e.text.text;
            else if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_BACKSPACE && !inputText.empty()) {
                    inputText.pop_back();
                }
                else if (e.key.key == SDLK_C && SDL_GetModState() & SDLK_LCTRL) {
                    SDL_SetClipboardText(inputText.c_str());
                }
                else if (e.key.key == SDLK_V && SDL_GetModState() & SDLK_LCTRL) {
                    inputText = SDL_GetClipboardText();
                }

                // all of the supported commands that can be entered into the command window
                else if (e.key.key == SDLK_RETURN) {
                    std::cout << "COMMAND INPUT: " << inputText << std::endl;
                    ////////////////////////////////////////////////////////////////////////
                    // runs the command window that allows for text input
                       commands();
                    ////////////////////////////////////////////////////////////////////////

                    inputText.clear();
                    recalculateRange();
                    axesBad = true;
                    funcBad = true;

                    quit = true;
                }
            }
        }

        // the box that renders over the previous text when updated
        SDL_SetRenderDrawColor(renderer, 49,54,58, 255);
        SDL_RenderFillRect(renderer, &textRect);

        // only updates the screen if there is something in the string
        if (!inputText.empty()) {
            SDL_Color textColor = { 210, 210, 210, 255 };
            SDL_Surface* textSurface = TTF_RenderText_Blended(font, inputText.c_str(), inputText.length(), textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FRect renderQuad = { textRect.x, textRect.y, static_cast<float>(textSurface->w), static_cast<float>(textSurface->h) };
            SDL_RenderTexture(renderer, textTexture, nullptr, &renderQuad);
            SDL_DestroySurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }

        // renders the function every time a new text input is checked for
        SDL_RenderPresent(renderer);
        break;
    }
    SDL_StopTextInput(window);
}
