// physics.c
#include "physics.h"
#include <stdio.h>

void update_physics(Snake *snake, Food *food, GameState *state) {
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

    // ── Self-collision ────────────────────────────────────────────────────────
    // TODO: walk the linked list from snake->head to NULL.
    //       If any segment's (x, y) matches (new_x, new_y), the snake has
    //       collided with itself — handle the same way as a wall collision.

    // ── Move the snake ────────────────────────────────────────────────────────
    // Add the new head position, then remove the tail (unless eating food).
    snake_add_head(new_x, new_y);

    if (food->is_active && new_x == food->x && new_y == food->y) {
        // Snake eats food — grow by NOT removing the tail.
        food->is_active = 0;

        // TODO: increment state->score.
        // TODO: increase difficulty — decrease state->speed_ms (minimum ~60).
        // TODO: call spawn_food() to place new food.

        printf("Score: %d\n", state->score);  // temporary — replace with HUD
    } else {
        // Normal move — remove the tail to keep length constant.
        snake_remove_tail();
    }
}
