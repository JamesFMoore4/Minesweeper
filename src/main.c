#include "tile.h"

size_t bytes_allocated;

int main(void)
{
  tile** tiles;
  size_t size;

  bytes_allocated = 0;
  size = 16;
  
  InitWindow(800, 800, "window");
  SetTargetFPS(60);

  tiles = init_tiles(size);
  printf("KB allocated: %zu\n", bytes_allocated / 1000);

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("test", 400, 225, 12, RED);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
