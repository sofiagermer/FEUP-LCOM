#include <lcom/lcf.h>
#include <stdint.h>

#include "game.h"

//TIMER
extern unsigned int timer_counter;

//KEYBOARD
extern bool keyboard_done_getting_scancodes;
extern uint8_t scan_code;

//MOUSE
extern bool mouse_last_byte_of_packet;
extern uint8_t packet[];

//RTC
extern Time rtc_time;
extern Date rtc_date;

//UART
extern uint8_t ser_byte;
extern bool error_reading;

WhacAMole *load_game()
{
    //srand(time(NULL)); // Initialization, should only be called once.
    WhacAMole *new_game = (WhacAMole *)malloc(sizeof(WhacAMole));
    new_game->game_time = 30;
    new_game->cursor = load_cursor(cursor_xpm);

    xpm_load(background_xpm, XPM_8_8_8_8, &(new_game->background[0]));
    xpm_load(leaderboard_background_xpm, XPM_8_8_8_8, &(new_game->background[1]));
    xpm_load(player_background_xpm, XPM_8_8_8_8, &(new_game->background[2]));
    xpm_load(numbers_xpm, XPM_8_8_8_8, &new_game->game_time_numbers_font);
    xpm_load(moles_missed_xpm, XPM_8_8_8_8, &new_game->moles_missed);
    xpm_load(moles_hitted_xpm, XPM_8_8_8_8, &new_game->moles_hitted);
    xpm_load(score_numbers_xpm, XPM_8_8_8_8, &new_game->numbers_font);
    xpm_load(table_xpm, XPM_8_8_8_8, &new_game->table);
    xpm_load(clock_icon_xpm, XPM_8_8_8_8, &new_game->clock_icon);
    xpm_load(font_small_xpm, XPM_8_8_8_8, &(new_game->letters_small_font));
    xpm_load(win_xpm, XPM_8_8_8_8, &(new_game->game_win));
    xpm_load(waiting_for_player_xpm, XPM_8_8_8_8, &(new_game->waiting_for_player));
    xpm_load(lose_xpm, XPM_8_8_8_8, &(new_game->game_lost));

    new_game->num_moles = 6;
    new_game->moles = (Mole *)malloc(sizeof(Mole) * new_game->num_moles);
    for (int i = 0; i < 6; i++)
    {
        Mole *new_mole = createMole(i);
        new_game->moles[i] = *new_mole;
    }

    new_game->menu = load_menu();
    new_game->player_settings = load_player_settings();
    new_game->player = load_player(new_game->player_settings->avatars[0]);
    new_game->leaderboard = load_leaderboard();
    new_game->cursor = load_cursor(cursor_xpm);
    new_game->game_over = load_game_over();
    new_game->exit = load_exit();
    new_game->instructions = load_instructions();
    new_game->game_state = MAIN_MENU;
    new_game->host = true;
    new_game->multiplayer = false;
    new_game->opponent_end = false;
    new_game->sent_hitted_moles = false;
    new_game->running = true;
    return new_game;
}

GameOver *load_game_over()
{
    GameOver *game_over = (GameOver *)malloc(sizeof(GameOver));

    xpm_load(game_over_xpm, XPM_8_8_8_8, &(game_over->logo_game_over));
    xpm_load(ballon_xpm, XPM_8_8_8_8, &(game_over->ballon));
    xpm_load(game_over_missed_moles_xpm, XPM_8_8_8_8, &(game_over->missed_moles));
    xpm_load(game_over_hitted_moles_xpm, XPM_8_8_8_8, &(game_over->hitted_moles));
    xpm_load(game_over_numbers_xpm, XPM_8_8_8_8, &(game_over->numbers));

    game_over->cursor = load_cursor(cursor_xpm);
    game_over->num_buttons = 3;
    game_over->buttons = (Button **)malloc(sizeof(Button *) * game_over->num_buttons);
    game_over->buttons[0] = load_button(MAIN_MENU_X, MAIN_MENU_Y, main_menu_normal_xpm, main_menu_active_xpm);
    game_over->buttons[1] = load_button(LEADERB_GM_OV_X, LEADERB_GM_OV_Y, leaderboard_normal_xpm, leaderboard_active_xpm);
    game_over->buttons[2] = load_button(EXIT_GM_OV_X, EXIT_GM_OV_Y, exit_normal_xpm, exit_active_xpm);
    game_over->new_score = false;
    return game_over;
}

Instructions *load_instructions()
{
    Instructions *instructions = (Instructions *)malloc(sizeof(Instructions));
    xpm_load(instructions_xpm, XPM_8_8_8_8, &(instructions->instructions));
    return instructions;
}

Exit *load_exit()
{
    Exit *exit = (Exit *)malloc(sizeof(Exit));
    xpm_load(good_bye_message_xpm, XPM_8_8_8_8, &(exit->logo));
    xpm_load(good_bye_mole_xpm, XPM_8_8_8_8, &(exit->mole));
    xpm_load(credits_xpm, XPM_8_8_8_8, &(exit->credits));
    exit->x_mole = 0;
    exit->animation_timer = 3;
    return exit;
}

int game_main_loop(WhacAMole *new_game)
{
    int ipc_status, r;
    message msg;
    uint8_t irq_auxiliar = 0;
    rtc_start();

    //Subscribing all devices
    if (timer_subscribe_int(&new_game->timer_irq) != OK)
    {
        return 1;
    }
    if (mouse_issue_cmd_to_kbc(WRITE_BYTE_TO_MOUSE, EN_DATA_REPORT) != OK)
    {
        return 1;
    }
    if (mouse_subscribe_int(&new_game->mouse_irq) != OK)
    {
        return 1;
    }
    if (kbd_subscribe_int(&new_game->keyboard_irq) != OK)
    {
        return 1;
    }
    if (rtc_subscribe_int(&irq_auxiliar) != OK)
    {
        return 1;
    }
    new_game->irq_rtc = BIT(irq_auxiliar);
    if (ser_subscribe_int(&irq_auxiliar) != OK)
    {
        return 1;
    }
    new_game->uart_irq = BIT(irq_auxiliar);
    ser_flush_rx();

    while (new_game->running)
    {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status))
        { // received notification
            switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE: // hardware interrupt notification
                if (msg.m_notify.interrupts & new_game->keyboard_irq)
                {
                    kbc_ih();
                    if (keyboard_done_getting_scancodes)
                        GeneralInterrupt(KEYBOARD, new_game);
                }

                if (msg.m_notify.interrupts & new_game->timer_irq)
                {
                    timer_int_handler();
                    if (timer_counter % (60 / GAME_FPS) == 0)
                        GeneralInterrupt(TIMER, new_game);
                    update_buffer();
                }
                if (msg.m_notify.interrupts & new_game->mouse_irq)
                {
                    mouse_ih();
                    if (mouse_last_byte_of_packet)
                        GeneralInterrupt(MOUSE, new_game);
                }
                if (msg.m_notify.interrupts & new_game->irq_rtc)
                {
                    rtc_int_handler();
                    GeneralInterrupt(RTC, new_game);
                }
                if (msg.m_notify.interrupts & new_game->uart_irq)
                {
                    rtc_int_handler();
                    GeneralInterrupt(UART, new_game);
                }
                break;
            default:
                break;
            }
        }
    }
    free(new_game->moles);
    free(new_game->menu->buttons);
    free(new_game->menu);
    free(new_game->player_settings->avatars);
    free(new_game->player_settings->buttons);
    free(new_game->player_settings);
    free(new_game->player);
    free(new_game->cursor);
    free(new_game->instructions);
    free(new_game->game_over->buttons);
    free(new_game->game_over->cursor);
    free(new_game->game_over);
    free(new_game->leaderboard->buttons);
    free(new_game->leaderboard->score_records);
    free(new_game->leaderboard);
    free(new_game->exit);
    free(new_game);
    //Unsubscribing all devices
    if (kbd_unsubscribe_int() != OK)
    {
        return 1;
    }
    if (timer_unsubscribe_int() != OK)
    {
        return 1;
    }
    if (mouse_unsubscribe_int() != OK)
    {
        return 1;
    }
    if (issue_command_to_mouse(DIS_DATA_REPORT) != OK)
    {
        return 1;
    }
    if (rtc_unsubscribe_int() != OK)
    {
        return 1;
    }
    if (ser_unsubscribe_int() != OK)
    {
        return 1;
    }

    return OK;
}

void GeneralInterrupt(device device, WhacAMole *new_game)
{
    switch (new_game->game_state)
    {
    case MAIN_MENU:
        Main_Menu_interrupt_handler(device, new_game);
        break;
    case INSTRUCTIONS:
        Instructions_interrupt_handler(device, new_game);
        break;
    case PLAYER_SETTINGS:
        Player_Settings_interrupt_handler(device, new_game);
        break;
    case WAITING:
        Waiting_interrupt_handler(device, new_game);
        break;
    case SINGLE_PLAYER:
        Single_Player_interrupt_handler(device, new_game);
        break;
    case MULTI_PLAYER:
        Multi_Player_interrupt_handler(device, new_game);
        break;
    case GAME_OVER:
        Game_Over_interrupt_handler(device, new_game);
        break;
    case WIN:
        Win_interrupt_handler(device, new_game);
        break;
    case LOST:
        Lost_interrupt_handler(device, new_game);
        break;
    case LEADERBOARD:
        Leaderboard_interrupt_handler(device, new_game);
        break;
    case EXIT:
        Exit_interrupt_handler(device, new_game);
        break;
    default:
        break;
    }
}

void Main_Menu_interrupt_handler(device device, WhacAMole *game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    switch (device)
    {
    case TIMER:
        vg_draw_xpm((uint32_t *)game->background[0].bytes, game->background[0], X_ORIGIN, Y_ORIGIN);
        draw_logo(game->menu);
        draw_buttons(game->menu->buttons, game->menu->num_buttons);
        if (game->menu->buttons[5]->state == ACTIVE)
            draw_date(game->numbers_font, CALLENDAR_NUM_X, CALLENDAR_NUM_Y, rtc_date);
        if (game->menu->buttons[6]->state == ACTIVE)
        {
            draw_time(game->numbers_font, CLOCK_NUM_X, CLOCK_NUM_Y, rtc_time);
        }
        draw_cursor(game->cursor);
        break;
    case KEYBOARD:
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, game->cursor);
        update_buttons(game->cursor, game->menu->buttons, game->menu->num_buttons);
        if (game->menu->buttons[0]->state == ACTIVE && mouse_event.type == LB_RELEASED)
        {
            game->game_state = PLAYER_SETTINGS;
            game->multiplayer = false;
        }
        else if (game->menu->buttons[1]->state == ACTIVE && mouse_event.type == LB_RELEASED)
        {
            game->game_state = PLAYER_SETTINGS;
            game->multiplayer = true;
        }
        else if (game->menu->buttons[2]->state == ACTIVE && mouse_event.type == LB_RELEASED)
            game->game_state = LEADERBOARD;
        else if (game->menu->buttons[3]->state == ACTIVE && mouse_event.type == LB_RELEASED)
            game->game_state = INSTRUCTIONS;
        else if (game->menu->buttons[4]->state == ACTIVE && mouse_event.type == LB_RELEASED)
            game->game_state = EXIT;
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        ser_byte = UNREQUESTED_BYTE;
        break;
    }
}
void Instructions_interrupt_handler(device device, WhacAMole *game)
{
    Instructions *instructions = game->instructions;
    switch (device)
    {
    case TIMER:
        vg_draw_xpm((uint32_t *)instructions->instructions.bytes, instructions->instructions, X_ORIGIN, Y_ORIGIN);
        break;
    case KEYBOARD:
        if (scan_code == ESC_BREAK)
            game->game_state = MAIN_MENU;
        break;
    case MOUSE:
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        ser_byte = UNREQUESTED_BYTE;
        break;
    }
}

void Player_Settings_interrupt_handler(device device, WhacAMole *game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    Player_Settings *player_sets = game->player_settings;
    switch (device)
    {
    case TIMER:
        vg_draw_xpm((uint32_t *)game->background[2].bytes, game->background[2], X_ORIGIN, Y_ORIGIN);
        vg_draw_xpm((uint32_t *)player_sets->background_title.bytes, player_sets->background_title, X_ORIGIN, Y_ORIGIN);
        draw_avatars(player_sets->avatars);
        draw_buttons(player_sets->buttons, player_sets->num_buttons);
        draw_player_name(player_sets->font, NM_PLACE_X, NM_PLACE_Y, game->player->name, game->player->max_name_length);
        if (player_sets->name_maximum_length)
            draw_name_lenght_warning(player_sets);
        draw_cursor(game->cursor);
        break;
    case KEYBOARD:
        if (player_sets->buttons[2]->state == ACTIVE)
        {
            if (scan_code == BACK_SPACE_BREAK)
            {
                update_player_name(player_sets, game->player, true, ' ');
            }
            else
            {
                char new_letter = kbd_manager(scan_code);
                if (new_letter != '.')
                    update_player_name(player_sets, game->player, false, kbd_manager(scan_code));
            }
        }
        if (scan_code == ENTER_BREAK)
        {
            player_sets->buttons[2]->state = NORMAL;
            player_sets->name_maximum_length = false;
        }
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, game->cursor);
        update_buttons(game->cursor, player_sets->buttons, player_sets->num_buttons);

        if (mouse_over(player_sets->buttons[0], game->cursor) && mouse_event.type == LB_RELEASED)
            move_left_avatar(player_sets);
        else if (mouse_over(player_sets->buttons[1], game->cursor) && mouse_event.type == LB_RELEASED)
            move_right_avatar(player_sets);
        else if (mouse_over(player_sets->buttons[2], game->cursor) && mouse_event.type == LB_RELEASED)
        {
            if (game->player->name[game->player->max_name_length - 1] != ' ')
                player_sets->name_maximum_length = true;
        }
        else if (mouse_over(player_sets->buttons[3], game->cursor) && mouse_event.type == LB_RELEASED)
        {
            game->cursor->cursor_image = get_hammer(player_sets);
            if (!game->multiplayer)
                game->game_state = SINGLE_PLAYER;
            else if (game->host)
            {
                game->game_state = WAITING;
            }
            else
            {
                ser_send_byte_wait(CONFIRMATION);
                game->game_state = MULTI_PLAYER;
            }
        }
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        if (ser_byte == READY_TO_PLAY_FROM_HOST)
        {
            game->host = false;
        }
        else
            ser_byte = UNREQUESTED_BYTE;
        break;
    }
}

void Waiting_interrupt_handler(device device, WhacAMole *game)
{
    switch (device)
    {
    case TIMER:
        vg_draw_xpm((uint32_t *)game->background[1].bytes, game->background[1], X_ORIGIN, Y_ORIGIN);
        vg_draw_xpm((uint32_t *)game->waiting_for_player.bytes, game->waiting_for_player, 10, 300);
        ser_send_byte_wait(READY_TO_PLAY_FROM_HOST);
        break;
    case KEYBOARD:
        if (scan_code == ESC_BREAK)
            game->game_state = EXIT;
        break;
    case MOUSE:
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        switch (ser_byte)
        {
        case CONFIRMATION:
            game->host = true;
            game->game_state = MULTI_PLAYER;
            break;
        default:
            ser_byte = UNREQUESTED_BYTE;
            break;
        }
        break;
    }
}

void Single_Player_interrupt_handler(device device, WhacAMole *game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    Position mole_position;
    static int time_duration = 0;
    switch (device)
    {
    case TIMER:
        vg_draw_xpm((uint32_t *)game->background[0].bytes, game->background[0], X_ORIGIN, Y_ORIGIN);
        draw_all_moles(game->moles, game->num_moles);
        vg_draw_xpm((uint32_t *)game->table.bytes, game->table, SCORE_TABLE_X, SCORE_TABLE_Y);
        vg_draw_xpm((uint32_t *)game->moles_hitted.bytes, game->moles_missed, MOLES_MISS_FRAME_X, MOLES_MISS_FRAME_Y);
        vg_draw_xpm((uint32_t *)game->moles_missed.bytes, game->moles_hitted, MOLES_HIT_FRAME_X, MOLES_HIT_FRAME_Y);
        draw_number(game->numbers_font, 13, MOLES_MISS_NUM_X, MOLES_MISS_NUM_Y, game->player->missed_moles, false);
        draw_number(game->numbers_font, 13, MOLES_HIT_NUM_X, MOLES_HIT_NUM_Y, game->player->hitted_moles, false);
        vg_draw_xpm((uint32_t *)game->clock_icon.bytes, game->clock_icon, CLOCK_ICON_X, CLOCK_ICON_Y);
        draw_number(game->game_time_numbers_font, 10, GAME_TIMER_X, GAME_TIMER_Y, time_duration / GAME_FPS, true);
        draw_cursor(game->cursor);
        time_duration++;
        for (int mole_index = 0; mole_index < 6; mole_index++)
        {
            Mole *ml = &game->moles[mole_index];
            //if (rand() % 10000 > MOLE_PROBABILITY - (((timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * PROBABILITY_LIMIT_DECREMENT))
            if (ml->position == HIDED && (rand() % 10000 > MOLE_PROBABILITY))
            {
                ml->position = UP_1;
                ml->time_up = 0;
            }
            else if (ml->position == UP_4)
            {
                ml->time_up++;
                //if (ml->time_up >= (MOLE_UP_TIME - ((double)(timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * TIME_UP_LIMIT_DECREMENT))
                if (ml->time_up >= MOLE_UP_TIME)
                {
                    ml->position = DOWN_MISSED_4;
                    ml->time_up = 0;
                }
            }
            else if (ml->position != HIDED)
            {
                ml->position++;
                if (ml->position == DOWN_MISSED_1 || ml->position == DOWN_HIT_1)
                    ml->position = HIDED;
            }
        }
        if (time_duration / GAME_FPS >= game->game_time)
        {
            game->game_state = GAME_OVER;
            time_duration = 0;
            game->cursor->cursor_image = game->cursor->cursor_image_default;
        }
        break;
    case KEYBOARD:
        for (int mole_index = 0; mole_index < 6; mole_index++)
        {
            mole_position = game->moles[mole_index].position;
            if (game->moles[mole_index].kbd_key == kbd_manager(scan_code))
            {
                if (mole_position == UP_4)
                {
                    game->moles[mole_index].time_up = 0;
                    game->moles[mole_index].position = DOWN_HIT_4;

                    game->player->hitted_moles++;
                    break;
                }
                else
                {
                    game->player->missed_moles++;
                }
            }
        }
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, game->cursor);
        if (mouse_event.type == LB_RELEASED)
        {
            game->player->missed_moles++;
            for (int i = 0; i < game->num_moles; i++)
            {
                if (game->moles[i].position == UP_4 && check_over_mole(&game->moles[i], game->cursor->x, game->cursor->y) && mouse_event.type == LB_RELEASED)
                {
                    game->moles[i].time_up = 0;
                    game->moles[i].position = DOWN_HIT_4;
                    game->player->hitted_moles++;
                    game->player->missed_moles--;
                    break;
                }
            }
        }
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        ser_byte = UNREQUESTED_BYTE;
        break;
    default:
        break;
    }
}

void Multi_Player_interrupt_handler(device device, WhacAMole *game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    Position mole_position;
    static int time_duration = 0;
    switch (device)
    {
    case TIMER:
        /*if (game->host)
        {
            vg_draw_xpm((uint32_t *)game->background[0].bytes, game->background[0], X_ORIGIN, Y_ORIGIN);
        }
        else
        {
            vg_draw_xpm((uint32_t *)game->background[1].bytes, game->background[1], X_ORIGIN, Y_ORIGIN);
        }*/
        vg_draw_xpm((uint32_t *)game->background[0].bytes, game->background[0], X_ORIGIN, Y_ORIGIN);

        draw_all_moles(game->moles, game->num_moles);
        vg_draw_xpm((uint32_t *)game->table.bytes, game->table, SCORE_TABLE_X, SCORE_TABLE_Y);
        vg_draw_xpm((uint32_t *)game->moles_hitted.bytes, game->moles_hitted, MOLES_MISS_FRAME_X, MOLES_MISS_FRAME_Y);
        vg_draw_xpm((uint32_t *)game->moles_missed.bytes, game->moles_missed, MOLES_HIT_FRAME_X, MOLES_HIT_FRAME_Y);
        draw_number(game->numbers_font, 13, MOLES_MISS_NUM_X, MOLES_MISS_NUM_Y, game->player->missed_moles, false);
        draw_number(game->numbers_font, 13, MOLES_HIT_NUM_X, MOLES_HIT_NUM_Y, game->player->hitted_moles, false);
        vg_draw_xpm((uint32_t *)game->clock_icon.bytes, game->clock_icon, CLOCK_ICON_X, CLOCK_ICON_Y);
        draw_number(game->game_time_numbers_font, 10, GAME_TIMER_X, GAME_TIMER_Y, time_duration / GAME_FPS, true);
        draw_cursor(game->cursor);
        time_duration++;
        bool created_mole = false;
        for (uint8_t mole_index = 0; mole_index < 6; mole_index++)
        {
            Mole *ml = &game->moles[mole_index];
            //if (rand() % 10000 > MOLE_PROBABILITY - (((timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * PROBABILITY_LIMIT_DECREMENT))
            if (game->host && !created_mole && ml->position == HIDED && (rand() % 10000 > MOLE_PROBABILITY_MULTIPLAYER))
            {
                ml->position = UP_1;
                ml->time_up = 0;
                created_mole = true;
                ser_send_byte(MOLE_UP | mole_index);
            }
            else if (ml->position == UP_4)
            {
                ml->time_up++;
                //if (ml->time_up >= (MOLE_UP_TIME - ((double)(timer_counter / (sys_hz() / GAME_FPS)) / GAME_DURATION) * TIME_UP_LIMIT_DECREMENT))
                if (ml->time_up >= MOLE_UP_TIME)
                {
                    ml->position = DOWN_MISSED_4;
                    ml->time_up = 0;
                }
            }
            else if (ml->position != HIDED)
            {
                ml->position++;
                if (ml->position == DOWN_MISSED_1 || ml->position == DOWN_HIT_1)
                    ml->position = HIDED;
            }
        }
        if (time_duration / GAME_FPS >= game->game_time)
        {
            ser_send_byte_wait(GAME_OVER_BYTE);
            game->game_state = GAME_OVER;
            game->cursor->cursor_image = game->cursor->cursor_image_default;
        }
        break;
    case KEYBOARD:
        for (uint8_t mole_index = 0; mole_index < 6; mole_index++)
        {
            mole_position = game->moles[mole_index].position;
            if (game->moles[mole_index].kbd_key == kbd_manager(scan_code))
            {
                if (mole_position == UP_4)
                {
                    ser_send_byte(MOLE_DOWN | mole_index);
                    game->moles[mole_index].time_up = 0;
                    game->moles[mole_index].position = DOWN_HIT_4;

                    game->player->hitted_moles++;
                    break;
                }
                else
                {
                    game->player->missed_moles++;
                }
            }
        }
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, game->cursor);
        if (mouse_event.type == LB_RELEASED)
        {
            game->player->missed_moles++;
            for (uint8_t mole_index = 0; mole_index < game->num_moles; mole_index++)
            {
                if (game->moles[mole_index].position == UP_4 && check_over_mole(&game->moles[mole_index], game->cursor->x, game->cursor->y) && mouse_event.type == LB_RELEASED)
                {
                    ser_send_byte(MOLE_DOWN | mole_index);
                    game->moles[mole_index].time_up = 0;
                    game->moles[mole_index].position = DOWN_HIT_4;
                    game->player->hitted_moles++;
                    game->player->missed_moles--;
                    break;
                }
            }
        }
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        if (ser_byte & IRRELEVANT_BYTE)
        {
            ser_byte = UNREQUESTED_BYTE;
            break;
        }
        else if (ser_byte == GAME_OVER_BYTE)
        {
            ser_send_byte_wait(GAME_OVER_BYTE);
            game->game_state = GAME_OVER;
            game->opponent_end = true;
            game->cursor->cursor_image = game->cursor->cursor_image_default;
            break;
        }
        else if (ser_byte & MOLE_UP)
        {
            if (game->moles[ser_byte & MOLE_NO].position == HIDED)
            {
                game->moles[ser_byte & MOLE_NO].position = UP_1;
                game->moles[ser_byte & MOLE_NO].time_up = 0;
            }
        }
        else
        {
            if (game->moles[ser_byte & MOLE_NO].position == UP_4)
            {
                game->moles[ser_byte & MOLE_NO].time_up = 0;
                game->moles[ser_byte & MOLE_NO].position = DOWN_HIT_4;
            }
        }
        break;
    default:
        break;
    }
}

void Game_Over_interrupt_handler(device device, WhacAMole *game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    static int y_auxiliar = 0;
    GameOver *game_over = game->game_over;
    switch (device)
    {
    case TIMER:
        if (game->multiplayer && game->opponent_end)
        {
            ser_send_byte_wait(game->player->hitted_moles);
            break;
        }
        if (game->multiplayer)
            break;
        if (add_new_score(game->leaderboard, game->player) && !game_over->new_score)
        {
            game_over->new_score = true;
        }
        vg_draw_xpm((uint32_t *)game->background[0].bytes, game->background[0], X_ORIGIN, Y_ORIGIN);
        vg_draw_xpm((uint32_t *)game_over->logo_game_over.bytes, game_over->logo_game_over, GM_OV_LOGO_X, GM_OV_LOGO_Y);
        vg_draw_xpm((uint32_t *)game_over->hitted_moles.bytes, game_over->hitted_moles, HIT_MOLES_TITLE_X, HIT_MOLES_TITLE_Y);
        vg_draw_xpm((uint32_t *)game_over->missed_moles.bytes, game_over->missed_moles, MISS_MOLES_TITLE_X, MISS_MOLES_TITLE_Y);
        draw_number(game->numbers_font, 13, HIT_MOLES_CT_X, HIT_MOLES_CT_Y, game->player->hitted_moles, false);
        draw_number(game->numbers_font, 13, MISS_MOLES_CT_X, MISS_MOLES_CT_Y, game->player->missed_moles, false);
        for (int i = 0; i < game_over->num_buttons; i++)
        {
            draw_button(game_over->buttons[i]);
        }
        draw_cursor(game->cursor);
        if (game_over->new_score)
        {
            y_auxiliar += 25;
            vg_draw_xpm((uint32_t *)game_over->ballon.bytes, game_over->ballon, BALLON_X, BALLON_Y - y_auxiliar);
        }
        break;
    case KEYBOARD:
        break;
    case MOUSE:
        if (game->multiplayer)
        {
            break;
        }
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, game->cursor);
        update_buttons(game->cursor, game->game_over->buttons, game->game_over->num_buttons);
        if (game->game_over->buttons[0]->state == ACTIVE && mouse_event.type == LB_RELEASED)
        {
            game->player_settings = load_player_settings();
            game->game_state = MAIN_MENU;
            reset_moles(game->moles, game->num_moles);
            game->player->hitted_moles = 0;
            game->player->missed_moles = 0;
        }
        if (game->game_over->buttons[1]->state == ACTIVE && mouse_event.type == LB_RELEASED)
        {
            game->game_state = LEADERBOARD;
            reset_moles(game->moles, game->num_moles);
            game->player->hitted_moles = 0;
            game->player->missed_moles = 0;
        }
        if (game->game_over->buttons[2]->state == ACTIVE && mouse_event.type == LB_RELEASED)
            game->game_state = EXIT;
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        if (ser_byte & IRRELEVANT_BYTE)
        {
            ser_byte = UNREQUESTED_BYTE;
            break;
        }
        else if (ser_byte == GAME_OVER_BYTE)
        {
            game->opponent_end = true;
            ser_byte = UNREQUESTED_BYTE;
            break;
        }
        if (ser_byte < MAX_NO_MOLES)
        {
            if (ser_byte > game->player->hitted_moles)
                game->game_state = LOST;
            else
                game->game_state = WIN;
        }
        break;
    }
}

void Leaderboard_interrupt_handler(device device, WhacAMole *game)
{
    struct mouse_ev mouse_event;
    struct packet new_packet;
    Leaderboard *leaderboard = game->leaderboard;
    switch (device)
    {
    case TIMER:
        vg_draw_xpm((uint32_t *)game->background[1].bytes, game->background[1], X_ORIGIN, Y_ORIGIN);
        draw_leaderboard_table(leaderboard);
        draw_buttons(leaderboard->buttons, leaderboard->num_buttons);
        draw_player_names(game->letters_small_font, leaderboard->score_records, leaderboard->num_score_records);
        draw_player_scores(leaderboard);
        draw_player_dates(leaderboard);
        draw_cursor(game->cursor);
        break;
    case KEYBOARD:
        break;
    case MOUSE:
        mouse_parse_packet(packet, &new_packet);
        mouse_event = mouse_get_event(&new_packet);
        move_cursor(&new_packet, game->cursor);
        update_buttons(game->cursor, leaderboard->buttons, leaderboard->num_buttons);
        if (leaderboard->buttons[0]->state == ACTIVE && mouse_event.type == LB_RELEASED)
            game->game_state = MAIN_MENU;
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        ser_byte = UNREQUESTED_BYTE;
        break;
    }
}

void Exit_interrupt_handler(device device, WhacAMole *game)
{
    static int exit_time = 0;
    Exit *exit = game->exit;
    switch (device)
    {
    case TIMER:
        exit_time++;
        if ((exit_time / GAME_FPS) < exit->animation_timer)
        {
            vg_draw_xpm((uint32_t *)game->background[0].bytes, game->background[0], X_ORIGIN, Y_ORIGIN);
            vg_draw_xpm((uint32_t *)exit->logo.bytes, exit->logo, GD_BYE_LOGO_X, GD_BYE_LOGO_Y);
            vg_draw_xpm((uint32_t *)exit->credits.bytes, exit->credits, CREDITS_X, CREDITS_Y);
            if (timer_counter % 5 == 0)
                exit->x_mole += 18;
            vg_draw_xpm((uint32_t *)exit->mole.bytes, exit->mole, exit->x_mole, MOLE_ANIMATION_Y);
        }
        else
        {
            save_scores(game->leaderboard);
            game->running = false;
        }
        break;
    case KEYBOARD:
        break;
    case MOUSE:
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        ser_byte = UNREQUESTED_BYTE;
        break;
    }
}

void Win_interrupt_handler(device device, WhacAMole *game)
{
    switch (device)
    {
    case TIMER:
        if(!game->sent_hitted_moles){
            ser_send_byte_wait(game->player->hitted_moles);
            game->sent_hitted_moles = true;
        }
        vg_draw_xpm((uint32_t *)game->background[0].bytes, game->background[0], X_ORIGIN, Y_ORIGIN);
        vg_draw_xpm((uint32_t *)game->game_win.bytes, game->game_win, WIN_LOSE_X, WIN_LOSE_Y);
        break;
    case KEYBOARD:
        if (scan_code == ESC_BREAK)
            game->game_state = EXIT;
        break;
    case MOUSE:
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        ser_byte = UNREQUESTED_BYTE;
        break;
    }
}

void Lost_interrupt_handler(device device, WhacAMole *game)
{
    switch (device)
    {
    case TIMER:
        if(!game->sent_hitted_moles){
            ser_send_byte_wait(game->player->hitted_moles);
            game->sent_hitted_moles = true;
        }
        vg_draw_xpm((uint32_t *)game->background[0].bytes, game->background[0], X_ORIGIN, Y_ORIGIN);
        vg_draw_xpm((uint32_t *)game->game_lost.bytes, game->game_lost, WIN_LOSE_X, WIN_LOSE_Y);
        break;
    case KEYBOARD:
        if (scan_code == ESC_BREAK)
            game->game_state = EXIT;
        break;
    case MOUSE:
        break;
    case RTC:
        break;
    case UART:
        ser_ih();
        ser_byte = UNREQUESTED_BYTE;
        break;
    }
}

void draw_number(xpm_image_t font, int font_info, int xi, int yi, int number, bool left_number_drawing)
{
    int left_number = number / 10;
    int right_number = number % 10;
    int number_width = font.width / font_info;
    int number_height = font.height;
    if (left_number != 0)
        vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi, yi, left_number * number_width, (left_number + 1) * number_width, 0, number_height);
    else if (left_number_drawing)
        vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi, yi, left_number * number_width, (left_number + 1) * number_width, 0, number_height);
    vg_draw_part_of_xpm((uint32_t *)font.bytes, font, xi + number_width, yi, right_number * number_width, (right_number + 1) * number_width, 0, number_height);
}
