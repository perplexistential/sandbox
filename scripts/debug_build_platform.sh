#! /bin/bash

scripts/debug_build_game.sh

gcc src/platform.cpp -g -Wuninitialized -Wall -Werror -lGL -lglut -lGLEW -ldl -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -Lbuild/game -o build/platform
