#ifndef GRID
#define GRID

#include "tile.h"
#include "wrappers.h"
#include "gamedef.h"

typedef struct
{
  tile_t** tiles;
  int size;
  int posx;
  int posy;
  int width;
  int height;
} grid_t;

grid_t* grid_init(int, int, int, int, int);
void grid_free(grid_t*);
void grid_draw(grid_t*, int);
void grid_highlight(grid_t*, int);
void grid_flag(grid_t*);
tile_t* grid_get_selected_tile(grid_t*);
void grid_set_mines(grid_t*, tile_t*);
void grid_set_num_mines(grid_t*);
void grid_resize(grid_t*);
void grid_discover_safe_tiles(tile_t*, bitvec_t);
int grid_all_tiles_discovered(grid_t*);
float grid_get_text_scale(int);

#endif
