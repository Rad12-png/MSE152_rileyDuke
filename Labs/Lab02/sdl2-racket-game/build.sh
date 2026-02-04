#!/bin/bash

# Ensure the script exits on errors
set -e

# compile the program
gcc -o sdl2-racket-game main.c -lSDL2

echo "Build successful. Run ./sld2-racket-game to execute the program."
