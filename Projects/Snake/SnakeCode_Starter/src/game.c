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
// constants for obstacles - rad12
Obstacle obstacles[MAX_OBSTACLES];
int num_obstacles = 10;

// Stores the highest score across game sessions
int high_score = 0;

// Loads the high score from a file (if it exists)
void load_high_score() {
    // Try to open file in read mode
    FILE *file = fopen("highscore.txt", "r");

    if (file != NULL) {
        // Read integer from file into high_score
        fscanf(file, "%d", &high_score);

        // Close file after reading
        fclose(file);
    }
}

// Saves the current high score to a file
void save_high_score() {
    // Open file in write mode
    FILE *file = fopen("highscore.txt", "w");

    if (file != NULL) {
        // Write the high score value into the file
        fprintf(file, "%d", high_score);

        // Close the file after writing
        fclose(file);
    }
} 

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
    if(snake.head == NULL) return;

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
    state.speed_ms = 150;
    // TODO: add 3 segments using snake_add_head()
    //       Start from the rearmost segment and work forward so the list
    //       ends with the head at (GRID_COLS/2, GRID_ROWS/2).
    //
    // Example order:
         snake_add_head(GRID_COLS/2 - 2, GRID_ROWS/2);  // tail (puts segment two tiles from center)
         snake_add_head(GRID_COLS/2 - 1, GRID_ROWS/2);  // middle (puts segment one tile from center)
         snake_add_head(GRID_COLS/2,     GRID_ROWS/2);  // head (puts segment in center)
}

// Check if a coordinate is adjacent to any existing active obstacle
// this allows us to keep the obstacles from cluttering and potentially blocking off part of the map

int is_near_obstacle(int x, int y, int current_count){
	for(int i = 0; i < current_count; i++){
		if(!obstacles[i].is_active) continue;

		// check the 3x3 area around obstacle
		if (abs(obstacles[i].x - x) <= 1 && abs(obstacles[i].y - y) <= 1){
			return 1;
		}
	}
	return 0;
}

// spawn_obstacles - pick random coordinates, checks to ensure the wall doesn't spawn on the snake or other obstacles

void spawn_obstacles(void){

	// make sure max obstacles is respected
	if(num_obstacles > MAX_OBSTACLES){
		num_obstacles = MAX_OBSTACLES;
	}

	// reset all the obstacles
	for(int i = 0; i < MAX_OBSTACLES; i++) obstacles[i].is_active = 0;

	for(int i = 0; i < num_obstacles; i++){
		int rx = 0, ry = 0;
		int valid = 0;
		int attempts = 0; // variable to keep track of loops to prevent infinite loops

		while(!valid && attempts  < 100){
			attempts++;
			rx = rand() % GRID_COLS;
			ry = rand() % GRID_ROWS;
			valid = 1;

			// don't spawn on the initial snake (middle row)
			if (ry == GRID_ROWS / 2 && rx >= (GRID_COLS / 2 - 5) && rx <= (GRID_COLS / 2 + 5)){
				valid = 0;
				continue;
			}

			// don't spawn on the food
			if(food.is_active && rx == food.x && ry == food.y){
				valid = 0;
				continue;
			}

			// dont spawn on or next to another obstacle
			if(is_near_obstacle(rx, ry, i)){
				valid = 0;
				continue;
			}
		}

		if(valid){
		obstacles[i].x = rx;
		obstacles[i].y = ry;
		obstacles[i].is_active = 1;
		}
	}

}

/*
 * spawn_food — place food at a random unoccupied grid cell.
 *
 * The current implementation picks a random cell without checking for the
 * snake. TODO: improve it so food never spawns inside the snake body.
 */

void spawn_food(void) {
	int valid = 0;
	while(!valid){ 
   		food.x = rand() % GRID_COLS;
    		food.y = rand() % GRID_ROWS;
   		valid = 1;

		// check collision with snake
		for(Segment *s = snake.head; s; s = s->next){
			if(s->x == food.x && s->y == food.y){
				valid = 0;
				break;
			}
		}
		if (!valid) continue;

		// check collision with obstacles
		for (int i = 0; i < num_obstacles; i++){
			if(obstacles[i].is_active && obstacles[i].x == food.x && obstacles[i].y == food.y){
				valid = 0;
				break;
			}
		}
	}
	food.is_active = 1;
    // TODO: walk the snake list; if the chosen cell is occupied, pick again.
}

void init_level(void){

	// increase obstacles by 5 per level, but cap at MAX_OBSTACLES
	num_obstacles = 10 + (state.level -1) * 5;
	if(num_obstacles > MAX_OBSTACLES) {
		num_obstacles = MAX_OBSTACLES;
	}

	// increase target score by 2 per level
	state.target_score = 10 + (state.level - 1) * 2;
	state.score = 0;

	reset_snake();
	spawn_obstacles();
	spawn_food();
}


void init_game(void) {
    snake_free();
    state.menu_index = 0; //initialize  menu index
    srand((unsigned int)SDL_GetTicks());
    state.flags    = FLAG_MENU;
    state.score    = 0;
    state.total_score = 0;
    state.lives    = 1;
    state.level    = 1; 
    state.speed_ms = 150;
    snake.head = NULL;
    snake.tail = NULL;
    snake.length = 0;
    input_queue.head = input_queue.tail = input_queue.count = 0;
    init_level(); 
}

// ─────────────────────────────────────────────────────────────────────────────
// Game loop
// ─────────────────────────────────────────────────────────────────────────────
void start_game(SDL_Renderer *renderer) {
    init_game();

    // Load save high score at the start of a game
    load_high_score();

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

        render_game(renderer, &snake, &food, &state, obstacles);
        SDL_Delay(10);
    }

    // Check if current score beats high score
    if (state.total_score > high_score) {
        high_score = state.total_score;

        // Save new high score to file
        save_high_score();
    }

    // TODO: display a game-over screen and wait for a restart or quit input.
    //       On restart: call init_game() and re-enter the loop.
    // printf("Game Over! Final score: %d\n", state.score);

    while (1) { //while true
        SDL_Event e; //creates variable to hold input 
        while (SDL_PollEvent(&e)) { //catches if player inputs a key
            if (e.type == SDL_QUIT) { //checks if player closed window
                snake_free(); //free memory
                render_cleanup(); //clean render before quitting
                return;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) { //if player presses R
                start_game(renderer); //restart game 
                return;
            }
            
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) { //if player presses esc
		snake_free(); //free snake memory
		render_cleanup(); //clean up all render operations
		SDL_Quit(); //quit game
		return;
		}
        }
        render_game(renderer, &snake, &food, &state, obstacles); //keep game over screen until input is sent
        SDL_Delay(10); //small delay
    }
    snake_free();
    render_cleanup();
}
