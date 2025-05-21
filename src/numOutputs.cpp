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

    std::istringstream fI(ff);
    std::string token;

    bool expectOperand = true; // true when expecting a number next in the string

    token.clear();

    for (size_t i = 0; i < ff.length(); ++i) {
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
                std::cerr << "ERROR: UNKNOWN TOKEN INPUT\n";
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
            else if (c == "sin" || c == "cos" || c == "tan" || c == "log" || c == "ln" || c == "sqrt") {
                double v = eval.top(); eval.pop();

                if      (c == "sin") result =   std::sin(v);
                else if (c == "cos") result =   std::cos(v);
                else if (c == "tan") result =   std::tan(v);
                else if (c == "log") result =   std::log10(v);
                else if (c == "ln") result =    std::log(v);
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

    x = DOMAIN_MIN;
    for (int i = 0; i <= FUNC_RES; i++, x += INCREMENT) {
        xi = x;
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