#include "game.h"

size_t bytes_allocated;
panel tile_panel;

static mode cur_mode;

tile** game_init(size_t* size)
{
  tile** tiles;
  
  bytes_allocated = 0;
  *size = 16;

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Minesweeper");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  SetTargetFPS(60);

  srand(time(NULL));

  cur_mode = PREGAME;
  tile_panel.posx = 25;
  tile_panel.posy = 25;
  tile_panel.height = 0.75f * WINDOW_HEIGHT;
  tile_panel.width = tile_panel.height;
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
    ClearBackground((Color){75, 75, 75, 255});
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
  tile* clicked;
  
  if (cur_mode == PREGAME)
  {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
	(clicked = get_tile(tiles, size)))
    {
      set_mines(tiles, clicked, size);
      // Color tiles, display numbers
      cur_mode = INGAME;
    }
  }
  
  if (cur_mode != MENU)
  {
    resize(tiles, size);
    highlight(tiles, size);
    flag(tiles, size);
  }
}

static void draw(tile** tiles, size_t size)
{ 
  if (cur_mode != MENU)
  {
    draw_tiles(tiles, size);
  }
}

static void highlight(tile** tiles, size_t size)
{
  static tile* hl_tile = NULL;
  static Color prev_color = GRAY;
  
  tile* tile;
  int i, j, hoffset, voffset;

  if (!hl_tile)
    hl_tile = &tiles[1][1];

  if (tile = get_tile(tiles, size))
  {
    hl_tile->color = prev_color;
    hl_tile = tile;
    prev_color = hl_tile->color;
    hl_tile->color = (Color){prev_color.r + 20, prev_color.g + 20, prev_color.b + 20, 255};
  }
  else
  {
    hl_tile->color = prev_color;
    hl_tile = NULL;
  }
  
}

static void flag(tile** tiles, size_t size)
{
  tile* tile;
  
  if (!IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || !(tile = get_tile(tiles, size)))
    return;

  if (!UNKNOWN(tile->info))
    return;

  if (FLAGGED(tile->info))
  {
    tile->info = UNSET(tile->info, BIT_FLAG);
    tile->info = SET(tile->info, BIT_QFLAG);
  }
  else if (QFLAGGED(tile->info))
  {
    tile->info = UNSET(tile->info, BIT_QFLAG);
  }
  else
  {
    tile->info = SET(tile->info, BIT_FLAG);
  }
}

static void resize(tile** tiles, size_t size)
{
  static int prev_height = WINDOW_HEIGHT;
  static int prev_width = WINDOW_WIDTH;

  if (GetScreenHeight() != prev_height || GetScreenWidth() != prev_width)
  {
    tile_panel.height = 0.75f * GetScreenHeight();
    tile_panel.width = tile_panel.height;
    resize_tiles(tiles, size);
  }
}

//TODO
static void set_mines(tile** tiles,
		      tile* clicked_tile, size_t size)
{
  size_t i, j, mines;
  tile* temp;
  const size_t max_mines = 0.20f * (size * size);

  mines = 0;
  while (mines < max_mines)
  {
    i = (size_t) (rand() % size + 1);
    j = (size_t) (rand() % size + 1);
    if (1) // insert condition that guarantees neighbor tiles around first clicked tile are safe
    {
      tiles[i][j].info = SET(tiles[i][j].info, BIT_MINE);
      mines++;
    }
  }
  
}
