#pragma once
#include <lcom/lcf.h>

int cnt_sys_inb(port_t port, uint8_t *byte);

int kbd_subscribe_int(uint8_t *bit_no);

int kbd_unsubscribe_int();

void read_status_register(uint8_t *stat);

int check_status_register();

int output_full();

int input_empty();

int read_out_buffer(uint8_t *info);

void (kbc_ih)(void);

int issue_cmd_to_kbc(uint8_t command, uint8_t argument);

int kbc_write_cmd(uint8_t command);

int kbc_write_argument(uint8_t argument);
