#include "menu.h"

static int menu_update(button_t*);
static void menu_draw(button_t*);
static void menu_buttons_init(button_t*);
static void* ret8(void*);
static void* ret16(void*);
static void* ret32(void*);

int menu_loop(void)
{
  int size;
  button_t* buttons;

  buttons = calloc(3, sizeof(button_t));
  menu_buttons_init(buttons);
  size = 0;

  while (!size)
  {
    size = menu_update(buttons);
    menu_draw(buttons);
  }

  free_buttons(buttons, 3);
  return size;
}

void free_buttons(button_t* buttons, int num)
{
  int i;

  for (i = 0; i < num; i++)
    free(buttons[i].text);
  free(buttons);
}

static int menu_update(button_t* buttons)
{
  return 16;
}

static void menu_draw(button_t* buttons)
{
  
}

static void menu_buttons_init(button_t* buttons)
{
  int i, base;

  base = 8;

  for (i = 0; i < 3; i++)
  {
    buttons[i].color = GRAY;
    buttons[i].text_color = BLACK;
    buttons[i].width = 0.25f * GetScreenWidth();
    buttons[i].height = 0.75f * GetScreenHeight();
    buttons[i].text = (char*)malloc(6);
    sprintf(buttons[i].text, "%dx%d", base, base);
    base *= 2;
  }

  buttons[0].on_click = ret8;
  buttons[0].posx = 5;
  buttons[0].posy = 5;
  
  buttons[1].on_click = ret16;
  buttons[2].on_click = ret32;
}

static void* ret8(void*)
{
  return (void*) 8;
}

static void* ret16(void*)
{
  return (void*) 16;
}

static void* ret32(void*)
{
  return (void*) 32;
}
