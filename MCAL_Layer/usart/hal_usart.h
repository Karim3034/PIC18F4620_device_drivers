/* 
 * File:   hal_usart.h
 * Author: fathy
 *
 * Created on August 17, 2024, 2:35 AM
 */

#ifndef HAL_USART_H
#define	HAL_USART_H

/* Section : Includes */
#include "hal_usart_cfg.h"
#include "pic18f4620.h"
#include "../MCAL_std_types.h"
#include "../hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations */
#define EUSART_SYNCHRONOUS_MODE_SELECT                  1
#define EUSART_ASYNCHRONOUS_MODE_SELECT                 0

#define EUSART_MODULE_ENABLE                            1
#define EUSART_MODULE_DISABLE                           0

#define EUSART_ASYNCHRONOUS_9_BIT_TX_ENABLE             1
#define EUSART_ASYNCHRONOUS_9_BIT_TX_DISABLE            0

#define EUSART_ASYNCHRONOUS_TX_ENABLE                   1
#define EUSART_ASYNCHRONOUS_TX_DISABLE                  0

#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE         1
#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE        0

#define EUSART_ASYNCHRONOUS_HIGH_SPEED                  1
#define EUSART_ASYNCHRONOUS_LOW_SPEED                   0

#define EUSART_ASYNCHRONOUS_TRANSMIT_SHIFT_REG_EMPTY    1
#define EUSART_ASYNCHRONOUS_TRANSMIT_SHIFT_REG_FULL     0

#define EUSART_ASYNCHRONOUS_RX_ENABLE                   1
#define EUSART_ASYNCHRONOUS_RX_DISABLE                  0

#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE         1
#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE        0

#define EUSART_ASYNCHRONOUS_9_BIT_RX_ENABLE             1
#define EUSART_ASYNCHRONOUS_9_BIT_RX_DISABLE            0

#define EUSART_ASYNCHRONOUS_FRAMING_ERROR_DETECTED      1
#define EUSART_ASYNCHRONOUS_FRAMING_ERROR_CLEARED       0

#define EUSART_ASYNCHRONOUS_OVERRUN_ERROR_DETECTED      1
#define EUSART_ASYNCHRONOUS_OVERRUN_ERROR_CLEARED       0

#define EUSART_ASYNCHRONOUS_16_BIT_BAUD_RATE_REGISTER_ENABLE                1
#define EUSART_ASYNCHRONOUS_8_BIT_BAUD_RATE_REGISTER_ENABLE                 0

/* Section : Macro Functions Declarations */
#define Usart_Tx_Cfg(_config)                          (TXSTAbits.TXEN = _config);
#define Usart_Rx_Cfg(_config)                          (RCSTAbits.CREN = _config);
#define Usart_Tx_9bit_Cfg(_config)                     (TXSTAbits.TX9 = _config);
#define Usart_Rx_9bit_Cfg(_config)                     (RCSTAbits.RX9 = _config);
/* Section : Data Types Declarations */

typedef enum{
    BAUDRATE_ASYN_8BIT_LOW_SPEED,
    BAUDRATE_ASYN_8BIT_HIGH_SPEED,
    BAUDRATE_ASYN_16BIT_LOW_SPEED,
    BAUDRATE_ASYN_16BIT_HIGH_SPEED,
    BAUDRATE_SYN_8BIT,
    BAUDRATE_SYN_16BIT
}baud_rate_gen_t;

typedef struct{
    uint8 reserved:5;
    uint8 usart_tx_enable:1;
    uint8 usart_tx_interrupt_enable:1;
    uint8 usart_tx_9bit_enable:1;
    interrupt_priority tx_priority;
}usart_tx_config_t;
typedef struct{
    uint8 reserved:5;
    uint8 usart_rx_enable:1;
    uint8 usart_rx_interrupt_enable:1;
    uint8 usart_rx_9bit_enable:1;
    interrupt_priority rx_priority;
}usart_rx_config_t;

typedef union{
    struct{
        uint8 reserved:6;
        uint8 usart_ferr:1;
        uint8 usart_oerr:1;
    };
    uint8 status;
}usart_error_status_t;

typedef struct{
    uint16 baudrate;
    baud_rate_gen_t baud_rate_cfg;
    usart_tx_config_t usart_tx_cfg;
    usart_rx_config_t usart_rx_cfg;
    usart_error_status_t usart_error_status;
    void(*EUSART_TxDefaultInterruptHandler)(void);
    void(*EUSART_RxDefaultInterruptHandler)(void);
    void(*EUSART_FerrDefaultInterruptHandler)(void);
    void(*EUSART_OerrDefaultInterruptHandler)(void);
}usart_t;
/* Section : Function Declarations */
Std_ReturnType eusart_async_init(const usart_t *usart_obj);
Std_ReturnType eusart_async_DeInit(const usart_t *usart_obj);
Std_ReturnType eusart_async_ReadByteBlocking(uint8*data);
Std_ReturnType eusart_async_ReadByteNonBlocking(uint8*data);
Std_ReturnType eusart_async_RX_Restart(void);
Std_ReturnType eusart_async_WriteByteBlocking(uint8 data);
Std_ReturnType eusart_async_WriteStringBlocking(uint8*data,uint16 length);
Std_ReturnType eusart_async_WriteByteNonBlocking(uint8 data);
Std_ReturnType eusart_async_WriteStringNonBlocking(uint8*data,uint16 length);

#endif	/* HAL_USART_H */

