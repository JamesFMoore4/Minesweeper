#include "tile.h"
#include "game.h"

int main(void)
{
  tile** tiles;
  size_t size;

  tiles = game_init(&size);
  game_loop(tiles, size);
  game_close(tiles, size);
  
  return 0;
}
