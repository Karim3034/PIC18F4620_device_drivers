/* 
 * File:   hal_timer0.h
 * Author: fathy
 *
 * Created on July 16, 2024, 8:01 PM
 */

#ifndef HAL_TIMER0_H
#define	HAL_TIMER0_H
/* Section : Includes */
#include "pic18f4620.h"
#include "../MCAL_std_types.h"
#include "hal_timer0_cfg.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../GPIO/../hal_gpio.h"

/* Section : Macro Declarations */
#define TIMER0_8BIT_REGISTER_SIZE               1
#define TIMER0_16BIT_REGISTER_SIZE              0
#define TIMER0_TIMER_MODE                       0
#define TIMER0_COUNTER_MODE                     1
#define TIMER0_INCREMENT_ON_RISING_EDGE         0
#define TIMER0_INCREMENT_ON_FALLING_EDGE        1
#define TIMER0_PRESCALER_Enable                 0
#define TIMER0_PRESCALER_Disable                1

/* Section : Macro Functions Declarations */
#define TIMER0_Enable()                        (T0CONbits.TMR0ON = 1)
#define TIMER0_Disable()                       (T0CONbits.TMR0ON = 0)
#define TIMER0_Set_8bit_Register_Size()        (T0CONbits.T08BIT = 1)
#define TIMER0_Set_16bit_Register_Size()       (T0CONbits.T08BIT = 0)
#define TIMER0_Set_Timer_Mode()                (T0CONbits.T0CS=0)
#define TIMER0_Set_Counter_Mode()              (T0CONbits.T0CS=1)
#define TIMER0_Set_Increment_On_Rising_Edge()  (T0CONbits.T0SE = 0)
#define TIMER0_Set_Increment_On_Falling_Edge() (T0CONbits.T0SE = 1)
#define TIMER0_Set_Prescaler_Enable()          (T0CONbits.PSA = 0)
#define TIMER0_Set_Prescaler_Disable()         (T0CONbits.PSA = 1)
#define TIMER0_Set_Prescaler_Value(_config)    (T0CONbits.T0PS = _config)
/* Section : Data Types Declarations */
typedef enum {
    PRESCALER_DIV_2,
    PRESCALER_DIV_4,
    PRESCALER_DIV_8,
    PRESCALER_DIV_16,
    PRESCALER_DIV_32,
    PRESCALER_DIV_64,
    PRESCALER_DIV_128,
    PRESCALER_DIV_256
}timer0_prescaler_value_t;

typedef struct{
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(*TIMER0_InterruptHandler)(void);
    interrupt_priority priority;
#endif
    timer0_prescaler_value_t prescaler_value;
    uint16 preload_value;
    uint8 timer0_register_size:1;
    uint8 timer0_Mode : 1;
    uint8 timer0_increment_type : 1;
    uint8 timer0_prescaler_status : 1;
    uint8 :1;
}timer0_t;
/* Section : Function Declarations */
Std_ReturnType TIMER0_Init(timer0_t* timer_obj);
Std_ReturnType TIMER0_DeInit(timer0_t* timer_obj);
Std_ReturnType TIMER0_Read(timer0_t* timer_obj,uint16 *value);
Std_ReturnType TIMER0_Write(timer0_t* timer_obj,uint16 value);
#endif	/* HAL_TIMER0_H */

