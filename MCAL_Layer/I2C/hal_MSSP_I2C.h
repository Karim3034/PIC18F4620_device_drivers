/* 
 * File:   hal_MSSP_I2C.h
 * Author: fathy
 *
 * Created on August 29, 2024, 2:10 PM
 */

#ifndef HAL_MSSP_I2C_H
#define	HAL_MSSP_I2C_H
#include "hal_MSSP_I2C_cfg.h"
#include "pic18f4620.h"
#include "../MCAL_std_types.h"
#include "../hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"


#define I2C_MASTER_MODE_SELECTED             1
#define I2C_SLAVE_MODE_SELECTED              0

#define SLEW_RATE_DISABLE           1
#define SLEW_RATE_ENABLE            0

#define SMBUS_ENABLE                1
#define SMBUS_DISABLE               0

#define LAST_BYTE_DATA              1
#define LAST_BYTE_ADDRESS           0

#define STOP_BIT_DETECTED           1
#define STOP_BIT_NOT_DETECTED       0

#define START_BIT_DETECTED          1
#define START_BIT_NOT_DETECTED      0

#define SLAVE_READ                  1
#define SLAVE_WRITE                 0

#define MASTER_TRANSMIT_IS_IN_PROGRESS 1
#define MASTER_TRANSMIT_IS_NOT_IN_PROGRESS 0

#define UPDATE_ADDRESS_10BIT         1        
#define DO_NOT_UPDATE_ADDRESS_10BIT  0

#define SSPBUFF_IS_FULL              1
#define SSPBUFF_IS_EMPTY             0

#define RELEASE_CLOCK                1
#define HOLDS_CLOCK                  0

#define MSSP_I2C_ENABLE              1
#define MSSP_I2C_DISABLE             0

#define I2C_SLAVE_MODE_10BIT_ADDRESS_WITH_INTERRUPT         0x0F
#define I2C_SLAVE_MODE_7BIT_ADDRESS_WITH_INTERRUPT          0x0E
#define I2C_MASTER_MODE_FIRMWARE_CONTROLLED                 0x0B
#define I2C_MASTER_MODE                                     0x08
#define I2C_SLAVE_MODE_10BIT_ADDRESS                        0x07
#define I2C_SLAVE_MODE_7BIT_ADDRESS                         0x06

#define GENERAL_CALL_ENABLE                                 1
#define GENERAL_CALL_DISABLE                                0

#define MASTER_RECEIVE_ENABLE                               1
#define MASTER_RECEIVE_DISABLE                              0
        
#define ACK_NOT_RECEIVED_FROM_SLAVE                        1                        
#define ACK_RECEIVED_FROM_SLAVE                            0

#define I2C_MASTER_SEND_NACK                                1                        
#define I2C_MASTER_SEND_ACK                                 0   

#define MSSP_I2C_ENBALE_CFG()                                (SSPCON1bits.SSPEN = 1)
#define MSSP_I2C_DISABLE_CFG()                               (SSPCON1bits.SSPEN = 0)
#define SLEW_RATE_ENABLE_CFG()                               (SSPSTATbits.SMP=0)
#define SLEW_RATE_DISABLE_CFG()                              (SSPSTATbits.SMP=1)
#define SMBUS_ENABLE_CFG()                                   (SSPSTATbits.CKE=1)
#define SMBUS_DISABLE_CFG()                                  (SSPSTATbits.CKE=0)
#define GENERAL_CALL_ENABLE_CFG()                            (SSPCON2bits.GCEN=1)
#define GENERAL_CALL_DISABLE_CFG()                           (SSPCON2bits.GCEN=0)
#define MSSP_I2C_MODE_CFG(_config)                           (SSPCON1bits.SSPM = _config)
#define MASTER_RECEIVE_ENABLE_CFG()                          (SSPCON2bits.RCEN = 1)
#define MASTER_RECEIVE_DISABLE_CFG()                         (SSPCON2bits.RCEN = 0)
#define I2C_CLOCK_RELEASE_CFG()                              (SSPCON1bits.CKP = 1)
#define I2C_CLOCK_STRETCH_CFG()                              (SSPCON1bits.CKP = 0)
typedef struct{
    uint8 mssp_i2c_mode_cfg;
    uint8 i2c_slave_address;
    uint8 i2c_mode:1;
    uint8 i2c_slew_rate:1;
    uint8 i2c_SMBus:1;
    uint8 i2c_general_call:1;
    uint8 MASTER_RECEIVE:1;
    uint8 reserved:3;
}i2c_config_t;
typedef struct{
    uint32 i2c_clock;
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void(*I2C_Write_Collision_Interrupt_Handler)(void);
    void(*I2C_RECEIVE_OVERFLOW_Interrupt_Handler)(void);
    void(*I2C_Default_Interrupt_Handler)(void);
    interrupt_priority mssp_i2c_priority;
    interrupt_priority mssp_i2c_bc_priority;
#endif
    i2c_config_t i2c_cfg;
}mssp_i2c_t;


Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_DeInit(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Start(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Stop(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Write(const mssp_i2c_t *i2c_obj,uint8 data,uint8*_ack);
Std_ReturnType MSSP_I2C_Slave_Write(const mssp_i2c_t *i2c_obj,uint8 data,uint8*_ack);
Std_ReturnType MSSP_I2C_Read(const mssp_i2c_t *i2c_obj,uint8 ack,uint8* data);
#endif	/* HAL_MSSP_I2C_H */

