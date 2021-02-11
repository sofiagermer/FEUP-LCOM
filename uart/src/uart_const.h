#ifndef _LCOM_UART_CONST__H_
#define _LCOM_UART_CONST__H_

#include <lcom/lcf.h>

#define FAIL                1
#define MAX_NO_TRIES        20
#define DELAY_US            20000

//PROJ  
#define NO_BITS_PROJ        8
#define NO_STOP_BITS_PROJ   1
#define PARITY_PROJ         1  //ODD
#define BITRATE_PROJ        1200

//UART  
#define COM1_ADDR           0x3F8
#define COM1_IRQ            4
#define COM2_ADDR           0x2F8
#define COM2_IRQ            3

//UART REGS 
#define RBR                 0
#define THR                 0
#define IER                 1
#define IIR                 2
#define FCR                 2
#define LCR                 3
#define MCR                 4
#define LSR                 5
#define MSR                 6
#define SR                  7
#define DLM                 1 
#define DLL                 0


//LCR
#define WORLD_LENGTH_SEL            (BIT(1)|BIT(0))
#define NO_STOP_BITS                BIT(2)
#define PARITY                      (BIT(5)|BIT(4)|BIT(3))
#define SET_BREAK_ENABLE            BIT(6)
#define DLAB                        BIT(7)

#define WORD_LENGTH_5               0
#define WORD_LENGTH_6               BIT(0)
#define WORD_LENGTH_7               BIT(1)
#define WORD_LENGTH_8               (BIT(1)|BIT(0))

#define NO_STOP_BITS_1              0
#define NO_STOP_BITS_2              BIT(2)

#define PARITY_NONE                 0  //VER convençao
#define PARITY_ODD                  BIT(3)
#define PARITY_EVEN                 (BIT(4)|BIT(3))
#define PARITY_1                    (BIT(5)|BIT(3))
#define PARITY_0                    (BIT(5)|BIT(4)|BIT(3))

#define SET_BREAK_EN                BIT(6)

#define SEL_DL                      BIT(7)
#define SEL_DATA                    0

#define DL_CONST                    115200

//LSR   
#define REC_DATA                    BIT(0)
#define OVERRUN_ERROR               BIT(1)
#define PARITY_ERROR                BIT(2)
#define FRAMING_ERROR               BIT(3)
#define BREAK_INT                   BIT(4)
#define TRANS_HOLD_REG_EMPTY        BIT(5)
#define TRANS_EMPTY_REG             BIT(6)
#define FIFO_ERROR                  BIT(7)

//IER   
#define EN_REC_DATA_INT             BIT(0)
#define EN_TRANS_EMPTY_INT          BIT(1)
#define EN_REC_LINE_STATUS_INT      BIT(2)
#define EN_MODEM_STATUS_INT         BIT(3)

//IIR   
#define INT_STATUS                  BIT(0)
#define INT_ORIGIN                  (BIT(3)|BIT(2)|BIT(1))
#define _64_BYTE_FIFO               BIT(5)
#define FIFO_STATUS                 (BIT(7)|BIT(6))

#define INT_PENDING                 0
#define INT_NOT_PENDING             BIT(0)

#define INT_ORIG_MODEM_STATUS       0
#define INT_ORIG_TRANS_EMPTY        BIT(1)
#define INT_ORIG_CHAR_TIMEOUT       BIT(3)
#define INT_ORIG_REC_DATA_AVAIL     BIT(2)
#define INT_ORIG_LINE_STATUS        (BIT(2)|BIT(1))

#define _64_BYTE_FIFO_              BIT(5)

#define FIFO_STATUS_NO_FIFO         0
#define FIFO_STATUS_UNUSABLE        BIT(7)
#define FIFO_STATUS_EN              (BIT(7)|BIT(6))

//FCR   
#define EN_FIFO                     BIT(0)
#define CLEAR_REC_FIFO              BIT(1)
#define CLEAR_TRANS_FIFO            BIT(2)
#define DMA_MODE_SEL                BIT(3)
#define EN_64_BYTE_FIFO             BIT(5)
#define RX_INFO_INT_TRIG_LEVEL      (BIT(7)|BIT(6))

#define RX_FIFO_INT_TRIG_LEVEL_1    0
#define RX_FIFO_INT_TRIG_LEVEL_4    BIT(6)
#define RX_FIFO_INT_TRIG_LEVEL_8    BIT(7)
#define RX_FIFO_INT_TRIG_LEVEL_16   (BIT(7)|BIT(6)) 


#endif /* _LCOM_UART_H */
