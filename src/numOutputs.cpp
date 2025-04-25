#include <iostream>
#include "numOutputs.hpp"
#include <cmath>
#include <vector>

void numOutputs::userInputFunction() {
    // this function is meant to change based on what you want it input :)
    y = pow(x,3);
}

void numOutputs::executeFunctionCalculation() {
    function_res = 5000;
    domain_min = -3;
    domain_max = 5;
    range_min = -10;
    range_max = 10;
    double function_res_render_increment = (domain_max - domain_min) / function_res;

    x_arr.clear();
    y_arr.clear();

    x = domain_min;
    for (int i = 0; i <= function_res; i++) {
        userInputFunction();

        // only appends points that exist in the domain, and are within the window boundary
        if (std::isfinite(y) && y >= range_min && y <= range_max) {
            x_arr.push_back(x);
            y_arr.push_back(y);
        }

        x += function_res_render_increment;
    }
    
    x_min = x_arr[0];
    x_max = x_arr[x_arr.size() - 1];
    y_min = y_arr[0];
    y_max = y_arr[0];
    for (double y_val : y_arr) {
        if (y_val < y_min) y_min = y_val;
        if (y_val > y_max) y_max = y_val;
    }

    x_range = x_max - x_min;
    y_range = y_max - y_min;
}