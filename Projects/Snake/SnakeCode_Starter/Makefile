# ─────────────────────────────────────────────────────────────────────────────
# Snake — Makefile
# Supports: macOS (Homebrew) and Ubuntu/Debian Linux
# ─────────────────────────────────────────────────────────────────────────────

CC     = gcc
TARGET = bin/snake

SRCS = src/main.c src/game.c src/input.c src/physics.c src/render.c
OBJS = $(SRCS:.c=.o)

CFLAGS  = -Wall -Wextra -g -Iinc
LDFLAGS =

# ── OS detection ──────────────────────────────────────────────────────────────
UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
    # ── macOS ─────────────────────────────────────────────────────────────────
    BOTH_PC := $(shell pkg-config --exists sdl2 SDL2_ttf SDL2_mixer 2>/dev/null && echo yes)
    ifeq ($(BOTH_PC), yes)
        CFLAGS  += $(shell pkg-config --cflags sdl2 SDL2_ttf SDL2_mixer)
        LDFLAGS += $(shell pkg-config --libs   sdl2 SDL2_ttf SDL2_mixer)
    else
        BREW_SDL2 := $(shell brew --prefix sdl2     2>/dev/null)
        BREW_TTF  := $(shell brew --prefix sdl2_ttf 2>/dev/null)
        ifeq ($(BREW_SDL2),)
            $(error SDL2 not found — run: make deps)
        endif
        ifeq ($(BREW_TTF),)
            $(error SDL2_ttf not found — run: make deps)
        endif
        CFLAGS  += -I$(BREW_SDL2)/include/SDL2 -I$(BREW_TTF)/include/SDL2
        LDFLAGS += -L$(BREW_SDL2)/lib -lSDL2 -L$(BREW_TTF)/lib -lSDL2_ttf
    endif
    DEPS_CMD = brew install sdl2 sdl2_ttf sdl2_mixer

else
    # ── Linux ─────────────────────────────────────────────────────────────────
    BOTH_PC := $(shell pkg-config --exists sdl2 SDL2_ttf SDL2_mixer 2>/dev/null && echo yes)
    ifeq ($(BOTH_PC), yes)
        CFLAGS  += $(shell pkg-config --cflags sdl2 SDL2_ttf SDL2_mixer)
        LDFLAGS += $(shell pkg-config --libs   sdl2 SDL2_ttf SDL2_mixer)
    else
        $(error SDL2 or SDL2_ttf not found — run: make deps)
    endif
    DEPS_CMD = sudo apt-get update && sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev

endif

TEST_TARGET = bin/test_snake
TEST_SRCS   = tests/test_snake.c src/game.c src/input.c src/physics.c src/render.c

# ── Build targets ─────────────────────────────────────────────────────────────
.PHONY: all clean run deps test

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bin:
	mkdir -p bin

# ── Run ───────────────────────────────────────────────────────────────────────
run: all
	./$(TARGET)

# ── Unit tests (no display required) ──────────────────────────────────────────
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS) | bin
	$(CC) $(CFLAGS) $(TEST_SRCS) -o $@ $(LDFLAGS)

# ── Clean ─────────────────────────────────────────────────────────────────────
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_TARGET)

# ── Install dependencies ──────────────────────────────────────────────────────
deps:
	@echo ">>> Installing SDL2 and SDL2_ttf..."
	$(DEPS_CMD)
	@echo ">>> Done. Run 'make' to build."
