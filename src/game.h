#ifndef GAME
#define GAME

#include "tile.h"
#include "panel.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

typedef enum {PREGAME, INGAME, MENU} mode;

tile** game_init(size_t* size);
void game_loop(tile** tiles, size_t size);
void game_close(tile** tiles, size_t size);

static void update(tile** tiles, size_t size);
static void draw(tile** tiles, size_t size);
static void highlight(tile** tiles, size_t size);
static void flag(tile** tiles, size_t size);
static void resize(tile** tiles, size_t size);
static void set_mines(tile** tiles,
		      tile* clicked, size_t size);
static int safe(size_t vindex, size_t hindex, tile** tiles, tile* clicked);
static void set_safe_tile_colors(tile* clicked, uint8_t flags);
static void process_tile_click(tile* clicked);
static void win_check(tile** tiles, size_t size);

#endif

