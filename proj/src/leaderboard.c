#include <lcom/lcf.h>
#include <stdint.h>

#include "leaderboard.h"

Leaderboard *load_leaderboard()
{
    //Allocating memory
    Leaderboard *leaderboard = (Leaderboard *)malloc(sizeof(Leaderboard));

    xpm_load(leaderboard_crown_xpm, XPM_8_8_8_8, &(leaderboard->crown));
    xpm_load(leaderboard_table_xpm, XPM_8_8_8_8, &(leaderboard->table));
    xpm_load(score_numbers_xpm, XPM_8_8_8_8, &(leaderboard->numbers));

    leaderboard->max_name_length = 7;
    leaderboard->num_score_records = 5;

    leaderboard->score_records = (Score_Record *)malloc(sizeof(Score_Record) * leaderboard->num_score_records);
    leaderboard->score_records[0].player_name = (char *)malloc(leaderboard->max_name_length);
    leaderboard->score_records[1].player_name = (char *)malloc(leaderboard->max_name_length);
    leaderboard->score_records[2].player_name = (char *)malloc(leaderboard->max_name_length);
    leaderboard->score_records[3].player_name = (char *)malloc(leaderboard->max_name_length);
    leaderboard->score_records[4].player_name = (char *)malloc(leaderboard->max_name_length);

    load_scores(leaderboard);

    leaderboard->num_buttons = 1;
    leaderboard->buttons = (Button **)malloc(sizeof(Button *) * leaderboard->num_buttons);
    leaderboard->buttons[0] = load_button(CLOSE_X, CLOSE_Y, score_close_normal_xpm, score_close_active_xpm);

    return leaderboard;
}

void draw_leaderboard_table(Leaderboard *leaderboard)
{
    uint32_t *crown_map = (uint32_t *)leaderboard->crown.bytes;
    vg_draw_xpm(crown_map, leaderboard->crown, LDBRD_CROWN_X, LDBRD_CROWN_Y);
    uint32_t *table_map = (uint32_t *)leaderboard->table.bytes;
    vg_draw_xpm(table_map, leaderboard->table, LDBRD_TABLE_X, LDBRD_TABLE_Y);
}

void draw_player_names(xpm_image_t font, Score_Record* score_records, uint8_t num_records)
{
    for (int i = 0; i < num_records; i++)
    {
        Score_Record *curr_score_record = &score_records[i];

        if (curr_score_record->player_name_size != 0)
            draw_player_name(font, LDBRD_NAME_STEP_FROM_X, i * LDBRD_NAME_STEP_FROM_LINE + LDBRD_NAME_STEP_FROM_Y, curr_score_record->player_name, curr_score_record->player_name_size);
    }
}

void draw_player_scores(Leaderboard *leaderboard)
{
    for (int i = 0; i < leaderboard->num_score_records; i++)
    {
        Score_Record *curr_score_record = &leaderboard->score_records[i];
        if (curr_score_record->player_name_size != 0)
            draw_player_score(leaderboard->numbers, LDBRD_SCORE_STEP_FROM_X, i * LDBRD_SCORE_STEP_FROM_LINE + LDBRD_SCORE_STEP_FROM_Y, curr_score_record->score);
    }
}

void draw_player_dates(Leaderboard *leaderboard)
{
    for (int i = 0; i < leaderboard->num_score_records; i++)
    {
        Score_Record *curr_score_record = &leaderboard->score_records[i];
        if (curr_score_record->player_name_size != 0)
            draw_date(leaderboard->numbers, LDBRD_DATE_STEP_FROM_X, i * LDBRD_DATE_STEP_FROM_LINE + LDBRD_DATE_STEP_FROM_Y, curr_score_record->date);
    }
}

void draw_player_score(xpm_image_t font, int xi, int yi, int score)
{
    uint32_t *font_pixmap = (uint32_t *)font.bytes;
    int number_width = font.width / 13; // 10 numbers plus slash, two points and percentage symbol
    int number_height = font.height;

    int left_score_number, mid_score_number, right_score_number;

    right_score_number = score % 10;
    mid_score_number = (score / 10) % 10;
    left_score_number = score / 100;

    vg_draw_part_of_xpm(font_pixmap, font, xi, yi, 12 * number_width, 13 * number_width, 0, number_height); //Percentage symbol
    vg_draw_part_of_xpm(font_pixmap, font, xi - number_width, yi, right_score_number * number_width, (right_score_number + 1) * number_width, 0, number_height);

    if (mid_score_number == 0 && left_score_number - 2 != 0)
    {
        vg_draw_part_of_xpm(font_pixmap, font, xi - 2 * number_width, yi, mid_score_number * number_width, (mid_score_number + 1) * number_width, 0, number_height);
        vg_draw_part_of_xpm(font_pixmap, font, xi - number_width * 3, yi, left_score_number * number_width, (left_score_number + 1) * number_width, 0, number_height);
    }
    else if (mid_score_number != 0)
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

    for (int i = 0; i < leaderboard->num_score_records; i++)
    {
        Score_Record *curr_score_record = &(leaderboard->score_records[i]);
        fprintf(leaderboard_file, "%s\n", curr_score_record->player_name);
        fprintf(leaderboard_file, "%d\n", curr_score_record->score);
        fprintf(leaderboard_file, "%d/%d/%d\n", curr_score_record->date.day, curr_score_record->date.month, curr_score_record->date.year);
    }

    fclose(leaderboard_file);
}

void load_scores(Leaderboard *leaderboard)
{
    FILE *leaderboard_file;
    leaderboard_file = fopen("/home/lcom/labs/proj/src/leaderboard.txt", "r");

    if (leaderboard_file == NULL)
    {
        for (int i = 0; i < leaderboard->num_score_records; i++)
        {
            leaderboard->score_records[i].score = 0;
            leaderboard->score_records[i].player_name_size = 0;
            leaderboard->score_records[i].player_name[0] = ' ';
        }
        return;
    }


    for (int i = 0; i < leaderboard->num_score_records; i++)
    {   
        fgets(leaderboard->score_records[i].player_name, leaderboard->max_name_length, leaderboard_file);
        strtok(leaderboard->score_records[i].player_name, "\n");
        if(strncmp(leaderboard->score_records[i].player_name, "nobody", 6) == 0) leaderboard->score_records[i].player_name_size = 0;
        else leaderboard->score_records[i].player_name_size = strlen(leaderboard->score_records[i].player_name);

        fscanf(leaderboard_file, "%d", &leaderboard->score_records[i].score);
        fgetc(leaderboard_file);

        fscanf(leaderboard_file, "%d/%d/%d", &leaderboard->score_records[i].date.day, &leaderboard->score_records[i].date.month, &leaderboard->score_records[i].date.year);
        fgetc(leaderboard_file);
    }

    fclose(leaderboard_file);
}

bool add_new_score(Leaderboard* leaderboard, Player* player) {
    if(player->hitted_moles == 0)
        return false;
    
    int aux = player->hitted_moles*100;
    int score = 0;
    while (aux >= (player->hitted_moles + player->missed_moles)) {
        aux -= player->hitted_moles + player->missed_moles;
        score++;
    }

    int index_new_score = -1;
    for (int i = leaderboard->num_score_records-1; i >= 0; i--) {
        Score_Record* curr_score_record = &leaderboard->score_records[i];
        if (score > curr_score_record->score) {
            index_new_score = i;
        }
    }
    if (index_new_score == -1)
        return false;

    for (int i = leaderboard->num_score_records-1; i > index_new_score; i--) {
        if (leaderboard->score_records[i-1].score == 0)
            continue;
        leaderboard->score_records[i] = leaderboard->score_records[i-1];
    }

    leaderboard->score_records[index_new_score].player_name = player->name;
    leaderboard->score_records[index_new_score].player_name_size = player->max_name_length;
    leaderboard->score_records[index_new_score].score = score;
    leaderboard->score_records[index_new_score].date = get_date();
    return true;
}

