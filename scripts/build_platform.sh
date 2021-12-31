#! /bin/bash

scripts/build_game.sh

gcc src/platform.cpp -lGL -lglut -ldl -lSDL2 -lSDL2main -Lbuild/game -o build/platform
