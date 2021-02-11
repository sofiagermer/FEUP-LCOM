// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "mouse.h"

extern uint8_t packet[];               //array of bytes, packet read from the mouse
extern bool mouse_last_byte_of_packet; //signals that the last byte of a packet was read
extern unsigned int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  int ipc_status, r;
  message msg;
  uint16_t mouse_id;
  
  if (issue_cmd_to_kbc(WRITE_BYTE_TO_MOUSE,EN_DATA_REPORT) != OK) {
    printf("ERROR::Unable to enable data report!\n");
    return FAIL;
  }
  //mouse_enable_data_reporting();

  if (mouse_subscribe_int(&mouse_id) != OK) {
    printf("ERROR: Subsribe failed!\n");
    return FAIL;
  }

  while (cnt > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                              // hardware interrupt notification
          if (msg.m_notify.interrupts & mouse_id) { // subscribed interrupt BIT MASK
            mouse_ih();
            if (mouse_last_byte_of_packet) {
              cnt--;
              struct packet new_packet;
              mouse_parse_packet(packet, &new_packet);
              mouse_print_packet(&new_packet);
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int() != OK) {
    printf("ERROR::Unsubsribe failed!\n");
    return FAIL;
  }

  if (issue_cmd_to_kbc(WRITE_BYTE_TO_MOUSE,DIS_DATA_REPORT) != OK) {
    printf("ERROR::Unable to enable data report!\n");
    return FAIL;
  }

  return OK;
}

int(mouse_test_async)(uint8_t idle_time) {
int ipc_status, r;
  message msg;
  uint16_t mouse_id;
  uint8_t timer_id;

  if (issue_command_to_mouse(EN_DATA_REPORT) != OK) {
    printf("ERROR::Unable to enable data report!\n");
    return FAIL;
  }

  if (mouse_subscribe_int(&mouse_id) != OK) {
    printf("ERROR: Subsribe failed!\n");
    return FAIL;
  }

  if (timer_subscribe_int(&timer_id) != OK) {
    printf("ERROR: Subsribe failed!\n");
    return FAIL;
  }

  while (counter < (idle_time*60)) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                              // hardware interrupt notification
          if (msg.m_notify.interrupts & mouse_id) { // subscribed interrupt BIT MASK
            mouse_ih();
            counter = 0;

            if (mouse_last_byte_of_packet) {
              struct packet new_packet;
              mouse_parse_packet(packet, &new_packet);
              mouse_print_packet(&new_packet);
            }
          }

          if (msg.m_notify.interrupts & timer_id) { // subscribed interrupt BIT MASK
            timer_int_handler();
          }

          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }

  if (timer_unsubscribe_int() != OK) {
    printf("ERROR::Unsubsribe failed!\n");
    return FAIL;
  }

  if (mouse_unsubscribe_int() != OK) {
    printf("ERROR::Unsubsribe failed!\n");
    return FAIL;
  }

  if (issue_command_to_mouse(DIS_DATA_REPORT) != OK) {
    printf("ERROR::Unable to disable data report!\n");
    return FAIL;
  }

  return OK;
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status, r;
  message msg;
  uint16_t mouse_id;
  bool gesture_complete = false;
  mouse_last_byte_of_packet = false;

  if (issue_command_to_mouse(EN_DATA_REPORT) != OK) 
    return FAIL;

  if (mouse_subscribe_int(&mouse_id) != OK) {
    printf("ERROR: Subsribe failed!\n");
    return FAIL;
  }

  while (!gesture_complete) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                              // hardware interrupt notification
          if (msg.m_notify.interrupts & mouse_id) { // subscribed interrupt BIT MASK
            mouse_ih();
            if(mouse_last_byte_of_packet) {
              struct packet new_packet;
              mouse_parse_packet(packet, &new_packet);
              mouse_print_packet(&new_packet);
              gesture_complete = drawing_handler(x_len, tolerance, &new_packet);
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int() != OK) {
    printf("ERROR::Unsubsribe failed!\n");
    return FAIL;
  }

  if (issue_command_to_kbc(WRITE_BYTE_TO_MOUSE, DIS_DATA_REPORT) != OK) {
    printf("ERROR::Unable to disable data report!\n");
    return FAIL;
  }

  return OK;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  
  if (issue_command_to_kbc(WRITE_BYTE_TO_MOUSE, READ_DATA) != OK) {
      printf("ERROR::Unable to send read data command!\n");
      return FAIL;
  }

  while (cnt > 0) {
    
    if (mouse_check_status_register() == OK & output_full() == OK) {
      mouse_ih();

      if (mouse_last_byte_of_packet) {
        if (issue_command_to_kbc(WRITE_BYTE_TO_MOUSE, READ_DATA) != OK) {
          printf("ERROR::Unable to send read data command!\n");
          return FAIL;
        }
        cnt--;
        struct packet new_packet;
        mouse_parse_packet(packet, &new_packet);
        mouse_print_packet(&new_packet);
      }
    }

    tickdelay(micros_to_ticks(period * 1000));
  }
  
  if (issue_command_to_kbc(WRITE_BYTE_TO_MOUSE, DIS_DATA_REPORT) != OK) {
    printf("ERROR::Unable to disable data report!\n");
    return FAIL;
  }
  
  if (issue_command_to_kbc(WRITE_BYTE_TO_MOUSE, SET_STREAM_MODE) != OK) {
    printf("ERROR::Unable to set stream mode!\n");
    return FAIL;
  }

  uint32_t default_cmd_byte = (uint32_t) minix_get_dflt_kbc_cmd_byte();

  if (issue_command_to_kbc(WRITE_CMD_BYTE, default_cmd_byte) != OK) {
    printf("ERROR::Unable to write default command byte!\n");
    return FAIL;
  }

  return OK;
}
