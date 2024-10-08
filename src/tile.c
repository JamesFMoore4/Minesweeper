#include "tile.h"

void tile_set_num_mines(tile_t* selected)
{
  int i;
  bitvec_t num_mines;

  if (tile_is_mined(selected)) return;

  for (i = 0, num_mines = 0; i < 8; i++)
    if (tile_is_mined(selected->neighbors[i]))
      num_mines++;

  selected->info |= num_mines;
}

void tile_set_unknown(tile_t* selected, int set)
{
  if (set)
    selected->info |= BIT_UNKNOWN;
  else
    selected->info &= ~BIT_UNKNOWN;
}

void tile_set_mined(tile_t* selected)
{
  selected->info |= BIT_MINE; 
}

void tile_set_flagged(tile_t* selected, int set)
{
  if (set)
    selected->info |= BIT_FLAG;
  else
    selected->info &= ~BIT_FLAG;
}

void tile_set_qflagged(tile_t* selected, int set)
{
  if (set)
  {
    selected->info |= BIT_QFLAG;
    tile_set_flagged(selected, 0);
  }
  else
    selected->info &= ~BIT_QFLAG;
}

void tile_set_multiple(tile_t* selected, int set, bitvec_t flags)
{
  if (set)
    selected->info |= flags;
  else
    selected->info &= ~flags;
}

int tile_is_unknown(tile_t* selected)
{
  return selected->info & BIT_UNKNOWN;
}

int tile_is_mined(tile_t* selected)
{
  return selected->info & BIT_MINE;
}

int tile_is_flagged(tile_t* selected)
{
  return selected->info & BIT_FLAG;
}

int tile_is_qflagged(tile_t* selected)
{
  return selected->info & BIT_QFLAG;
}

void tile_highlight(tile_t* selected, int reset)
{
  static tile_t* hl_tile = NULL;
  tile_t* last;
  Color temp;

  if (reset)
  {
    hl_tile = NULL;
    return;
  }

  if (hl_tile && tile_is_unknown(hl_tile))
    hl_tile->color = GRAY;

  if (!selected || !tile_is_unknown(selected))
  {
    hl_tile = NULL;
  }
  else
  {
    temp = selected->color;
    hl_tile = selected;
    hl_tile->color = (Color){temp.r+20,temp.g+20,temp.b+20,255};
  }
}

int tile_get_num_mines(tile_t* selected)
{
  return (int) (selected->info & 0xF);
}

int tile_get_num_flags(tile_t* selected)
{
  int i, num_flags;

  for (i = 0, num_flags = 0; i < 8; i++)
    if (tile_is_flagged(selected->neighbors[i]))
      num_flags++;

  return num_flags;
}

int tile_flags_correct(tile_t* selected)
{
  int i;

  if (tile_get_num_mines(selected) != tile_get_num_flags(selected))
    return -1;

  for (i = 0; i < 8; i++)
    if (tile_is_mined(selected->neighbors[i]) && !tile_is_flagged(selected->neighbors[i]))
      return 0;

  return 1;
}

int tile_collision(tile_t* selected, int mposx, int mposy)
{
  int collision;

  collision = mposx > selected->posx;
  collision = collision && (mposx < selected->posx + selected->width);
  collision = collision &&  (mposy > selected->posy);
  collision = collision && (mposy < selected->posy + selected->height);

  return collision;
}
