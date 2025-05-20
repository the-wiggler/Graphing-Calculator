// this is the header file that holds the classes for operating the other files 
#ifndef GRAPHING
#define GRAPHING

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <variant>
#include <sstream>
#include <SDL3/SDL.h>


#ifdef _WIN32
    constexpr float WINDOW_SIZE_X = 800;
    constexpr float WINDOW_SIZE_Y = 800;
#else
    constexpr float WINDOW_SIZE_X = 1000;
    constexpr float WINDOW_SIZE_Y = 1000;
#endif

inline std::string ff = "x";

inline int FUNC_RES = 5000;
inline float DOMAIN_MIN = -10;
inline float DOMAIN_MAX = 10;
inline float RANGE_MIN = -10;
inline float RANGE_MAX = 10;
inline float DOMAIN_INTERVAL = DOMAIN_MAX - DOMAIN_MIN;
inline float RANGE_INTERVAL = RANGE_MAX - RANGE_MIN;
inline double INCREMENT = (DOMAIN_MAX - DOMAIN_MIN) / FUNC_RES;

inline bool funcBad = true; // recompute f(x) if it its previous state is no longer valid 
                            //(function command changed the function string value)
inline bool axesBad = true;

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

    double y_min, y_max, x_min, x_max, x_range, y_range, x, y = 0.0, f_val;
    bool func_valid = false;
    double xi; // NOTE: YOU'RE REQUIRED TO DEFINE A VALUE FOR xi BEFORE THE FUNCTION EXECUTES

    std::stack<std::string> operators;
    std::queue<std::variant<double, std::string>> expression;

    void fInputParse();
    int determinePrecedence(const std::string& op);
    void executeFunctionCalculation();
    void executeParseCalc();
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
    uiMain(SDL_Window* w, SDL_Renderer* r) : window(w), renderer(r) {}


    bool running = true;

    void commandWindow();
    void textInput();
    void commands();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif

