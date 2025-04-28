// this is the header file that holds the classes for operating the other files 
#ifndef GRAPHING
#define GRAPHING

#include <vector>
#include <SDL2/SDL.h>

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 800

// THIS IS RESPONSIBLE FOR HOLDING THE MATHEMATICAL ARRAY DATA
class numOutputs {
public:
    const int FUNC_RES = 1000;
    const float DOMAIN_MIN = -5;
    const float DOMAIN_MAX = 5;
    const float DOMAIN_INTERVAL = DOMAIN_MAX - DOMAIN_MIN;
    const float RANGE_MIN = -5;
    const float RANGE_MAX = 3;
    const float RANGE_INTERVAL = RANGE_MAX - RANGE_MIN;
    const double INCREMENT = (DOMAIN_MAX - DOMAIN_MIN) / FUNC_RES;

    typedef struct coordinate {
        double x, y;
    } coordinate;
    std::vector<coordinate> fpoints;

    double y_min, y_max, x_min, x_max, x_range, y_range, x, y = 0.0;
    bool func_valid = false;

    void executeFunctionCalculation();
    void userInputFunction();

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

// THIS IS RESOPNSIBLE FOR MANAGING THE WINDOW
class windowMain {
public:
    bool running = true;
    void keepWindowOpen();
};

// THIS IS RESPONSIBLE FOR MANAGING THE UI
class uiMain {
public:
    uiMain(SDL_Renderer* r) : renderer(r) {}

    void commandWindow();

private:
    SDL_Renderer* renderer;
};

#endif