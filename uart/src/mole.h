#include <lcom/lcf.h>

#include <Sprites/hole.xpm>
#include <Sprites/mole_up1.xpm>
#include <Sprites/mole_up2.xpm>
#include <Sprites/mole_up3.xpm>
#include <Sprites/mole_up4.xpm>

#include <Sprites/mole_down_hit1.xpm>
#include <Sprites/mole_down_hit2.xpm>
#include <Sprites/mole_down_hit3.xpm>
#include <Sprites/mole_down_hit4.xpm>

#include "vd_card.h"

#define MOLE_HEIGHT  150
#define MOLE_WIDHT   175
#define STEP_FROM_X  180
#define STEP_FROM_Y  180
#define GAME_FPS     12
#define MOLE_UP_TIME GAME_FPS*3
#define GAME_DURATION GAME_FPS*30
#define MOLE_PROBABILITY 9950
#define TIME_UP_LIMIT_DECREMENT 1
#define PROBABILITY_LIMIT_DECREMENT  50

#define KBD_KEY_0 'A'
#define KBD_KEY_1 'S'
#define KBD_KEY_2 'D'
#define KBD_KEY_3 'J'
#define KBD_KEY_4 'K'
#define KBD_KEY_5 'L'

typedef enum {HIDED, UP_1, UP_2, UP_3, UP_4, DOWN_MISSED_4, DOWN_MISSED_3, DOWN_MISSED_2, DOWN_MISSED_1, DOWN_HIT_4, DOWN_HIT_3, DOWN_HIT_2, DOWN_HIT_1} Position;

typedef struct  {
    xpm_image_t atual_img;
    xpm_image_t hided;
    xpm_image_t up_or_down_missed[4];
    xpm_image_t down_hit[4];
    
    int index;
    char kbd_key;
    int x,y;
    int time_up;
    Position position;
} Mole;

Mole* createMole();

void draw_mole(Mole* mole);
