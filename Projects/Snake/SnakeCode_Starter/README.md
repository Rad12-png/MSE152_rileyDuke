# Snake — MSE 152 Final Project Starter

## Overview
A starter codebase for the MSE 152 Snake game project. The SDL2 setup,
rendering, and input handling are provided. Your job is to implement the
linked list that drives the snake, wire up the physics, and add the required
game features.

## Requirements
- GCC
- SDL2
- Linux (Ubuntu/Debian) or macOS (with Homebrew)

## Quick Start

```bash
# 1. Install SDL2 (first time only)
make deps

# 2. Build
make

# 3. Run
make run
```

## Controls
| Key | Action |
|-----|--------|
| Arrow keys / WASD | Move |
| P | Pause / unpause |
| Escape | Quit |

## Project Structure
```
SnakeCode_Starter/
├── inc/
│   ├── game.h       # Structs, constants, bitwise flags, function declarations
│   ├── input.h
│   ├── physics.h
│   └── render.h
├── src/
│   ├── main.c       # SDL2 init — no changes needed
│   ├── game.c       # ★ Implement linked list operations here
│   ├── input.c      # Key event handling
│   ├── physics.c    # ★ Implement movement and collision here
│   └── render.c     # Rendering — no changes needed for base game
├── bin/             # Compiled binary (created by make)
└── Makefile
```

## What You Need to Implement
See `TODO` comments throughout `game.c` and `physics.c`.

### Base Game
- [ ] `create_segment()` — allocate a new list node with `malloc`
- [ ] `snake_add_head()` — prepend a node to the linked list
- [ ] `snake_remove_tail()` — remove and free the last node
- [ ] `snake_free()` — free the entire list (zero leaks — check with Valgrind)
- [ ] `reset_snake()` — build the initial 3-segment snake
- [ ] Wall and self-collision detection in `physics.c`
- [ ] Score, lives, and speed increase in `physics.c`
- [ ] Game-over screen and restart in `game.c`
- [ ] Bitwise game state flags (already defined in `game.h` as `FLAG_*`)

### Verify with Valgrind
```bash
valgrind --leak-check=full ./bin/snake
```

## Features Implemented (update this as you go)
- [ ] Score system
- [ ] Lives system
- [ ] Game over & restart
- [ ] Speed increase on food
- [ ] ... (add your enhancements and challenges here)
