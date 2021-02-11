#pragma once
#include <lcom/lcf.h>

#include <Sprites/leaderboard_crown.xpm>
#include <Sprites/leaderboard_table.xpm>

#include <Sprites/score_numbers.xpm>
#include <Sprites/Buttons_img/normal/score_close_normal.xpm>
#include <Sprites/Buttons_img/active/score_close_active.xpm>

#include "rtc.h"
#include "player_settings.h"


/**
 * @struct Score_Record
 * @var Score_Record:: player_name
 * Player's name
 * @var Score_Record:: player_name_size
 * Player name's size
 * @var Score_Record:: date
 * Game date
 * @var Score_Record:: score
 * Player's score
 * */
typedef struct {
    char* player_name;
    int player_name_size;
    Date date;
    int score;
} Score_Record;

/**
 * @struct Leaderboard

 * @var Leaderboard:: crown
 * Leaderboard's crown xpm
 * @var Leaderboard:: table
 * Leaderboard's crown xpm
 * @var Leaderboard:: numbers
 * Leaderboard's numbers xpm
 * @var Leaderboard:: buttons
 * Leaderboard's buttons
 * @var Leaderboard:: num_buttons
 * Number of Buttons of LeaderBoard
 * @var Leaderboard:: score_records
 * Leaderboard's Score_Record
 * @var Leaderboard:: num_score_records
 * Leaderboard's num of score records 
 * @var Leaderboard:: max_name_length
 * Leaderboard's max name lenght
 * */
typedef struct  {
    xpm_image_t crown;
    xpm_image_t table;
    xpm_image_t numbers;
    Button** buttons;
    int num_buttons;
    Score_Record* score_records;
    int num_score_records;
    int max_name_length;
} Leaderboard;

/**
 * @brief: loads leaderboard and sets it's variables
 * @return: leaderboard
 * */
Leaderboard* load_leaderboard();

/**
 *  @brief draws leaderboard's table
 *  @param leaderboard
 * */
void draw_leaderboard_table(Leaderboard* leaderboard);

/**
 *  @brief draws leaderboard's player names
 *  @param font font that will be used to draw the names
 *  @param score_records
 *  @param num_records  
 * */
void draw_player_names(xpm_image_t font, Score_Record* score_records, uint8_t num_records);

/**
 *  @brief draws players's scores
 *  @param leaderboard
 * */
void draw_player_scores(Leaderboard* leaderboard);

/**
 *  @brief draws players's dates
 *  @param leaderboard
 * */
void draw_player_dates(Leaderboard *leaderboard);

/**
 *  @brief draws players's score
 *  @param font font that will be used to draw the score
 *  @param xi 
 *  @param yi
 *  @param score
 * 
 * */
void draw_player_score(xpm_image_t font, int xi, int yi, int score);

/**
 * @brief: Function that saves scores information on txt file
 * @param leaderboard
 * */
void save_scores(Leaderboard* leaderboard);

/**
 * @brief: Function that reads scores information of txt file
 * @param leaderboard
 * */
void load_scores(Leaderboard* leaderboard);

/**
 * @brief: Function that adds new score to stuct if the score is better than the last placed scoree
 * @param leaderboard
 * @param player
 * */
bool add_new_score(Leaderboard* leaderboard, Player* player);
