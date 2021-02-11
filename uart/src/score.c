#include <lcom/lcf.h>
#include <stdint.h>

#include "score.h"

Leaderboard *load_leaderboard()
{
    //Allocating memory
    Leaderboard *leaderboard = (Leaderboard *)malloc(sizeof(Leaderboard));

    xpm_image_t img;
    xpm_load(leaderboard_background_xpm, XPM_8_8_8_8, &img);
    leaderboard->background = img;
    xpm_load(leaderboard_crown_xpm, XPM_8_8_8_8, &img);
    leaderboard->crown = img;
    xpm_load(leaderboard_table_xpm, XPM_8_8_8_8, &img);
    leaderboard->table = img;
    xpm_load(font_small_xpm, XPM_8_8_8_8, &img);
    leaderboard->font = img;
    xpm_load(score_numbers_xpm, XPM_8_8_8_8, &img);
    leaderboard->numbers = img;

    leaderboard->max_name_length = 7;
    leaderboard->max_score_records = 5;

    leaderboard->score_records = (Score_Record *)malloc(sizeof(Score_Record) * leaderboard->max_score_records);
    leaderboard->score_records[0].player_name = (char *)malloc(leaderboard->max_name_length);
    leaderboard->score_records[1].player_name = (char *)malloc(leaderboard->max_name_length);
    leaderboard->score_records[2].player_name = (char *)malloc(leaderboard->max_name_length);
    leaderboard->score_records[3].player_name = (char *)malloc(leaderboard->max_name_length);
    leaderboard->score_records[4].player_name = (char *)malloc(leaderboard->max_name_length);
    load_scores(leaderboard);

    leaderboard->score_records[0].player_name[0] = 'P';
    leaderboard->score_records[0].player_name[1] = 'E';
    leaderboard->score_records[0].player_name[2] = 'D';
    leaderboard->score_records[0].player_name[3] = 'R';
    leaderboard->score_records[0].player_name[4] = 'O';
    leaderboard->score_records[0].player_name[5] = ' ';
    leaderboard->score_records[0].player_name[6] = ' ';
    leaderboard->score_records[0].player_name_size = 7;
    leaderboard->score_records[0].accuracy = 85;

    leaderboard->score_records[1].player_name[0] = 'P';
    leaderboard->score_records[1].player_name[1] = 'L';
    leaderboard->score_records[1].player_name[2] = 'A';
    leaderboard->score_records[1].player_name[3] = 'Y';
    leaderboard->score_records[1].player_name[4] = 'E';
    leaderboard->score_records[1].player_name[5] = 'R';
    leaderboard->score_records[1].player_name[6] = ' ';
    leaderboard->score_records[1].player_name_size = 7;
    leaderboard->score_records[1].accuracy = 76;

    leaderboard->score_records[2].player_name[0] = 'P';
    leaderboard->score_records[2].player_name[1] = 'E';
    leaderboard->score_records[2].player_name[2] = 'D';
    leaderboard->score_records[2].player_name[3] = 'R';
    leaderboard->score_records[2].player_name[4] = 'O';
    leaderboard->score_records[2].player_name[5] = ' ';
    leaderboard->score_records[2].player_name[6] = ' ';
    leaderboard->score_records[2].player_name_size = 7;
    leaderboard->score_records[2].accuracy = 43;

    leaderboard->score_records[3].player_name[0] = 'S';
    leaderboard->score_records[3].player_name[1] = 'O';
    leaderboard->score_records[3].player_name[2] = 'F';
    leaderboard->score_records[3].player_name[3] = 'I';
    leaderboard->score_records[3].player_name[4] = 'A';
    leaderboard->score_records[3].player_name[5] = ' ';
    leaderboard->score_records[3].player_name[6] = ' ';
    leaderboard->score_records[3].player_name_size = 7;
    leaderboard->score_records[3].accuracy = 42;

    leaderboard->score_records[4].player_name[0] = 'L';
    leaderboard->score_records[4].player_name[1] = 'U';
    leaderboard->score_records[4].player_name[2] = 'I';
    leaderboard->score_records[4].player_name[3] = 'S';
    leaderboard->score_records[4].player_name[4] = ' ';
    leaderboard->score_records[4].player_name[5] = ' ';
    leaderboard->score_records[4].player_name[6] = ' ';
    leaderboard->score_records[4].player_name_size = 7;
    leaderboard->score_records[4].accuracy = 40;

    leaderboard->num_buttons = 1;
    leaderboard->buttons = (Button **)malloc(sizeof(Button *) * leaderboard->num_buttons);
    leaderboard->buttons[0] = load_button(CLOSE_STEP_FROM_X, CLOSE_STEP_FROM_Y, score_close_normal_xpm, score_close_active_xpm);

    return leaderboard;
}

/*Score_Record* load_score_record(int index) {
    Score_Record *score_record = (Score_Record *)malloc(sizeof(Score_Record));
    score_record->player = player;
    score_record->accuracy = score;
}*/

/*void draw_leaderboard(Leaderboard *leaderboard)
{
    draw_leaderboard_backgound(leaderboard);
    draw_table__(leaderboard);
    draw_buttons(leaderboard->buttons, leaderboard->max_score_records);
    draw_player_names(leaderboard);
    draw_player_scores(leaderboard);
    draw_cursor(leaderboard->cursor);
}*/

void draw_leaderboard_backgound(Leaderboard *leaderboard)
{
    uint32_t *background_map = (uint32_t *)leaderboard->background.bytes;
    vg_draw_xpm(background_map, leaderboard->background, 0, 0);
}

void draw_table__(Leaderboard *leaderboard)
{
    uint32_t *crown_map = (uint32_t *)leaderboard->crown.bytes;
    vg_draw_xpm(crown_map, leaderboard->crown, 380, 15);
    uint32_t *table_map = (uint32_t *)leaderboard->table.bytes;
    vg_draw_xpm(table_map, leaderboard->table, 180, 50);
}

void draw_player_names(Leaderboard *leaderboard)
{
    for (int i = 0; i < leaderboard->max_score_records; i++)
    {
        Score_Record *curr_score_record = &leaderboard->score_records[i];
        if (curr_score_record->player_name_size != 0)
            draw_player_name_ll(leaderboard->font, NAME_STEP_FROM_X, i * NAME_STEP_FROM_LINE + NAME_STEP_FROM_Y, curr_score_record->player_name, curr_score_record->player_name_size);
    }
}

void draw_player_name_ll(xpm_image_t font, int xi, int yi, char name[], int name_size) {
  uint32_t *font_pixmap = (uint32_t *)font.bytes;
  int letter_width = font.width / (26/2); // 26 letters in two rows equals 13 letter per row
  int letter_height = font.height / 2; //two rows of letters
  printf("--- ");
  printf("%d", font.height);
  printf(" ---\n");
  int index = 0;
  for (int i = 0; i < name_size; i++) {
    if (name[i] == ' ')
      break;
    index = (int)name[i] - (int)'A';
    if (index < 13) {
        //first row
        vg_draw_part_of_xpm(font_pixmap, font, xi+i*letter_width, yi, index*letter_width, index*letter_width + letter_width, 0, letter_height);
    }
    else {
        //second row
        vg_draw_part_of_xpm(font_pixmap, font, xi+i*letter_width, yi, index*letter_width, index*letter_width + letter_width, letter_height, letter_height*2);
    }
  }
}

void draw_player_scores(Leaderboard *leaderboard)
{
    for (int i = 0; i < leaderboard->max_score_records; i++)
    {
        Score_Record *curr_score_record = &leaderboard->score_records[i];
        if (curr_score_record->player_name_size != 0)
            draw_player_score(leaderboard->numbers, SCORE_STEP_FROM_X, i * SCORE_STEP_FROM_LINE + SCORE_STEP_FROM_Y, curr_score_record->accuracy);
    }
}

void draw_player_score(xpm_image_t font, int xi, int yi, int score)
{
    uint32_t *font_pixmap = (uint32_t *)font.bytes;
    int number_width = font.width / 12; // 10 numbers plus slash and percentage symbol
    int number_height = font.height;

    int left_score_number, mid_score_number, right_score_number;

    right_score_number = (score % 10) + 2;
    mid_score_number = ((score / 10) % 10) + 2;
    left_score_number = (score / 100) + 2;

    vg_draw_part_of_xpm(font_pixmap, font, xi, yi, number_width, 2 * number_width, 0, number_height); //Percentage symbol
    vg_draw_part_of_xpm(font_pixmap, font, xi - number_width, yi, right_score_number * number_width, (right_score_number + 1) * number_width, 0, number_height);

    if ((mid_score_number - 2) == 0 && (left_score_number - 2) != 0)
    {
        vg_draw_part_of_xpm(font_pixmap, font, xi - 2 * number_width, yi, mid_score_number * number_width, (mid_score_number + 1) * number_width, 0, number_height);
        vg_draw_part_of_xpm(font_pixmap, font, xi - number_width * 3, yi, left_score_number * number_width, (left_score_number + 1) * number_width, 0, number_height);
    }
    else if ((mid_score_number - 2) != 0)
    {
        vg_draw_part_of_xpm(font_pixmap, font, xi - 2 * number_width, yi, mid_score_number * number_width, (mid_score_number + 1) * number_width, 0, number_height);
    }
}

void save_scores(Leaderboard *leaderboard)
{
    FILE *leaderboard_file;
    leaderboard_file = fopen("/home/lcom/labs/proj/src/leaderboard.txt", "w");

    if (leaderboard_file == NULL) //SEE LATER
        return;

    for (int i = 0; i < leaderboard->max_score_records; i++)
    {
        Score_Record *curr_score_record = &leaderboard->score_records[i];
        fprintf(leaderboard_file, "%s\n", curr_score_record->player_name);

        fprintf(leaderboard_file, "%d\n", curr_score_record->accuracy);

        fprintf(leaderboard_file, "%02d/%02d/%02d\n", curr_score_record->date.year, curr_score_record->date.month, curr_score_record->date.day);
        fprintf(leaderboard_file, "%02d:%02d:%02d\n", curr_score_record->time.hour, curr_score_record->time.minute, curr_score_record->time.second);
    }

    fclose(leaderboard_file);
}

void load_scores(Leaderboard *leaderboard)
{
    FILE *leaderboard_file;
    leaderboard_file = fopen("/home/lcom/labs/proj/src/leaderboard.txt", "r");

    if (leaderboard_file == NULL)
    {
        for (int i = 0; i < leaderboard->max_score_records; i++)
        {
            leaderboard->score_records[i].accuracy = 0;
            leaderboard->score_records[i].player_name_size = 0;
            leaderboard->score_records[i].player_name[0] = ' ';
        }
        return;
    }

    int *year = NULL;
    int *month = NULL;
    int *day = NULL;
    int *hour = NULL;
    int *minute = NULL;
    int *second = NULL;

    for (int i = 0; i < leaderboard->max_score_records; i++)
    {
        Score_Record *curr_score_record = &leaderboard->score_records[i];

        fgets(curr_score_record->player_name, curr_score_record->player_name_size, leaderboard_file);

        fscanf(leaderboard_file, "%d", &curr_score_record->accuracy);

        fscanf(leaderboard_file, "%02d/%02d/%02d\n", year, month, day);
        curr_score_record->date.year = *year;
        curr_score_record->date.month = *month;
        curr_score_record->date.day = *day;

        fscanf(leaderboard_file, "%02d:%02d:%02d\n", hour, minute, second);
        curr_score_record->time.hour = *hour;
        curr_score_record->time.minute = *minute;
        curr_score_record->time.second = *second;
    }

    fclose(leaderboard_file);
}
/*
bool add_new_score(Leaderboard* leaderboard, Player* player) {
    int score = (player->moles_hitted / (player->moles_hitted + player->moles_missed)) * 100;
    
    int index_new_score = -1;
    for (int i = 0; i < leaderboard->max_score_records; i++) {
        Score_Record* curr_score_record = &leaderboard->score_records[i];
        if (score > curr_score_record->accuracy) {
            index_new_score = i;
        }
    }

    if (index_new_score == -1)
        return false;

    for (int i = (index_new_score+1); i < 5; i++) {
        leaderboard->score_records[i] = leaderboard->score_records[i-1]; 
    }

    leaderboard->score_records[index_new_score].player = player;
    leaderboard->score_records[index_new_score].accuracy = score;
    leaderboard->score_records[index_new_score].date = get_date();
    leaderboard->score_records[index_new_score].time = get_time();
}
*/
