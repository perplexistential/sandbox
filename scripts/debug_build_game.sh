#! /bin/bash

if [ ! -d "build" ]
then
    mkdir build
fi

gcc -c -Wall -Werror -Wuninitialized -fpic src/game.c -o build/game.o -g

gcc -shared -fpic -o build/libgame.so build/game.o -g

