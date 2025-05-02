// this is the header file that holds the classes for operating the other files 
#ifndef GRAPHING
#define GRAPHING

#include <iostream>
#include "graphing.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <stack>
#include <sstream>
#include <SDL2/SDL.h>


#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 1000

static SDL_Color black = { 0, 0, 0, 255 };

inline std::string ff = "2 * 50 + 2";

inline int FUNC_RES = 10000;
inline float DOMAIN_MIN = -10;
inline float DOMAIN_MAX = 10;
inline float RANGE_MIN = -10;
inline float RANGE_MAX = 10;
inline float DOMAIN_INTERVAL = DOMAIN_MAX - DOMAIN_MIN;
inline float RANGE_INTERVAL = RANGE_MAX - RANGE_MIN;
inline double INCREMENT = (DOMAIN_MAX - DOMAIN_MIN) / FUNC_RES;

inline void recalculateRange() {
    DOMAIN_INTERVAL = DOMAIN_MAX - DOMAIN_MIN;
    RANGE_INTERVAL = RANGE_MAX - RANGE_MIN;
    INCREMENT = (DOMAIN_MAX - DOMAIN_MIN) / FUNC_RES;
}

typedef struct coordinate {
    double x, y;
} coordinate;




// THIS IS RESPONSIBLE FOR HOLDING THE MATHEMATICAL ARRAY DATA
class numOutputs {
public:
    std::vector<coordinate> fpoints;

    double y_min, y_max, x_min, x_max, x_range, y_range, x, y = 0.0;
    bool func_valid = false;

    void executeFunctionCalculation();
    void fInputParser();
    bool isOperator(char c);
};

// THIS IS RESPONSIBLE FOR GRAPHING THE ARRAY OF FUNCTION DATA ONTO THE WINDOW (USED BY graph.cpp)
class graphMain {
public:
    graphMain(SDL_Renderer* r) : renderer(r) {}

    void axesRender();
    void functionRender();

private:
    SDL_Renderer* renderer;
};

// THIS IS RESPONSIBLE FOR MANAGING THE UI
class uiMain {
public:
    uiMain(SDL_Renderer* r) : renderer(r) {}

    bool running = true;

    void commandWindow();
    void textInput();
    void commands();

private:
    SDL_Renderer* renderer;
};

#endif

