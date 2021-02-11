#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "mouse.h"

static int mouse_hook_id = MOUSE_IRQ;   //hook id used for the mouse
uint8_t packet[3];                      //array of bytes, packet read from the mouse
bool mouse_last_byte_of_packet = false; //signals that the last byte of a packet was read
uint8_t delta_x, delta_y;
bool left_press = false, right_press = false, mid_press = false;

int mouse_subscribe_int(uint16_t *bit_no) {
  *bit_no = BIT(mouse_hook_id);
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK) {
    printf("mouse_subscribe_int::ERROR in setting policy!\n");
    return FAIL;
  }
  return OK;
}

int mouse_unsubscribe_int() {
  if (sys_irqrmpolicy(&mouse_hook_id) != OK) {
    printf("kbd_unsubscribe_int::ERROR removing policy!\n");
    return FAIL;
  };
  return OK;
}

void mouse_read_status_register(uint8_t *stat) {
  if (util_sys_inb(STAT_REG, stat) != OK) {
    printf("ERROR::Unable to read keyboard status register!\n");
  }
}

int mouse_check_status_register() {
  uint8_t temp=0; //hold the status
  mouse_read_status_register(&temp);
  if ((temp & (KBD_PAR_ERROR | KBD_TIME_ERROR)) != 0) {
      return 1;
  }
  return 0;
}

int mouse_output_full() {
  uint8_t st;
  mouse_read_status_register(&st);
  if (st & KBD_OBF && !mouse_check_status_register())
    return OK;
  return FAIL;
}

int mouse_input_empty() {
  uint8_t st;
  mouse_read_status_register(&st);
  if (st & KBD_IBF && !mouse_check_status_register())
    return FAIL;
  return OK;
}

int mouse_read_out_buffer(uint8_t *info) {
  if (util_sys_inb(OUT_BUFF, info) != OK) {
    printf("ERROR::Error reading the out buffer!\n");
    return FAIL;
  }
  return OK;
}

void(mouse_ih)(void) {
  uint8_t aux;
  static uint8_t index = 0;
  if (mouse_last_byte_of_packet) {
    index = 0;
    mouse_last_byte_of_packet = false;
  }
  if (mouse_output_full() == OK) {
    if (mouse_read_out_buffer(&aux) != OK) {
      printf("ERROR::Error reading the out buffer!\n");
      return;
    }
    packet[index++] = aux;
    if (index == 3) {
      mouse_last_byte_of_packet = true;
    }
  }
}

void mouse_parse_packet(uint8_t packet[], struct packet *new_packet) {
  uint8_t first_byte = packet[0];
  new_packet->bytes[0] = packet[0];
  new_packet->bytes[1] = packet[1];
  new_packet->bytes[2] = packet[2];
  new_packet->rb = (first_byte & RB);
  new_packet->lb = (first_byte & LB);
  new_packet->mb = (first_byte & MB);
  new_packet->x_ov = (first_byte & X_OVFL);
  new_packet->y_ov = (first_byte & Y_OVFL);
  new_packet->delta_x = (uint16_t) packet[1];
  new_packet->delta_y = (uint16_t) packet[2];

  if (first_byte & MSB_X_DELTA)
    new_packet->delta_x |= (0xFF<<8);
  

  if (first_byte & MSB_Y_DELTA)
    new_packet->delta_y |= (0xFF<<8);
  
}

int issue_command_to_kbc(uint8_t command, uint8_t arguments) {
  uint8_t attemps = 0; //number of attemps the function will try to issue the command

  //stops after 4 attemps
  while (attemps < 4) {
    attemps++;
    // input buffer should not be full
    if (mouse_input_empty() == OK) {
      //writtes command
      if (sys_outb(CMD_REG, command) == OK) {
        //in case the command is WriteCommandByte it needs the new command (arguments)
        if (command == WRITE_CMD_BYTE) {
          if (sys_outb(CMD_ARG_REG, arguments) == OK) {
            return OK;
          }
        }
        else {
          return OK;
        }
      }
    }

    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 0;
}

int mouse_issue_cmd_to_kbc(uint8_t command, uint8_t argument) {
  uint8_t mouse_response, tries=4;
  
  while (tries > 0) {
    if (mouse_kbc_write_cmd(command) != OK) {
      printf("ERROR::writing the command!\n");
      return 1;
    }

    if (command == WRITE_CMD_BYTE) {
      if (mouse_kbc_write_argument(argument) != OK)
        printf("ERROR::writing the new command byte!\n");
        return 1;
    }

    if (command == WRITE_BYTE_TO_MOUSE) {
      if (mouse_write_command(argument,&mouse_response) != OK) 
        return 1;
      
      if (mouse_response == ACK)
        return OK;
      else if (mouse_response == ERROR)
        return 1;
    }

    tries--;
    tickdelay(micros_to_ticks(DELAY_US));
  }

  printf("After 4 tries, kbc was not ready to receive the command\n");
  return 1;
}

int issue_command_to_mouse(uint8_t command) {
  while (true) {
    if(mouse_issue_cmd_to_kbc(WRITE_BYTE_TO_MOUSE, 0) != OK) 
      return 1;
    if (mouse_input_empty() == OK)
      if(sys_outb(IN_BUFF,command) != OK) 
        return 1;
    uint8_t response;
    //if (output_full() == OK) {
      if (util_sys_inb(OUT_BUFF, &response) != OK)
        return 1;
      if (response == ACK)
        return OK;
      if (response == ERROR)
        return 1;
    //}
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

int mouse_kbc_write_cmd(uint8_t command) {
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

  printf("mouse_kbc_write_cmd\n");
  printf("After 4 tries, kbc was not ready to receive the command\n");
  return 1;
}

int mouse_kbc_write_argument(uint8_t argument) {
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
  printf("mouse_kbc_write_argument \n");
  printf("After 4 tries, kbc was not ready to receive the argument\n");
  return 1;
}

int mouse_write_command(uint8_t command, uint8_t*response) {
  uint8_t status, tries=4;
  if (util_sys_inb(STAT_REG,&status) != OK) {
    printf("ERROR::Unable to read the status register!\n");
    return 1;
  }
  while (tries > 0) {
    if ((status & KBD_IBF) == 0) {
      if (sys_outb(CMD_ARG_REG,command) != OK) {
        return 1;
      }
      tickdelay(micros_to_ticks(DELAY_US));
      
      if (util_sys_inb(OUT_BUFF,response) != OK) {
        return 1;
      }
      printf("OK!\n");
      return OK;
    }

    tries--;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  
  printf("After 4 tries, mouse was not ready\n");
  return 1;
}


struct mouse_ev mouse_get_event(struct packet *packet) {
    
    struct mouse_ev mouse_event;

    mouse_event.delta_x = packet->delta_x;
    mouse_event.delta_y = packet->delta_y;

    if (left_press && !packet->lb && !right_press && !packet->rb && !mid_press && !packet->mb) {
        left_press = false;
        mouse_event.type = LB_RELEASED; 
    }
    else if (!left_press && packet->lb && !right_press && !packet->rb && !mid_press && !packet->mb) {
        left_press = true;
        mouse_event.type = LB_PRESSED;
    }

    else if (!left_press && !packet->lb && right_press && !packet->rb && !mid_press && !packet->mb) {
        right_press = false;
        mouse_event.type = RB_RELEASED; 
    }
    else if (!left_press && !packet->lb && !right_press && packet->rb && !mid_press && !packet->mb) {
        right_press = true;
        mouse_event.type = RB_PRESSED; 
    }

    else if (!mid_press && packet->mb) {
        mid_press = true;
        mouse_event.type = BUTTON_EV; 
    }
    else if (mid_press && !packet->mb) {
        mid_press = false;
        mouse_event.type = BUTTON_EV; 
    }

    else {
        mouse_event.type = MOUSE_MOV;
    }
        
    return mouse_event;
}
