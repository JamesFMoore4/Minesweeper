#ifndef GAME
#define GAME

#include "tile.h"

tile** game_init(size_t* size);
void game_loop(tile** tiles, size_t size);
void game_close(tile** tiles, size_t size);

static void update(tile** tiles, size_t size);
static void draw(tile** tiles, size_t size);

#endif

