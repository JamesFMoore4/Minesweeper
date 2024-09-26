#include "tile.h"

extern size_t bytes_allocated;

tile** init_tiles(size_t size)
{
  tile** tiles, *temp;
  size_t i, j;
  int scr_width, scr_height, hoffset, voffset;

  scr_width = GetScreenWidth();
  scr_height = GetScreenHeight();
  hoffset = scr_width / size;
  voffset = scr_height / size;
  
  tiles = (tile**)malloc(size*sizeof(tile*));
  bytes_allocated += size*sizeof(tile*);
  for (i = 0; i < size; i++)
  {
    tiles[i] = (tile*)malloc(size*sizeof(tile));
    bytes_allocated += size*sizeof(tile);
    for (j = 0; j < size; j++)
    {
      temp = &tiles[i][j];
      temp->color = GRAY;
      temp->posx = hoffset * j;
      temp->posy = voffset * i;
      temp->width = hoffset;
      temp->height = voffset;
      temp->flagged = 0;
      temp->mined = 0;
    }
  }

  set_neighbors(tiles, size);
}

void free_tiles(tile** tiles, size_t size)
{
  size_t i, j;

  for (i = 0; i < size; i++)
      free(tiles[i]);
  free(tiles);
  bytes_allocated = 0;
}

void set_neighbors(tile** tiles, size_t size)
{
  size_t i, j;


  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
    {
      
    }
  }
}

int edge_tile(size_t i, size_t j, size_t size)
{
  int v, h;

  v = i == 0 || i == size-1;
  h = j == 0 || j == size-1;
  
  return v || h;
}
