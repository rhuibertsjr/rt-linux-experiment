#!/bin/bash

mkdir -p build

cd build

gcc -g -Wall ../src/main.c -o main

cd ..
