#include "game.h"

size_t bytes_allocated;

tile** game_init(size_t* size)
{
  tile** tiles;
  
  bytes_allocated = 0;
  *size = 16;

  InitWindow(800, 800, "Minesweeper");
  SetTargetFPS(60);

  tiles = init_tiles(*size);
  printf("KB allocated: %zu\n",
	 bytes_allocated / 1000);

  return tiles;
}

void game_loop(tile** tiles, size_t size)
{
  while (!WindowShouldClose())
  {
    update(tiles, size);
    BeginDrawing();
    ClearBackground(GRAY);
    draw(tiles, size);
    EndDrawing();    
  }
}

void game_close(tile** tiles, size_t size)
{
  free_tiles(tiles, size);
  CloseWindow();
}

static void update(tile** tiles, size_t size)
{
  // Last highlighted tile & previous color
  static tile* hl_tile = NULL;
  static Color prev_color = GRAY;
  
  tile* temp;
  int i, j, hoffset, voffset;

  if (!hl_tile)
    hl_tile = &tiles[1][1];

  hoffset = GetScreenWidth() / size;
  voffset = GetScreenHeight() / size;

  i = GetMouseY() / voffset + 1;
  j = GetMouseX() / hoffset + 1;

  temp = &tiles[i][j];
  hl_tile->color = prev_color;
  hl_tile = temp;
  prev_color = hl_tile->color;
  hl_tile->color = (Color){prev_color.r + 20, prev_color.g + 20, prev_color.b + 20, 255};
  
}

static void draw(tile** tiles, size_t size)
{
  draw_tiles(tiles, size);
}

