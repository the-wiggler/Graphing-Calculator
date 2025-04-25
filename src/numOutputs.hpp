#ifndef NUM_OUTPUTS_HPP
#define NUM_OUTPUTS_HPP

#include <vector>

class numericalOutputs {
public:
    int function_res;
    std::vector<double> x_arr, y_arr;
    double y_min, y_max, x_min, x_max, x_range, y_range;

    void executeFunctionCalculation();
};

#endif // NUMERICAL_OUTPUTS_HPP