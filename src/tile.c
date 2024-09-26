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
  tile* temp;
  
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
    {
      temp = &tiles[i][j];
      memset(temp->neighbors, 0, sizeof(temp->neighbors));
      if (edge_tile(i, j, size))
      {
	if (!(i+j))
	{
	  temp->neighbors[0] = &tiles[i][j+1];
          temp->neighbors[1] = &tiles[i+1][j];
          temp->neighbors[2] = &tiles[i+1][j+1];
	}
	else if (i+j == size-1)
	{
	  if (!i)
	  {
            temp->neighbors[0] = &tiles[i][j-1];
	    temp->neighbors[1] = &tiles[i+1][j-1];
	    temp->neighbors[2] = &tiles[i+1][j];
          }
	  else
	  {
	    temp->neighbors[0] = &tiles[i-1][j];
	    temp->neighbors[1] = &tiles[i-1][j+1];
	    temp->neighbors[2] = &tiles[i][j+1];
	  }
	}
	else if (i+j == 2*(size-1))
	{
	  temp->neighbors[0] = &tiles[i-1][j];
	  temp->neighbors[1] = &tiles[i-1][j-1];
	  temp->neighbors[2] = &tiles[i][j-1];
	}
	else if (i+j == j)
	{
	  temp->neighbors[0] = &tiles[i][j-1];
	  temp->neighbors[1] = &tiles[i][j+1];
	  temp->neighbors[2] = &tiles[i+1][j-1];
	  temp->neighbors[3] = &tiles[i+1][j];
	  temp->neighbors[4] = &tiles[i+1][j+1]; 	  
	}
	else if (i+j == j + (size-1))
	{
	  temp->neighbors[0] = &tiles[i][j-1];
	  temp->neighbors[1] = &tiles[i][j+1];
	  temp->neighbors[2] = &tiles[i-1][j-1];
	  temp->neighbors[3] = &tiles[i-1][j];
	  temp->neighbors[4] = &tiles[i-1][j+1]; 	  
	}
	else if (i+j == i)
	{
	  temp->neighbors[0] = &tiles[i-1][j];
	  temp->neighbors[1] = &tiles[i-1][j+1];
	  temp->neighbors[2] = &tiles[i][j+1];
	  temp->neighbors[3] = &tiles[i+1][j];
	  temp->neighbors[4] = &tiles[i+1][j+1]; 	  
	}
	else if (i+j == i + (size-1))
	{
	  temp->neighbors[0] = &tiles[i-1][j];
	  temp->neighbors[1] = &tiles[i-1][j-1];
	  temp->neighbors[2] = &tiles[i][j-1];
	  temp->neighbors[3] = &tiles[i+1][j];
	  temp->neighbors[4] = &tiles[i+1][j-1]; 	  
	}
      }
      else
      {
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
}

int edge_tile(size_t i, size_t j, size_t size)
{
  int v, h;

  v = i == 0 || i == size-1;
  h = j == 0 || j == size-1;
  
  return v || h;
}
