#include <iostream>
#include "numOutputs.hpp"
#include <cmath>
#include <vector>

void numOutputs::userInputFunction() {
    // this function is meant to change based on what you want it input :)
    y = sin(x);
}

void numOutputs::executeFunctionCalculation() {
    const int FUNC_RES = 5000;
    const float DOMAIN_MIN = -5;
    const float DOMAIN_MAX = 5;
    const float RANGE_MIN = -1;
    const float RANGE_MAX = 1;
    const double INCREMENT = (DOMAIN_MAX - DOMAIN_MIN) / FUNC_RES;

    // output arrays
    x_arr.clear();
    y_arr.clear();

    // updates to true when the function is seen in the domain
    func_valid = false;

    x = DOMAIN_MIN;
    for (int i = 0; i <= FUNC_RES; i++, x += INCREMENT) {
        userInputFunction();

        // only appends points that exist in the domain, and are within the window boundary
        if (y >= RANGE_MIN && y <= RANGE_MAX) {
            func_valid = true;
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