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
int numOutputs::determinePrecedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 0;
}

void numOutputs::fInputParse() {

    while (!operators.empty()) operators.pop();

    std::istringstream fI(ff);
    std::string token;

    bool expectOperand = true;

    token.clear();

    for (size_t i = 0; i < ff.length(); ++i) {
        char c = ff[i];

        // skip spaces if user input them for some reason
        if (std::isspace(c)) continue;

        // if it's a digit or decimal, parse a full number
        if (std::isdigit(c) || c == '.') {
            token.clear();
            while (i < ff.length() && (std::isdigit(ff[i]) || ff[i] == '.')) {
                token += ff[i++];
            }
            --i; // step back one char
            expression.emplace(std::stod(token));
            expectOperand = false;
            
        }

        // if it is a variable (currently only works for variable x)
        else if (std::isalpha(c)) {
            while (i < ff.length() && std::isalpha(ff[i])) {
                token += ff[i++];
            }
            --i;
            token = c;
            expression.emplace(token);
            expectOperand = false;
        }

        // if it is an operator or parenthesis
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '^') {
            token = c;
            if (c == '-' && expectOperand == true) {
                expression.emplace(0.0);
                operators.push(token);
            }
            else if (token == "(") {
                operators.push(token);
                expectOperand = true;
            }
            else if (token == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    expression.emplace(operators.top());
                    operators.pop();
                }
                if (!operators.empty() && operators.top() == "(") operators.pop();
                expectOperand = false;
            }
            else {
                while (!operators.empty() && operators.top() != "(") {
                    int op1 = determinePrecedence(token);
                    int op2 = determinePrecedence(operators.top());

                    if (op2 >= op1) {
                        expression.emplace(operators.top());
                        operators.pop();
                    }
                    else {
                        break;
                    }
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
        else if (std::holds_alternative<std::string>(tok)) {
            std::string c = std::get<std::string>(tok);

            // if the character is x, replace x with xi to simulate it being number
            if (c == "x") {
                eval.push(xi);
                continue;
            }

            // if the character is an operator, perform operation
            else if (c == "+" || c == "-" || c == "*" || c == "/") {
                double right = eval.top(); eval.pop();
                double left = eval.top(); eval.pop();
                double result;

                switch (c[0]) {
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

    x = DOMAIN_MIN;
    for (int i = 0; i <= FUNC_RES; i++, x += INCREMENT) {
        xi = x;
        fInputParse(); // its pretty inefficient to reparse this every time this should be fixed
        executeParseCalc(); 
        y = f_val;

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
