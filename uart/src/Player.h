#include <lcom/lcf.h>

#include <Sprites/player_background.xpm>
#include <Sprites/player_title.xpm>
#include <Sprites/name_length_warning.xpm>
#include <Sprites/font.xpm>

#include <Sprites/player_avatars/hammer_0_big.xpm>
#include <Sprites/player_avatars/hammer_1_big.xpm>
#include <Sprites/player_avatars/hammer_2_big.xpm>
#include <Sprites/player_avatars/hammer_3_big.xpm>
#include <Sprites/player_avatars/hammer_0_big_bright.xpm>
#include <Sprites/player_avatars/hammer_1_big_bright.xpm>
#include <Sprites/player_avatars/hammer_2_big_bright.xpm>
#include <Sprites/player_avatars/hammer_3_big_bright.xpm>
#include <Sprites/player_avatars/hammer_0_small.xpm>
#include <Sprites/player_avatars/hammer_1_small.xpm>
#include <Sprites/player_avatars/hammer_2_small.xpm>
#include <Sprites/player_avatars/hammer_3_small.xpm>

#include <Sprites/Buttons_img/normal/name_box_normal.xpm>
#include <Sprites/Buttons_img/active/name_box_active.xpm>
#include <Sprites/Buttons_img/normal/arrow_left_normal.xpm>
#include <Sprites/Buttons_img/active/arrow_left_active.xpm>
#include <Sprites/Buttons_img/normal/arrow_right_normal.xpm>
#include <Sprites/Buttons_img/active/arrow_right_active.xpm>
#include <Sprites/Buttons_img/normal/start_normal.xpm>
#include <Sprites/Buttons_img/active/start_active.xpm>

#include "vd_card.h"
#include "button.h"

typedef enum {NOT_SELECTED, SELECTED} button_state;

typedef struct  {
    xpm_image_t avatar;
    int x,y;
    int missed_moles;
    int hitted_moles;
    char name[7];
    int name_max_size;
} Player;

typedef struct  {
    xpm_image_t normal;
    xpm_image_t selected;
    xpm_image_t small;
    xpm_image_t atual;
    button_state state;
} Avatar;

typedef struct  {
    xpm_image_t background;
    xpm_image_t background_title;
    xpm_image_t name_length_warning;
    bool name_maximum_length;
    Button* name_box;
    xpm_image_t font;
    Player* player;
    Avatar* avatars[4];
    Button** buttons;
    int num_buttons;
} Player_Settings;


Player* load_player(Avatar* default_avatar);


Avatar* load_avatar(xpm_row_t *normal, xpm_row_t *selected, xpm_row_t *small);


Player_Settings *load_player_settings();


void draw_player(Player *player);


void draw_background__(Player_Settings *player_settings);


void draw_name_lenght_warning(Player_Settings* player_settings);


void draw_avatars(Avatar* avatars[]);


//void draw_menu_player_settings(Player_Settings* player_settings);


void move_left_avatar(Player_Settings* player_settings);


void move_right_avatar(Player_Settings* player_settings);


xpm_image_t get_hammer(Player_Settings* player_settings);


void draw_player_name(xpm_image_t font, int xi, int yi, char name[], int name_size);


void update_player_name(Player_Settings* player_settings, bool delete_letter, char new_letter);
