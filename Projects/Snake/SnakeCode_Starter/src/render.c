// render.c  — PROVIDED: do not modify for the base game.
// All drawing is implemented here so you can focus on the C data structures.
// Your work belongs in game.c (linked list) and physics.c (movement/collision).
// Enhancements that require new visuals (e.g. Obstacle Walls) may extend this file.

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "render.h"

static TTF_Font *font_lg = NULL;   // titles
static TTF_Font *font_sm = NULL;   // HUD / hints

// ── Font loading ──────────────────────────────────────────────────────────────

static const char *FONT_PATHS[] = {
    // Linux
    "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
    "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf",
    "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf",
    // macOS
    "/System/Library/Fonts/Supplemental/Arial Bold.ttf",
    "/System/Library/Fonts/Supplemental/Arial.ttf",
    "/System/Library/Fonts/Helvetica.ttc",
    "/Library/Fonts/Arial Bold.ttf",
    "/Library/Fonts/Arial.ttf",
    NULL
};

static TTF_Font *try_load_font(int size) {
    for (int i = 0; FONT_PATHS[i]; i++) {
        TTF_Font *f = TTF_OpenFont(FONT_PATHS[i], size);
        if (f) return f;
    }
    return NULL;
}

static void ensure_fonts(void) {
    if (font_lg) return;
    if (TTF_Init() < 0) { fprintf(stderr, "TTF_Init: %s\n", TTF_GetError()); return; }
    font_lg = try_load_font(30);
    font_sm = try_load_font(17);
}

void render_cleanup(void) {
    if (font_lg) { TTF_CloseFont(font_lg); font_lg = NULL; }
    if (font_sm) { TTF_CloseFont(font_sm); font_sm = NULL; }
    TTF_Quit();
}

// ── Text helpers ──────────────────────────────────────────────────────────────

static void draw_text(SDL_Renderer *r, TTF_Font *font,
                      const char *text, int x, int y, SDL_Color col) {
    if (!font || !text) return;
    SDL_Surface *surf = TTF_RenderText_Solid(font, text, col);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_Rect dst = {x, y, surf->w, surf->h};
    SDL_FreeSurface(surf);
    if (!tex) return;
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

static void draw_text_centered(SDL_Renderer *r, TTF_Font *font,
                                const char *text, int cy, SDL_Color col) {
    if (!font || !text) return;
    int w, h;
    TTF_SizeText(font, text, &w, &h);
    draw_text(r, font, text, (WINDOW_WIDTH - w) / 2, cy - h / 2, col);
}

// ── Grid ──────────────────────────────────────────────────────────────────────

static void render_grid(SDL_Renderer *r) {
    SDL_SetRenderDrawColor(r, 28, 28, 28, 255);
    for (int c = 0; c <= GRID_COLS; c++)
        SDL_RenderDrawLine(r, c * CELL_SIZE, HUD_HEIGHT,
                              c * CELL_SIZE, HUD_HEIGHT + GRID_ROWS * CELL_SIZE);
    for (int row = 0; row <= GRID_ROWS; row++)
        SDL_RenderDrawLine(r, 0,            HUD_HEIGHT + row * CELL_SIZE,
                              WINDOW_WIDTH, HUD_HEIGHT + row * CELL_SIZE);
}

// ── HUD ───────────────────────────────────────────────────────────────────────

static void render_hud(SDL_Renderer *r, const GameState *state) {
    SDL_SetRenderDrawColor(r, 18, 18, 18, 255);
    SDL_Rect bar = {0, 0, WINDOW_WIDTH, HUD_HEIGHT};
    SDL_RenderFillRect(r, &bar);

    SDL_SetRenderDrawColor(r, 45, 45, 45, 255);
    SDL_RenderDrawLine(r, 0, HUD_HEIGHT - 1, WINDOW_WIDTH, HUD_HEIGHT - 1);

    char buf[48];
    SDL_Color white = {230, 230, 230, 255};
    SDL_Color red   = {210,  70,  70, 255};

    if (font_sm) {
        int th = TTF_FontHeight(font_sm);
        int ty = (HUD_HEIGHT - th) / 2;

        snprintf(buf, sizeof(buf), "SCORE %d", state->score);
        draw_text(r, font_sm, buf, 12, ty, white);

        snprintf(buf, sizeof(buf), "LIVES %d", state->lives);
        { int w, h; TTF_SizeText(font_sm, buf, &w, &h);
          draw_text(r, font_sm, buf, WINDOW_WIDTH - w - 12, ty, red); }
    }

    if (state->flags & FLAG_PAUSED) {
        SDL_Color grey = {170, 170, 170, 255};
        draw_text_centered(r, font_sm, "-- PAUSED --", HUD_HEIGHT / 2, grey);
    }
}

// ── Food ──────────────────────────────────────────────────────────────────────

static void render_food(SDL_Renderer *r, const Food *food) {
    if (!food->is_active) return;
    SDL_SetRenderDrawColor(r, 220, 55, 55, 255);
    SDL_Rect rect = {
        food->x * CELL_SIZE + 3,
        HUD_HEIGHT + food->y * CELL_SIZE + 3,
        CELL_SIZE - 6, CELL_SIZE - 6
    };
    SDL_RenderFillRect(r, &rect);
}

// ── Snake — walks the linked list ─────────────────────────────────────────────

static void render_snake(SDL_Renderer *r, const Snake *snake) {
    int idx = 0;
    for (Segment *s = snake->head; s; s = s->next, idx++) {
        if (idx == 0)
            SDL_SetRenderDrawColor(r, 120, 235, 120, 255);  // head: bright green
        else
            SDL_SetRenderDrawColor(r,  50, 175,  50, 255);  // body: darker green
        SDL_Rect rect = {
            s->x * CELL_SIZE + 1,
            HUD_HEIGHT + s->y * CELL_SIZE + 1,
            CELL_SIZE - 2, CELL_SIZE - 2
        };
        SDL_RenderFillRect(r, &rect);
    }
}

// ── Hint overlay — shown until create_segment() is implemented ────────────────

static void render_hint(SDL_Renderer *r) {
    // Draw ghost squares where the initial snake body will appear
    SDL_SetRenderDrawColor(r, 35, 55, 35, 255);
    for (int i = 0; i < 3; i++) {
        SDL_Rect rect = {
            (GRID_COLS / 2 - i) * CELL_SIZE + 1,
            HUD_HEIGHT + (GRID_ROWS / 2) * CELL_SIZE + 1,
            CELL_SIZE - 2, CELL_SIZE - 2
        };
        SDL_RenderFillRect(r, &rect);
    }

    int mid = HUD_HEIGHT + GRID_ROWS * CELL_SIZE / 2;
    SDL_Color yellow = {220, 200,  50, 255};
    SDL_Color grey   = {150, 150, 150, 255};
    draw_text_centered(r, font_lg, "No snake yet!",                    mid - 44, yellow);
    draw_text_centered(r, font_sm, "Implement create_segment() in game.c", mid + 4, grey);
    draw_text_centered(r, font_sm, "then run:  make test",             mid + 28, grey);
}

// ── Main render ───────────────────────────────────────────────────────────────

void render_game(SDL_Renderer *renderer, const Snake *snake,
                 const Food *food, const GameState *state) {
    ensure_fonts();

    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    SDL_RenderClear(renderer);

    render_grid(renderer);
    render_hud(renderer, state);
    render_food(renderer, food);

    if (snake->head != NULL)
        render_snake(renderer, snake);
    else
        render_hint(renderer);

    SDL_RenderPresent(renderer);
}
