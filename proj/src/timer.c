#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

unsigned int timer_counter = 1; // declared extern at lab2.c, global variable to count interrupts from the timer
int timer_hook_id = TIMER0_IRQ; // global variable used in subscribe_int and unsubscribe_int

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t status = 0, controlWord, lsb, msb;
  uint16_t initial_cont_value;
  if(freq > TIMER_FREQ || freq < TIMER_FREQ_MIN)
  {
    printf("timer_set_frequency::Invalid input, frequency must be between (19 and 1193182)Hz\n");
    return 1;
  }

  if (timer_get_conf(timer, &status) != OK) 
  {
    printf("timer_set_frequency::ERROR in getting the timer initial configuration!\n");
    return FAIL;
  } //Reading the timer initial configuration

  status &= LST_4_BITS;  //the first 4 bits from the previous configuration are preserved
  controlWord = status | TIMER_LSB_MSB; //setting up the read back command 
  
  switch (timer) { 
    case 0:
      controlWord |= TIMER_SEL0;
      break;
    case 1:
      controlWord |= TIMER_SEL1;
      break;
    case 2:
      controlWord |= TIMER_SEL2;
      break;
  }
  
  //Writing the new configuration
  if (sys_outb(TIMER_CTRL, controlWord) != OK) 
  { 
    printf("timer_set_frequency::ERROR in writing the new timer configuration!\n");
    return FAIL; 
  } 
  initial_cont_value = TIMER_FREQ / freq;
  if (util_get_LSB(initial_cont_value, &lsb) != OK) { return 1; }
  if (util_get_MSB(initial_cont_value, &msb) != OK) { return 1; }

  //writing lsb followed by the msb values of the frequency to the timer port
  if(sys_outb(TIMER_PORT_SEL(timer), lsb)) {
    printf("timer_set_frequency::ERROR in writing the lsb value to the timer port!\n");
    return FAIL; 
  }
  if(sys_outb(TIMER_PORT_SEL(timer), msb)) {
    printf("timer_set_frequency::ERROR in writing the msb value to the timer port!\n");
    return FAIL; 
  }

  return OK;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    *bit_no = BIT(timer_hook_id);
    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id)!= OK){
      printf("timer_subscribe_int::ERROR in setting policy !\n");
      return FAIL;
    }
    return OK;
}

int (timer_unsubscribe_int)() {
    if(sys_irqrmpolicy(&timer_hook_id)!= OK){
      printf("timer_unsubscribe_int::ERROR in disabling IQR line!\n");
      return FAIL;
    }
    return OK;
}

void (timer_int_handler)() {
    timer_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  // testing the timer argument: 0,1,2 are the only valid timers
  if(timer < 0 || timer > 2){
    printf("timer_get_conf::Invalid input, timer index out of range!\n"); 
    return FAIL;
  }
  // writing the ReadBack Command
  uint8_t control_word = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); 
  if (sys_outb(TIMER_CTRL, control_word) != OK) {
	  printf("timer_get_conf::ERROR writing the control_word to the timer control register!\n"); 
    return FAIL;
  }

  char TIMER; // auxiliar to hold the port of the timer pretended
  //selects wich port will be read based on the timer passed as an argument
  switch (timer) { 
    case 0:
      TIMER = TIMER_0;
      break;
    case 1:
      TIMER = TIMER_1;
      break;
    case 2:
      TIMER = TIMER_2;
      break;
  }

  if (util_sys_inb(TIMER, st) != OK) {
	  printf("timer_get_conf::ERROR receiving the output from the timer!\n"); 
    return FAIL; 
  }
	return OK;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
	// testing the timer argument: 0,1,2 are the only valid timers
  if(timer < 0 || timer > 2){
    printf("timer_display_conf::Invalid input, timer index out of range!\n"); 
    return FAIL;
  }

  union timer_status_field_val config; // will hold the info about the configuration asked
	
  if (field == tsf_all) {              // configuration in hexadecimal 
    config.byte = st;
  }
  else if (field == tsf_initial) {     // timer initialization mode 
    // select bits 4 and 5 
    switch (st & TIMER_LSB_MSB) {
      case TIMER_LSB:
        config.in_mode = LSB_only; 
        break;
      
      case TIMER_MSB:
        config.in_mode = MSB_only;
        break;
      
      case TIMER_LSB_MSB:
        config.in_mode = MSB_after_LSB;
        break;
      
      default:
        config.in_mode = INVAL_val;
    }
  }
  else if (field == tsf_mode) {       // timer counting mode 
     //selecting bits 3, 2 and 1
      switch(st & TIMER_OPERATING_MODE) {
        case(TIMER_INT_TERM_COUNT):    // MODE 0
          config.count_mode = 0;
          break;
      
        case(TIMER_HW_RETRIG_ONE_SH):    // MODE 1 
          config.count_mode = 1;
          break;
        
        case(TIMER_RATE_GEN):          // MODE 2 
          config.count_mode = 2;
          break;
        
        case(BIT(3) | TIMER_RATE_GEN): // because of don't care bits MODE 2 can also be 110
          config.count_mode = 2; 
          break;

        case(TIMER_SQR_WAVE):          // MODE 3 
          config.count_mode = 3;
          break;

        case(BIT(3) | TIMER_SQR_WAVE): //because of don't care bits MODE 3 can also be 111
          config.count_mode = 3; 
          break;

        case(TIMER_SW_TRIG_STROBE):   // MODE 4
          config.count_mode = 4;
          break;

        case(TIMER_HW_TRIG_STROBE):   // MODE 5 
          config.count_mode = 5;
          break;
      }
  } 
  else if (field == tsf_base) {                              // timer counting base 
    if (st & TIMER_BCD) config.bcd = true; 
    else config.bcd = false;
  }

  else {
    printf("timer_display_config::Invalid field to be read!\n");
    return FAIL;
  }

  if (timer_print_config(timer, field, config) != OK) { 
    printf("timer_display_config::ERROR in timer_print_config!\n");
    return FAIL;
  }
  return OK;
}

