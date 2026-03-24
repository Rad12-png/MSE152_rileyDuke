// game.h
#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdint.h>

// ── Grid & window configuration ───────────────────────────────────────────────
#define CELL_SIZE      20
#define GRID_COLS      40
#define GRID_ROWS      30
#define HUD_HEIGHT     40
#define WINDOW_WIDTH   (GRID_COLS * CELL_SIZE)
#define WINDOW_HEIGHT  (GRID_ROWS * CELL_SIZE + HUD_HEIGHT)
#define MAX_OBSTACLES  50

// ── Game state bitwise flags ──────────────────────────────────────────────────
// Pack multiple boolean states into a single uint8_t using bitwise operations.
// Example — set paused:    state.flags |=  FLAG_PAUSED;
// Example — clear paused:  state.flags &= ~FLAG_PAUSED;
// Example — check paused:  state.flags &   FLAG_PAUSED
#define FLAG_STARTED    (1 << 0)   // game has begun
#define FLAG_PAUSED     (1 << 1)   // game is currently paused
#define FLAG_GAME_OVER  (1 << 2)   // player has lost all lives
#define FLAG_WALL_WRAP  (1 << 3)   // walls wrap instead of kill (enhancement)

// ── Segment — one node in the snake's singly linked list ─────────────────────
typedef struct Segment {
    int x, y;              // position in grid coordinates
    struct Segment *next;  // pointer to the next (older) segment
} Segment;

// ── Snake ─────────────────────────────────────────────────────────────────────
typedef struct {
    Segment *head;    // front of the linked list (newest segment)
    Segment *tail;    // back of the linked list  (oldest segment)
    int length;       // total number of segments

    int dx, dy;       // current movement direction in grid units
    int next_dx;      // queued direction change (applied next update)
    int next_dy;
} Snake;

// ── Food ──────────────────────────────────────────────────────────────────────
typedef struct {
    int x, y;
    int is_active;
} Food;

// ── GameState ─────────────────────────────────────────────────────────────────
typedef struct {
    uint8_t flags;    // bitwise game state (see FLAG_* above)
    int score;
    int lives;
    int speed_ms;     // milliseconds between physics updates (lower = faster)
} GameState;

// ── Obstacles ─────────────────────────────────────────────────────────────────
// made by riley
typedef struct {
    int x;
    int y;
    int is_active;
}Obstacle;

// ── Input queue — circular buffer (Enhancement) ───────────────────────────────
// Stores a sequence of direction inputs so fast keypresses are never dropped.
#define INPUT_QUEUE_SIZE 4
typedef struct {
    int dx[INPUT_QUEUE_SIZE];
    int dy[INPUT_QUEUE_SIZE];
    int head;
    int tail;
    int count;
} InputQueue;

// ── Globals ───────────────────────────────────────────────────────────────────
extern Snake      snake;
extern Food       food;
extern GameState  state;
extern InputQueue input_queue;
extern Obstacle obstacles[MAX_OBSTACLES];
extern int num_obstacles;
// ── Function declarations ─────────────────────────────────────────────────────
void      start_game(SDL_Renderer *renderer);
void      init_game(void);
void      reset_snake(void);
void      spawn_food(void);

// Linked list operations — implement these in game.c
Segment  *create_segment(int x, int y);
void      snake_add_head(int x, int y);
void      snake_remove_tail(void);
void      snake_free(void);

// Input queue operations — implement these in game.c (Enhancement)
void      iq_push(InputQueue *q, int dx, int dy);
int       iq_pop(InputQueue *q, int *dx, int *dy);

#endif
