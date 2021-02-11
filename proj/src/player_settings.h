#pragma once
#include <lcom/lcf.h>

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

#include "menu.h"

typedef enum {NOT_SELECTED, SELECTED} button_state;

/**
 * @struct Player
 * @var Player:: avatar
 * Player's avatar
 * @var Player:: missed_moles
 * How many moles the Player has missed
 * @var Player:: hitted_moles
 * How many moles the Player has hitted
 * @var Player:: name
 * Player's name
 * @var Player:: max_name_length
 * Player's name max size
 * */
typedef struct  {
    xpm_image_t avatar;
    int missed_moles;
    int hitted_moles;
    char* name;
    int max_name_length;
} Player;

/**
 * @struct Avatar
 * @var: sprites[3]
 * @var: state
 * @var:x
 * @var:y
 * */
typedef struct  {
    xpm_image_t sprites[3];
    button_state state;
    uint8_t x,y;
} Avatar;

/**
 * @struct Player_Settings
 * @var Player_Settins:: background
 * Player Setting's backgound xpm
 * @var Player_Settins:: background_title
 * Player Setting's backgound title xpm
 * @var Player_Settins:: name_length_warning
 * Player Setting's name has reached maximum length warning xpm
 * @var Player_Settins:: font
 * Player Setting's font xpm
 * @var Player_Settins:: name_maximum_length
 * Boolean: false if not reached maximum lenght; else true
 * @var Player_Settins:: avatars[4]
 * Player Setting's avatars
 * @var Player_Settins:: buttons
 * Player Setting's buttons
 * @var Player_Settins:: num_buttons
 * Player Setting's number of buttonfs
 * */
typedef struct  {
    xpm_image_t background;
    xpm_image_t background_title;
    xpm_image_t name_length_warning;
    xpm_image_t font;
    bool name_maximum_length;
    //Player* player;
    Avatar* avatars[4];
    Button** buttons;
    int num_buttons;
} Player_Settings;

/**
 * @brief: loads Player's xpm, sets hitted and missed moles both as null, set's PLayers initial default name and it's name's maximum lenght
 * @param:default_avatar
 * @return: player
 * */
Player* load_player(Avatar* default_avatar);

/**
 * @brief: loads Avatars's xpm, sets avatars initial state
 * @param:default_avatar
 * @return: player
 * */
Avatar* load_avatar(xpm_row_t *normal, xpm_row_t *selected, xpm_row_t *small);

/**
 * @brief loads Player Settings : loads it's xpm and sets it's variavles
 * @return: player_settings
 * */
Player_Settings *load_player_settings();

/**
 * @brief: draws Player Settings mode background
 * @param player_sets
 * */
void draw_background__(Player_Settings *player_sets);

/**
 * @brief: draws Player Settings name lennght waring
 * @param player_sets
 * */
void draw_name_lenght_warning(Player_Settings* player_sets);

/**
 * @brief: draws Player Settings avatar's according to their state
 * @param player_sets
 * */
void draw_avatars(Avatar* avatars[]);

/**
 * @brief: moves one avatar to left, defining it as SELECTED and the other ones as NOT_SELECTED
 * @param player_sets
 * */
void move_left_avatar(Player_Settings* player_sets);

/**
 * @brief: moves one avatar to right, defining it as SELECTED and the other ones as NOT_SELECTED
 * @param player_sets
 * */
void move_right_avatar(Player_Settings* player_sets);

/**
 * @brief: get the hammer sellected by the player 
 * @param player_sets
 * @return choosen hammer xpm
 * */

xpm_image_t get_hammer(Player_Settings* player_sets);


/**
 * @brief: draws Player Nmae
 * @param font xpm of letter to bue used to right the name
 * @param xi
 * @param yi
 * @param name
 * @param name_size
 * */

void draw_player_name(xpm_image_t font, int xi, int yi, char name[], int name_size);

/**
 * @brief:
 * @param player_settings
 * @param player 
 * @param delete_letter Bool that is true if the Player wants to delete a lette of his name
 * @param new_letter letter to be added to Player's name
 * */

void update_player_name(Player_Settings* player_settings, Player *player, bool delete_letter, char new_letter);
