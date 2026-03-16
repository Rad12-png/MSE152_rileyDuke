// game.c
#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "input.h"
#include "physics.h"
#include "render.h"

Snake      snake;
Food       food;
GameState  state;
InputQueue input_queue;

// ─────────────────────────────────────────────────────────────────────────────
// Linked list operations
// Implement all four functions below. The rest of the game depends on them.
// ─────────────────────────────────────────────────────────────────────────────

/*
 * create_segment — allocate a new Segment on the heap at grid position (x, y).
 *
 * Requirements:
 *   - Use malloc() to allocate memory for a Segment.
 *   - Set the x, y fields.
 *   - Set next to NULL.
 *   - Return the pointer. If malloc fails, print an error and return NULL.
 */
Segment *create_segment(int x, int y) {
    // Allocate memory on the heap for one segment node
    // sizeof(Segment) ensures enough memory for the entire struct
    Segment *seg = malloc(sizeof(Segment));

    // Always check if malloc failed (return null if failed)
    if (seg == NULL) {
        return NULL;
    }

    // Initialize the position of this segment
    // Represent grid coordinates of the snake body piece
    seg->x = x;
    seg->y = y;

    // Newly created node does not point to another segment yet
    // Will be later updated when nodes are linked together
    seg->next = NULL;

    // Return pointer to new segment
    // Pointer will be used as part of the snake linked list
    return seg;
}

/*
 * snake_add_head — prepend a new segment at (x, y) to the snake's linked list.
 *
 * Requirements:
 *   - Call create_segment() to make the new node.
 *   - Point the new segment's next to the current snake.head.
 *   - Update snake.head to the new segment.
 *   - If the list was empty (snake.head was NULL), also update snake.tail.
 *   - Increment snake.length.
 */
void snake_add_head(int x, int y) {
    // Create a new segment that will become the new head
    Segment *new_head = create_segment(x,y);

    // If memory allocation failed, stop
    if (new_head == NULL) {
        return;
    }

    // Link the new node to the current head
    new_head->next = snake.head;

    // Update head to point to the new first node
    snake.head = new_head;

    // If the snake was empty before, tail must also point to this node
    if (snake.tail == NULL) {
        snake.tail = new_head;
    }

    // One segment was added, so increase length
    snake.length++;
}

/*
 * snake_remove_tail — remove and free the last segment in the linked list.
 *
 * Requirements:
 *   - If the list is empty, do nothing.
 *   - If there is only one segment, free it and set head and tail to NULL.
 *   - Otherwise, walk the list to find the second-to-last node.
 *     Set its next to NULL, free the old tail, update snake.tail.
 *   - Decrement snake.length.
 */
void snake_remove_tail(void) {
    // If the snake is empty, do nothing
    if (snake.head == NULL) {
        return;
    }

    // If the snake only has one segment
    if (snake.head ==  snake.tail) {
        free(snake.head);
        snake.head = NULL;
        snake.tail = NULL;
        snake.length = 0;
        return;
    }

    // Traverse the list to find the segment just before the tail
    Segment *current = snake.head;

    while (current->next != snake.tail) {
        current = current->next;
    }

    // Free the current tail
    free(snake.tail);

    // Update the new tail
    snake.tail = current;
    snake.tail->next = NULL;

    // Decrease snake length
    snake.length--;

}

/*
 * snake_free — free every segment in the linked list.
 *
 * Requirements:
 *   - Walk from snake.head to NULL, freeing each Segment.
 *   - Reset snake.head = NULL, snake.tail = NULL, snake.length = 0.
 *   - This must leave zero memory leaks (verify with Valgrind).
 */
void snake_free(void) {
    // Start at the head of the linked list
    Segment *current = snake.head;

    // Walk through the list one node at a time
    while (current != NULL) {
        // Save the nest node before freeing the current one
        Segment *next_node = current->next;

        // Free the current segment
        free(current);

        // Move to the next segment
        current = next_node;
    }

    // After freeing everything, reset the snake to an empty state
    snake.head = NULL;
    snake.tail = NULL;
    snake.length = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// Input queue — circular buffer (Enhancement)
// ─────────────────────────────────────────────────────────────────────────────

/*
 * iq_push — add a direction (dx, dy) to the back of the queue.
 *
 * Requirements:
 *   - If the queue is full (count == INPUT_QUEUE_SIZE), drop the input silently.
 *   - Otherwise store dx/dy at index q->tail, advance tail (wrap with modulo),
 *     and increment count.
 */
void iq_push(InputQueue *q, int dx, int dy) {
    // TODO: implement
    (void)q; (void)dx; (void)dy;
}

/*
 * iq_pop — remove and return the front direction from the queue.
 *
 * Requirements:
 *   - If the queue is empty (count == 0), return 0 without modifying dx or dy.
 *   - Otherwise copy q->dx[head] and q->dy[head] into *dx and *dy, advance head
 *     (wrap with modulo), decrement count, and return 1.
 */
int iq_pop(InputQueue *q, int *dx, int *dy) {
    // TODO: implement
    (void)q; (void)dx; (void)dy;
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// Initialisation
// ─────────────────────────────────────────────────────────────────────────────

/*
 * reset_snake — free the current snake and create a fresh 3-segment snake
 *               pointing right, centred in the grid.
 *
 * Hint: call snake_add_head() three times to build the initial body.
 *       Add the tail segments first so the head ends up at the front.
 */
void reset_snake(void) {
    snake_free();
    snake.dx = 1;  snake.dy = 0;
    snake.next_dx = 1; snake.next_dy = 0;

    // TODO: add 3 segments using snake_add_head()
    //       Start from the rearmost segment and work forward so the list
    //       ends with the head at (GRID_COLS/2, GRID_ROWS/2).
    //
    // Example order:
    //   snake_add_head(GRID_COLS/2 - 2, GRID_ROWS/2);  // tail
    //   snake_add_head(GRID_COLS/2 - 1, GRID_ROWS/2);  // middle
    //   snake_add_head(GRID_COLS/2,     GRID_ROWS/2);  // head
}

/*
 * spawn_food — place food at a random unoccupied grid cell.
 *
 * The current implementation picks a random cell without checking for the
 * snake. TODO: improve it so food never spawns inside the snake body.
 */
void spawn_food(void) {
    food.x = rand() % GRID_COLS;
    food.y = rand() % GRID_ROWS;
    food.is_active = 1;
    // TODO: walk the snake list; if the chosen cell is occupied, pick again.
}

void init_game(void) {
    srand((unsigned int)SDL_GetTicks());
    state.flags    = FLAG_STARTED;
    state.score    = 0;
    state.lives    = 3;
    state.speed_ms = 150;
    snake.head = NULL;
    snake.tail = NULL;
    snake.length = 0;
    input_queue.head = input_queue.tail = input_queue.count = 0;
    reset_snake();
    spawn_food();
}

// ─────────────────────────────────────────────────────────────────────────────
// Game loop
// ─────────────────────────────────────────────────────────────────────────────
void start_game(SDL_Renderer *renderer) {
    init_game();

    Uint32 last_update = SDL_GetTicks();

    while (!(state.flags & FLAG_GAME_OVER)) {
        handle_input(&state, &snake);

        Uint32 now = SDL_GetTicks();
        if (now - last_update >= (Uint32)state.speed_ms) {
            if (!(state.flags & FLAG_PAUSED)) {
                update_physics(&snake, &food, &state);
            }
            last_update = now;
        }

        render_game(renderer, &snake, &food, &state);
        SDL_Delay(10);
    }

    // TODO: display a game-over screen and wait for a restart or quit input.
    //       On restart: call init_game() and re-enter the loop.
    printf("Game Over! Final score: %d\n", state.score);
    snake_free();
    render_cleanup();
}
