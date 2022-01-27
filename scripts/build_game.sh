#! /bin/bash

if [ ! -d "build" ]
then
    mkdir build
fi

gcc -c -Wall -Werror -Wuninitialized -fpic src/game.cpp -o build/game.o

gcc -shared -o build/libgame.so build/game.o

