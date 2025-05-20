This is a work in progress project to make a simple and fast graphing calculator in C++.
The goal is to have it somewhat comparable to Desmos, but locally computed to handle more complex
calculations and have other more computationally expensive tasks available.

The calculator functions with a typed input interface in the top corner. Here you can enter a number of operations:
* 'xmax || xmin || ymax || ymin' <number>  : sets the windows boundaries on which the funciton may render
* '=' <function expression> : sets the function to render on the graph (default is y=x)

Currently supported operators:
* basic arithmetic (+-*/)
* powers (^)
* basic trig functions (sin,cos,tan)
* square root (sqrt)
* log (base 10) and ln

Dependencies to compile:
* SDL2
* SDL2_ttf
