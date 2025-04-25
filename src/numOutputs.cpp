#include "numOutputs.hpp"
#include <cmath>

void numericalOutputs::executeFunctionCalculation() {
    function_res = 1000;
    double domain_min = -50;
    double domain_max = 5;
    double function_res_render_increment = (domain_max - domain_min) / function_res;

    double x = domain_min;
    for (int i = 0; i < function_res + 1; i++) {
        double y = sin(10 * x) * cos(12 * pow(x, 2)) * exp(-0.1 * pow(x, 2)) +
            0.4 * tanh(5 * sin(8 * x)) * cos(20 * x) +
            0.2 * sin(30 * x) * pow(cos(x * x), 2) * sin(x * x * x) +
            0.15 * sin(50 * x) / (1 + pow(x, 2)) * cos(x * x * 5) *
            (sin(x * 25) > 0 ? sin(x * 25) : 0.3 * cos(x * 15));

        x_arr.push_back(x);
        y_arr.push_back(y);

        x += function_res_render_increment;
    }

    x_min = domain_min;
    x_max = domain_max;
    y_min = y_arr[0];
    y_max = y_arr[0];
    for (double y : y_arr) {
        if (y < y_min) y_min = y;
        if (y > y_max) y_max = y;
    }

    x_range = domain_max - domain_min;
    y_range = y_max - y_min;
}
