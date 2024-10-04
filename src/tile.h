#ifndef TILE
#define TILE

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#define BIT_MINE 0x10
#define BIT_FLAG 0x20
#define BIT_QFLAG 0x40
#define BIT_UNKNOWN 0x80

typedef uint8_t bitvec_t;

typedef struct tile_t
{
  struct tile_t* neighbors[8];
  Color color;
  int posx;
  int posy;
  int width;
  int height;
  int visited;
  bitvec_t info;
} tile_t;

void tile_set_num_mines(tile_t*);
void tile_set_unknown(tile_t*, int);
void tile_set_mined(tile_t*);
void tile_set_flagged(tile_t*, int);
void tile_set_qflagged(tile_t*, int);
void tile_set_multiple(tile_t*, int, uint8_t);

int tile_is_unknown(tile_t*);
int tile_is_mined(tile_t*);
int tile_is_flagged(tile_t*);
int tile_is_qflagged(tile_t*);

void tile_highlight(tile_t*);

int tile_get_num_mines(tile_t*);
int tile_get_num_flags(tile_t*);
int tile_flags_correct(tile_t* clicked);

#endif
