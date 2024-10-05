#include "game.h"
#include "menu.h"

int main(void)
{
  int size;
  
  game_init();
  while (!WindowShouldClose())
  {
    if (size = menu_loop())
      game_loop(size);
  }
    
  game_close(); 
  return 0;
}
