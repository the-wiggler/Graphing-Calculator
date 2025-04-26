#include <iostream>
#include "numOutputs.hpp"
#include <cmath>
#include <vector>

void numOutputs::userInputFunction() {
    // this function is meant to change based on what you want it input :)
    y = x * x - 5;
}

void numOutputs::executeFunctionCalculation() {
    // output arrays
    x_arr.clear();
    y_arr.clear();

    x = DOMAIN_MIN;
    for (int i = 0; i <= FUNC_RES; i++, x += INCREMENT) {
        userInputFunction();

        // only appends points that exist in the domain, and are within the window boundary
        if (y >= RANGE_MIN && y <= RANGE_MAX) {
            func_valid = true; // makes the function valid (value stored in numOutputs.hpp)
            x_arr.push_back(x);
            y_arr.push_back(y);
        }
    }
    
    // calculates min and max for the x array and y array
    if (func_valid) {
        x_min = x_arr.front();
        x_max = x_arr.back();
        y_min = y_max = y_arr.front();
        for (double yy : y_arr) {
            if (yy < y_min) y_min = yy;
            if (yy > y_max) y_max = yy;
        }

        // range of the function
        x_range = x_max - x_min;
        y_range = y_max - y_min;
    }
}