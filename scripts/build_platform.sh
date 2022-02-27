#! /bin/bash

scripts/build_game.sh

gcc src/platform.c -Wall -Werror -Wuninitialized -lGL -lglut -lGLEW -ldl -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_net -Lbuild/game -o build/platform
