#include <lcom/lcf.h>
#include "menu.h"



Menu *load_menu()
{
  Menu *menu = malloc(sizeof(Menu));

  xpm_load(logo_xpm, XPM_8_8_8_8, &(menu->sprites[0]));
  menu->num_buttons = 7;
  menu->buttons = (Button**) malloc(sizeof(Button*) * menu->num_buttons);
  menu->buttons[0] = load_button(SINGLE_PLR_X, SINGLE_PLR_Y, singleplayer_normal_xpm, singleplayer_active_xpm);
  menu->buttons[1] = load_button(MULTI_PLR_X, MULTI_PLR_Y, multiplayer_normal_xpm, multiplayer_active_xpm);
  menu->buttons[2] = load_button(LEADERB_MENU_X, LEADERB_MENU_Y, leaderboard_normal_xpm, leaderboard_active_xpm);
  menu->buttons[3] = load_button(INSTRC_X, INSTRC_Y, instructions_normal_xpm, instructions_active_xpm);
  menu->buttons[4] = load_button(EXIT_MENU_X, EXIT_MENU_Y, exit_normal_xpm, exit_active_xpm);
  menu->buttons[5] = load_button(CALLENDAR_X, CALLENDAR_Y, calendar_normal_xpm, calendar_active_xpm);
  menu->buttons[6] = load_button(CLOCK_X, CLOCK_Y, clock_normal_xpm, clock_active_xpm);
  return menu;
}

void draw_logo(Menu *menu)
{
  uint32_t *logo_map = (uint32_t *)menu->sprites[0].bytes;
  vg_draw_xpm(logo_map, menu->sprites[0], MENU_LOGO_X, MENU_LOGO_Y);
}

void draw_date(xpm_image_t font, int xi, int yi, Date date)
{
    int number_width = font.width / 13; // 10 numbers plus slash, two points and percentage symbol
    int number_height = font.height;

    int left_day_number, right_day_number;
    left_day_number = date.day / 10;
    right_day_number = date.day % 10;

    int left_month_number, right_month_number;
    left_month_number = date.month / 10;
    right_month_number = date.month % 10;

    int left_year_number, right_year_number;
    left_year_number = date.year / 10;
    right_year_number = date.year % 10;
    
    //DAY 
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi, yi, left_day_number * number_width, (left_day_number + 1) * number_width, 0, number_height);
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+number_width, yi, right_day_number * number_width, (right_day_number + 1) * number_width, 0, number_height);
    //SLASH BAR
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+2*number_width, yi, 11 * number_width, 12 * number_width, 0, number_height);
    
    //MONTH
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+3*number_width, yi, left_month_number * number_width, (left_month_number + 1) * number_width, 0, number_height);
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+4*number_width, yi, right_month_number * number_width, (right_month_number + 1) * number_width, 0, number_height);
    //SLASH BAR
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+5*number_width, yi, 11 * number_width, 12 * number_width, 0, number_height);
    
    //YEAR 
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+6*number_width, yi, left_year_number * number_width, (left_year_number + 1) * number_width, 0, number_height);
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+7*number_width, yi, right_year_number * number_width, (right_year_number + 1) * number_width, 0, number_height);
}

void draw_time(xpm_image_t font, int xi, int yi, Time time)
{
    int number_width = font.width / 13; // 10 numbers plus slash, two points and percentage symbol
    int number_height = font.height;

    int left_hour_number, right_hour_number;
    left_hour_number = time.hour / 10;
    right_hour_number = time.hour % 10;

    int left_minutes_number, right_minutes_number;
    left_minutes_number = time.minute / 10;
    right_minutes_number = time.minute % 10;

    int left_seconds_number, right_seconds_number;
    left_seconds_number = time.second / 10;
    right_seconds_number = time.second % 10;
    
    //HOUR 
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi, yi, left_hour_number * number_width, (left_hour_number + 1) * number_width, 0, number_height);
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+number_width, yi, right_hour_number * number_width, (right_hour_number + 1) * number_width, 0, number_height);
    //TWO POINTS
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+2*number_width, yi, 10 * number_width, 11 * number_width, 0, number_height);
    
    //MINUTES
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+3*number_width, yi, left_minutes_number * number_width, (left_minutes_number + 1) * number_width, 0, number_height);
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+4*number_width, yi, right_minutes_number * number_width, (right_minutes_number + 1) * number_width, 0, number_height);
    //TWO POINTS
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+5*number_width, yi, 10 * number_width, 11 * number_width, 0, number_height);
    
    //SECONDS
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+6*number_width, yi, left_seconds_number * number_width, (left_seconds_number + 1) * number_width, 0, number_height);
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi+7*number_width, yi, right_seconds_number * number_width, (right_seconds_number + 1) * number_width, 0, number_height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Button *load_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *bright)
{
  Button *button = (Button *) malloc(sizeof(Button));

  if (button == NULL)
    return NULL;

  xpm_load(normal, XPM_8_8_8_8, &(button->sprites[0]));
  xpm_load(bright, XPM_8_8_8_8, &(button->sprites[1]));
  button->xi = xi;
  button->yi = yi;
  button->xf = xi + button->sprites[0].width;
  button->yf = yi + button->sprites[0].height;
  button->state = NORMAL;
  return button;
}
void update_buttons(Cursor* cursor, Button** buttons, int num_buttons) {
  for (int i = 0; i < num_buttons; i++) {
    if (mouse_over(buttons[i], cursor))
      buttons[i]->state = ACTIVE;
    else
      buttons[i]->state = NORMAL;
  }
}
void draw_button(Button *button)
{
  xpm_image_t current_img = button->sprites[(int) button->state];
  vg_draw_xpm((uint32_t*) current_img.bytes, current_img, button->xi, button->yi);
}
void draw_buttons(Button** buttons, int num_buttons)
{
  for (int i = 0; i < num_buttons; i++) {
    draw_button(buttons[i]);
  }
}
int mouse_over(Button *button, Cursor *cursor)
{
  if (button->xi <= cursor->x && cursor->x <= button->xf && button->yi <= cursor->y && cursor->y <= button->yf)
    return 1;
  else
    return 0;
}
////////////////////////////////////////////////////////////////
Cursor *load_cursor(xpm_row_t *img_cursor)
{
  Cursor *cursor = malloc(sizeof(Cursor));

  if (cursor == NULL)
    return NULL;

  xpm_load(cursor_xpm, XPM_8_8_8_8, &(cursor->cursor_image_default));
  xpm_load(cursor_xpm, XPM_8_8_8_8, &(cursor->cursor_image));
  cursor->x = 400;
  cursor->y = 300;
  return cursor;
}


void draw_cursor(Cursor *cursor)
{
  uint32_t *cursor_map = (uint32_t *)cursor->cursor_image.bytes;
  vg_draw_xpm(cursor_map, cursor->cursor_image, cursor->x, cursor->y);
}


void move_cursor(struct packet *packet, Cursor *cursor) {
  if (cursor->x + packet->delta_x < 0)
    cursor->x = 0;
  else {
    cursor->x += packet->delta_x;
    if (cursor->x > 800)
      cursor->x = 795;
  }

  if (cursor->y - packet->delta_y < 0)
    cursor->y = 0;
  else {
    cursor->y -= packet->delta_y;
    if (cursor->y > 600)
      cursor->y = 595;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////
