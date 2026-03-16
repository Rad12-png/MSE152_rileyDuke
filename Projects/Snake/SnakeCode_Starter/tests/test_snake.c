// tests/test_snake.c
// Unit tests for the Snake linked list and input queue.
// Uses only <assert.h> — no external framework required.
// Run with: make test
// Verify no leaks with: valgrind ./bin/test_snake

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

// ── Helper — reset globals to a clean state before each test ─────────────────
static void reset(void) {
    snake_free();
    snake.dx = 1;  snake.dy = 0;
    snake.next_dx = 1; snake.next_dy = 0;
    memset(&input_queue, 0, sizeof(input_queue));
}

// ── create_segment ────────────────────────────────────────────────────────────

static void test_create_segment(void) {
    Segment *s = create_segment(5, 10);
    assert(s != NULL);
    assert(s->x == 5);
    assert(s->y == 10);
    assert(s->next == NULL);
    free(s);
    printf("  [PASS] create_segment\n");
}

// ── snake_add_head ────────────────────────────────────────────────────────────

static void test_add_head_single(void) {
    reset();
    snake_add_head(3, 4);
    assert(snake.length == 1);
    assert(snake.head != NULL);
    assert(snake.head->x == 3 && snake.head->y == 4);
    assert(snake.tail == snake.head);   // only one node: head == tail
    snake_free();
    printf("  [PASS] snake_add_head — single segment\n");
}

static void test_add_head_multiple(void) {
    reset();
    snake_add_head(1, 0);   // added first  → becomes tail
    snake_add_head(2, 0);   // added second → middle
    snake_add_head(3, 0);   // added third  → head
    assert(snake.length == 3);
    assert(snake.head->x == 3);           // newest segment at head
    assert(snake.tail->x == 1);           // oldest segment at tail
    assert(snake.head->next->x == 2);     // middle is correctly linked
    assert(snake.tail->next == NULL);     // tail always points to NULL
    snake_free();
    printf("  [PASS] snake_add_head — multiple segments\n");
}

// ── snake_remove_tail ─────────────────────────────────────────────────────────

static void test_remove_tail_basic(void) {
    reset();
    snake_add_head(1, 0);
    snake_add_head(2, 0);
    snake_add_head(3, 0);   // list: 3 → 2 → 1(tail)
    snake_remove_tail();
    assert(snake.length == 2);
    assert(snake.tail->x == 2);        // 2 is now the tail
    assert(snake.tail->next == NULL);  // tail->next must be NULL
    snake_free();
    printf("  [PASS] snake_remove_tail — basic\n");
}

static void test_remove_tail_to_one(void) {
    reset();
    snake_add_head(5, 5);
    snake_add_head(6, 5);
    snake_remove_tail();
    assert(snake.length == 1);
    assert(snake.head == snake.tail);
    assert(snake.head->x == 6);
    snake_free();
    printf("  [PASS] snake_remove_tail — down to one segment\n");
}

static void test_remove_tail_to_empty(void) {
    reset();
    snake_add_head(5, 5);
    snake_remove_tail();
    assert(snake.length == 0);
    assert(snake.head == NULL);
    assert(snake.tail == NULL);
    printf("  [PASS] snake_remove_tail — to empty\n");
}

static void test_remove_tail_empty_noop(void) {
    reset();
    snake_remove_tail();    // calling on empty list must not crash
    assert(snake.length == 0);
    assert(snake.head == NULL);
    printf("  [PASS] snake_remove_tail — no-op on empty list\n");
}

// ── snake_free ────────────────────────────────────────────────────────────────

static void test_snake_free(void) {
    reset();
    for (int i = 0; i < 10; i++) snake_add_head(i, 0);
    assert(snake.length == 10);
    snake_free();
    assert(snake.head   == NULL);
    assert(snake.tail   == NULL);
    assert(snake.length == 0);
    printf("  [PASS] snake_free — 10 segments freed\n");
}

static void test_snake_free_empty(void) {
    reset();
    snake_free();   // freeing an already-empty list must not crash
    assert(snake.head == NULL);
    printf("  [PASS] snake_free — no-op on empty list\n");
}

// ── input queue (circular buffer) ────────────────────────────────────────────

static void test_iq_fifo_order(void) {
    memset(&input_queue, 0, sizeof(input_queue));
    int dx, dy;

    iq_push(&input_queue,  1,  0);
    iq_push(&input_queue,  0, -1);
    iq_push(&input_queue, -1,  0);

    assert(iq_pop(&input_queue, &dx, &dy) == 1);
    assert(dx ==  1 && dy ==  0);   // first in, first out

    assert(iq_pop(&input_queue, &dx, &dy) == 1);
    assert(dx ==  0 && dy == -1);

    assert(iq_pop(&input_queue, &dx, &dy) == 1);
    assert(dx == -1 && dy ==  0);

    printf("  [PASS] input queue — FIFO order\n");
}

static void test_iq_empty_returns_zero(void) {
    memset(&input_queue, 0, sizeof(input_queue));
    int dx, dy;
    assert(iq_pop(&input_queue, &dx, &dy) == 0);   // empty queue returns 0
    printf("  [PASS] input queue — pop from empty returns 0\n");
}

static void test_iq_full_drops_input(void) {
    memset(&input_queue, 0, sizeof(input_queue));
    for (int i = 0; i < INPUT_QUEUE_SIZE; i++) iq_push(&input_queue, i, 0);
    iq_push(&input_queue, 99, 0);    // queue full — must be silently dropped
    assert(input_queue.count == INPUT_QUEUE_SIZE);

    int dx, dy;
    iq_pop(&input_queue, &dx, &dy);
    assert(dx == 0);    // first item (0,0), not the dropped (99,0)
    printf("  [PASS] input queue — drop when full\n");
}

static void test_iq_wrap_around(void) {
    memset(&input_queue, 0, sizeof(input_queue));
    int dx, dy;

    // Fill, drain, refill to exercise circular wrap
    for (int i = 0; i < INPUT_QUEUE_SIZE; i++) iq_push(&input_queue, i, 0);
    for (int i = 0; i < INPUT_QUEUE_SIZE; i++) iq_pop(&input_queue, &dx, &dy);

    iq_push(&input_queue, 7, 3);
    assert(iq_pop(&input_queue, &dx, &dy) == 1);
    assert(dx == 7 && dy == 3);
    printf("  [PASS] input queue — circular wrap-around\n");
}

// ── main ──────────────────────────────────────────────────────────────────────

int main(void) {
    printf("Running Snake unit tests...\n\n");

    printf("create_segment:\n");
    test_create_segment();

    printf("snake_add_head:\n");
    test_add_head_single();
    test_add_head_multiple();

    printf("snake_remove_tail:\n");
    test_remove_tail_basic();
    test_remove_tail_to_one();
    test_remove_tail_to_empty();
    test_remove_tail_empty_noop();

    printf("snake_free:\n");
    test_snake_free();
    test_snake_free_empty();

    printf("input queue:\n");
    test_iq_fifo_order();
    test_iq_empty_returns_zero();
    test_iq_full_drops_input();
    test_iq_wrap_around();

    printf("\nAll %d tests passed.\n", 13);
    return 0;
}
