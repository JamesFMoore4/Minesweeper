#ifndef TILE
#define TILE

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define BIT_MINE 0x10
#define BIT_FLAG 0x20
#define BIT_QFLAG 0x40
#define BIT_UNKNOWN 0x80

#define MINED(info) ((info) & 0x10)
#define FLAGGED(info) ((info) & 0x20)
#define QFLAGGED(info) ((info) & 0x40)
#define UNKNOWN(info) ((info) & 0x80)
#define NUM_MINES(info) (((info) << 4) >> 4)
#define SET(info, bit) ((info) | (bit))
#define UNSET(info, bit) ((info) ^ (bit))

typedef uint8_t bitvec_t;

typedef struct tile
{
  struct tile* neighbors[8];
  Color color;
  int posx;
  int posy;
  int width;
  int height;
  bitvec_t info;
} tile;

tile** init_tiles(size_t size);
void free_tiles(tile** tiles, size_t size);
void set_neighbors(tile** tiles, size_t size);
void draw_tiles(tile** tiles, size_t size);
int compare(const void* x, const void* y);
tile* get_tile(tile** tiles, size_t size);

#endif
