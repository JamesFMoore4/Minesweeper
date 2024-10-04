#include "game.h"

static int game_update(grid_t*, mode*, int);
static void game_draw(grid_t*);
static int process_tile_click(tile_t*, int);

void game_init(void)
{
  InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Minesweeper");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetWindowMinSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  SetTargetFPS(60);

  srand(time(NULL));
}

void game_loop(int size)
{
  grid_t* grid;
  int dimension, game_status;
  mode cur_mode;
 
  cur_mode = PREGAME;
  game_status = 0;
  dimension = 0.75f * GetScreenHeight();
  grid = grid_init(size, GRID_HOFFSET, GRID_VOFFSET, dimension, dimension);
  
  while (!game_status)
  {
    if (WindowShouldClose()) break;
    game_status = game_update(grid, &cur_mode, game_status);
    game_draw(grid);
  }

  grid_free(grid);
}

void game_close(void)
{
  CloseWindow();
}

static int game_update(grid_t* grid, mode* cur_mode, int game_status)
{
  tile_t* selected;
  int tile_clicked;

  tile_clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
    (selected = grid_get_selected_tile(grid));

  if (*cur_mode == PREGAME && tile_clicked)
  {
    grid_set_mines(grid, selected);
    grid_set_num_mines(grid);
    grid_discover_safe_tiles(selected, BIT_UNKNOWN | BIT_FLAG | BIT_QFLAG);
    *cur_mode = INGAME;
  }
  else if (*cur_mode == INGAME && tile_clicked)
  {
    game_status = process_tile_click(selected, game_status);
    game_status  = !game_status ? grid_all_tiles_discovered(grid) : game_status;
  }
  
  grid_resize(grid);
  grid_highlight(grid);
  grid_flag(grid);

  return game_status;
}

static void game_draw(grid_t* grid)
{
  BeginDrawing();
  ClearBackground((Color){75, 75, 75, 255});
  grid_draw(grid);
  EndDrawing();
}

static int process_tile_click(tile_t* selected, int game_status)
{  
  if (IsKeyDown(KEY_LEFT_SHIFT) && !tile_is_unknown(selected))
  {    
    switch (tile_flags_correct(selected))
    {
    case -1:
      break;
    case 0:
      game_status = -1;
      break;
    case 1:
      grid_discover_safe_tiles(selected, BIT_UNKNOWN);
      break;
    }
    return game_status;
  }
  
  if (tile_is_mined(selected))
    return -1;

  if (!tile_get_num_mines(selected))
  {
    grid_discover_safe_tiles(selected, BIT_UNKNOWN);
  }
  else
  {
    selected->color = BEIGE;
    tile_set_multiple(selected, 0, BIT_UNKNOWN | BIT_FLAG | BIT_QFLAG);
  }

  return game_status;
}
