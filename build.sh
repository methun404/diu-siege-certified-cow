#!/bin/bash

clang++ -std=c++11 -Wno-deprecated-declarations -I./dependencies/include -L./dependencies/library -Wl,-rpath,./dependencies/library -lglfw.3 -framework OpenGL -framework GLUT *.cpp -o app
if [ $? -eq 0 ]; then
    echo "Build successful! Run ./app to start."
else
    echo "Build failed."
fi
