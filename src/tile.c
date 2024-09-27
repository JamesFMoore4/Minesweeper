#include "tile.h"

extern size_t bytes_allocated;

tile** init_tiles(size_t size)
{
  tile** tiles, *temp;
  size_t i, j, asize, amount;
  int scr_width, scr_height, hoffset, voffset;

  scr_width = GetScreenWidth();
  scr_height = GetScreenHeight();
  hoffset = scr_width / size;
  voffset = scr_height / size;
  asize = size + 2;
  amount = asize*sizeof(tile);
  
  tiles = (tile**)malloc(asize*sizeof(tile*));
  bytes_allocated += asize*sizeof(tile*);

  // Padding to eliminate bounds checking
  tiles[0] = (tile*)malloc(amount);
  tiles[asize-1] = (tile*)malloc(amount);
  memset(tiles[0], 0, amount);
  memset(tiles[asize-1], 0, amount);
  
  for (i = 1; i <= size; i++)
  {
    tiles[i] = (tile*)malloc(amount);
    bytes_allocated += amount;
    for (j = 1; j <= size; j++)
    {
      temp = &tiles[i][j];
      temp->color = GRAY;
      temp->posx = hoffset * (j-1);
      temp->posy = voffset * (i-1);
      temp->width = hoffset;
      temp->height = voffset;
      temp->flagged = 0;
      temp->mined = 0;
    }
  }

  for (i = 0; i < asize; i++)
  {
    memset(&tiles[i][0], 0, sizeof(tile));
    memset(&tiles[i][asize-1], 0, sizeof(tile));
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
      memset(temp->neighbors, 0, sizeof(temp->neighbors));
      
      temp->neighbors[0] = &tiles[i-1][j-1];
      temp->neighbors[1] = &tiles[i-1][j];
      temp->neighbors[2] = &tiles[i-1][j+1];
      
      temp->neighbors[3] = &tiles[i][j-1];
      temp->neighbors[4] = &tiles[i][j+1];
      
      temp->neighbors[5] = &tiles[i+1][j-1];
      temp->neighbors[6] = &tiles[i+1][j];
      temp->neighbors[7] = &tiles[i+1][j+1];

      // Push pointers to null tiles to back of array
      qsort(temp->neighbors, 8, sizeof(tile*), compare);
    }
  }
}

//TODO: fix
void draw_tiles(tile** tiles, size_t size)
{
  size_t i, j;
  tile* temp;

  for (i = 1; i <= size; i++)
  {
    for (j = 1; j <= size; j++)
    {
      temp = &tiles[i][j];
      DrawRectangle(temp->posx, temp->posy,
		    temp->width, temp->height,
		    temp->color);
      DrawRectangleLines(temp->posx, temp->posy,
		    temp->width, temp->height,
		    BLACK);
    }
  }
}

int compare(const void* x, const void* y)
{
  const tile* t1, *t2;

  t1 = (const tile*) x;
  t2 = (const tile*) y;

  return t1->width < t2->width ? 1 : 0;
}
