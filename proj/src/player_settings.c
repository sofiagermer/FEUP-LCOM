#include <lcom/lcf.h>
#include <stdint.h>
#include "player_settings.h"
#include "xpm_coordinates.h"

Player *load_player(Avatar *default_avatar)
{
  //Allocating memory
  Player *player = (Player *)malloc(sizeof(Player));
  player->name = (char *)malloc(player->max_name_length);
  player->name[0] = 'P';
  player->name[1] = 'L';
  player->name[2] = 'A';
  player->name[3] = 'Y';
  player->name[4] = 'E';
  player->name[5] = 'R';
  player->name[6] = ' ';
  player->hitted_moles = 0;
  player->missed_moles = 0;
  player->max_name_length = 7;
  player->avatar = default_avatar->sprites[0];
  return player;
}

Avatar *load_avatar(xpm_row_t *normal, xpm_row_t *selected, xpm_row_t *small)
{
  Avatar *avatar = malloc(sizeof(Avatar));
  if (avatar == NULL)
    return NULL;

  xpm_load(normal, XPM_8_8_8_8, &(avatar->sprites[0]));
  xpm_load(selected, XPM_8_8_8_8, &(avatar->sprites[1]));
  xpm_load(small, XPM_8_8_8_8, &(avatar->sprites[2]));
  avatar->state = NOT_SELECTED;
  return avatar;
}

Player_Settings *load_player_settings()
{
  Player_Settings *player_sets = malloc(sizeof(Player_Settings));

  xpm_load(player_title_xpm, XPM_8_8_8_8, &(player_sets->background_title));
  xpm_load(font_xpm, XPM_8_8_8_8, &(player_sets->font));
  xpm_load(name_length_warning_xpm, XPM_8_8_8_8, &(player_sets->name_length_warning));

  player_sets->avatars[0] = load_avatar(hammer_0_big_xpm, hammer_0_big_bright_xpm, hammer_0_small_xpm);
  player_sets->avatars[1] = load_avatar(hammer_1_big_xpm, hammer_1_big_bright_xpm, hammer_1_small_xpm);
  player_sets->avatars[2] = load_avatar(hammer_2_big_xpm, hammer_2_big_bright_xpm, hammer_2_small_xpm);
  player_sets->avatars[3] = load_avatar(hammer_3_big_xpm, hammer_3_big_bright_xpm, hammer_3_small_xpm);
  player_sets->avatars[0]->state = SELECTED;

  player_sets->num_buttons = 4;
  player_sets->buttons = (Button **) malloc(sizeof(Button *) * player_sets->num_buttons);
  player_sets->buttons[0] = load_button(ARROW_LEFT_X, ARROW_LEFT_Y, arrow_left_normal_xpm, arrow_left_active_xpm);
  player_sets->buttons[1] = load_button(ARROW_RIGHT_X, ARROW_RIGHT_Y, arrow_right_normal_xpm, arrow_right_active_xpm);
  player_sets->buttons[2] = load_button(NAME_BOX_X, NAME_BOX_Y, name_box_normal_xpm, name_box_active_xpm);
  player_sets->buttons[3] = load_button(START_X, START_Y, start_normal_xpm, start_active_xpm);

  player_sets->name_maximum_length = false;
  return player_sets;
}


void draw_name_lenght_warning(Player_Settings *player_sets)
{
  vg_draw_xpm((uint32_t *)player_sets->name_length_warning.bytes, player_sets->name_length_warning, NM_LENGTH_WR_X, NM_LENGTH_WR_Y);
}

void draw_avatars(Avatar *avatars[4])
{
  for (int i = 0; i < 4; i++)
  {
    xpm_image_t current_img = avatars[i]->sprites[(int)avatars[i]->state];
    vg_draw_xpm((uint32_t *)current_img.bytes, current_img, i * STEP_AVT_X + FIRST_AVT_Y, i * STEP_AVT_Y + FIRST_AVT_Y);
  }
}

void move_left_avatar(Player_Settings *player_sets)
{
  if (player_sets->avatars[0]->state == SELECTED)
    return;
  for (int i = 1; i <= 3; i++)
  {
    if (player_sets->avatars[i]->state == SELECTED)
    {
      player_sets->avatars[i]->state = NOT_SELECTED;
      player_sets->avatars[i - 1]->state = SELECTED;
    }
  }
}

void move_right_avatar(Player_Settings *player_sets)
{
  if (player_sets->avatars[3]->state == SELECTED)
    return;

  for (int i = 2; i >= 0; i--)
  {
    if (player_sets->avatars[i]->state == SELECTED)
    {
      player_sets->avatars[i]->state = NOT_SELECTED;
      player_sets->avatars[i + 1]->state = SELECTED;
    }
  }
}

xpm_image_t get_hammer(Player_Settings *player_sets)
{
  for (int i = 0; i < 4; i++)
  {
    if (player_sets->avatars[i]->state == SELECTED)
    {
      return player_sets->avatars[i]->sprites[2];
    }
  }
  return player_sets->avatars[0]->sprites[2];
}

void draw_player_name(xpm_image_t font, int xi, int yi, char name[], int name_size)
{
  int letter_width = font.width / (26 / 2); // 26 letters in two rows equals 13 letter per row
  int letter_height = font.height / 2;      //two rows of letters
  int index = 0;
  for (int i = 0; i < name_size; i++)
  {
    if (name[i] == ' ')
      break;
    index = (int)name[i] - (int)'A';
    if (index < 13)
      vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi + i * letter_width, yi, index * letter_width, index * letter_width + letter_width, 0, letter_height);
    else
      vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi + i * letter_width, yi, index * letter_width, index * letter_width + letter_width, letter_height, letter_height * 2);
  }
}

void update_player_name(Player_Settings* player_sets, Player *player, bool delete_letter, char new_letter)
{
  if (delete_letter)
  { //searches for the first empty space and deletes the letter before this space
    if (player->name[player->max_name_length - 1] != ' ')
    {
      player->name[player->max_name_length - 1] = ' ';
      player_sets->name_maximum_length = false;
      return;
    }
    for (int i = 0; i < player->max_name_length; i++)
    {
      if (player->name[i] == ' ' && i != 0)
      {
        player->name[i - 1] = ' ';
        player_sets->name_maximum_length = false;
        return;
      }
    }
  }
  else
  { //search for the first empty space to add a new letter
    for (int i = 0; i < player->max_name_length; i++)
    {
      if (player->name[i] == ' ')
      {
        player->name[i] = new_letter;
        if (i == (player->max_name_length - 1))
          player_sets->name_maximum_length = true;
        return;
      }
    }
  }
}
