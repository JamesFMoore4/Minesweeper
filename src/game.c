#include "game.h"

static int game_update(grid_t*, button_t*, mode*, int);
static void game_draw(grid_t*, button_t*, int);
static int process_tile_click(tile_t*);
static button_t* game_exit_button_init(grid_t*);
static void game_exit_button_resize(button_t*, grid_t*);

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
  button_t* exit_button;
 
  cur_mode = PREGAME;
  game_status = 0;
  dimension = GetScreenHeight() - (GRID_VOFFSET*2);
  grid = grid_init(size, GRID_HOFFSET, GRID_VOFFSET, dimension, dimension);
  exit_button = game_exit_button_init(grid);
  
  while (cur_mode != QUIT)
  {
    if (WindowShouldClose()) break;
    game_status = game_update(grid, exit_button, &cur_mode, game_status);
    game_draw(grid, exit_button, game_status);
  }

  grid_highlight(grid, 1);
  button_highlight(NULL, 1);
    
  free_buttons(exit_button, 1);
  grid_free(grid);
}

void game_close(void)
{
  CloseWindow();
}

static int game_update(grid_t* grid, button_t* exit_button, mode* cur_mode, int game_status)
{
  tile_t* selected;
  int tile_clicked, lmb_pressed;

  lmb_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

  if (lmb_pressed && button_collision(exit_button, GetMouseX(), GetMouseY()))
  {
    *cur_mode = QUIT;
    return game_status;
  }

  tile_clicked = lmb_pressed && (selected = grid_get_selected_tile(grid));

  if (*cur_mode == PREGAME && tile_clicked)
  {
    grid_set_mines(grid, selected);
    grid_set_num_mines(grid);
    grid_discover_safe_tiles(selected, BIT_UNKNOWN | BIT_FLAG | BIT_QFLAG);
    *cur_mode = INGAME;
  }
  else if (*cur_mode == INGAME && tile_clicked)
  {
    game_status = process_tile_click(selected);
    if (game_status)
      *cur_mode = POSTGAME;
  }
  else if (*cur_mode == INGAME)
  {
    if (game_status = grid_all_tiles_discovered(grid))
      *cur_mode = POSTGAME;
  }

  grid_resize(grid);
  grid_highlight(grid, 0);
  if (*cur_mode != POSTGAME)
    grid_flag(grid);

  button_highlight(exit_button, 0);
  game_exit_button_resize(exit_button, grid);

  return game_status;
}

static void game_draw(grid_t* grid, button_t* exit_button, int game_status)
{
  BeginDrawing();
  ClearBackground((Color){75, 75, 75, 255});
  grid_draw(grid, game_status);
  button_draw(exit_button);
  EndDrawing();
}

static int process_tile_click(tile_t* selected)
{
  int game_status;

  game_status = 0;
  
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

static button_t* game_exit_button_init(grid_t* grid)
{
  button_t* exit_button;

  exit_button = (button_t*) Malloc(sizeof(button_t));

  exit_button->color = GRAY;
  exit_button->outline = BLACK;
  exit_button->text_color = BLACK;
  exit_button->width = 0.20f * GetScreenWidth();
  exit_button->height = 0.10f * GetScreenHeight();
  exit_button->posx = grid->posx + grid->width + 25;
  exit_button->posy = grid->posy + grid->height - exit_button->height;
  exit_button->text = (char*) Calloc(12, sizeof(char));
  exit_button->tposx = exit_button->posx + (0.25f *
					    exit_button->width);
  exit_button->tposy = exit_button->posy + (0.25f *
					    exit_button->height);
  strncpy(exit_button->text, "Exit Game", 10);
  exit_button->on_click = NULL;
  
  return exit_button;
}

static void game_exit_button_resize(button_t* exit_button, grid_t* grid)
{
  static int prev_width = DEFAULT_WINDOW_WIDTH;
  static int prev_height = DEFAULT_WINDOW_HEIGHT;

  if (prev_width == GetScreenWidth() &&
    prev_height == GetScreenHeight())
    return;
  
  exit_button->width = 0.20f * GetScreenWidth();
  exit_button->height = 0.10f * GetScreenHeight();
  exit_button->posx = grid->posx + grid->width + GRID_HOFFSET;
  exit_button->posy = grid->posy + grid->height - exit_button->height;
  exit_button->tposx = exit_button->posx + (0.25f *
					    exit_button->width);
  exit_button->tposy = exit_button->posy + (0.25f *
					    exit_button->height);
}
