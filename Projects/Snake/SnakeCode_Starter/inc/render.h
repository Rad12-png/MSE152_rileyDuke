// render.h
#ifndef RENDER_H
#define RENDER_H

#include "game.h"

void render_game(SDL_Renderer *renderer, const Snake *snake,
                 const Food *food, const GameState *state, const Obstacle *obstacles);

void render_cleanup(void);

#endif
