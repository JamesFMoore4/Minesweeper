#include "button.h"

void button_highlight(button_t* selected)
{
  static button_t* hl_button = NULL;
  static Color prev_color = (Color){0};
  Color temp;

  if (hl_button)
    hl_button->color = prev_color;

  if (!selected)
  {
    hl_button = NULL;
    prev_color = (Color){0};
  }
  else
  {
    prev_color = selected->color;
    hl_button = selected;
    hl_button->color = (Color){prev_color.r+20, prev_color.g+20, prev_color.b+20, 255};
  }
}

int button_collision(button_t* selected, int mposx, int mposy)
{
  int collision;

  collision = mposx > selected->posx;
  collision = collision && (mposx < selected->posx + selected->width);
  collision = collision &&  (mposy > selected->posy);
  collision = collision && (mposy < selected->posy + selected->height);

  return collision;
}

void button_draw(button_t* button)
{
  DrawRectangle(button->posx, button->posy,
		button->width, button->height,
		button->color);
  DrawRectangleLines(button->posx, button->posy,
		     button->width, button->height,
		     button->outline);
  DrawText(button->text, button->posx +
	   (0.45f * button->width), button->posy +
	   (0.45f * button->height), 32,
	   button->text_color);
}
