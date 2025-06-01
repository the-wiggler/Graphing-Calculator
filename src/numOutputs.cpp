// this file is responsible for doing and numerical calculations associated with graphing the function
#include <iostream>
#include "graphing.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <sstream>
#include <variant>
#include <SDL3/SDL.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIS FUNCTION CHANGES THE ff (user input) string into usable mathematical expression for executeFunctionCalculation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int numOutputs::determinePrecedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    if (op == "sin" || op == "cos" || op == "tan" || op == "log" || op == "ln" || op == "sqrt") return 4;
    return 0;
}

static bool isComplexFunction(const std::string& funcToken) {
    return funcToken == "sin" || funcToken == "cos" || funcToken == "tan" ||
           funcToken == "log" || funcToken == "ln"  || funcToken == "sqrt";
}

void numOutputs::fInputParse() {
    while (!operators.empty())  operators.pop();
    while (!expression.empty()) expression.pop();

    std::istringstream fI(ff);
    std::string token;

    bool expectOperand = true; // true when expecting a number next in the string

    token.clear();

    for (std::size_t i = 0; i < ff.length(); ++i) {
        char c = ff[i];

        /* ignores whitespace -----------------------------------------------------------------*/
        if (std::isspace(c)) continue;

        /* if it is an operand associated character -------------------------------------------*/
        if (std::isdigit(c) || c == '.') {
            token.clear();
            while (i < ff.length() && (std::isdigit(ff[i]) || ff[i] == '.')) {
                token += ff[i++];
            }
            --i; // step back one char
            expression.emplace(std::stod(token));
            expectOperand = false;

            // Check for multiplication without "*"
            char next = ff[i+1];
            if (i + 1 < ff.length() && next == 'x') {
                operators.push("*");
            }
            else if (i + 1 < ff.length() && !(next == '+' || next == '-' || next == '*' || next == '/' || next == '^' || next == ')')) {
                operators.push("*");
            }
            
        }

        /* if it is a variable (x) ------------------------------------------------------------*/
        else if (c == 'x') {
            token.clear();
            while (i < ff.length() && std::isalpha(ff[i])) {
                token += ff[i++];
            }
            --i;
            token = c;
            expression.emplace(token);
            expectOperand = false;
        }

        /* if it is a multi-letter function: sin, cos, etc ---------  -------------------------*/
        else if (std::isalpha(c)) {
            token.clear();
            while (i < ff.length() && std::isalpha(ff[i])) {
                token += ff[i++];
            }
            -- i;
            if (isComplexFunction(token)) {
                operators.push(token);
                expectOperand = true;
            }
            else {
                std::cerr << "ERROR: You typed in the function wrong lol\n";
                return;
            }
        }

        /* if it is an operator or parenthesis ------------------------------------------------*/
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '^') {
            token.clear();
            token = c;
            // this first if statement functions to ignore a "-" if its already being used on a negative number
            if (c == '-' && expectOperand == true) {
                expression.emplace(0.0);
                operators.push(token);
            }
            // handles left parenthesis by adding it to the operators stack
            else if (token == "(") {
                operators.push(token);
                expectOperand = true;
            }
            // to handle order of operations relating to parinthesis, when it sees a ")" it begins adding the 
            // operators 'inside' of the parinthesis to the expression stack
            else if (token == ")") {
                // organizes considering order of operations regarding parintheses into RPN
                while (!operators.empty() && operators.top() != "(") {
                    expression.emplace(operators.top());
                    operators.pop();
                }
                // pop the left '(' when it no longer matters
                if (!operators.empty() && operators.top() == "(") operators.pop();
                // checks if there is a function before the "(", if so, adds to expression queue
                if (!operators.empty() && isComplexFunction(operators.top())) {
                    expression.emplace(operators.top());
                    operators.pop();
                }

                expectOperand = false;
            }
            // otherwise handles order of operations when parinthesis are not directly involved
            else {
                while (!operators.empty() && operators.top() != "(") {
                    int op1 = determinePrecedence(token);
                    int op2 = determinePrecedence(operators.top());

                    if (op2 >= op1) {
                        expression.emplace(operators.top());
                        operators.pop();
                    }
                    else break;
                }
                operators.push(token);
                expectOperand = true;
            }
        }
    }

    // if any operators remain in the stack, they are pushed into the expression queue
    while (!operators.empty()) {
        expression.emplace(operators.top());
        operators.pop();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIS EVALUATES THE FINAL FUNCTION VALUES OF THE ALGEBRAIC QUEUE CREATED BY FINPUTPARSE (expression)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void numOutputs::executeParseCalc() {
    // evaluates the newly created mathematical structure by inputting a value of xi
    std::queue<std::variant<double, std::string>> exprCopy = expression;
    std::stack<double> eval;
    
    while (!exprCopy.empty()) {
        auto tok = exprCopy.front();
        exprCopy.pop();

        if (std::holds_alternative<double>(tok)) {
            // its a number
            double n = std::get<double>(tok);
            eval.push(n);
            continue;
        }
        else if (std::holds_alternative<std::string>(tok)) {
            std::string c = std::get<std::string>(tok);
            double result = 0.0;

            // if the character is x, replace x with xi to simulate it being number
            if (c == "x") {
                eval.push(xi);
                continue;
            }

            // if the character is an operator, perform operation
            else if (c == "+" || c == "-" || c == "*" || c == "/" || c == "^") {
                double right = eval.top(); eval.pop();
                double left = eval.top(); eval.pop();

                if (c == "+") result = left + right;
                else if (c == "-") result = left - right;
                else if (c == "*") result = left * right;
                else if (c == "/") result = left / right;
                else if (c == "^") result = std::pow(left, right);

                eval.push(result);
            }

            // if the character is a *special~* function
            else if (c == "sin" || c == "cos" || c == "tan" || 
                     c == "log" || c == "ln"  || c == "sqrt") {
                double v = eval.top(); eval.pop();

                if      (c == "sin") result =   std::sin(v);
                else if (c == "cos") result =   std::cos(v);
                else if (c == "tan") result =   std::tan(v);
                else if (c == "log") result =   std::log10(v);
                else if (c == "ln")  result =   std::log(v);
                else if (c == "sqrt") result =  std::sqrt(v);

                eval.push(result);
            } 
        }
    }
    f_val = eval.top();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIS FUNCTION CALCULATES THE ARRAY OF POINTS, AS WELL AS MIN/MAX FOR THE USER INPUT GRAPH
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void numOutputs::executeFunctionCalculation() {
    fpoints.clear();
    func_valid = false;

    fInputParse();

    const double TOLERANCE = 0.0001; // once the difference in interpolated y and calculated y reaches this
                                  // limit, it will stop the calculation as it is "good enough"
                                  
    /*---sets the vector array with a min # of points-------*/
    x = DOMAIN_MIN; // starts at DOMAIN_MIN, and ends at DOMAIN_MAX
    double samples_per = DOMAIN_INTERVAL / (FUNC_RES - 1); // the increments in which x should increase until reaching DOMAIN_MAX
    for (int i = 0; i < FUNC_RES; i++, x += samples_per) {
        xi = x;
        executeParseCalc();
        y = f_val;
        fpoints.push_back({ x, y });
    }
    /*------------------------------------------------------*/

    /*---adaptive sampling operation------------------------*/
    bool finished_calculations = false;
    int current_iteration = 0;
    int MAX_ITERATIONS = 10; // to prevent getting stuck in an infinite loop (for now)

    while (!finished_calculations && current_iteration <= MAX_ITERATIONS) {
        bool did_add_points = false; // if points were actually added to the vector array, this is set to true

        for (std::size_t i = 0; i < fpoints.size() - 1; i++) {
            coordinate fx1 = fpoints[i];
            coordinate fx2 = fpoints[i + 1];

            double midpoint_x = (fx2.x + fx1.x) / 2; // calculates the midpoint between fx1 and fx2
            
            xi = midpoint_x;
            executeParseCalc(); // performs the operation that determines the y value at this x midpoint

            double y_at_mid = f_val; // sets the y value at the midpoint equal to y_at_pos

            // now we calculate the interpolation so it can be reference against out tolerance to check if its in an acceptable range yet!
            // this uses the linear interpolation equation (look it up on wikipedia if u forgot lol)
            double interpolated_y = fx1.y + (fx2.y - fx1.y) * ((midpoint_x - fx1.x) / (fx2.x - fx1.x));

            // if the TRUE value is not within acceptable range of the interpolated value, it will insert the new point we calculated.
            // the second statement after the and in the if statement is temporary and meant to fix issues when an asymptote exists and the function calculates
            // infinitely. For example tan(x), where the difference of asymptotes is very large, it will ignore performing adaptive sampling if the difference
            // in points is too great
            if (std::abs(y_at_mid - interpolated_y) > TOLERANCE) {
                // insert the new point right after fx1 (at position i+1)
                fpoints.insert(fpoints.begin() + i + 1, {midpoint_x, y_at_mid});
                did_add_points = true;
            }
        }

        if (!did_add_points || fpoints.size() >= MAX_POINT_COUNT) {
            finished_calculations = true;
        }

        current_iteration++;
    }
    /*------------------------------------------------------*/

    /*---calculates min and max for the x array and y array-*/
    x_min = fpoints[0].x;
    x_max = fpoints.back().x;
    y_min = y_max = fpoints.front().y;

    for (const coordinate& point : fpoints) {
        if (point.y < y_min) y_min = point.y;
        if (point.y > y_max) y_max = point.y;
    }

    x_range = x_max - x_min;
    y_range = y_max - y_min;
    /*------------------------------------------------------*/

    func_valid = true;
}