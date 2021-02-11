// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "vd_card.h"
#include "keyboard.h"
#include "i8042.h"

extern unsigned int counter;
extern uint8_t scan_code;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (vggg_init(mode) == NULL)
		return 1;

	tickdelay(micros_to_ticks(delay*1000000));

	if (vg_exit() != OK)
		return 1;

	return OK;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (vggg_init(mode) == NULL)
		return 1;

  /*int base = 100;
  int len = base;
  uint16_t x_atual;
  uint16_t y_atual = y;
  int line_num = 0; 

  while (line_num < y) {
    line_num++;
    x_atual = x+line_num;
    len = base - 2*line_num;
    for (int i = 0; i < len; i++) {
      vg_paint_pixelll(x_atual+i,y_atual,color);
    }
    y_atual++;
  }*/

  int h = 100;
  int len = 1;
  uint16_t x_atual;
  uint16_t y_atual = y;
  int line_num = 0; 

  while (line_num < h) {
    line_num++;
    x_atual = x-line_num;
    len = 1 + 2*line_num;
    for (int i = 0; i < len; i++) {
      vg_paint_pixelll(x_atual+i,y_atual,color);
    }
    y_atual++;
  }

	/*if (square_draw(x, y, width,height, color) != OK) {
    printf("ERROR drwaing the square!\n");
    return 1;
  }*/
	get_esc_break_key();

	if (vg_exit() != OK)
		return 1;

	return OK;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (vggg_init(mode) == NULL)
		return 1;

	if (draw_pattern(mode,no_rectangles, first, step) != OK) {
    printf("ERROR drwaing the square!\n");
    return 1;
  }
  printf("ARRIVED\n");
	get_esc_break_key();

	if (vg_exit() != OK)
		return 1;

	return OK;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (vggg_init(0x105) == NULL) {
    return 1;
  }
  xpm_image_t img;
  uint8_t *pixmap;
  
  // get the pixmap from the XPM
  pixmap =  xpm_load(xpm, XPM_INDEXED, &img);
  
  // copy it to graphics memory
  vg_draw_xpm (pixmap,  img, x, y);

  if(get_esc_break_key() == OK){
    if (vg_exit() == OK)
		  return OK;
  }
  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  int ipc_status, r;
  uint8_t keyboard_id;
  uint8_t timer_id;
	message msg;
  bool can_continue = true;
  xpm_image_t img;
  uint8_t *pixmap;
  uint16_t dx = xf-xi;
  uint16_t dy = yf-yi;
  bool not_final_pos = true;

  //checks if arguments are valid
  if(fr_rate >60){
    printf("ERROR tmakehe maximum refresh rate supported is 60 frames per second.!\n");
    return 1;
  }
  if(speed == 0){
    printf("ERROR speed must be different from zero!\n");
    return 1;
  }
  
  //subscribes interruptions
	if (kbd_subscribe_int(&keyboard_id) != OK) {
    printf("ERROR: Subsribe failed!\n");
    return 1;
  }
  if (timer_subscribe_int(&timer_id) != 0) {
    printf("ERROR: Subscribe failed");
    return 1;
  }  

  if(vggg_init(0x105) == NULL){
    return 1;
  }
  
  // get the pixmap from the XPM
  pixmap =  xpm_load(xpm, XPM_INDEXED, &img);
  // copy it to graphics memory
  vg_draw_xpm (pixmap,  img, xi, yi);
  while (can_continue == true) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source))  {
        case HARDWARE:                            // hardware interrupt notification
          if (msg.m_notify.interrupts & keyboard_id) { // subscribed interrupt BIT MASK
            kbc_ih();
            if(scan_code == ESC_BREAK_KEY){
              can_continue = false;
            }
          }

          if (msg.m_notify.interrupts & timer_id && not_final_pos ) { // subscribed interrupt 
            if(speed > 0){
              if (counter == 60/fr_rate  ) {
                counter = 0;

                  if(dx != 0){
                    for(uint16_t i = 0 ; i <= speed; i++){
                      for(uint16_t j = yi; j < yi + img.height; j++){
                          vg_paint_pixelll(xi + i, j , 256);
                      }
                      }
                    xi = xi + speed;
                    if(xi > xf){
                      xi = xf;
                    }
                    vg_draw_xpm (pixmap,  img, xi, yi);
                    if(xi == xf){
                      not_final_pos = false;
                    }
                  }
                  else if(dy != 0){

                    for(uint16_t j = 0 ; j <= speed; j++){
                      for(uint16_t i = xi; i < xi + img.width; i++){
                        vg_paint_pixelll(i, yi + j, 256);
                      }
                    }
                     yi = yi + speed;
                     vg_draw_xpm (pixmap,  img, xi, yi);
                     if(yi == yf){
                      not_final_pos = false;
                    }
                  }
                  }
                }
                if(speed < 0){
                  if(abs(counter) == abs(60/fr_rate*speed)) {
                    counter = 0;

                  if(dx != 0){
                    for(uint16_t i = xi; i <= xi + img.width; i++){
                      for(uint16_t j = yi; j < yi + img.height; j++){
                          vg_paint_pixelll(i, j , 256);
                      }
                      }
                    xi++;
                    if(xi > xf){
                      xi = xf;
                    }
                    vg_draw_xpm (pixmap,  img, xi, yi);
                    if(xi == xf){
                      not_final_pos = false;
                    }
                  }
                  else if(dy != 0){
                    for(uint16_t i = xi; i <= xi + img.width; i++){
                      for(uint16_t j = yi; j < yi + img.height; j++){
                          vg_paint_pixelll(i, j , 256);
                      }
                      }
                     yi++;
                     vg_draw_xpm (pixmap,  img, xi, yi);
                     if(yi == yf){
                      not_final_pos = false;
                    }
                  }
                  }

                }
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
  if (timer_unsubscribe_int() != 0) {
    printf("ERROR: Unsubscribe failed");
    return 1;    
  }

  if (vg_exit() == OK){
		  return OK;
  }

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
