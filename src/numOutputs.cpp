// this file is responsible for doing and numerical calculations associated with graphing the function
#include "graphing.hpp"
#include <cmath>
#include <functional>
#include <vector>
#include <SDL2/SDL.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIS FUNCTION CALCULATES THE ARRAY OF POINTS, AS WELL AS MIN/MAX FOR THE USER INPUT GRAPH
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void numOutputs::executeFunctionCalculation() {
    fpoints.clear();
    func_valid = false;

    x = DOMAIN_MIN;
    for (int i = 0; i <= FUNC_RES; i++, x += INCREMENT) {
        y = ff(x);

        // only appends points that exist in the domain, and are within the window boundary
        if (y >= RANGE_MIN && y <= RANGE_MAX) {
            func_valid = true; // makes the function valid (value stored in numOutputs.hpp)
            fpoints.push_back({ x, y });
        }
    }
    
    // calculates min and max for the x array and y array
    if (func_valid) {
        x_min = fpoints[0].x;
        x_max = fpoints.back().x;
        y_min = y_max = fpoints.front().y;
        for (const coordinate& point : fpoints) {
            if (point.y < y_min) y_min = point.y;
            if (point.y > y_max) y_max = point.y;
        }

        // range of the function
        x_range = x_max - x_min;
        y_range = y_max - y_min;
    }
}