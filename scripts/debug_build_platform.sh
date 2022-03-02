#! /bin/bash

if [ ! -d "build" ]
then
    mkdir build
fi

gcc -c -fpic src/platform.c -o build/platform.o -Wall -Werror -Wuninitialized -g

gcc -shared -fpic -o build/libplatform.so build/platform.o -lGL -lglut -lGLEW -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_net -g
