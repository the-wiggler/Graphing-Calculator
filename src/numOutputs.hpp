#ifndef NUM_OUTPUTS_HPP
#define NUM_OUTPUTS_HPP

#include <vector>

class numOutputs {
public:
    const int FUNC_RES = 5000;
    const float DOMAIN_MIN = -5;
    const float DOMAIN_MAX = 1;
    const float RANGE_MIN = -1;
    const float RANGE_MAX = 1;
    const double INCREMENT = (DOMAIN_MAX - DOMAIN_MIN) / FUNC_RES;

    std::vector<double> x_arr, y_arr;
    double y_min, y_max, x_min, x_max, x_range, y_range, x, y;
    bool func_valid;

    void executeFunctionCalculation();
    void userInputFunction();

};

#endif