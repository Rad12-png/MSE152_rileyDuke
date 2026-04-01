
// input.c
#include <SDL2/SDL.h>
#include "input.h"
#include "render.h"
void handle_input(GameState *state, Snake *snake) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            state->flags &= ~(FLAG_STARTED | FLAG_PAUSED | FLAG_MENU); //stop all proccesses
            state->flags |= FLAG_GAME_OVER; //run game over screen
            return;
        }

        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;

            // menu
            if (state->flags & FLAG_MENU) {
		//traversing thru menu
		if(key == SDLK_UP) { //if up is pressed go up
		    state->menu_index--;
		    if(state->menu_index < 0) //error handling
			state->menu_index = MAIN_MENU_SIZE - 1;
		}
	
  		if (key == SDLK_DOWN) { //if down is pressed go down
                     state->menu_index++;
                     if (state->menu_index >= MAIN_MENU_SIZE) //error handling
                          state->menu_index = 0;
                }

                if (key == SDLK_RETURN) { //if return key is pressed
		    if(state->menu_index == 0) {
		       state->flags &= ~(FLAG_MENU | FLAG_GAME_OVER); 
                       state->flags |= FLAG_STARTED;
                }else if (state->menu_index == 1) { // quit
		   snake_free();
    		   render_cleanup();
    		   SDL_Quit();
    		   exit(0);
            	}
	    }
	    }
            // game over screen
            if (state->flags & FLAG_GAME_OVER) {

                if (key == SDLK_RETURN) {
                    init_game();
                }

                if (key == SDLK_ESCAPE) {
                    state->flags &= ~FLAG_GAME_OVER;
                    state->flags |= FLAG_MENU;
                }

                return;
            }

            // paused
	    if (state->flags & FLAG_PAUSED) {
    		// Up / Down to move menu selection
    		if (key == SDLK_UP) {
        		state->menu_index--;
        		if (state->menu_index < 0)
            			state->menu_index = PAUSE_MENU_SIZE - 1;
    			}

    		if (key == SDLK_DOWN) {
        		state->menu_index++;
        		if (state->menu_index >= PAUSE_MENU_SIZE)
            			state->menu_index = 0;
    			}

    		// Enter to select
    		if (key == SDLK_RETURN) {
        		if (state->menu_index == 0) { // Resume
            			state->flags &= ~FLAG_PAUSED;
            			state->flags |= FLAG_STARTED;
        		}
        		else if (state->menu_index == 1) { // Restart
            			init_game();
            			state->flags = FLAG_STARTED;
        		}
        		else if (state->menu_index == 2) { // Main Menu
            			state->flags = FLAG_MENU;
            			state->menu_index = 0; // reset selection for menu
        		}
    		}

    		// ESC → back to menu
    		if (key == SDLK_ESCAPE) {
        		state->flags &= ~FLAG_PAUSED;
        		state->flags |= FLAG_MENU;
        		state->menu_index = 0;
    		}

    		return; // don’t let paused input fall through
	}

            // running
            if (state->flags & FLAG_STARTED) { //if game has started

                switch (key) {

                    // Direction keys (with no reverse)
                    case SDLK_UP:
                    case SDLK_w:
                        if (snake->dy != 1) {
                            snake->next_dx = 0;
                            snake->next_dy = -1;
                        }
                        break;

                    case SDLK_DOWN:
                    case SDLK_s:
                        if (snake->dy != -1) {
                            snake->next_dx = 0;
                            snake->next_dy = 1;
                        }
                        break;

                    case SDLK_LEFT:
                    case SDLK_a:
                        if (snake->dx != 1) {
                            snake->next_dx = -1;
                            snake->next_dy = 0;
                        }
                        break;

                    case SDLK_RIGHT:
                    case SDLK_d:
                        if (snake->dx != -1) {
                            snake->next_dx = 1;
                            snake->next_dy = 0;
                        }
                        break;

                    // Pause
                    case SDLK_p:
                        state->flags &= ~FLAG_STARTED; //stop game temporarily
                        state->flags |= FLAG_PAUSED; //start pause 
                        break;

                    // Escape → menu
                    case SDLK_ESCAPE:
                        state->flags &= ~FLAG_STARTED; //stop game temporarily
                        state->flags |= FLAG_MENU; // start menu screen
                        break;

                    default:
                        break;
                }
            }
        }
    }
}
