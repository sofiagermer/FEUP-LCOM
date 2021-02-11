
#include <lcom/lcf.h>
#include "test7.h"
#include <stdint.h>
#include "serial_port.h"
#include "uart_const.h"



int ser_test_conf(unsigned short base_addr) {
    uint8_t st, dll, dlm, st2;
    int wl,sb;
    if(util_sys_inb(base_addr + LCR, &st)) return 1;
    
    if((st & WORLD_LENGTH_SEL) == WORD_LENGTH_5) wl = 5;
    else if((st & WORLD_LENGTH_SEL) == WORD_LENGTH_6) wl = 6;
    else if((st & WORLD_LENGTH_SEL) == WORD_LENGTH_7) wl = 7;
    else wl = 8;
       
    if((st & NO_STOP_BITS) == NO_STOP_BITS_1) sb=1;
    else sb=2;

    printf("\nWord Length = %d \n",wl);
    printf("%d STOP BIT\n",sb);

    if((st & PARITY) == PARITY_NONE) printf("PARITY NONE\n");
    else if((st & PARITY) == PARITY_ODD) printf("PARITY ODD\n");
    else if((st & PARITY) == PARITY_EVEN) printf("PARITY EVEN\n");
    else if((st & PARITY) == PARITY_1) printf("PARITY 1\n");
    else printf("PARITY 0\n");

    uint32_t st_32 = st & (0x000000FF);
    st_32 |= SEL_DL;

    if(sys_outb(base_addr + LCR, st_32)) return 1;

    if(util_sys_inb(base_addr+DLL, &dll)) return 1;
    if(util_sys_inb(base_addr+DLM, &dlm)) return 1;

    st_32 = st & (0x000000FF);

    if(sys_outb(base_addr + LCR, st_32)) return 1;

    printf("DLM = %d  DLL = %d\n", dlm,dll);
    uint16_t dl = (dlm << 8) | dll;
    printf("BITRATE:  %d\n", DL_CONST/dl);



    if(util_sys_inb(base_addr + IER, &st2)) return 1;

    if(st2 & EN_REC_DATA_INT ) printf("Rx interrupts: ENABLED\n");
    else printf("Rx interrupts: DISABLED\n");

    if(st2 & EN_TRANS_EMPTY_INT ) printf("Tx interrupts: ENABLED\n");
    else printf("Tx interrupts: DISABLED\n");

    if(st2 & EN_REC_LINE_STATUS_INT ) printf("REC LINE interrupts: ENABLED\n");
    else printf("REC LINE interrupts: DISABLED\n");    

    return 0;
}

int ser_test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, 
	           long parity, unsigned long rate) { 
    
    uint32_t st, new_conf = 0;
    if(sys_inb(base_addr + LCR, &st)) return 1;

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

    if(sys_outb(base_addr + LCR, (st & BIT(6) | new_conf))) return 1;
    

    uint16_t dl = DL_CONST / rate;
    uint8_t dll , dlm;

    if(util_get_LSB(dl,&dll)) return 1;
    if(util_get_MSB(dl,&dlm)) return 1;

    uint32_t st_lcr, st_dl;
    if(sys_inb(base_addr + LCR, &st_lcr)) return 1;
    st_dl = st_lcr|SEL_DL;

    if(sys_outb(base_addr + LCR, st_dl)) return 1;

    if(sys_outb(base_addr + DLL, dll & 0xFFFFFFFF)) return 1;
    if(sys_outb(base_addr + DLM, dlm & 0xFFFFFFFF)) return 1;

    if(sys_outb(base_addr + LCR, st_lcr)) return 1;

    return 0;
}

int ser_test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, 
                    unsigned long stop, long parity, unsigned long rate, 
                    int stringc, char *strings[]) {
    /* To be completed */
    return 1;
}

int ser_test_int(/* details to be provided */) { 
    /* To be completed */
    return 1;
}

int ser_test_fifo(/* details to be provided */) {
    /* To be completed */
    return 1;
}
