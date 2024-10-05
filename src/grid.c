#include "grid.h"

static void grid_set_neighbors(tile_t**, int);
static int grid_safe_to_mine(grid_t*, tile_t*, int, int);

grid_t* grid_init(int size, int posx, int posy, int width, int height)
{
  grid_t* grid;
  tile_t** tiles;
  tile_t* temp;
  int i, j, asize, amount, hoffset, voffset;

  grid = (grid_t*)malloc(sizeof(grid_t));
  grid->size = size;
  grid->posx = posx;
  grid->posy = posy;
  grid->width = width;
  grid->height = height;

  hoffset = width / size;
  voffset = height / size;
  asize = size + 2;
  amount = asize * sizeof(tile_t);

  tiles = (tile_t**)malloc(asize*sizeof(tile_t*));

  // Padding to eliminate bounds checking
  tiles[0] = (tile_t*)calloc(asize, sizeof(tile_t));
  tiles[asize-1] = (tile_t*)calloc(asize, sizeof(tile_t));
  
  for (i = 1; i <= size; i++)
  {
    tiles[i] = (tile_t*)calloc(asize, sizeof(tile_t));
    for (j = 1; j <= size; j++)
    {
      temp = &tiles[i][j];
      temp->color = GRAY;
      temp->posx = hoffset * (j-1) + posx;
      temp->posy = voffset * (i-1) + posy;
      temp->width = hoffset;
      temp->height = voffset;
      tile_set_unknown(temp, 1);
    }
  }

  grid_set_neighbors(tiles, size);
  grid->tiles = tiles;
  return grid;
}

void grid_free(grid_t* grid)
{
  int i;

  for (i = 0; i < grid->size + 2; i++)
    free(grid->tiles[i]);
  free(grid->tiles);
  free(grid);
}

void grid_draw(grid_t* grid)
{
  int i, j, num_mines;
  tile_t* temp;
  char text[2];
  float scale;

  for (i = 1; i <= grid->size; i++)
  {
    for (j = 1; j <= grid->size; j++)
    {
      temp = &grid->tiles[i][j];
      num_mines = tile_get_num_mines(temp);
      scale = grid_get_text_scale(grid->size);

      if (tile_is_flagged(temp))
	strncpy(text, "F", 2);
      else if (tile_is_qflagged(temp))
	strncpy(text, "?", 2);
      else if (tile_is_unknown(temp) || !num_mines)
	strncpy(text, " ", 2);
      else
	sprintf(text, "%d", num_mines);

      DrawRectangle(temp->posx, temp->posy,
		    temp->width, temp->height,
		    temp->color);
      DrawRectangleLines(temp->posx, temp->posy,
			 temp->width, temp->height,
			 BLACK);
      DrawText(text, temp->posx + (0.25f * temp->width),
	       temp->posy + (0.25f * temp->height), 32*scale,
	       BLACK);
    }
  }
}

void grid_highlight(grid_t* grid)
{
  tile_highlight(grid_get_selected_tile(grid));
}

void grid_flag(grid_t* grid)
{
  tile_t* selected;
  int ret;

  ret = !IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
  ret = ret || !(selected = grid_get_selected_tile(grid));
  ret = ret || !tile_is_unknown(selected);

  if (ret) return;

  if (tile_is_flagged(selected))
    tile_set_qflagged(selected, 1);
  else if (tile_is_qflagged(selected))
    tile_set_qflagged(selected, 0);
  else
    tile_set_flagged(selected, 1);
}

tile_t* grid_get_selected_tile(grid_t* grid)
{
  static tile_t* last = NULL;
  tile_t temp;
  int i, j, mposx, mposy;

  mposx = GetMouseX();
  mposy = GetMouseY();

  temp.posx = grid->posx;
  temp.posy = grid->posy;
  temp.width = grid->width;
  temp.height = grid->height;

  if (!tile_collision(&temp, mposx, mposy))
    return (last = NULL);

  if (last)
  {
    if (tile_collision(last, mposx, mposy))
      return last;
    for (i = 0; i < 8; i++)
      if (last->neighbors[i]->width && tile_collision(last->neighbors[i], mposx, mposy))
	return (last = last->neighbors[i]);
  }
  
  for (i = 1; i <= grid->size; i++)
  {
    for (j = 1; j <= grid->size; j++)
    {
      if (tile_collision(&grid->tiles[i][j], mposx, mposy))
	return (last = &grid->tiles[i][j]);
    }
  }

  return (last = NULL);
}

void grid_set_mines(grid_t* grid, tile_t* selected)
{
  int i, j, mines, total_mines;

  mines = 0;
  total_mines = PERCENT_MINED * (grid->size * grid->size);

  while (mines < total_mines)
  {
    i = rand() % grid->size + 1;
    j = rand() % grid->size + 1;

    if (grid_safe_to_mine(grid, selected, i, j) && !tile_is_mined(&grid->tiles[i][j]))
    {
      tile_set_mined(&grid->tiles[i][j]);
      mines++;
    }
  }
}

void grid_set_num_mines(grid_t* grid)
{
  int i, j;

  for (i = 1; i <= grid->size; i++)
    for (j = 1; j <= grid->size; j++)
      tile_set_num_mines(&grid->tiles[i][j]);
}

void grid_resize(grid_t* grid)
{
  static int prev_width = DEFAULT_WINDOW_WIDTH;
  static int prev_height = DEFAULT_WINDOW_HEIGHT;
  int i, j, hoffset, voffset, dimension;
  tile_t* temp;

  if (prev_width == GetScreenWidth() &&
      prev_height == GetScreenHeight())
    return;

  dimension = GetScreenHeight() - (GRID_VOFFSET*2);

  grid->width = grid->height = dimension;
  hoffset = grid->width / grid->size;
  voffset = grid->width / grid->size;

  for (i = 1; i <= grid->size; i++)
  {
    for (j = 1; j <= grid->size; j++)
    {
      temp = &grid->tiles[i][j];

      temp->posx = hoffset * (j-1) + grid->posx;
      temp->posy = voffset * (i-1) + grid->posy;
      temp->width = hoffset;
      temp->height = voffset;
    }
  }
}

void grid_discover_safe_tiles(tile_t* selected, bitvec_t flags)
{
  int i, tile_is_blank, tile_not_mined;
  tile_t* temp;

  selected->color = BEIGE;
  tile_set_multiple(selected, 0, flags);

  for (i = 0; i < 8; i++)
  {
    temp = selected->neighbors[i];

    if (!temp->width)
      continue;

    tile_not_mined = !tile_is_mined(temp);
    tile_is_blank = !tile_get_num_mines(temp) && tile_not_mined;
    tile_is_blank = tile_is_blank && !temp->visited;

    temp->visited = 1;

    if (tile_is_blank)
    {
      grid_discover_safe_tiles(temp, flags);
    }
    else if (tile_not_mined)
    {
      tile_set_multiple(temp, 0, flags);
      temp->color = BEIGE;
    }
  }
}

static void grid_set_neighbors(tile_t** tiles, int size)
{
  int i, j;
  tile_t* temp;

  for (i = 1; i <= size; i++)
  {
    for (j = 1; j <= size; j++)
    {
      temp = &tiles[i][j];
      
      temp->neighbors[0] = &tiles[i-1][j-1];
      temp->neighbors[1] = &tiles[i-1][j];
      temp->neighbors[2] = &tiles[i-1][j+1];
      
      temp->neighbors[3] = &tiles[i][j-1];
      temp->neighbors[4] = &tiles[i][j+1];
      
      temp->neighbors[5] = &tiles[i+1][j-1];
      temp->neighbors[6] = &tiles[i+1][j];
      temp->neighbors[7] = &tiles[i+1][j+1];
    }
  }
}

// First clicked tile and its neighbors should not be mined
static int grid_safe_to_mine(grid_t* grid, tile_t* selected,
			     int vindex, int hindex)
{
  int i;
  tile_t* temp;

  temp = &grid->tiles[vindex][hindex];

  if (temp == selected)
    return 0;

  for (i = 0; i < 8; i++)
    if (temp == selected->neighbors[i])
      return 0;

  return 1;
}

int grid_all_tiles_discovered(grid_t* grid)
{
  int i, j, total_tiles;
  tile_t* temp;
  
  total_tiles = grid->size * grid->size;
  total_tiles -= PERCENT_MINED * total_tiles;

  for (i = 1; i <= grid->size; i++)
    for (j = 1; j <= grid->size; j++)
      if (!tile_is_unknown(&grid->tiles[i][j]))
	total_tiles--;

  return !total_tiles;
}

float grid_get_text_scale(int size)
{
  switch(size)
  {
  case 8:
    return 2.0f;
  case 16:
    return 1.0f;
  case 32:
    return 0.5f;
  }
}
