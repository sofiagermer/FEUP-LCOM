#include <lcom/lcf.h>
#include "serial_port.h"
#include "uart_const.h"


#include <stdint.h>

static int uart_com1_hook_id = COM1_IRQ;   //hook id used for the mouse
uint8_t initial_lcr, initial_ier, initial_dlm, initial_dll;
uint8_t ser_byte;
bool error_reading = false;

void test(){
    uint8_t iir;
    util_sys_inb(COM1_ADDR+IIR,&iir);
    if((iir &FIFO_STATUS) == FIFO_STATUS_NO_FIFO) printf("tou fdd\n");
}

int ser_save_init_conf(){
    uint32_t st;
    if(util_sys_inb(COM1_ADDR + LCR, &initial_lcr)) return 1;
    st = 0x000000FF & (initial_lcr | SEL_DL);

    if(sys_outb(COM1_ADDR + LCR, st)) return 1;

    if(util_sys_inb(COM1_ADDR+DLL, &initial_dll)) return 1;
    if(util_sys_inb(COM1_ADDR+DLM, &initial_dlm)) return 1;

    st = initial_lcr & (0x000000FF);

    if(sys_outb(COM1_ADDR + LCR, st)) return 1;

    if(util_sys_inb(COM1_ADDR + IER, &initial_ier)) return 1;

    return 0;
}

int ser_restore_init_conf(){
    uint32_t st = 0x000000FF & (initial_lcr | SEL_DL);
    if(sys_outb(COM1_ADDR + LCR, st)) return 1;

    if(sys_outb(COM1_ADDR + DLL, initial_dll & 0x000000FF)) return 1;
    if(sys_outb(COM1_ADDR + DLM, initial_dlm & 0x000000FF)) return 1;

    st = initial_lcr & (0x000000FF);

    if(sys_outb(COM1_ADDR + LCR, initial_lcr )) return 1;

    st = initial_ier & (0x000000FF);

    if(sys_outb(COM1_ADDR + IER, initial_ier)) return 1;

    return 0;
}

int ser_start_proj_config(){
    if(ser_set_conf(NO_BITS_PROJ, NO_STOP_BITS_PROJ, PARITY_PROJ, BITRATE_PROJ)) return 1;
    uint32_t proj_ier = 0x000000FF & ( initial_ier | EN_REC_DATA_INT | EN_REC_LINE_STATUS_INT );
    
    proj_ier = proj_ier & (~EN_TRANS_EMPTY_INT);
    if(sys_outb(COM1_ADDR + IER, proj_ier)) return 1;
    return 0; 
}

int ser_set_conf(unsigned long bits, unsigned long stop, 
	           long parity, /* -1: none, 0: even, 1: odd */
                unsigned long rate) { 
    
    uint32_t st, new_conf = 0;
    if(sys_inb(COM1_ADDR + LCR, &st)) return 1;

    switch(bits){
    case 5:
        new_conf |= WORD_LENGTH_5;
        break;
    case 6:
        new_conf |= WORD_LENGTH_6;
        break;
    case 7:
        new_conf |= WORD_LENGTH_7;
        break;
    case 8:
        new_conf |= WORD_LENGTH_8;
        break;
    default:
        return 1;    
    }

    switch(stop){
    case 1:
        new_conf |= NO_STOP_BITS_1;
        break;
    case 2:
        new_conf |= NO_STOP_BITS_2;
        break;
    default:
        return 1;
    }

    switch(parity){
    case -1:
        new_conf |= PARITY_NONE;
        break;
    case 0:
        new_conf |= PARITY_EVEN;
        break;
    case 1:
        new_conf |= PARITY_ODD;
        break;
    default:
        return 1;
    }

    if(sys_outb(COM1_ADDR + LCR, (st & BIT(6) | new_conf))) return 1;
    

    uint16_t dl = DL_CONST / rate;
    uint8_t dll , dlm;

    if(util_get_LSB(dl,&dll)) return 1;
    if(util_get_MSB(dl,&dlm)) return 1;

    uint32_t st_lcr, st_dl;
    if(sys_inb(COM1_ADDR + LCR, &st_lcr)) return 1;
    st_dl = st_lcr|SEL_DL;

    if(sys_outb(COM1_ADDR + LCR, st_dl)) return 1;

    if(sys_outb(COM1_ADDR + DLL, dll & 0x000000FF)) return 1;
    if(sys_outb(COM1_ADDR + DLM, dlm & 0x000000FF)) return 1;

    if(sys_outb(COM1_ADDR + LCR, st_lcr)) return 1;

    return 0;
}

int (ser_subscribe_int)(uint8_t* bit_no){
    *bit_no = uart_com1_hook_id;

    if (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &uart_com1_hook_id) != OK) {
        printf("ser_subscribe_int::ERROR in setting policy!\n");
        return FAIL;
    }

    if(ser_save_init_conf()) return 1;

    if(ser_start_proj_config()) return 1;

    return OK;
}

int (ser_unsubscribe_int)(){
    if(ser_restore_init_conf()) return 1;
    
    if (sys_irqrmpolicy(&uart_com1_hook_id) != OK) {
        printf("ser_unsubscribe_int::ERROR removing policy!\n");
        return FAIL;
    };
    return OK;
}

int ser_can_read(){
    uint8_t lsr;
    if(util_sys_inb(COM1_ADDR + LSR, &lsr)) return 1;
    if(lsr & REC_DATA){ 
        if (lsr & (OVERRUN_ERROR | PARITY_ERROR| FRAMING_ERROR)) return 2;
        return 0;
    }
    return 1;
}

bool ser_error_read(){
    uint8_t st;
    if(util_sys_inb(COM1_ADDR + LSR, &st) != OK ) return true;
    else if (st & (OVERRUN_ERROR | PARITY_ERROR| FRAMING_ERROR)){
        return true;
    }
    return false;
}

bool ser_can_send(){
    uint8_t lsr;
    if(util_sys_inb(COM1_ADDR + LSR, &lsr)) return false;
    if(lsr & TRANS_HOLD_REG_EMPTY) return true;
    return false;
}


int ser_send_byte(uint8_t data){
    int no_tries = MAX_NO_TRIES;
    while((no_tries--)>0){
        if(!ser_can_send()){
            tickdelay(micros_to_ticks(DELAY_US));
            continue;
        }
        if(sys_outb(COM1_ADDR + THR, data)) return 1;
        return 0;
    }
    return 1;
}

int ser_send_info(uint8_t* data, unsigned int length){
    for(unsigned int i = 0; i<length ; i++ ){
        if(ser_send_byte(data[i])) return 1;
        tickdelay(micros_to_ticks(200000));
        printf("%c\n", data[i]);
    }
    return 0;
}

int ser_read_byte(uint8_t* data){
    int no_tries = MAX_NO_TRIES;
    while((no_tries--)>0){
        if(ser_can_read() == 2) {
            error_reading = true;            
            return 1;
        }
        if(ser_can_read() == 1){
            tickdelay(micros_to_ticks(DELAY_US));
            continue;
        }

        if(util_sys_inb(COM1_ADDR + RBR, data))return 1;
        
        return 0;
    }
    return 1;
}

void ser_ih(){
    uint8_t iir;
    if(util_sys_inb(COM1_ADDR + IIR, &iir)) {
        error_reading = true;
        printf("oal123\n");
        return;
    }
    if( (iir & INT_STATUS) == INT_PENDING ) {
        switch( iir & INT_ORIGIN ) {
        case INT_ORIG_REC_DATA_AVAIL:
            ser_read_byte(&ser_byte);
            printf("ola\n");
            return;
        case INT_ORIG_LINE_STATUS:
            printf("ERROR ON UART's Receiver Buffer!\n");
            ser_read_byte(&ser_byte);
            return;
        case INT_ORIG_MODEM_STATUS:
            printf("MODEM STATUS \n");
            return;
        case INT_ORIG_TRANS_EMPTY:
            printf("TRANS EMPTY\n");
            return;
        case INT_ORIG_CHAR_TIMEOUT:
            printf("CHAR TIMEOUT\n");
            return;
        default:
            printf("ANOTHER TYPE INTERRUPT!\n");
            return;
        }
    }
    error_reading = true;
}

