#include <lcom/lcf.h>

#include <Sprites/leaderboard_background.xpm>
#include <Sprites/leaderboard_crown.xpm>
#include <Sprites/leaderboard_table.xpm>
#include <Sprites/font_small.xpm>
#include <Sprites/score_numbers.xpm>

#include "vd_card.h"
#include "rtc.h"
#include "button.h"

#define NAME_STEP_FROM_X 342
#define NAME_STEP_FROM_Y 151
#define NAME_STEP_FROM_LINE 88

#define SCORE_STEP_FROM_X 550
#define SCORE_STEP_FROM_Y 151
#define SCORE_STEP_FROM_LINE 88

#define AVATAR_STEP_FROM_X 304
#define AVATAR_STEP_FROM_Y 140
#define AVATAR_STEP_FROM_LINE 74

#define CLOSE_STEP_FROM_X 580
#define CLOSE_STEP_FROM_Y 52

typedef struct  {
    char* player_name;
    int player_name_size;
    Date date;
    Time time;
    int accuracy;
} Score_Record;

typedef struct  {
    xpm_image_t background;
    xpm_image_t crown;
    xpm_image_t table;
    xpm_image_t font;
    xpm_image_t numbers;
    Button** buttons;
    int num_buttons;
    Score_Record* score_records;
    int max_name_length;
    int max_score_records;
} Leaderboard;


Leaderboard* load_leaderboard();

//Score_Record* load_score_record(Player* player, int score);

void draw_leaderboard(Leaderboard* leaderboard);

void draw_leaderboard_backgound(Leaderboard* leaderboard);

void draw_table__(Leaderboard* leaderboard);

void draw_player_names(Leaderboard* leaderboard);

void draw_player_scores(Leaderboard* leaderboard);

void draw_player_score(xpm_image_t font, int xi, int yi, int score);

void save_scores(Leaderboard* leaderboard);

void load_scores(Leaderboard* leaderboard);

//bool add_new_score(Leaderboard* leaderboard, Player* player);

void draw_player_name_ll(xpm_image_t font, int xi, int yi, char name[], int name_size);
