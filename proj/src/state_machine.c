#include "state_machine.h"

uint8_t delta_x, delta_y;
bool left_press = false, right_press = false, mid_press = false;

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

