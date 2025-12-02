#!/bin/sh
g++ main.cpp -O3 -std=c++20 -lraylib -lraygui -lGL -lm -lpthread -ldl -lrt -lX11 -o main.bin
