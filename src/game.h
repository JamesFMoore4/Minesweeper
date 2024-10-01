#ifndef GAME
#define GAME

#include "tile.h"
#include "panel.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

tile** game_init(size_t* size);
void game_loop(tile** tiles, size_t size);
void game_close(tile** tiles, size_t size);

static void update(tile** tiles, size_t size);
static void draw(tile** tiles, size_t size);
static void highlight(tile** tiles, size_t size);
static void flag(tile** tiles, size_t size);
static void resize(tile** tiles, size_t size);

#endif

