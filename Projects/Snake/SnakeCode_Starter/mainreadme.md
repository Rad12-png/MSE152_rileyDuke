# MSE 152 Snake Game Final Project

## Group 21: Levi Guan, Riley Duke, and Aarav Chaudhry

### Team Member's Contributions (fill in for yourself)

| Riley | Levi | Aarav |
|-------|------|-------|
|- Random obstacle generation |- Snake body||
|- Level system |- Food Spawning, eating, snake growth||
|- Fixed immediate reversal issue (180 degree self collision) |- Zero unit test valgrind errors||
|- High score vs Current score (within levels) |- Persistent high score||
||- Eat and death sound ||

### All Features (add more if any are missing)
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

### Build and Run Instructions
---
In the directory SnakeCode_Starter:
- enter: "make"
- enter: "make run"
- once the game has ended enter: "make clean"

---
