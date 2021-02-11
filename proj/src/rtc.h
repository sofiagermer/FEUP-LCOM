#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>

/**
 * @struct Date
 * @var Date::year
 * Actual year
 * @var Date::month
 * Acual month
 * @var Date::day
 * Actual day
 * */
typedef struct{
    uint32_t year;
    uint32_t month;
    uint32_t day;
} Date;

/**
 * @struct Time 
 * @var Time:: hour
 * Actual hour
 * @var Time:: minute
 * Actual minute
 * @var Time:: second
 * Actual second
 * */
typedef struct{
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
} Time;


/**
Waits until it is possible to read the rtc without it being in the middle of an update
*/
void wait_valid_rtc();

/**
Converts a number in Binary-coded decimal format to binary
@param uint32_t* - the number to be conveted
*/
void bcd_to_dec(uint32_t* bcd);

/**
Check if the rtc is in the middle of an update
@return bool - true if the rtc is in the middle of an update false otherwise
*/
bool updating_rtc();

/**
Checks if the rtc is in Binary-coded decimal format
@return bool - true if the rtc is in Binary-coded decimal format, false otherwise
*/
bool bcd_format();

/**
Checks if the rtc is in military time format
@return bool - true if the rtc is in military format, false otherwise
*/
bool military_time();

/**
Reads the current date
@param Date* - Date to be changed into the current date
@return int - 1 if there was an error, 0 otherwise
*/
int read_date(Date* date);

/**
Reads the current time
@param Time* - Time to be changed into the current time
@return int - 1 if there was an error, 0 otherwise
*/
int read_time(Time* time);

/**
Reads the current date, when it is possible
@return Date - Current date
*/
Date get_date();

/**
Reads the current time, when it is possible
@return Time - Current time
*/
Time get_time();

/**
Enables interrupts of the RTC
@return int - 1 if there was an error, 0 otherwise
*/
int (enable_rtc_int)();

/**
Disables interrupts of the RTC
@return int - 1 if there was an error, 0 otherwise
*/
int (disable_rtc_int)();

/**
Enables the Update-ended interrupts of the RTC
@return int - 1 if there was an error, 0 otherwise
*/
int enable_rtc_UI();

/**
Disables the Update-ended interrupts of the RTC
@return int - 1 if there was an error, 0 otherwise
*/
int disable_rtc_UI();

/**
Subscribes and enables the interrupts of the RTC 
and enables the Update-ended interrupts in the RTC 
@param uint8_t* - will change to the position of the IRQ_SET bit 
@return int - 1 if there was an error, 0 otherwise
*/
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
Unsubscribes and disables the interrupts of the RTC 
and disaables the Update-ended interrupts in the RTC  
@return int - 1 if there was an error, 0 otherwise
*/
int (rtc_unsubscribe_int)();

/**
Handles the RTC interrupts by the reading the time
*/
void (rtc_int_handler)();

/**
Initializes variables that track the time and date
*/
void rtc_start();

#endif
