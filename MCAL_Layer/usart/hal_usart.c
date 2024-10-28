/* 
 * File:   hal_usart.c
 * Author: fathy
 *
 * Created on August 17, 2024, 2:35 AM
 */
#include "hal_usart.h"
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(*EUSART_TxInterruptHandler)(void);
#endif
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(*EUSART_RxInterruptHandler)(void);
static void(*EUSART_FerrInterruptHandler)(void);
static void(*EUSART_OerrInterruptHandler)(void);
#endif
void static eusart_baudrate_config(const usart_t *usart_obj);
void static eusart_async_tx_config(const usart_t *usart_obj);
void static eusart_async_rx_config(const usart_t *usart_obj);

Std_ReturnType eusart_async_init(const usart_t *usart_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == usart_obj) {
        ret = E_NOT_OK;
    } else {
        RCSTAbits.SPEN = EUSART_MODULE_DISABLE;
        TRISCbits.RC6 = 1;
        TRISCbits.RC7 = 1;
        eusart_baudrate_config(usart_obj);
        eusart_async_tx_config(usart_obj);
        eusart_async_rx_config(usart_obj);
        RCSTAbits.SPEN = EUSART_MODULE_ENABLE;
    }
    return ret;
}

Std_ReturnType eusart_async_DeInit(const usart_t *usart_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == usart_obj) {
        ret = E_NOT_OK;
    } else {
        RCSTAbits.SPEN = EUSART_MODULE_DISABLE;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType eusart_async_ReadByteBlocking(uint8*data) {
    Std_ReturnType ret = E_OK;
    while (!PIR1bits.RCIF);
    *data = RCREG;
    return ret;
}

Std_ReturnType eusart_async_ReadByteNonBlocking(uint8*data) {
    Std_ReturnType ret = E_OK;
    if (PIR1bits.RCIF) {
        *data = RCREG;
    } else {
        ret = E_NOT_OK;
    }
    return ret;
}
Std_ReturnType eusart_async_RX_Restart(void){
    RCSTAbits.CREN = 0;
    RCSTAbits.CREN = 1;
}
Std_ReturnType eusart_async_WriteByteBlocking(uint8 data) {
    Std_ReturnType ret = E_OK;
    while (!TXSTAbits.TRMT);
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    EUSART_TX_InterruptEnable();
#endif
    TXREG = data;
    return ret;
}

Std_ReturnType eusart_async_WriteStringBlocking(uint8*data, uint16 length) {
    Std_ReturnType ret = E_NOT_OK;
    uint16 l_counter_data = 0;
    if (NULL == data) {
        ret = E_NOT_OK;
    } else {
        for (l_counter_data = 0; l_counter_data < length; l_counter_data++) {
            ret = eusart_async_WriteByteBlocking(data[l_counter_data]);
        }
    }
    return ret;
}
Std_ReturnType eusart_async_WriteByteNonBlocking(uint8 data){
    
}
Std_ReturnType eusart_async_WriteStringNonBlocking(uint8*data,uint16 length){
    
}
void static eusart_baudrate_config(const usart_t *usart_obj) {
    volatile float baudrate_temp = 0;
    switch (usart_obj->baud_rate_cfg) {
        case BAUDRATE_ASYN_8BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE_SELECT;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUS_8_BIT_BAUD_RATE_REGISTER_ENABLE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            baudrate_temp = ((_XTAL_FREQ / (float) usart_obj->baudrate) / 64.0) - 1;
            break;
        case BAUDRATE_ASYN_8BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE_SELECT;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUS_8_BIT_BAUD_RATE_REGISTER_ENABLE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            baudrate_temp = ((_XTAL_FREQ / (float) usart_obj->baudrate) / 16.0) - 1;
            break;
        case BAUDRATE_ASYN_16BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE_SELECT;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUS_16_BIT_BAUD_RATE_REGISTER_ENABLE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            baudrate_temp = ((_XTAL_FREQ / (float) usart_obj->baudrate) / 16.0) - 1;
            break;
        case BAUDRATE_ASYN_16BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE_SELECT;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUS_16_BIT_BAUD_RATE_REGISTER_ENABLE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            baudrate_temp = ((_XTAL_FREQ / (float) usart_obj->baudrate) / 4.0) - 1;
            break;
        case BAUDRATE_SYN_8BIT:
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE_SELECT;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUS_8_BIT_BAUD_RATE_REGISTER_ENABLE;
            baudrate_temp = ((_XTAL_FREQ / (float) usart_obj->baudrate) / 4.0) - 1;
            break;
        case BAUDRATE_SYN_16BIT:
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE_SELECT;
            BAUDCONbits.BRG16 = EUSART_ASYNCHRONOUS_16_BIT_BAUD_RATE_REGISTER_ENABLE;
            baudrate_temp = ((_XTAL_FREQ / (float) usart_obj->baudrate) / 4.0) - 1;
            break;
        default:
            break;
    }
    SPBRG = (uint8) ((uint32) baudrate_temp);
    SPBRGH = (uint8) (((uint32) baudrate_temp) >> 8);
}

void static eusart_async_tx_config(const usart_t *usart_obj) {
    if (usart_obj->usart_tx_cfg.usart_tx_enable == EUSART_ASYNCHRONOUS_TX_ENABLE ||
            usart_obj->usart_tx_cfg.usart_tx_enable == EUSART_ASYNCHRONOUS_TX_DISABLE) {
        Usart_Tx_Cfg(usart_obj->usart_tx_cfg.usart_tx_enable);
        EUSART_TxInterruptHandler = usart_obj->EUSART_TxDefaultInterruptHandler;
        if (usart_obj->usart_tx_cfg.usart_tx_interrupt_enable == EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE) {
            PIE1bits.TXIE = EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE;
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            EUSART_TX_InterruptEnable();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelsEnable();
            if (INTERRUPT_HIGH_PRIORITY == usart_obj->usart_tx_cfg.tx_priority) {
                INTERRUPT_GlobalInterruptHighEnable();
                EUSART_TX_HighPrioritySet();
            } else if (INTERRUPT_LOW_PRIORITY) {
                INTERRUPT_GlobalInterruptLowEnable();
                EUSART_TX_LowPrioritySet();
            }
#else
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        } else {
            PIE1bits.TXIE = EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE;
        }

        if (usart_obj->usart_tx_cfg.usart_tx_9bit_enable == EUSART_ASYNCHRONOUS_9_BIT_TX_ENABLE ||
                usart_obj->usart_tx_cfg.usart_tx_9bit_enable == EUSART_ASYNCHRONOUS_9_BIT_TX_DISABLE) {
            Usart_Tx_9bit_Cfg(usart_obj->usart_tx_cfg.usart_tx_9bit_enable);
        } else {
            Usart_Tx_9bit_Cfg(EUSART_ASYNCHRONOUS_9_BIT_TX_DISABLE);
        }
    } else {
        Usart_Tx_Cfg(EUSART_ASYNCHRONOUS_TX_DISABLE);
    }
}

void static eusart_async_rx_config(const usart_t *usart_obj) {
    if (usart_obj->usart_rx_cfg.usart_rx_enable == EUSART_ASYNCHRONOUS_RX_ENABLE ||
            usart_obj->usart_rx_cfg.usart_rx_enable == EUSART_ASYNCHRONOUS_RX_DISABLE) {
        Usart_Rx_Cfg(usart_obj->usart_rx_cfg.usart_rx_enable);
        EUSART_RxInterruptHandler = usart_obj->EUSART_RxDefaultInterruptHandler;
        EUSART_FerrInterruptHandler = usart_obj->EUSART_FerrDefaultInterruptHandler;
        EUSART_OerrInterruptHandler = usart_obj->EUSART_OerrDefaultInterruptHandler;
        if (usart_obj->usart_rx_cfg.usart_rx_interrupt_enable == EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE) {
            PIE1bits.RCIE = EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE;
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            EUSART_RX_InterruptEnable();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelsEnable();
            if (INTERRUPT_HIGH_PRIORITY == usart_obj->usart_rx_cfg.rx_priority) {
                INTERRUPT_GlobalInterruptHighEnable();
                EUSART_RX_HighPrioritySet();
            } else if (INTERRUPT_LOW_PRIORITY) {
                INTERRUPT_GlobalInterruptLowEnable();
                EUSART_RX_LowPrioritySet();
            }
#else
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        } else {
            PIE1bits.RCIE = EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE;
        }

        if (usart_obj->usart_rx_cfg.usart_rx_9bit_enable == EUSART_ASYNCHRONOUS_9_BIT_RX_ENABLE ||
                usart_obj->usart_rx_cfg.usart_rx_9bit_enable == EUSART_ASYNCHRONOUS_9_BIT_RX_DISABLE) {
            Usart_Rx_9bit_Cfg(usart_obj->usart_rx_cfg.usart_rx_9bit_enable);
        } else {
            Usart_Rx_9bit_Cfg(EUSART_ASYNCHRONOUS_9_BIT_RX_DISABLE);
        }
    } else {
        Usart_Rx_Cfg(EUSART_ASYNCHRONOUS_RX_DISABLE);
    }
}

void EUSART_TX_ISR(void) {
    EUSART_TX_InterruptDisable();
    if (EUSART_TxInterruptHandler) {
        EUSART_TxInterruptHandler();
    } else {
        /*nothing*/
    }
}

void EUSART_RX_ISR(void) {
    if (EUSART_RxInterruptHandler) {
        EUSART_RxInterruptHandler();
    } else {
        /*nothing*/
    }
    if (EUSART_FerrInterruptHandler) {
        EUSART_FerrInterruptHandler();
    } else {
        /*nothing*/
    }
    if (EUSART_OerrInterruptHandler) {
        EUSART_OerrInterruptHandler();
    } else {
        /*nothing*/
    }
}