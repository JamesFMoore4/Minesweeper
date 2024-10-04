#ifndef GAME
#define GAME

#include "grid.h"
#include "gamedef.h"

typedef enum {MENU, PREGAME, INGAME} mode;

grid_t* game_init(void);
void game_loop(grid_t*);
void game_close(grid_t*);

#endif

