#include "tile.h"

extern size_t bytes_allocated;
extern panel tile_panel;

tile** init_tiles(size_t size)
{
  tile** tiles, *temp;
  size_t i, j, asize, amount;
  int hoffset, voffset;
  
  hoffset = tile_panel.width / size;
  voffset = tile_panel.height / size;
  asize = size + 2;
  amount = asize * sizeof(tile);
  
  tiles = (tile**)malloc(asize*sizeof(tile*));
  bytes_allocated += asize*sizeof(tile*);

  // Padding to eliminate bounds checking
  tiles[0] = (tile*)calloc(asize, sizeof(tile));
  tiles[asize-1] = (tile*)calloc(asize, sizeof(tile));
  
  for (i = 1; i <= size; i++)
  {
    tiles[i] = (tile*)calloc(asize, sizeof(tile));
    bytes_allocated += amount;
    
    for (j = 1; j <= size; j++)
    {
      temp = &tiles[i][j];
      temp->color = GRAY;
      temp->posx = hoffset * (j-1) + tile_panel.posx;
      temp->posy = voffset * (i-1) + tile_panel.posy;
      temp->width = hoffset;
      temp->height = voffset;
      temp->visited = 0;
      temp->info = SET(temp->info, BIT_UNKNOWN);
    }
  }

  set_neighbors(tiles, size);
  return tiles;
}

void free_tiles(tile** tiles, size_t size)
{
  size_t i, asize;

  asize = size + 2;
  for (i = 0; i < asize; i++)
    free(tiles[i]);
  free(tiles);
  bytes_allocated = 0;
}

void set_neighbors(tile** tiles, size_t size)
{
  size_t i, j;
  tile* temp;
  
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

void draw_tiles(tile** tiles, size_t size)
{
  size_t i, j;
  tile* temp;
  char text[2];

  for (i = 1; i <= size; i++)
  {
    for (j = 1; j <= size; j++)
    {
      temp = &tiles[i][j];

      if (FLAGGED(temp->info))
	strncpy(text, "F", 2);
      else if (QFLAGGED(temp->info))
	strncpy(text, "?", 2);
      else if (UNKNOWN(temp->info) ||
	       !NUM_MINES(temp->info))
	strncpy(text, " ", 2);
      else
	sprintf(text, "%d", NUM_MINES(temp->info));

      
      DrawRectangle(temp->posx, temp->posy,
		    temp->width, temp->height,
		    temp->color);
      DrawRectangleLines(temp->posx, temp->posy,
		    temp->width, temp->height,
		    BLACK);
      DrawText(text, temp->posx + (0.25f * temp->width),
	       temp->posy + (0.25f * temp->height), 32, BLACK);
    }
  }
}

tile* get_tile(tile** tiles, size_t size)
{
  int i, j, mposx, mposy;
  int posx, posy, width, height;

  mposx = GetMouseX();
  mposy = GetMouseY();

  for (i = 1; i <= size; i++)
  {
    for (j = 1; j <= size; j++)
    {
      posx = tiles[i][j].posx;
      posy = tiles[i][j].posy;
      width = tiles[i][j].width;
      height = tiles[i][j].height;

      if (mposx > posx &&
	mposx < posx + width &&
	mposy > posy &&
	mposy < posy + height)
      {
	return &tiles[i][j];
      }
    }
  }

  return NULL;
}

void resize_tiles(tile** tiles, size_t size)
{
  size_t i, j;
  int hoffset, voffset;
  tile* temp;
  
  hoffset = tile_panel.width / size;
  voffset = tile_panel.height / size;

  for (i = 1; i <= size; i++)
  {
    for (j = 1; j <= size; j++)
    {
      temp = &tiles[i][j];
      
      temp->posx = hoffset * (j-1) + tile_panel.posx;
      temp->posy = voffset * (i-1) + tile_panel.posy;
      temp->width = hoffset;
      temp->height = voffset;
    }
  }
}

void set_num_mines(tile* tile)
{
  size_t i;
  uint8_t num_mines;

  if (MINED(tile->info))
    return;
  
  num_mines = 0;
  for (i = 0; i < 8; i++)
    if (MINED(tile->neighbors[i]->info))
      num_mines++;

  tile->info = SET(tile->info, num_mines);
}

void set_all_num_mines(tile** tiles, size_t size)
{
  size_t i, j;

  for (i = 1; i < size; i++)
    for (j = 1; j < size; j++)
      set_num_mines(&tiles[i][j]);
}

uint8_t num_flags(tile* clicked)
{
  size_t i;
  uint8_t flags;

  flags = 0;
  for (i = 0; i < 8; i++)
  {
    if (FLAGGED(clicked->neighbors[i]->info))
      flags++;
  }

  return flags;
}

int flags_correct(tile* clicked)
{
  size_t i;
  tile* temp;

  for (i = 0; i < 8; i++)
  {
    temp = clicked->neighbors[i];
    if (MINED(temp->info) && !FLAGGED(temp->info))
      return 0;
  }

  return 1;
}
