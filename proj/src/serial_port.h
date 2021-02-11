#ifndef _LCOM_SERIAL_PORT__H_
#define _LCOM_SERIAL_PORT__H_

#include <lcom/lcf.h>

//void test();

/**
Saves the initial configuration of the serial port (COM 1) into local variables
@return int - 1 if there was an error, 0 otherwise
*/
int ser_save_init_conf();

/**
Restores the initial configuration of the serial port (COM 1)
@return int - 1 if there was an error, 0 otherwise
*/
int ser_restore_init_conf();

/**
Sets the serial port (COM 1) configurations into the project's configuration
@return int - 1 if there was an error, 0 otherwise
*/
int ser_start_proj_config();

/**
Changes the configuration of the serial port (COM 1) 
@param unsigned long - Word Length in bits
@param unsigned long - Number of stop bits
@param long - Parity  -1: none, 0: even, 1: odd 
@param unsigned long - Bitrate
@return int - 1 if there was an error, 0 otherwise
*/
int ser_set_conf(unsigned long bits, unsigned long stop,long parity, unsigned long rate);

/**
Subscribes and enables the interrupts of the Serial Port (COM 1) 
and enables the Received Data and Receiver Line Status interrupts in the Serial Port (COM 1)
@param uint8_t* - will change to the position of the IRQ_SET bit 
@return int - 1 if there was an error, 0 otherwise
*/
int (ser_subscribe_int)(uint8_t* bit_no);

/**
Unsubscribes and disables the interrupts of the Serial Port (COM 1) 
and disables the Received Data and Receiver Line Status interrupts in the Serial Port (COM 1) 
@return int - 1 if there was an error, 0 otherwise
*/
int (ser_unsubscribe_int)();

/**
Checks if it is possible to read the ReceiverBuffer, and checks for errors 
@return int - 2 if there is an error in the Receiver Buffer, 1 if there was an error, 0 otherwise
*/
int ser_can_read();

/**
Cleans the Receiver Buffer.
@return int - 1 if there was an error, 0 otherwise
*/ 
int ser_flush_rx();

/**
Checks if there is an error in the Receiver Buffer
@return bool - true if there is an error, false otherwise
*/
bool ser_error_read();

/**
Checks if it is possible to send information by writing into the Transmitter Holding 
@return int - true if it is possible to send info, false otherwise
*/
bool ser_can_send();

/**
Sends a byte to the Serial Port (COM 1)'s Transmitter Holding
@param uint8_t - byte to send 
@return int - 1 if there was an error, 0 otherwise
*/
int ser_send_byte(uint8_t data);

/**
Sends a byte to the Serial Port (COM 1)'s Transmitter Holding and the waits
@param uint8_t - byte to send 
@return int - 1 if there was an error, 0 otherwise
*/
int ser_send_byte_wait(uint8_t data);

/**
Sends a number of bytes to the Serial Port (COM 1)'s Transmitter Holding
@param uint8_t* - bytes to send
@param unsigned int - number of bytes to send 
@return int - 1 if there was an error, 0 otherwise
*/
int ser_send_info(uint8_t* data, unsigned int length);

/**
Reads a byte of the Serial Port (COM 1)'s Receiver Buffer
@param uint8_t* - byte where will be written the info in the Receiver Buffer 
@return int - 1 if there was an error, 0 otherwise
*/
int ser_read_byte(uint8_t* data);

/**
Handles the Serial Port (COM 1) interrupts by reading the Receiver Buffer
*/
void ser_ih();

#endif /* _LCOM_SERIAL_PORT_H */
