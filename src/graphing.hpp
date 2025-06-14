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
#include <SDL3_ttf/SDL_ttf.h>

#ifdef _WIN32
    constexpr float WINDOW_SIZE_X = 1500;
    constexpr float WINDOW_SIZE_Y = 1500;
#else
    constexpr float WINDOW_SIZE_X = 1000;
    constexpr float WINDOW_SIZE_Y = 1000;
#endif

inline std::string ff = "x";

inline float DOMAIN_MIN = -10;
inline float DOMAIN_MAX = 10;
inline float RANGE_MIN = -10;
inline float RANGE_MAX = 10;
inline float DOMAIN_INTERVAL = DOMAIN_MAX - DOMAIN_MIN;
inline float RANGE_INTERVAL = RANGE_MAX - RANGE_MIN;
inline int FUNC_RES = 100; // the initial resolution at which the function should display. This is the baseline for adaptive sampling to work from
inline size_t MAX_POINT_COUNT = 1000; // the maximum amount of points that can be calculated for the screen at once
inline double INCREMENT = (DOMAIN_MAX - DOMAIN_MIN) / FUNC_RES;

extern TTF_Font* font; // this is so all classes can see the font file variable. It MUST be opened in main.cpp in order to work at all.
extern TTF_Font* tickFontSmall;

inline bool funcBad = true; // recompute f(x) if it its previous state is no longer valid 
                            //(function command changed the function string value)
inline bool axesBad = true; // same as funcBad but for the axes

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
    std::vector<coordinate> fpoints; // the array that the coordinate points for the function are stored
    std::vector<double> vertAsymptotes;
    std::vector<size_t> asymptotePos; // a number occupying this array represents the position of a point in fpoints
                                   // that exists right BEFORE an asymptote

    double y_min, y_max, x_min, x_max, x_range, y_range, x, y = 0.0, f_val;
    bool func_valid = false;
    bool hasAsymptote = false;
    double xi; // NOTE: YOU'RE REQUIRED TO DEFINE A VALUE FOR xi BEFORE THE FUNCTION EXECUTES

    std::stack<std::string> operators;
    std::queue<std::variant<double, std::string>> expression;

    void fInputParse();
    int determinePrecedence(const std::string& op);
    void executeFunctionCalculation();
    void executeParseCalc();
    void determineAsymptote();
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
    bool mouseHeld; // if the mouse is held down
    float cPosX, cPosY; // current position of the mouse

    void commandWindow();
    void textInput();
    void commands();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif

// colors :)
constexpr SDL_Color textColor = { 210, 210, 210, 255 };