#ifndef NUM_OUTPUTS_HPP
#define NUM_OUTPUTS_HPP

#include <vector>
#include <SDL2/SDL.h>

class numOutputs {
public:
    const int FUNC_RES = 100000;
    const float DOMAIN_MIN = -5;
    const float DOMAIN_MAX = 5;
    const float DOMAIN_INTERVAL = DOMAIN_MAX - DOMAIN_MIN;
    const float RANGE_MIN = -5;
    const float RANGE_MAX = 10;
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

#endif