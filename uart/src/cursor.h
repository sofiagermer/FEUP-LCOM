#include <lcom/lcf.h>
#include <Sprites/cursor.xpm>
#include "vd_card.h"


typedef struct{
    xpm_image_t cursor_image;
    uint16_t x;
    uint16_t y;
} Cursor;


Cursor *load_cursor(xpm_row_t *img_cursor);


void draw_cursor(Cursor *cursor);


void move_cursor(struct packet *packet, Cursor *cursor);
