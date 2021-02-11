
#pragma once
#include <lcom/lcf.h>

#include <Sprites/numbers.xpm>
#include <Sprites/moles_hitted.xpm>
#include <Sprites/moles_missed.xpm>
#include <Sprites/numbers_moles.xpm>
#include <Sprites/game_over.xpm>
#include <Sprites/game_over_missed_moles.xpm>
#include <Sprites/game_over_hitted_moles.xpm>
#include <Sprites/game_over_numbers.xpm>
#include <Sprites/clock_icon.xpm>
#include <Sprites/instructions.xpm>

#include <Sprites/font_small.xpm>

#include <Sprites/background.xpm>
#include <Sprites/leaderboard_background.xpm>
#include <Sprites/player_background.xpm>

#include <Sprites/table.xpm>
#include <Sprites/good_bye_message.xpm>
#include <Sprites/good_bye_mole.xpm>
#include <Sprites/credits.xpm>
#include <Sprites/ballon.xpm>
#include <Sprites/enter_score.xpm>

#include<Sprites/waiting_for_player.xpm>
#include<Sprites/win.xpm>
#include<Sprites/lose.xpm>

#include "xpm_coordinates.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "serial_port.h"
#include "uart_const.h"
#include "mole.h"
#include "menu.h"
#include "kbd_manager.h"
#include "mouse.h"
#include "rtc.h"
#include "player_settings.h"
#include "leaderboard.h"


typedef enum {MAIN_MENU, PLAYER_SETTINGS, WAITING, INSTRUCTIONS, SINGLE_PLAYER, MULTI_PLAYER, GAME_OVER, WIN, LOST, LEADERBOARD, EXIT} game_state;
typedef enum {TIMER, KEYBOARD, MOUSE, RTC, UART} device;

/** 
 * @struct GameOver
 * @brief Struct relative to GameOver
 * @var Gamelogo_game_over
 * Game Over's logo xpm
 * @var GameOver:: numbers
 * Game Over's number xpm
 * @var GameOver:: missed_moles
 * Game Over's  missed_moles xpm
 * @var GameOver:: hitted_moles
 * Game Over's hitted_moles xpm
 * @var GameOver:: cursor
 * Game Over's cursor
 * @var GameOver:: buttons
 * Game Over's buttons
 * @var GameOver:: num_buttons
 * how many buttons Game Over has
 */
typedef struct {
	xpm_image_t logo_game_over;
	xpm_image_t ballon;
	xpm_image_t numbers;
	xpm_image_t missed_moles;
	xpm_image_t hitted_moles;
	Cursor *cursor;
	Button** buttons;
    int num_buttons;
	bool new_score;
} GameOver;

/**
 * @struct Exit
 * @bried Struct realtive to Exit
 * @var GameOver:: logo
 * Exit's logo xpm
 * @var GameOver::mole
 * Exit's mole xpm
 * @var GameOver:: credits
 * Exit's credits xpm 
 * @var GameOver:: x_mole
 * Exit's mole's initial x axis position
 * 
 * 
 * */
typedef struct {
	xpm_image_t logo;
	xpm_image_t mole;
	xpm_image_t credits;
	uint16_t x_mole;
	int animation_timer;
} Exit;

/**
 * @struct Instructions
 * @var::instructions
 * Instruction's xpm
 * */
typedef struct {
	xpm_image_t instructions;
} Instructions;

/**
 * @struct WhackAMole
 * @var GameOver:: numbers_font
 * WhackAMole's numbers font xpm
 * @var GameOver:: letters_small_font
 * WhackAMole's  small letters font xpm
 * @var GameOver:: clock_icon
 * WhackAMole's clock icon xpm
 * @var GameOver:: moles_missed
 * WhackAMole's moles missed xpm
 * @var GameOver:: moles_hitted
 * WhackAMole's moles hitted xpm
 * @var GameOver:: table
 * WhackAMole's table where missed moles and hitted moles will be displayed xpm
 * @var GameOver::game_win
 * WWhackAMole win message xpm
 * @var GameOver:: game_lost
 * WhackAMole's game lost message xpm
 * @var GameOver:: timer_irq
 * timer IRQ 
 * @var GameOver:: keyboard_irq
 * keyboard IRQ
 * @var GameOver:: mouse_irq
 * mouse IRQ
 * @var GameOver:: irq_rtc
 * rtc IRQ
 * @var GameOver:: game_time
 * WhackAMole's game_time
 * @var GameOver:: moles[6]
 * WhackAMole's moles
 * @var GameOver:: menu
 * WhackAMole's menu
 * @var GameOver:: player_settings
 * WhackAMole's Player Settings
 * @var GameOver:: player
 * WhackAMole's Player
 * @var GameOver:: cursor
 * WhackAMole's Cursor
 * @var GameOver:: instructions
 * WhackAMole's Instructions
 * @var GameOver:: game_over
 * WhackAMole's Game Over
 * @var GameOver:: leaderboard
 * WhackAMole's Leaderboard
 * @var GameOver:: good_bye
 * WhackAMole's Good Bye
 * @var GameOver:: game_state
 * WhackAMole's game state
 * @bool GameOver:: running
 * Boolean : it's true while the user has not yet chosen to exit the game, else it's false 
 * 
 * */
typedef struct {
	xpm_image_t background[3];
	xpm_image_t game_time_numbers_font;
	xpm_image_t numbers_font;
	xpm_image_t letters_small_font;
	xpm_image_t clock_icon;
	xpm_image_t moles_missed;
	xpm_image_t moles_hitted;
	xpm_image_t table;
	xpm_image_t game_win;
	xpm_image_t game_lost;
	xpm_image_t waiting_for_player;
	uint8_t timer_irq, keyboard_irq, uart_irq;
	uint16_t mouse_irq, irq_rtc;

	uint8_t game_time;
	
	Mole* moles;
	int num_moles;
	Menu* menu;
	Player_Settings* player_settings;
	Player* player;
	Cursor* cursor;
	Instructions *instructions;
	GameOver* game_over;
	Leaderboard* leaderboard;
	Exit* exit;

	game_state game_state;
	bool multiplayer;
	bool opponent_end;
	bool host;
	bool sent_hitted_moles;
	bool running;
} WhacAMole;


/**
 * @brief loads Game : Loads all Game's sprites,  Creates  Mole, Menu, Player_Settings, Player,Cursor, Instructions, Leaderboard, GoodBye, initializates the game at MAIN_MENU and set's bool running as true
 * 
 * */
WhacAMole* load_game();

/**
 * @brief loads Game over: loads all it's xpm, creates it's Curson and all it's Buttons 
 * */
GameOver* load_game_over();

/**
 * @brief loads Exit : loads all it's sprites and sets it's moles first position
 * */
Exit *load_exit();

/**
 * @brief loads Instructions : loads it's sprite 
 * */
Instructions *load_instructions();


int game_main_loop(WhacAMole* game);

/**
 * @brief General Interrupt Control for any device
 * @param device device Device that generated interrupts
 * */
void GeneralInterrupt(device device,WhacAMole* game);

/**
 *  @brief Main Menu's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Main_Menu_interrupt_handler(device device, WhacAMole *new_game);
/**
 *  @brief  Instruction's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Instructions_interrupt_handler(device device, WhacAMole *new_game);
/**
 *  @brief  Player_Setting's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Player_Settings_interrupt_handler(device device, WhacAMole *new_game);


void Waiting_interrupt_handler(device device, WhacAMole *new_game);
void Win_interrupt_handler(device device, WhacAMole *new_game);
void Lost_interrupt_handler(device device, WhacAMole *new_game);


/**
 *  @brief  Single_Player's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Single_Player_interrupt_handler(device device, WhacAMole* game);

/**
 *  @brief  Multi_Player's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Multi_Player_interrupt_handler(device device, WhacAMole* game);

/**
 *  @brief  Game Over's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Game_Over_interrupt_handler(device device, WhacAMole* game);

/**
 *  @brief  Leaderboard's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Leaderboard_interrupt_handler(device device, WhacAMole *new_game);

/**
 *  @brief Exit's Interrupt Handler for any of the devices
 *  @param device device Device that generated interrupts
 *  @param WhackAMole *new_game actual game
 * */
void Exit_interrupt_handler(device device, WhacAMole* new_game);


/**
 *  @brief Draw a number at a given position through a given font 
 *  @param xpm_image_t font xpm used to draw the number
 *  @param int font_info
 *  @param int xi upper left xpm's x position
 *  @param int yi upper left xpm's y position
 * */
void draw_number(xpm_image_t font, int font_info, int xi, int yi, int number, bool left_number);
