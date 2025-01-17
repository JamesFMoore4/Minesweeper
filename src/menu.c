#include "menu.h"

static int menu_update(button_t*);
static void menu_draw(button_t*);
static void menu_buttons_init(button_t*);
static int ret8(void);
static int ret16(void);
static int ret32(void);

int menu_loop(void)
{
  int size;
  button_t* buttons;

  buttons = (button_t*)Calloc(NUM_SIZE_CHOICES, sizeof(button_t));
  menu_buttons_init(buttons);
  size = 0;

  while (!size)
  {
    if (WindowShouldClose()) break;
    size = menu_update(buttons);
    menu_draw(buttons);
  }

  button_highlight(NULL, 1);

  free_buttons(buttons, NUM_SIZE_CHOICES);
  return size;
}

void menu_resize(button_t* buttons)
{
  static int prev_width = DEFAULT_WINDOW_WIDTH;
  static int prev_height = DEFAULT_WINDOW_HEIGHT;
  int i, base, scr_width, scr_height;
  int posx, posy, width, height;

  scr_width = GetScreenWidth();
  scr_height = GetScreenHeight();

  if (prev_width == scr_width && prev_height == scr_height)
    return;
  
  width = 0.25f * scr_width;
  height = 0.75f * scr_height;
  posx = (0.33f * scr_width) - width;
  posy = 0.125f * scr_height;

  for (i = 0; i < NUM_SIZE_CHOICES; i++)
  {
    buttons[i].width = width;
    buttons[i].height = height;
    buttons[i].posx = posx;
    buttons[i].posy = posy;
    posx += width + 25;
  }
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
  int i, size, mposx, mposy;

  size = 0;
    
  mposx = GetMouseX();
  mposy = GetMouseY();
  
  for (i = 0; i < NUM_SIZE_CHOICES; i++)
  {
    if (button_collision(&buttons[i], mposx, mposy))
    {
      button_highlight(&buttons[i], 0);
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        size = buttons[i].on_click();
      break;
    }
  }

  menu_resize(buttons);
    
  return size;
}

static void menu_draw(button_t* buttons)
{
  int i;
  
  BeginDrawing();
  ClearBackground((Color){75, 75, 75, 255});
  
  for (i = 0; i < NUM_SIZE_CHOICES; i++)
    button_draw(&buttons[i]);

  EndDrawing();
}

static void menu_buttons_init(button_t* buttons)
{
  int i, base, scr_width, scr_height;
  int posx, posy, width, height;
  button_t* temp;

  scr_width = GetScreenWidth();
  scr_height = GetScreenHeight();
  width = 0.25f * scr_width;
  height = 0.75f * scr_height;
  posx = (0.33f * scr_width) - width;
  posy = 0.125f * scr_height;
  base = 8;

  for (i = 0; i < NUM_SIZE_CHOICES; i++)
  {
    buttons[i].color = GRAY;
    buttons[i].outline = BLACK;
    buttons[i].text_color = BLACK;
    buttons[i].width = width;
    buttons[i].height = height;
    buttons[i].posx = posx;
    buttons[i].posy = posy;
    buttons[i].text = (char*)Calloc(8, sizeof(char));
    buttons[i].tposx = buttons[i].posx + (0.40f *
					  buttons[i].width);
    buttons[i].tposy = buttons[i].posy + (0.40f *
					  buttons[i].height);
    sprintf(buttons[i].text, "%dx%d", base, base);
    base *= 2;
    posx += width + 25;
  }

  buttons[0].on_click = ret8;
  buttons[1].on_click = ret16;
  buttons[2].on_click = ret32;
}

static int ret8(void)
{
  return 8;
}

static int ret16(void)
{
  return 16;
}

static int ret32(void)
{
  return 32;
}
