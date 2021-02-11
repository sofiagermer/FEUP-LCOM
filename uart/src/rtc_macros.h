#ifndef _LCOM_RTC_MACROS_H_
#define _LCOM_RTC_MACROS_H_

#include <lcom/lcf.h>

#define FAIL 1

#define RTC_IRQ 8
#define UIE BIT(4)
#define RTC_UF  BIT(4)

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_REGISTER_A 10
#define RTC_REGISTER_B 11
#define RTC_REGISTER_C 12

#define UIP BIT(7)
#define DM BIT(2)
#define MILITARY_TIME BIT(1)

#define SEC_REG 0
#define MIN_REG 2
#define HOUR_REG 4
#define DAY_REG 7
#define MONTH_REG 8
#define YEAR_REG 9

#define LIMIT_HOUR 0x12
#define OFFSET_FOR_PM_TIME 0x80


#endif
