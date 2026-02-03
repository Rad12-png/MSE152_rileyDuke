#include <SDL2/SDL.h>
#include <stdio.h>

// Define constants for window dimensions and object sizes
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define BALL_SIZE 20
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20

// Global variables for SDL window and renderer
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Global variables for ball position and velocity
float ball_x = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
float ball_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
float ball_dx = 2.0f;  // Ball Velocity in the x-direction
float ball_dy = 2.0f;  // Ball Velocity in the y-direction

// Global variables for paddle position
float paddle_x = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2;
float paddle_y = WINDOW_HEIGHT - PADDLE_HEIGHT - 10;

// Function to initialize SDL and create a window and renderer
int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Racket Game",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    return 1;
}

// Function to handle ball movement and collision
void moveBall() {

    // 1) Move the ball (Position += Velocity)
    ball_x += ball_dx;
    ball_y += ball_dy;

    // 2) Check for collision with walls (Left/Right)
    if (ball_x <= 0 || ball_x + BALL_SIZE >= WINDOW_WIDTH) {
        ball_dx = -ball_dx;
    }

    // 3) Check for collision with ceiling (Top)
    if (ball_y <= 0) {
        ball_dy = -ball_dy;
    }

    // 4) Check for collision with paddle region (Bottom)
    // Want to detect if ball overlaps paddle rectangle
    // Ball rectangle: [ball_x, ball_y, BALL_SIZE, BALL_SIZE]
    // Paddle rectangle: [paddle_x, paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT]
    if (ball_y + BALL_SIZE >= paddle_y &&
        ball_y <= paddle_y + PADDLE_HEIGHT &&
        ball_x + BALL_SIZE >= paddle_x &&
        ball_x <= paddle_x + PADDLE_WIDTH) {

        // If the ball hit the paddle (Coming down onto it), bounce
        if (ball_dy > 0) {
            // Reverse y direction of the ball
	    ball_dy = -ball_dy;

	    // Prevent ball from sticking inside paddle
	    ball_y = paddle_y - BALL_SIZE;

        } else {

            // If somehow it overlaps while moving upward, ignore or still bounce
            ball_dy = -ball_dy;
        }

    } else {

        // If ball goes below screen print the message "GAME OVER" in the terminal
        if (ball_y > WINDOW_HEIGHT) {
            printf("Game Over\n");
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(0);
        }
    }
}

// Function to render the ball
void renderBall() {
    SDL_Rect ball_rect = { (int)ball_x, (int)ball_y, BALL_SIZE, BALL_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &ball_rect);
}

// Function to render the paddle
void renderPaddle() {
    SDL_Rect paddle_rect = { (int)paddle_x, (int)paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);
}

int main(int argc, char* argv[]) {
    // Initialize SDL and create window and renderer
    if (!initSDL()) {
        return 1;
    }

    const int FPS = 60;                 // Target FPS
    const int frameDelay = 1000 / FPS;  // Delay per frame in milliseconds

    Uint32 frameStart;  // Start time of the frame
    Uint32 frameTime;   // Time taken by the frame

    SDL_Event e;
    int quit = 0;

    while (!quit) {
        frameStart = SDL_GetTicks();  // Record the start time of the frame

        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_KEYDOWN) {
                // Move paddle within screen boundaries
                if (e.key.keysym.sym == SDLK_LEFT && paddle_x > 0) {
                    paddle_x -= 20;
                } else if (e.key.keysym.sym == SDLK_RIGHT && paddle_x + PADDLE_WIDTH < WINDOW_WIDTH) {
                    paddle_x += 20;
                }
            }
        }

        // Move the ball
        moveBall();

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the ball and paddle
        renderBall();
        renderPaddle();

        // Update the screen
        SDL_RenderPresent(renderer);

        // Calculate frame time and delay to maintain FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
