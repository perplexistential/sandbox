# live reloading sandbox

## build

> scripts/build_platform.sh

## run

> build/platform

## demo

a box of size 50x50 will bounce around the screen within 300x500 extents.

modify anything in game.cpp you like, then run scripts/build_game.sh to produce a new shared library

## platform

the platform is for linux as it relies on `dlsym`, `dlopen`, and `dlclose` to reload the game lib.

memory is allocated here and provided to the game code on reload
