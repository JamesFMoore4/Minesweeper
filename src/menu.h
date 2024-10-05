#ifndef MENU
#define MENU

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "button.h"
#include "gamedef.h"

int menu_loop(void);
void menu_resize(button_t*);
void free_buttons(button_t*, int);

#endif
