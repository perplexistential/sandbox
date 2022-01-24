#! /bin/bash

if [ ! -d "build" ]
then
    mkdir build
fi

gcc -g -c -Wall -Werror -fpic src/game.cpp -o build/game.o

gcc -g -shared -o build/libgame.so build/game.o

