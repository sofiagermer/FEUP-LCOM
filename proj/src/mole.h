#pragma once
#include <lcom/lcf.h>

#include <Sprites/hole.xpm>
#include <Sprites/mole_up1.xpm>
#include <Sprites/mole_up2.xpm>
#include <Sprites/mole_up3.xpm>
#include <Sprites/mole_up4.xpm>
#include <Sprites/mole_down_miss1.xpm>
#include <Sprites/mole_down_miss2.xpm>
#include <Sprites/mole_down_miss3.xpm>
#include <Sprites/mole_down_miss4.xpm>
#include <Sprites/mole_down_hit1.xpm>
#include <Sprites/mole_down_hit2.xpm>
#include <Sprites/mole_down_hit3.xpm>
#include <Sprites/mole_down_hit4.xpm>

#include "vd_card.h"

#define GAME_FPS     30
#define MOLE_UP_TIME GAME_FPS*3
#define GAME_DURATION GAME_FPS*30
#define MOLE_PROBABILITY 9800
#define MOLE_PROBABILITY_MULTIPLAYER  9700
#define TIME_UP_LIMIT_DECREMENT 1
#define PROBABILITY_LIMIT_DECREMENT  50
#define MAX_NO_MOLES  62

#define KBD_KEY_0 'A'
#define KBD_KEY_1 'S'
#define KBD_KEY_2 'D'
#define KBD_KEY_3 'J'
#define KBD_KEY_4 'K'
#define KBD_KEY_5 'L'

typedef enum {HIDED = 0, UP_1, UP_2, UP_3, UP_4, DOWN_MISSED_4, DOWN_MISSED_3, DOWN_MISSED_2, DOWN_MISSED_1, DOWN_HIT_4, DOWN_HIT_3, DOWN_HIT_2, DOWN_HIT_1} Position;
/**
 * @struct Mole
 * @var Mole:: sprites[13]
 * Mole's xpm for all it's possible positions
 * @var Mole::index
 * Mole index on the game
 * @var Mole::kbd_key 
 * Kbd key corresponding to that mole
 * @var Mole::x
 * Mole's position on x axis
 * @var Mole::y
 * Mole's position on y axis
 * @var Mole::time_up
 * Time that the Mole is up
 * @var Mole::position
 * Mole's current position
 * */
typedef struct  {
    xpm_image_t sprites[13];
    uint8_t index;
    char kbd_key;
    int x,y;
    int time_up;
    Position position;
} Mole;

/**
 * @brief Function that create a Mole: loads all mole sprites(for every different mole Position) and sets up all moles variables
 * */
Mole* createMole();

/**
 * @brief Function that draws the mole at it's current position
 * @param Mole* the mole to be drawn
 * */
void draw_mole(Mole* mole);

/**
 * @brief Function that draws some moles at it's current positions
 * @param Mole* the moles to be drawn
 * @param int the number of moles to be drawn
 * */
void draw_all_moles(Mole* moles, int num_moles);

/**
 * @brief checks if cursor its over mole
 * @param Mole* the mole to be checked
 * @param int cursor_x the x position of the cursor
  * @param int cursor_y the y position of the cursor
 * @return bool - true if the cursor is over the mole, false otherwise
 * */
bool check_over_mole(Mole *mole, int cursor_x, int cursor_y);


void reset_moles(Mole* moles, int num_moles);
