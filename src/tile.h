#ifndef TILE
#define TILE

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#include "panel.h"

#define BIT_MINE 0x10
#define BIT_FLAG 0x20
#define BIT_QFLAG 0x40
#define BIT_UNKNOWN 0x80

#define MINED(info) ((info) & BIT_MINE)
#define FLAGGED(info) ((info) & BIT_FLAG)
#define QFLAGGED(info) ((info) & BIT_QFLAG)
#define UNKNOWN(info) ((info) & BIT_UNKNOWN)
#define NUM_MINES(info) ((info) & 0xF)
#define SET(info, bit) ((info) | (bit))
#define UNSET(info, bit) ((info) & ~(bit))

typedef uint8_t bitvec_t;

typedef struct tile
{
  struct tile* neighbors[8];
  Color color;
  int posx;
  int posy;
  int width;
  int height;
  int visited;
  bitvec_t info;
} tile;

tile** init_tiles(size_t size);
void free_tiles(tile** tiles, size_t size);
void set_neighbors(tile** tiles, size_t size);
void draw_tiles(tile** tiles, size_t size);
tile* get_tile(tile** tiles, size_t size);
void resize_tiles(tile** tiles, size_t size);
void set_num_mines(tile* tile);
void set_all_num_mines(tile** tiles, size_t size);
uint8_t num_flags(tile* clicked);
int flags_correct(tile* clicked);

#endif
