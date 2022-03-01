#! /bin/bash

scripts/build_game.sh
scripts/build_platform.sh

gcc src/app.c -Wuninitialized -Wall -Werror -ldl -Lbuild/game -Lbuild/platform -o build/app
