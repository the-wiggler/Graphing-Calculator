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
#include <SDL2/SDL.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIS FUNCTION CHANGES THE ff (user input) string into usable mathematical expression for executeFunctionCalculation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void uiParse::fInputParse() {
    std::stack<char> operators;
    std::queue <std::variant<double, char>> expression;

    std::istringstream fI(ff);
    std::string token;

    while (fI >> token) {
        // adds to expression if the token is a number
        if (std::isdigit(token.front()) || token.front() == '.') {
            expression.emplace(std::stod(token));
            continue;
        } 
        else if (token[0] == 'x') {
            expression.emplace('x');
            continue;
        }

        char op = token[0]; // if it isnt a number, we interpret it as some sort of operator

        // operators
        if (op == '+' || op == '-' || op == '*' || op == '/') {
            while (!operators.empty()) {
                // operator precedence
                int prec1;
                if (op == '+' || op == '-') { prec1 = 1; }
                else { prec1 = 2; }

                int prec2;
                if (operators.top() == '+' || operators.top() == '-') { prec2 = 1; }
                else { prec2 = 2; }

                if (prec2 >= prec1) {
                    expression.emplace(operators.top());
                    operators.pop();
                }
                else { break; }
            }
            operators.push(op);
            continue;
        }
    }
    // if any operators remain in the stack, they are pushed into the expression queue
    while (!operators.empty()) {
        expression.emplace(operators.top());
        operators.pop();
    }


    // evaluates the newly created mathematical structure by inputting a value of xi
    std::stack<double> eval;
    
    while (!expression.empty()) {
        auto tok = expression.front();
        expression.pop();

        if (std::holds_alternative<double>(tok)) {
            // its a number
            double n = std::get<double>(tok);
            eval.push(n);
            continue;
        }
        else if (std::holds_alternative<char>(tok)) {
            char c = std::get<char>(tok);
            // if the character is x, replace x with xi to simulate it being number
            if (c == 'x') {
                eval.push(xi);
                continue;
            }
            // if the character is an operator, perform operation
            else if (c == '+' || c == '-' || c == '*' || c == '/') {
                double right = eval.top(); eval.pop();
                double left = eval.top(); eval.pop();
                double result;

                switch (c) {
                case '+': result = left + right; break;
                case '-': result = left - right; break;
                case '*': result = left * right; break;
                case '/': result = left / right; break;
                }

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

    uiParse par;

    x = DOMAIN_MIN;
    for (int i = 0; i <= FUNC_RES; i++, x += INCREMENT) {
        par.xi = x;
        par.fInputParse();
        y = par.f_val;

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