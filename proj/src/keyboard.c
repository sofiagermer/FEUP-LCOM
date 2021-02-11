#include <lcom/lcf.h>
#include <stdint.h>

#include "keyboard.h"
#include "i8042.h"


static int kbd_hook_id = KBD_IRQ; //hook id used for the keyboard

bool keyboard_done_getting_scancodes = false; //signals that there is one more byte to read from the outbuuf
uint8_t bytes_read[2];
int scan_code_size=0;
uint8_t scan_code;


int kbd_subscribe_int(uint8_t *bit_no) {
  *bit_no = BIT(kbd_hook_id);
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != OK) {
    printf("kbd_subscribe_int::ERROR in setting policy!\n");
    return FAIL;
  }
  return OK;
}


int kbd_unsubscribe_int() {
  if (sys_irqrmpolicy(&kbd_hook_id) != OK) {
    printf("kbd_unsubscribe_int::ERROR removing policy!\n");
    return FAIL;
  }
  return OK;
}


void read_status_register(uint8_t *stat) {
  if (util_sys_inb(STAT_REG, stat) != OK) {
    printf("ERROR::Unable to read keyboard status register!\n");
  }
}


int check_status_register() {
  uint8_t temp=0; //hold the status
  read_status_register(&temp);
  if ((temp & (KBD_PAR_ERROR | KBD_TIME_ERROR | KBD_AUX)) != 0) {
      return FAIL;
    }
  return OK;
}


int output_full() {
    uint8_t st;
    read_status_register(&st);
    if(st & KBD_OBF)
      return OK;
    return FAIL;
}


int input_empty() {
    uint8_t st;
    read_status_register(&st);
    if(st & KBD_IBF)
      return FAIL;
    return OK;
}


int read_out_buffer(uint8_t *info) {
  if (util_sys_inb(OUT_BUFF, info) != OK) {
    printf("ERROR::Error reading the out buffer!\n");
    return FAIL;
  }
  return OK;
}


void(kbc_ih)(void) {
  if (keyboard_done_getting_scancodes) {
      keyboard_done_getting_scancodes = false;
      scan_code_size = 0;
  }
  // checks if the output buffer is full  
  if (output_full()==OK) {

    //reads scan code from output buffer               
    if (read_out_buffer(&scan_code) != OK) { 
      scan_code = 0;
      return;                                
    }

    //checks if there is signal of an error
    if (check_status_register() != OK) { 
      // discards the scan code
      scan_code = 0;                                      
      return;                                          
    }

    bytes_read[scan_code_size++] = scan_code;

    if (scan_code != TWO_BYTES_CODE) 
      keyboard_done_getting_scancodes = true;
  }
}


int issue_cmd_to_kbc(uint8_t command, uint8_t argument) {
  uint8_t tries=4;
  
  while (tries > 0) {
    if (kbc_write_cmd(command) != OK) {
      printf("ERROR::writing the command!\n");
      return FAIL;
    }

    if (command == WRITE_CMD_BYTE) {
      if (kbc_write_argument(argument) != OK)
        printf("ERROR::writing the new command byte!\n");
        return FAIL;
    }

    tries--;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  printf(" issue_cmd_to_kbc ");
  printf("After 4 tries, kbc was not ready to receive the command\n");
  return FAIL;
}


int kbc_write_cmd(uint8_t command) {
  uint8_t status, tries=4;
  if (util_sys_inb(STAT_REG,&status) != OK) {
    printf("ERROR::Unable to read the status register!\n");
    return 1;
  }

  while (tries > 0) {
    if ((status & KBD_IBF) == 0) {

      if (sys_outb(CMD_REG, command) != OK) 
        return 1;

      return OK;
    }

    tries--;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  printf("kbc_write_cmd");
  printf("After 4 tries, kbc was not ready to receive the command\n");
  return 1;
}

int kbc_write_argument(uint8_t argument) {
  uint8_t status, tries = 4;
  if (util_sys_inb(STAT_REG,&status) != OK ) {
    printf("ERROR::Unable to read the status register!\n");
    return 1;
  }

  while (tries > 0) {
    if((status & KBD_IBF) == 0) {

      if (sys_outb(CMD_ARG_REG,argument) != OK) {
        return 1;
      }

      return OK;
    }

    tries--;
    tickdelay(micros_to_ticks(DELAY_US));
  }

  printf("After 4 tries, kbc was not ready to receive the argument\n");
  return 1;
}



