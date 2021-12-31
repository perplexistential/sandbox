#! /bin/bash

scripts/build_game

gcc src/platform.cpp -lGL -lglut -ldl -lSDL2 -lSDL2main -Lbuild/game -o build/platform
