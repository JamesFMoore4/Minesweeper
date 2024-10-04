#include "game.h"

static int game_status;
static mode cur_mode;

static void update(grid_t*);
static void draw(grid_t*);
static void process_tile_click(tile_t*);

grid_t* game_init(void)
{
  int size, dimension;
  grid_t* grid;
  
  InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Minesweeper");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetWindowMinSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  SetTargetFPS(60);

  srand(time(NULL));

  cur_mode = PREGAME;
  game_status = 0;
  size = SIZE;
  dimension = 0.75f * DEFAULT_WINDOW_HEIGHT;
  grid = grid_init(SIZE, GRID_HOFFSET, GRID_VOFFSET, dimension, dimension);

  return grid;
}

void game_loop(grid_t* grid)
{
  while (!WindowShouldClose() && !game_status)
  {
    update(grid);
    BeginDrawing();
    ClearBackground((Color){75, 75, 75, 255});
    draw(grid);
    EndDrawing();    
  }
}

void game_close(grid_t* grid)
{
  grid_free(grid);
  CloseWindow();
}

static void update(grid_t* grid)
{
  tile_t* selected;
  int tile_clicked;

  tile_clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
    (selected = grid_get_selected_tile(grid));

  if (cur_mode == MENU)
  {
    // Handle menu events
  }
  else if (cur_mode == PREGAME && tile_clicked)
  {
    grid_set_mines(grid, selected);
    grid_set_num_mines(grid);
    grid_discover_safe_tiles(selected, BIT_UNKNOWN | BIT_FLAG | BIT_QFLAG);
    cur_mode = INGAME;
  }
  else if (cur_mode == INGAME && tile_clicked)
  {
    process_tile_click(selected);
    game_status  = !game_status ? grid_all_tiles_discovered(grid) : game_status;
  }
  
  if (cur_mode != MENU)
  {
    grid_resize(grid);
    grid_highlight(grid);
    grid_flag(grid);
  }
}

static void draw(grid_t* grid)
{ 
  if (cur_mode != MENU)
  {
    grid_draw(grid);
  }
}

static void process_tile_click(tile_t* selected)
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

    return;
  }
  
  if (tile_is_mined(selected))
  {
    game_status = -1;
    return;
  }

  if (!tile_get_num_mines(selected))
  {
    grid_discover_safe_tiles(selected, BIT_UNKNOWN);
  }
  else
  {
    selected->color = BEIGE;
    tile_set_multiple(selected, 0, BIT_UNKNOWN | BIT_FLAG | BIT_QFLAG);
  }
}

