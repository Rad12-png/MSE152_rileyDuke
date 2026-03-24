// input.c
#include <SDL2/SDL.h>
#include "input.h"

void handle_input(GameState *state, Snake *snake) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            state->flags |= FLAG_GAME_OVER;
            return;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {

                // Direction keys — queue the next move.
                // TODO: prevent a 180° reversal (e.g. can't go LEFT if currently moving RIGHT).
                case SDLK_UP:
                case SDLK_w:
		  if (snake->dy != 1) {
                    snake->next_dx =  0; snake->next_dy = -1;
		  }
                    break;
                case SDLK_DOWN:
                case SDLK_s:
		  if (snake->dy != -1){
                    snake->next_dx =  0; snake->next_dy =  1;
		  }
 		    break;
                case SDLK_LEFT:
                case SDLK_a:
		  if(snake->dx != 1){
                    snake->next_dx = -1; snake->next_dy =  0;
		  }
		    break;
                case SDLK_RIGHT:
                case SDLK_d:
		  if(snake->dx != -1){
                    snake->next_dx =  1; snake->next_dy =  0;
		  }
  		    break;

                // Toggle pause with P
                case SDLK_p:
                    state->flags ^= FLAG_PAUSED;
                    break;

                // Quit with Escape
                case SDLK_ESCAPE:
                    state->flags |= FLAG_GAME_OVER;
                    break;

                default:
                    break;
            }
        }
    }
}
