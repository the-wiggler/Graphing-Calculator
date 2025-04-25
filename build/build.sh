#!/bin/bash
cmake -G "MinGW Makefiles" ..
printf "COMPILING CMAKE DONE...BUILDING FILE\n\n"
mingw32-make
printf "RUNNING FILE:\n##################################################################\n\n"
./bin/calc.exe