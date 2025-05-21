#!/bin/bash

if [ ! -d "build" ]; then
    mkdir "build"
    echo "Build directory created"
fi

if [ ! -d "build/bin" ]; then
    mkdir "build/bin"
fi

if [ ! -f "build/bin/font.ttf" ]; then
    cp "src/assets/font.ttf" "build/bin/font.ttf"
    echo "Font file copied to build/bin/"
fi

cd build

cmake .. -G "MinGW Makefiles"

mingw32-make

cd bin
./calc.exe
cd ..
cd ..
