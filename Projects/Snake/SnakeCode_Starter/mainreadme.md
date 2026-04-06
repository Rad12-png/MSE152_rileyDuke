# MSE 152 Snake Game Final Project

## Group 21: Levi Guan, Riley Duke, and Aarav Chaudhry

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
├── assets/
│   ├── die.wav      # death sound effect
│   └── eat.wav      # eating sound effect
├── inc/
│   ├── game.h       # Defines the core data structures, grid configurations, and global function prototypes that coordinate the entire game.
│   ├── input.h      # Declares the handle_input function used to process user keyboard events via SDL2.
│   ├── physics.h    # Declares the update_physics function, which serves as the bridge between the game state and the movement logic.
│   └── render.h     # Declares the high-level function for drawing the game elements to the screen and cleaning up graphical resources. 
├── src/
│   ├── main.c       # SDL2 init
│   ├── game.c       # Implements the main game logic, including linked list management for the snake's body, level initialization, high score persistence, and the primary game loop. 
│   ├── input.c      # Key event handling and prevents illegal 180 degree turns.
│   ├── physics.c    # Handles the rules of the game such as calculating the snake's new position and detecting collisions with the walls, obstacles, food, or itself. 
│   └── render.c     # All visual output, s.a. drawing the grid, the HUD, Game over screen, etc. 
├── bin/             # Compiled binary (created by make)
├── Makefile
└── Highscore.txt    # Simple txt file that updates with the highest score played
```

## Team Member's Contributions

| Riley | Levi | Aarav |
|-------|------|-------|
|- Random obstacle generation |- Snake body||
|- Level system |- Food Spawning, eating, snake growth||
|- Fixed immediate reversal issue (180 degree self collision) |- Zero unit test valgrind errors||
|- High score vs Current score (within levels) |- Persistent high score||
||- Eat and death sound ||

## All Features
---
Base Game
- Snake body as a singly linked list
- Snake movement, grid-based update lop, directional control
- Food spawning, eating, and snake growth
- Collision detection - wall and self
- Score display, game over screen, restart
- Bitwise game state flags
- Unit tests pass with zero Valgrind errors
- Increasing speed with increased snake size

Enhancements
- Persistent high score
- Obstacle Walls, static walls placed on the grid that vary per level

Challenges
- Full pause menu system: main menu, pause, high scores, settings
- Procedural level generation (random obstacle generation on each level)
- Levels of increasing difficulty (more obstacles and larger score win-conditions)

Bonus
- Sound effects for eating and dying
