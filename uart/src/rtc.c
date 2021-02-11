#include <lcom/lcf.h>
#include "rtc.h"
#include "rtc_macros.h"
#include <stdint.h>


int rtc_hook_id = RTC_IRQ;
Time rtc_time;
Date rtc_date;

void bcd_to_dec(uint32_t* bcd){
    *bcd = *bcd -6*(*bcd >> 4);
}

void wait_valid_rtc() {
   uint32_t reg_a_data = 0;
    do {
        disable_rtc_int();
        sys_outb(RTC_ADDR_REG, RTC_REGISTER_A);
        sys_inb(RTC_DATA_REG, &reg_a_data);
        enable_rtc_int();
    } while ( reg_a_data & UIP);
}

bool updating_rtc(){
    uint32_t reg_a_data;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_A)) return true;
    if(sys_inb(RTC_DATA_REG, &reg_a_data)) return true;

    if(reg_a_data & UIP) return true;
    return false;
}

bool bcd_format(){
    uint32_t reg_b_data=0;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_B)) return true;
    if(sys_inb(RTC_DATA_REG, &reg_b_data)) return true;

    if(!(reg_b_data & DM)) return true;
    return false;
}

bool military_time(){
    uint32_t reg_b_data=0;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_B)) return true;
    if(sys_inb(RTC_DATA_REG, &reg_b_data)) return true;

    if(reg_b_data & MILITARY_TIME) return true;
    return false;
}

int read_date(Date* date){
    if(sys_outb(RTC_ADDR_REG,YEAR_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(date->year))) return 1;

    if(sys_outb(RTC_ADDR_REG,MONTH_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(date->month))) return 1;

    if(sys_outb(RTC_ADDR_REG,DAY_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(date->day))) return 1;

    if(bcd_format()){
        bcd_to_dec(&(date->year));
        bcd_to_dec(&(date->month));
        bcd_to_dec(&(date->day));
    }

    return 0;
}

int read_time(Time* time){

    if(sys_outb(RTC_ADDR_REG,HOUR_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(time->hour))) return 1;

    if(sys_outb(RTC_ADDR_REG,MIN_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(time->minute))) return 1;

    if(sys_outb(RTC_ADDR_REG,SEC_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(time->second))) return 1;

    if(!military_time() && time->hour > LIMIT_HOUR){ // not military time and PM
            time->hour -= OFFSET_FOR_PM_TIME;
    }

    if(bcd_format()){   
        bcd_to_dec(&(time->hour));     
        bcd_to_dec(&(time->minute));
        bcd_to_dec(&(time->second));
    }

    return 0;
}


Date get_date(){
    Date res;

    while(true){
        if(!updating_rtc()){
            read_date(&res);
            return res;
        }
    }
}

Time get_time(){
    Time res;

    while(true){
        if(!updating_rtc()){
            read_time(&res);
            return res;
        }
    }
}


int (enable_rtc_int)(){
    if(sys_irqenable(&rtc_hook_id)) return 1;
    return 0;
}

int (disable_rtc_int)(){
    if(sys_irqdisable(&rtc_hook_id)) return 1;
    return 0;
}

int enable_rtc_UI(){
    uint32_t reg_b_data=0;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_B)) return 1;
    if(sys_inb(RTC_DATA_REG, &reg_b_data)) return 1;

    reg_b_data |= UIE;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_B)) return 1;
    if(sys_outb(RTC_DATA_REG, reg_b_data)) return 1;
    
    return 0;
}

int disable_rtc_UI(){
    uint32_t reg_b_data=0;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_B)) return 1;
    if(sys_inb(RTC_DATA_REG, &reg_b_data)) return 1;

    reg_b_data &= ~UIE;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_B)) return 1;
    if(sys_outb(RTC_DATA_REG, reg_b_data)) return 1;
    
    return 0;
}

int (rtc_subscribe_int)(uint8_t *bit_no) {
    *bit_no = rtc_hook_id;
    if(sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id)!= OK){
      printf("rtc_subscribe_int::ERROR in setting policy !\n");
      return FAIL;
    }

    if(enable_rtc_UI()) return FAIL;

    return OK;
}

int (rtc_unsubscribe_int)() {
    if(sys_irqrmpolicy(&rtc_hook_id)!= OK){
      printf("rtc_unsubscribe_int::ERROR in disabling IQR line!\n");
      return FAIL;
    }

    if(disable_rtc_UI()) return FAIL;

    return OK;
}


void (rtc_int_handler)(){
    uint32_t reg_c_data=0;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_C)) return;
    if(sys_inb(RTC_DATA_REG, &reg_c_data)) return;

    if(reg_c_data & RTC_UF){
        wait_valid_rtc();
        read_time(&rtc_time);
    }
}

void rtc_start(){
    rtc_time = get_time();
    rtc_date = get_date();
}
