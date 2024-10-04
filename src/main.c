#include "game.h"

int main(void)
{
  grid_t* grid;

  grid = game_init();
  game_loop(grid);
  game_close(grid);
  
  return 0;
}
