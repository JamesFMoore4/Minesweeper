#ifndef BUTTON
#define BUTTON

#include <stddef.h>

#include "raylib.h"

typedef struct
{
  Color color;
  Color outline;
  int posx;
  int posy;
  int width;
  int height;
  Color text_color;
  char* text;
  int tposx;
  int tposy;
  int (*on_click)(void); // Modify later if necessary
} button_t;

void button_highlight(button_t*, int);
int button_collision(button_t*, int, int);
void button_draw(button_t*);

#endif
