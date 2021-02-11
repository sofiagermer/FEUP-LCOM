#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

typedef enum {INITIAL, UP, MID, DOWN} State;

struct mouse_ev mouse_get_event(struct packet *packet);

bool (drawing_handler)(uint8_t xlen, uint8_t tolerance, struct packet * pack);
