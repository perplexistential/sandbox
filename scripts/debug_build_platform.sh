#! /bin/bash

scripts/debug_build_game.sh

gcc src/platform.c -g -Wuninitialized -Wall -Werror -lGL -lglut -lGLEW -ldl -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_net -Lbuild/game -o build/platform
