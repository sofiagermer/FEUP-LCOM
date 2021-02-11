#ifndef _LCOM_UART__H_
#define _LCOM_UART__H_

#include <lcom/lcf.h>

void test();

int ser_save_init_conf();

int ser_restore_init_conf();

int ser_start_proj_config();

int ser_set_conf(unsigned long bits, unsigned long stop, 
	     long parity, /* -1: none, 0: even, 1: odd */
	     unsigned long rate);


int (ser_subscribe_int)(uint8_t* bit_no);

int (ser_unsubscribe_int)();

int ser_can_read();

bool ser_error_read();

bool ser_can_send();

int ser_send_byte(uint8_t data);

int ser_send_info(uint8_t* data, unsigned int length);

int ser_read_byte(uint8_t* data);

void ser_ih();

#endif /* _LCOM_UART_H */
