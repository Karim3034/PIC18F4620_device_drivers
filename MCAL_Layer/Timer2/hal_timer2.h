/* 
 * File:   hal_timer2.h
 * Author: fathy
 *
 * Created on July 19, 2024, 5:48 AM
 */
#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H
/* Section : Includes */
#include "pic18f4620.h"
#include "../MCAL_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../GPIO/../hal_gpio.h"

/* Section : Macro Declarations */


/* Section : Macro Functions Declarations */
#define TIMER2_Enable()                        (T2CONbits.TMR2ON = 1)
#define TIMER2_Disable()                       (T2CONbits.TMR2ON = 0)
#define TIMER2_Set_Prescaler_Value(_config)    (T2CONbits.T2CKPS =_config)
#define TIMER2_Set_Postscaler_Value(_config)   (T2CONbits.TOUTPS =_config)
/* Section : Data Types Declarations */
typedef enum {
    TIMER2_PRESCALER_DIV_1,
    TIMER2_PRESCALER_DIV_4,
    TIMER2_PRESCALER_DIV_16,
}timer2_prescaler_value_t;
typedef enum {
    TIMER2_POSTSCALER_DIV_1,
    TIMER2_POSTSCALER_DIV_2,
    TIMER2_POSTSCALER_DIV_3,
    TIMER2_POSTSCALER_DIV_4,
    TIMER2_POSTSCALER_DIV_5,
    TIMER2_POSTSCALER_DIV_6,
    TIMER2_POSTSCALER_DIV_7,
    TIMER2_POSTSCALER_DIV_8,
    TIMER2_POSTSCALER_DIV_9,
    TIMER2_POSTSCALER_DIV_10,
    TIMER2_POSTSCALER_DIV_11,
    TIMER2_POSTSCALER_DIV_12,
    TIMER2_POSTSCALER_DIV_13,
    TIMER2_POSTSCALER_DIV_14,
    TIMER2_POSTSCALER_DIV_15,
    TIMER2_POSTSCALER_DIV_16
}timer2_postscaler_value_t;

typedef struct{
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(*TIMER2_InterruptHandler)(void);
    interrupt_priority priority;
#endif
    uint8 preload_value;
    timer2_postscaler_value_t postscaler_value;
    timer2_prescaler_value_t prescaler_value;
    uint8 reserved:2;
}timer2_t;
/* Section : Function Declarations */
Std_ReturnType TIMER2_Init(timer2_t* timer_obj);
Std_ReturnType TIMER2_DeInit(timer2_t* timer_obj);
Std_ReturnType TIMER2_Read(timer2_t* timer_obj,uint8 *value);
Std_ReturnType TIMER2_Write(timer2_t* timer_obj,uint8 value);
#endif	/* HAL_TIMER3_H */


