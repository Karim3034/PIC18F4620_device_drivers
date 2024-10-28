/* 
 * File:   hal_timer3.h
 * Author: fathy
 *
 * Created on July 19, 2024, 8:28 AM
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H
/* Section : Includes */
#include "pic18f4620.h"
#include "../MCAL_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../GPIO/../hal_gpio.h"

/* Section : Macro Declarations */
#define TIMER3_8BIT_REGISTER_SIZE                      0
#define TIMER3_16BIT_REGISTER_SIZE                     1
#define TIMER3_TIMER_MODE                              0
#define TIMER3_COUNTER_MODE                            1
#define TIMER3_SYNCHRONOUS_EXTERNAL_CLOCK_INPUT_CFG    0
#define TIMER3_ASYNCHRONOUS_EXTERNAL_CLOCK_INPUT_CFG   1
/* Section : Macro Functions Declarations */
#define TIMER3_Enable()                                 (T3CONbits.TMR3ON = 1)
#define TIMER3_Disable()                                (T3CONbits.TMR3ON = 0)
#define TIMER3_Set_Timer_Mode()                         (T3CONbits.TMR3CS=0)        
#define TIMER3_Set_Counter_Mode()                       (T3CONbits.TMR3CS=1)
#define TIMER3_Synchronous_External_Clock_Input()       (T3CONbits.T3SYNC=0)
#define TIMER3_Asynchronouse_External_Clock_Input()     (T3CONbits.T3SYNC=1)
#define TIMER3_Set_Prescaler_Value(_config)             (T3CONbits.T3CKPS = _config)
#define TIMER3_Set_8bit_Register_Size()                 (T3CONbits.RD16 = 0)
#define TIMER3_Set_16bit_Register_Size()                (T3CONbits.RD16 = 1)
/* Section : Data Types Declarations */
typedef enum {
    TIMER3_PRESCALER_DIV_1,
    TIMER3_PRESCALER_DIV_2,
    TIMER3_PRESCALER_DIV_4,
    TIMER3_PRESCALER_DIV_8,
}timer3_prescaler_value_t;

typedef struct{
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(*TIMER3_InterruptHandler)(void);
    interrupt_priority priority;
#endif
    uint16 preload_value;
    timer3_prescaler_value_t prescaler_value;
    uint8 timer3_Mode : 1;
    uint8 timer3_counter_mode : 1;
    uint8 timer3_register_size:1;
    uint8 reserved:3;
}timer3_t;
/* Section : Function Declarations */
Std_ReturnType TIMER3_Init(timer3_t* timer_obj);
Std_ReturnType TIMER3_DeInit(timer3_t* timer_obj);
Std_ReturnType TIMER3_Read(timer3_t* timer_obj,uint16 *value);
Std_ReturnType TIMER3_Write(timer3_t* timer_obj,uint16 value);
#endif	/* HAL_TIMER3_H */

