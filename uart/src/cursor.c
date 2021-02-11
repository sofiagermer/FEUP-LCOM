#include <lcom/lcf.h>
#include <stdint.h>
#include "cursor.h"


Cursor *load_cursor(xpm_row_t *img_cursor)
{
  Cursor *cursor = malloc(sizeof(Cursor));

  if (cursor == NULL)
    return NULL;

  xpm_image_t img;
  xpm_load(cursor_xpm, XPM_8_8_8_8, &img);
  cursor->cursor_image = img;
  cursor->x = 50;
  cursor->y = 50;
  return cursor;
}


void draw_cursor(Cursor *cursor)
{
  uint32_t *cursor_map = (uint32_t *)cursor->cursor_image.bytes;
  vg_draw_xpm(cursor_map, cursor->cursor_image, cursor->x, cursor->y);
}


void move_cursor(struct packet *packet, Cursor *cursor) {
  cursor->x += packet->delta_x;
  if (cursor->x > 790)
  {
    cursor->x = 790 - cursor->cursor_image.width;
  }
  else if (cursor->x < 0)
  {
    cursor->x = 0;
  }

  cursor->y -= packet->delta_y;
  if (cursor->y > 590)
  {
    cursor->y = 590 - cursor->cursor_image.height;
  }
  else if (cursor->y < 0)
  {
    cursor->y = 0;
  }
}
