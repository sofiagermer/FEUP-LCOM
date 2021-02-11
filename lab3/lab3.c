#include <lcom/lab3.h>
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"
extern uint32_t sys_inb_counter;
extern unsigned int counter;

extern bool keyboard_done_getting_scancodes; //signals that there is one more byte to read from the outbuuf
extern uint8_t bytes_read[2];
extern int scan_code_size;
extern uint8_t scan_code;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t kbd_id;
  int ipc_status, r;
  message msg;

  if (kbd_subscribe_int(&kbd_id) != OK) {
    printf("ERROR: Subsribe failed!\n");
    return 1;
  }

  while (scan_code != ESC_BREAK_KEY) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                            // hardware interrupt notification
          if (msg.m_notify.interrupts & kbd_id) { // subscribed interrupt BIT MASK
            kbc_ih();                         // reads the scan code and checks if there were some error

            if (keyboard_done_getting_scancodes) {
              if (kbd_print_scancode(!(scan_code & BREAK_CODE), scan_code_size, bytes_read) != OK) {
                printf("ERROR::Failure in kbd_print_scancode()!\n");
                return 1;
              }
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

  if (kbd_unsubscribe_int() != OK) {
    printf("ERROR: Unsubsribe failed!\n");
    return 1;
  }

  if (kbd_print_no_sysinb(sys_inb_counter) != OK) {
    sys_inb_counter = 0;
    printf("ERROR: Printing number of sys_inb calls failed!\n");
    return 1;
  }

  return 0;
}

int(kbd_test_poll)() {
  while (scan_code != ESC_BREAK_KEY) {

    if (check_status_register() == OK) { //polls the status register and sees if its ok to read the OB
      kbc_ih();

      if (keyboard_done_getting_scancodes) {
        if (kbd_print_scancode(!(scan_code & BREAK_CODE), scan_code_size, bytes_read) != OK) {
          printf("ERROR::Failure in kbd_print_scancode()!\n");
          return 1;
        }
      }
    }
  }

  if (kbd_print_no_sysinb(sys_inb_counter) != OK) {
    sys_inb_counter = 0;
    printf("ERROR: Printing number of sys_inb calls failed!\n");
    return 1;
  }

  uint8_t old_command_byte;
  uint8_t new_command_byte;

  if (issue_command(READ_CMD_BYTE, 0) != OK) {
    printf("ERROR:Unable to write command to KBC!\n");
    return 1;
  }

  if (read_out_buffer(&old_command_byte) != OK) {
    printf("ERROR:Unable to read command byte from buffer!\n");
    return 1;
  }

  new_command_byte = old_command_byte | CMD_BYTE_EN_INT_K;

  if (issue_command(WRITE_CMD_BYTE, new_command_byte) != OK) {
    printf("ERROR:Unable to write command to KBC!\n");
    return 1;
  }

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t kbd_id, timer_id;
  int ipc_status, r;
  message msg;
  
  if (n < 1) {
    printf("ERROR:Invalid input, n should be higher than 1!\n");
    return 1;
  }

  if (kbd_subscribe_int(&kbd_id) != OK) {
    printf("ERROR: Subsribe failed!\n");
    return 1;
  }

  if (timer_subscribe_int(&timer_id) != OK) {
    printf("ERROR: Subsribe failed!\n");
  }

  while (scan_code != ESC_BREAK_KEY && counter < (n*60)) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                            // hardware interrupt notification
          if (msg.m_notify.interrupts & kbd_id) { // subscribed interrupt BIT MASK
            kbc_ih();                             // reads the scan code and checks if there were some error

            if (keyboard_done_getting_scancodes) {
              if (kbd_print_scancode(!(scan_code & BREAK_CODE), scan_code_size, bytes_read) != OK) {
                printf("ERROR::Failure in kbd_print_scancode()!\n");
                return 1;
              }
            }
            counter = 0;
          }

          if (msg.m_notify.interrupts & timer_id) {
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

  if (kbd_unsubscribe_int() != OK) {
    printf("ERROR: Unsubsribe failed!\n");
    return 1;
  }

  if (timer_unsubscribe_int() != OK) {
    printf("ERROR: Unsubsribe failed!\n");
    return 1;
  }

  return 0;
}
