#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define FAIL 1
#define DELAY_US 20000
#define KBD_IRQ  1 /**< @brief KBD IRQ line */
#define MOUSE_IRQ  12 /**< @brief KBD IRQ line */

// Registers
#define STAT_REG    0x64 /**< @brief keyboard status register port */
#define CMD_REG     0x64 /**< @brief keyboard commands register port */
#define CMD_ARG_REG 0x60 /**< @brief keyboard command arguments register port */
#define OUT_BUFF    0x60 /**< @brief keyboard output buffer port */
#define IN_BUFF     0x60 /**< @brief keyboard input buffer port */


// KBC Status Register 
#define KBD_PAR_ERROR  BIT(7) /**< @brief Status register parity error */
#define KBD_TIME_ERROR BIT(6) /**< @brief Status register timeout error */
#define KBD_AUX        BIT(5) /**< @brief Status register mouse data */
#define KBD_INH        BIT(4) /**< @brief Status register inhibit flag */
#define KBD_A2         BIT(3) /**< @brief Status register A2 input line */
#define KBD_SYS        BIT(2) /**< @brief Status register system flag */
#define KBD_IBF        BIT(1) /**< @brief Status register input buffer full */
#define KBD_OBF        BIT(0) /**< @brief Status register output buffer full */

// KBC Commands 
#define READ_CMD_BYTE  0x20 /**< @brief return command byte */
#define WRITE_CMD_BYTE 0x60 /**< @brief writes new command byte passed as argument (to port 0x60) */
#define CHECK_KBC      0xAA /**< @brief self-test: returns 0x55 if OK, returns 0xFC if error */
#define CHECK_KB_IFC   0xAB /**< @brief check keyboard interface: returns 0, if OK */
#define DISABLE_KBD    0xAD /**< @brief disables keyboard interface */
#define ENABLE_KBD     0xAE /**< @brief enables keyboard interface */
#define DISABLE_MOUSE  0xA7 /**< @brief disables mouse interface */
#define ENABLE_MOUSE   0xA8 /**< @brief enables mouse interface */
#define CHEK_MOUSE_INTERFACE 0xA9 /**< @brief checks mouse interface */
#define WRITE_BYTE_TO_MOUSE  0xD4 /**< @brief writes bytes to mouse */


// KBC Command Byte
#define CMD_BYTE_DIS_M    BIT(5) /**< @brief disable mouse interface */
#define CMD_BYTE_DIS_K    BIT(4) /**< @brief disable keyboard interface */
#define CMD_BYTE_EN_INT_M BIT(1) /**< @brief enable interrupt on OBF, from mouse */
#define CMD_BYTE_EN_INT_K BIT(0) /**< @brief enable interrupt on OBF, from keyboard */


// Mouse Commands
#define RESET           0XFF /**< @brief mouse reset */
#define RESEND          0XFE /**< @brief for serial communications errors */
#define SET_DEFAULTS    0XF6 /**< @brief set default values */
#define DIS_DATA_REPORT 0XF5 /**< @brief in stream mode, should be sent before any other command */
#define EN_DATA_REPORT  0XF4 /**< @brief in stream mode only */
#define SET_SAMPLE_RATE 0XF3 /**< @brief sets state sampling rate */
#define SET_REMOTE_MODE 0XF0 /**< @brief send data on request only */
#define READ_DATA       0XEB /**< @brief send data packet request */
#define SET_STREAM_MODE 0XEA /**< @brief send data on events */
#define STAT_REQUEST    0XE9 /**< @brief */
#define SET_RESOLUTION  0XE8 /**< @brief get mouse configuration (3 bytes) */
#define SET_SCALING_2_1 0XE7 /**< @brief acceleration mode */
#define SET_SCALING_1_1 0XE6 /**< @brief linear mode */


// Mouse Responses
#define ACK   0XFA /**< @brief enable interrupt on OBF, from keyboard */
#define NACK  0XFE /**< @brief enable interrupt on OBF, from keyboard */
#define ERROR 0XFC /**< @brief enable interrupt on OBF, from keyboard */


// Parsing packet
#define Y_OVFL          BIT(7)   /**< @brief  */
#define X_OVFL          BIT(6)   /**< @brief  */
#define MSB_Y_DELTA     BIT(5)   /**< @brief  */
#define MSB_X_DELTA     BIT(4)   /**< @brief  */
#define FIRST_OF_PACKET BIT(3)   /**< @brief checks if its the first byte of a packet (of 3 bytes) */
#define MB              BIT(2)   /**< @brief midle button pressed */
#define RB              BIT(1)   /**< @brief right button pressed */
#define LB              BIT(0)   /**< @brief left button pressed */


// Other important macros
#define ESC_BREAK_KEY  0x81   /**< @brief break code corresponding to ESC key */
#define TWO_BYTES_CODE 0xE0   /**< @brief two bytes scan code always starts with 0xE0 */
#define BREAK_CODE     BIT(7) /**< @brief break codes always have bit 7 high */


/**@}*/

#endif /* _LCOM_I8042_H */




