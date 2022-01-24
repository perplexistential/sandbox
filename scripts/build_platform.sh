#! /bin/bash

scripts/build_game.sh

gcc src/platform.cpp -lGL -lglut -lGLEW -ldl -lSDL2 -lSDL2main -lSDL2_image -Lbuild/game -o build/platform
