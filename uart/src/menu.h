#include <lcom/lcf.h>
#include <Sprites/background.xpm>
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

#include "vd_card.h"
#include "button.h"

typedef struct {
    xpm_image_t logo;         
    Button** buttons;
    int num_buttons;          
} Menu;

void load_background();


Menu *load_menu();


void draw_background();


void draw_logo(Menu *menu);


//void draw_menu(Menu *menu);
