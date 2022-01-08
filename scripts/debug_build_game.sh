#! /bin/bash

if [ ! -d "build" ]
then
    mkdir build
fi

gcc -g -c -Wall -Werror -fpic src/game.cpp -o build/gamedebug.o

gcc -shared -o build/libgamedebug.so build/gamedebug.o

