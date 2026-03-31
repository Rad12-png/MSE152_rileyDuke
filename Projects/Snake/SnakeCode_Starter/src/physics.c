// physics.c
#include "physics.h"
#include <stdio.h>

void update_physics(Snake *snake, Food *food, GameState *state) {
    //stop all movement unless game is actively runni
    if(!(state->flags & FLAG_STARTED)) {
	return;
    }

    // Guard: linked list not yet implemented — nothing to move
    if (snake->head == NULL) return;

    // Apply the queued direction
    snake->dx = snake->next_dx;
    snake->dy = snake->next_dy;

    // Compute the new head position
    int new_x = snake->head->x + snake->dx;
    int new_y = snake->head->y + snake->dy;

    // ── Wall collision ────────────────────────────────────────────────────────
    // TODO: if FLAG_WALL_WRAP is set in state->flags, wrap new_x and new_y
    //       around the grid (use the modulo operator).
    //       Otherwise, if new_x or new_y is out of bounds, handle losing a life:
    //         - Decrement state->lives.
    //         - If lives reach 0, set FLAG_GAME_OVER.
    //         - Otherwise reset the snake (call reset_snake()).
    
    //checks if the flag wall wrap bit is turned on and if the player has it enabled
    if (state->flags & FLAG_WALL_WRAP) { 
        if (new_x < 0) new_x = GRID_COLS - 1; //went off left edge so appear on the right side
        if (new_x >= GRID_COLS) new_x = 0; //went off right edge so appear on the left side
        if (new_y < 0) new_y = GRID_ROWS - 1; //went off top edge so appear on the bottom side
        if (new_y >= GRID_ROWS) new_y = 0; //went off bottom edge so appear on the top side	
    } else {
	//if wall wrap is off check if the new position is outside grid
        if (new_x < 0 || new_x >= GRID_COLS || new_y < 0 || new_y >= GRID_ROWS) {
            state->lives--; //remove a life
            if (state->lives <= 0) { // if lost all live
		state->flags &= ~FLAG_STARTED; //stop game
                state->flags |= FLAG_GAME_OVER; //show game over screen
            } else { //if lives remain
                reset_snake(); //reset snake to starting position
            }
            return;
        }
    }
    // ── Self-collision ────────────────────────────────────────────────────────
    // TODO: walk the linked list from snake->head to NULL.
    //       If any segment's (x, y) matches (new_x, new_y), the snake has
    //       collided with itself — handle the same way as a wall collision.
    
    //pointer at the second segment of the snake
    Segment *current = snake->head->next;
    while (current != NULL) { //keep looping until it reaches the end of the snake
        if (current->x == new_x && current->y == new_y) { //check if the head positino matches the segment position
            state->lives--; //lose a life
            if (state->lives <= 0) { //if no lives left
		state->flags &= ~FLAG_STARTED; //stop game
                state->flags |= FLAG_GAME_OVER; //set game over flag
            } else { //if lives remain
                reset_snake(); //reset the snake back to the starting position
            }
            return;
        }
        current = current->next; //move to next segment in the linked list 
    } 

    // ── Obstacle collision ────────────────────────────────────────────────────
	for(int i = 0; i < num_obstacles; i++){
		// if the node is registered as an obstacle, and the new x & y values are the same
		if(obstacles[i].is_active && new_x == obstacles[i].x && new_y == obstacles[i].y){
			state->lives--;
			if(state->lives <= 0){
				state->flags &= ~FLAG_STARTED;
				state->flags |= FLAG_GAME_OVER;
			}else{
				reset_snake();
			}
			return;
		}
	}

    // ── Move the snake ────────────────────────────────────────────────────────
    // Add the new head position, then remove the tail (unless eating food).
    snake_add_head(new_x, new_y);

    if (food->is_active && new_x == food->x && new_y == food->y) {
        // Snake eats food — grow by NOT removing the tail.
        food->is_active = 0;

        // increment state->score.
	state->score++;

        // increase difficulty — decrease state->speed_ms (minimum ~60).
	if (state->speed_ms > 60) {
	    state->speed_ms -= 5;
        }

        // call spawn_food() to place new food.
	spawn_food();

        printf("Score: %d\n", state->score);  // temporary — replace with HUD
    } else {
        // Normal move — remove the tail to keep length constant.
        snake_remove_tail();
    }
}
