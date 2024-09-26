#ifndef TILE
#define TILE

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tile
{
  struct tile* neighbors[8];
  int mined;
  int flagged;
  int posx;
  int posy;
  int width;
  int height;
  Color color;
} tile;

tile** init_tiles(size_t size);
void free_tiles(tile** tiles, size_t size);
void set_neighbors(tile** tiles, size_t size);
int edge_tile(size_t i, size_t j, size_t size);

#endif
