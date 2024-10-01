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
      temp->posx = hoffset * (j-1) + tile_panel.posx;
      temp->posy = voffset * (i-1) + tile_panel.posy;
      temp->width = hoffset;
      temp->height = voffset;
      temp->info = BIT_UNKNOWN;
    }
  }

  // Zero out edge tiles
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

int compare(const void* x, const void* y)
{
  const tile* t1, *t2;

  t1 = (const tile*) x;
  t2 = (const tile*) y;

  if (t1->width < t2->width) return 1;
  return t1->width == t2->width ? 0 : -1;
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
