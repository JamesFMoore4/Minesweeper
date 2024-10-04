#include "game.h"
#include "menu.h"

int main(void)
{
  game_init();
  while (!WindowShouldClose())
  {
    game_loop(menu_loop());
  }
  game_close(); 
  return 0;
}
