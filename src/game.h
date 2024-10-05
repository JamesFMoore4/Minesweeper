#ifndef GAME
#define GAME

#include "grid.h"
#include "gamedef.h"
#include "menu.h"

typedef enum {PREGAME, INGAME, POSTGAME} mode;

void game_init(void);
void game_loop(int);
void game_close(void);

#endif

