#! /bin/bash

scripts/build_game.sh
scripts/build_platform.sh

gcc src/app.c -o build/app -Wuninitialized -Wall -Werror -Lbuild/game -Lbuild/platform -ldl -lGL -lglut -lGLEW -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_net
