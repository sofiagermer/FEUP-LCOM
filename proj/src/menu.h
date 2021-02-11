#pragma once
#include <lcom/lcf.h>
#include <Sprites/logo.xpm>
#include <Sprites/Buttons_img/normal/singleplayer_normal.xpm>
#include <Sprites/Buttons_img/active/singleplayer_active.xpm>
#include <Sprites/Buttons_img/normal/multiplayer_normal.xpm>
#include <Sprites/Buttons_img/active/multiplayer_active.xpm>
#include <Sprites/Buttons_img/normal/leaderboard_normal.xpm>
#include <Sprites/Buttons_img/active/leaderboard_active.xpm>
#include <Sprites/Buttons_img/normal/instructions_normal.xpm>
#include <Sprites/Buttons_img/active/instructions_active.xpm>
#include <Sprites/Buttons_img/normal/exit_normal.xpm>
#include <Sprites/Buttons_img/active/exit_active.xpm>
#include <Sprites/Buttons_img/normal/calendar_normal.xpm>
#include <Sprites/Buttons_img/active/calendar_active.xpm>
#include <Sprites/Buttons_img/normal/clock_normal.xpm>
#include <Sprites/Buttons_img/active/clock_active.xpm>

/////////////////////////////////////////////////////////////////////////////
#include <Sprites/Buttons_img/normal/main_menu_normal.xpm>
#include <Sprites/Buttons_img/active/main_menu_active.xpm>
#include <Sprites/cursor.xpm>

#include "xpm_coordinates.h"
#include "rtc.h"
#include <vd_card.h>
#include <mouse.h>

typedef enum {NORMAL=0, ACTIVE} Button_state;
typedef struct {
    xpm_image_t sprites[2];
    Button_state state;
    uint16_t xi, xf, yi, yf;
} Button;

typedef struct{
    xpm_image_t cursor_image;
    xpm_image_t cursor_image_default;
    uint16_t x;
    uint16_t y;
} Cursor;
Button *load_button(uint16_t xi, uint16_t yi, xpm_row_t *normal, xpm_row_t *active);
void update_buttons(Cursor* cursor, Button** buttons, int num_buttons);
void draw_button(Button *button);
void draw_buttons(Button** buttons, int num_buttons);
int mouse_over(Button *button, Cursor *cursor);

Cursor *load_cursor(xpm_row_t *img_cursor);
void draw_cursor(Cursor *cursor);
void move_cursor(struct packet *packet, Cursor *cursor);
/////////////////////////////////////////////////////////////////////////////////

/**
 * @struct Menu
 * @var Menu:: sprites[2]
 * Menu's sprites xpm
 * @var Menu:: buttons
 * Menu's buttons
 * @var Menu::num_buttons
 * Menu's number of buttons
 * */
typedef struct {
    xpm_image_t sprites[1];
    Button** buttons;
    int num_buttons;     
} Menu;


void load_background();

/**
 * @brief Function that creates a menu: loads the background xpm, the WhackaMole's logo and all menu's buttons
 * @param xmp_row_t *img_cursor
 * @return Cursor *cursor
 * */
Menu *load_menu();

/**
 * @brief Function that draws the game's logo
 * @param Menu * menu
 * */
void draw_logo(Menu *menu);

/**
 * @brief Function that draws the game's date
 * @param xpm_image_t font
 * @param int xi
 * @param int yi
 * @param Date date 
 * */
void draw_date(xpm_image_t font, int xi, int yi, Date date);

/**
 * @brief Function that draws the current's time
 * @param xpm_image_t font
 * @param int xi
 * @param int yi
 * @param Time time 
 * */
void draw_time(xpm_image_t font, int xi, int yi, Time time);



