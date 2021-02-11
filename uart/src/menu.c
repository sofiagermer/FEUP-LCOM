#include "menu.h"
#include <lcom/lcf.h>

//background xpm
xpm_image_t background;

Menu *load_menu()
{
  Menu *menu = malloc(sizeof(Menu));

  xpm_image_t img;
  xpm_load(logo_xpm, XPM_8_8_8_8, &img);
  menu->logo = img;
  //ver coordenadas onde vou por os botões
  menu->num_buttons = 5;
  menu->buttons = (Button**) malloc(sizeof(Button*) * menu->num_buttons);
  menu->buttons[0] = load_button(192, 441, singleplayer_normal_xpm, singleplayer_active_xpm);
  menu->buttons[1] = load_button(192, 494, multiplayer_normal_xpm, multiplayer_active_xpm);
  menu->buttons[2] = load_button(192, 547, leaderboard_normal_xpm, leaderboard_active_xpm);
  menu->buttons[3] = load_button(430, 441, instructions_normal_xpm, instructions_active_xpm);
  menu->buttons[4] = load_button(430, 494, exit_normal_xpm, exit_active_xpm);

  return menu;
}

void load_background()
{
  xpm_load(background_xpm, XPM_8_8_8_8, &background);
}

void draw_background()
{
  uint32_t *background_map = (uint32_t *)background.bytes;
  vg_draw_xpm(background_map, background, 0, 0);
}

void draw_logo(Menu *menu)
{
  uint32_t *logo_map = (uint32_t *)menu->logo.bytes;
  vg_draw_xpm(logo_map, menu->logo, 168, 118);
}


/*void draw_menu(Menu *menu)
{
  draw_background();
  draw_logo(menu);
  draw_buttons(menu->buttons, menu->num_buttons);
}*/



