#include <lcom/lcf.h>
#include "cursor.h"
#include "vd_card.h"
#include <Sprites/Buttons_img/normal/main_menu_normal.xpm>
#include <Sprites/Buttons_img/active/main_menu_active.xpm>
#include <Sprites/Buttons_img/normal/score_close_normal.xpm>
#include <Sprites/Buttons_img/active/score_close_active.xpm>




typedef enum {NORMAL, ACTIVE} Button_state;

typedef struct {
    xpm_image_t img[2];
    xpm_image_t atual_img;
    Button_state state;
    uint16_t xi, xf, yi, yf;
} Button;


Button *load_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *bright);


void update_buttons(Cursor* cursor, Button** buttons, int num_buttons);


void draw_button(Button *button);


void draw_buttons(Button** buttons, int num_buttons);


int mouse_over(Button *button, Cursor *cursor);
