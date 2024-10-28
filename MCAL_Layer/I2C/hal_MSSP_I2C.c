/* 
 * File:   hal_MSSP_I2C.C
 * Author: fathy
 *
 * Created on August 29, 2024, 2:10 PM
 */
#include "hal_MSSP_I2C.h"
static void master_clock_cfg(const mssp_i2c_t *i2c_obj);
static void slave_general_call_cfg(const mssp_i2c_t *i2c_obj);
static void slew_rate_cfg(const mssp_i2c_t *i2c_obj);
static void SMBus_cfg(const mssp_i2c_t *i2c_obj);
static void slave_general_call_cfg(const mssp_i2c_t *i2c_obj);
static void I2C_Interrupt_cfg(const mssp_i2c_t *i2c_obj);
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void(*I2C_Write_Collision_Default_Interrupt_Handler)(void);
static void(*I2C_Default_Interrupt_Handler)(void);
#endif

Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t *i2c_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == i2c_obj) {
        ret = E_NOT_OK;
    } else {
        MSSP_I2C_DISABLE_CFG();
        if (i2c_obj->i2c_cfg.i2c_mode == I2C_MASTER_MODE_SELECTED) {
            master_clock_cfg(i2c_obj);
        } else if (i2c_obj->i2c_cfg.i2c_mode == I2C_SLAVE_MODE_SELECTED) {
            slave_general_call_cfg(i2c_obj);
            MSSP_I2C_MODE_CFG(i2c_obj->i2c_cfg.mssp_i2c_mode_cfg);
            SSPCON1bits.WCOL = 0;
            SSPCON1bits.SSPOV = 0;
            SSPCON1bits.CKP = 1;
            SSPADD = i2c_obj->i2c_cfg.i2c_slave_address;
        } else {
            /*nothing*/
        }
        slew_rate_cfg(i2c_obj);
        SMBus_cfg(i2c_obj);
        TRISC = 0b00011000;
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        I2C_Interrupt_cfg(i2c_obj);
#endif
        MSSP_I2C_ENBALE_CFG();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_DeInit(const mssp_i2c_t *i2c_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == i2c_obj) {
        ret = E_NOT_OK;
    } else {
        MSSP_I2C_DISABLE_CFG();
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        MSSP_I2C_InterruptDisable();
        MSSP_I2C_Bus_collision_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Send_Start(const mssp_i2c_t *i2c_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == i2c_obj) {
        ret = E_NOT_OK;
    } else {
        /*initiate start condition on SDA/SCL*/
        SSPCON2bits.SEN = 1;
        /*wait for completion of start condition*/
        while (SSPCON2bits.SEN);
        /*clear SSPIF interrupt flag*/
        PIR1bits.SSPIF = 0;
        /*Report start condition detected*/
        if (START_BIT_DETECTED == SSPSTATbits.S) {
            ret = E_OK;
        } else {
            ret = E_NOT_OK;
        }
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start(const mssp_i2c_t *i2c_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == i2c_obj) {
        ret = E_NOT_OK;
    } else {
        /*initiate repeated start condition on SDA/SCL*/
        SSPCON2bits.RSEN = 1;
        /*wait for completion of repeated start condition*/
        while (SSPCON2bits.RSEN);
        /*clear SSPIF interrupt flag*/
        PIR1bits.SSPIF = 0;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Send_Stop(const mssp_i2c_t *i2c_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == i2c_obj) {
        ret = E_NOT_OK;
    } else {
        /*initiate stop condition on SDA/SCL*/
        SSPCON2bits.PEN = 1;
        /*wait for completion of start condition*/
        while (SSPCON2bits.PEN);
        /*clear SSPIF interrupt flag*/
        PIR1bits.SSPIF = 0;
        /*Report STOP Condition detected*/
        if (STOP_BIT_DETECTED == SSPSTATbits.P) {
            ret = E_OK;
        } else {
            ret = E_NOT_OK;
        }
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Write(const mssp_i2c_t *i2c_obj, uint8 data, uint8*_ack) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == i2c_obj || NULL == _ack) {
        ret = E_NOT_OK;
    } else {
        /*Write data to data register*/
        SSPBUF = data;
        /*wait the transmission to be completed*/
        while (!PIR1bits.SSPIF);
        /*Clear SSPIF Interrupt Flag */
        PIR1bits.SSPIF = 0;
        /*Report ACK*/
        if (ACK_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT) {
            *_ack = ACK_RECEIVED_FROM_SLAVE;
        } else {
            *_ack = ACK_NOT_RECEIVED_FROM_SLAVE;
        }
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Read(const mssp_i2c_t *i2c_obj, uint8 ack, uint8* data) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == i2c_obj || NULL == data) {
        ret = E_NOT_OK;
    } else {
        MASTER_RECEIVE_ENABLE_CFG();
        while (!SSPSTATbits.BF);
        *data = SSPBUF;
        if (I2C_MASTER_SEND_ACK == ack) {
            SSPCON2bits.ACKDT = I2C_MASTER_SEND_ACK;
        } else if (I2C_MASTER_SEND_NACK == ack) {
            SSPCON2bits.ACKDT = I2C_MASTER_SEND_NACK;
        } else {
            /*nothing*/
        }
        SSPCON2bits.ACKEN = 1;
        ret = E_OK;
    }
    return ret;
}

static void master_clock_cfg(const mssp_i2c_t *i2c_obj) {
    MSSP_I2C_MODE_CFG(i2c_obj->i2c_cfg.mssp_i2c_mode_cfg);
    SSPADD = (uint8) ((_XTAL_FREQ / (4 * i2c_obj->i2c_clock)) - 1);
}

static void slave_general_call_cfg(const mssp_i2c_t *i2c_obj) {
    if (i2c_obj->i2c_cfg.i2c_general_call == GENERAL_CALL_ENABLE) {
        GENERAL_CALL_ENABLE_CFG();
    } else if (i2c_obj->i2c_cfg.i2c_general_call == GENERAL_CALL_DISABLE) {
        GENERAL_CALL_DISABLE_CFG();
    } else {
        /*nothing*/
    }
}

static void slew_rate_cfg(const mssp_i2c_t *i2c_obj) {
    if (i2c_obj->i2c_cfg.i2c_slew_rate == SLEW_RATE_ENABLE) {
        SLEW_RATE_ENABLE_CFG();
    } else if (i2c_obj->i2c_cfg.i2c_slew_rate == SLEW_RATE_DISABLE) {
        SLEW_RATE_DISABLE_CFG();
    } else {
        /*nothing*/
    }
}

static void SMBus_cfg(const mssp_i2c_t *i2c_obj) {
    if (i2c_obj->i2c_cfg.i2c_SMBus == SMBUS_ENABLE) {
        SLEW_RATE_ENABLE_CFG();
    } else if (i2c_obj->i2c_cfg.i2c_slew_rate == SLEW_RATE_DISABLE) {
        SLEW_RATE_DISABLE_CFG();
    } else {
        /*nothing*/
    }
}

void I2C_Default_ISR(void) {
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE

    MSSP_I2C_InterruptFlagClear();
    if (I2C_Default_Interrupt_Handler) {
        I2C_Default_Interrupt_Handler();
    }
#endif
}

void I2C_Write_Collision_ISR(void) {
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE

    MSSP_I2C_InterruptFlagClear();
    if (I2C_Write_Collision_Default_Interrupt_Handler) {
        I2C_Write_Collision_Default_Interrupt_Handler();
    }
#endif
}

static void I2C_Interrupt_cfg(const mssp_i2c_t *i2c_obj) {
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    MSSP_I2C_InterruptEnable();
    MSSP_I2C_Bus_collision_InterruptEnable();
    MSSP_I2C_InterruptFlagClear();
    MSSP_I2C_Bus_collision_InterruptFlagClear();
    I2C_Default_Interrupt_Handler = i2c_obj->I2C_Default_Interrupt_Handler;
    I2C_Write_Collision_Default_Interrupt_Handler = i2c_obj->I2C_Write_Collision_Interrupt_Handler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if (INTERRUPT_HIGH_PRIORITY == i2c_obj->I2C_Default_Interrupt_Handler) {
        INTERRUPT_GlobalInterruptHighEnable();
        MSSP_I2C_HighPrioritySet();
    } else if (INTERRUPT_LOW_PRIORITY == i2c_obj->I2C_Default_Interrupt_Handler) {
        INTERRUPT_GlobalInterruptLowEnable();
        MSSP_I2C_LowPrioritySet();
    }
    if (INTERRUPT_HIGH_PRIORITY == i2c_obj->I2C_Default_Interrupt_Handler) {
        INTERRUPT_GlobalInterruptHighEnable();
        MSSP_I2C_Bus_collision_HighPrioritySet();

    } else if (INTERRUPT_LOW_PRIORITY == i2c_obj->) {
        INTERRUPT_GlobalInterruptLowEnable();
        MSSP_I2C_Bus_collision_LowPrioritySet();
    }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
}