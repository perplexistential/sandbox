#! /bin/bash

scripts/debug_build_game.sh

gcc src/platform.cpp -g -lGL -lglut -ldl -lSDL2 -lSDL2main -lSDL2_image -Lbuild/gamedebug -o build/platform.debug
