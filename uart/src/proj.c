// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "test7.h"
#include "serial_port.h"
#include "uart_const.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
	
	// if(ser_set_conf(5,1,-1,115200)){
  //   printf("ERRO test_set\n");
  //   return 1;
  // }


  ser_test_conf(COM1_ADDR);
  if(ser_start_proj_config()){
    printf("ERRO test_set\n");
    return 1;
  }
  // int i= 1;
  // while((i--) > 0){
  //   if(ser_send_byte("|"[0])) return 1;
  // }


  int no_tries =50;
  uint8_t mensagem[] = "ola bom dia\n";
  while((no_tries--)>0){
      tickdelay(micros_to_ticks(200));
      ser_send_info(mensagem,12);
  }
  
  ser_test_conf(COM1_ADDR);

  test();


  // if (vggg_init(0x115) == NULL)
  //   return 1;
  

  // WhacAMole* new_game = load_game();

  
  // game_main_loop(new_game);

  // vg_exit();

    // int ipc_status;
    // message msg;
    // uint8_t bit_no = 0;
    // unsigned long r;

    // if (ser_subscribe_int(&bit_no) != 0) {
    //     printf("ERROR: Subscribe failed");
    //     return 1;
    // }

    // extern uint8_t ser_byte;
    // extern bool error_reading;
    // uint8_t irq_uart = BIT(bit_no);

    // while (!error_reading) { // para nao durar para sempre, nao relevante
    //     if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
    //         printf("driver_receive failed with: %d", r);
    //         continue;
    //     }

    //     if (is_ipc_notify(ipc_status)) { // received notification
    //         switch (_ENDPOINT_P(msg.m_source)) {
    //             case HARDWARE: // hardware interrupt notification
    //                 if (msg.m_notify.interrupts & irq_uart) { // subscribed interrupt BIT MASK
    //                     ser_ih();
    //                     printf("ola\n");
    //                 }
    //                 break;
    //             default:
    //                 break; /* no other notifications expected: do nothing */
    //         }
    //     } else { /* received a standard message, not a notification */
    //         /* no standard messages expected: do nothing */
    //     }
    // }

    // if (ser_unsubscribe_int() != 0) {
    //     printf("ERROR: Unsubscribe failed");
    //     return 1;    
    // }

  return OK;
}



