#! /bin/bash

scripts/build_game.sh

gcc src/platform.cpp -Wall -Werror -Wuninitialized -lGL -lglut -lGLEW -ldl -lSDL2 -lSDL2main -lSDL2_image -Lbuild/game -o build/platform
