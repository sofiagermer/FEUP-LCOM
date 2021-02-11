#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

//extern variable to be incremented on timer.c, void (timer_int_handler)
extern unsigned int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv)) //chama timer_test_int()
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  
  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  // testing the timer argument: 0,1,2 are the only valid timers
  if(timer < 0 || timer > 2){
    printf("timer_test_read_config::Invalid input, timer index out of range!\n"); return 1;
  }
  uint8_t status; // will hold the timer status
  // checks if timer_get_conf was successful
  if (timer_get_conf(timer, &status) != OK) { return 1; } 
  // checks if timer_display_conf was successful
  if (timer_display_conf(timer, status, field) != OK) { return 1; } 
  
  return 1;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  // testing the timer argument: 0,1,2 are the only valid timers
  if(timer < 0 || timer > 2){
    printf("timer_test_read_config::Invalid input, timer index out of range!\n"); return 1;
  }
  if (timer_set_frequency(timer, freq) != OK) { return 1; }

  return 0;
}

int(timer_test_int)(uint8_t time) {
    if (time == 0) {
        printf("timer_test_int::Time interval cannot be zero\n");
        return 1;
    }
    int ipc_status;
    message msg;
    uint8_t bit = 0;
    unsigned long r;
    if (timer_subscribe_int(&bit) != 0) {
        printf("ERROR: Subscribe failed");
        return 1;
    }
    while (!(time <= counter / 60)) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: // hardware interrupt notification
                    if (msg.m_notify.interrupts & bit) { // subscribed interrupt BIT MASK
                        timer_int_handler();
                        if (counter % 60 == 0) { //invoke once per second the timer_print_elapsed_time()
                            timer_print_elapsed_time();
                        }
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    if (timer_unsubscribe_int() != 0) {
        printf("ERROR: Unsubscribe failed");
        return 1;    
    }

    return 0;
}

