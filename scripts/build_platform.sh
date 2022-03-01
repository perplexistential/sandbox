#! /bin/bash

if [ ! -d "build" ]
then
    mkdir build
fi

gcc -c -Wall -Werror -Wuninitialized -lGL -lglut -lGLEW -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_net -fpic src/platform.c -o build/platform.o

gcc -shared -o build/libplatform.so build/platform.o

