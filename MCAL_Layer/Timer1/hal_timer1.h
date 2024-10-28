/* 
 * File:   hal_timer1.h
 * Author: fathy
 *
 * Created on July 17, 2024, 6:01 PM
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H
/* Section : Includes */
#include "pic18f4620.h"
#include "../MCAL_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../GPIO/../hal_gpio.h"

/* Section : Macro Declarations */
#define TIMER1_8BIT_REGISTER_SIZE                      0
#define TIMER1_16BIT_REGISTER_SIZE                     1
#define TIMER1_TIMER_MODE                              0
#define TIMER1_COUNTER_MODE                            1
#define TIMER1_OSCILLATOR_ENABLE_CFG                   1
#define TIMER1_OSCILLATOR_DISABLE_CFG                  0  
#define TIMER1_SYNCHRONOUS_EXTERNAL_CLOCK_INPUT_CFG    0
#define TIMER1_ASYNCHRONOUS_EXTERNAL_CLOCK_INPUT_CFG   1
/* Section : Macro Functions Declarations */
#define TIMER1_Enable()                                 (T1CONbits.TMR1ON = 1)
#define TIMER1_Disable()                                (T1CONbits.TMR1ON = 0)
#define TIMER1_Set_Timer_Mode()                         (T1CONbits.TMR1CS=0)        
#define TIMER1_Set_Counter_Mode()                       (T1CONbits.TMR1CS=1)
#define TIMER1_Synchronous_External_Clock_Input()       (T1CONbits.T1SYNC=0)
#define TIMER1_Asynchronouse_External_Clock_Input()     (T1CONbits.T1SYNC=1)
#define Timer1_Oscillator_Enable()                      (T1CONbits.T1OSCEN=1)
#define Timer1_Oscillator_Disable()                     (T1CONbits.T1OSCEN=0)
#define TIMER1_Set_Prescaler_Value(_config)             (T1CONbits.T1CKPS = _config)
#define TIMER1_SYSTEM_CLOCK_STATUS()                    (T1CONbits.T1RUN)
#define TIMER1_Set_8bit_Register_Size()                 (T1CONbits.RD16 = 0)
#define TIMER1_Set_16bit_Register_Size()                (T1CONbits.RD16 = 1)
/* Section : Data Types Declarations */
typedef enum {
    PRESCALER_DIV_BY_1,
    PRESCALER_DIV_BY_2,
    PRESCALER_DIV_BY_4,
    PRESCALER_DIV_BY_8,
}timer1_prescaler_value_t;

typedef struct{
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(*TIMER1_InterruptHandler)(void);
    interrupt_priority priority;
#endif
    uint16 preload_value;
    timer1_prescaler_value_t prescaler_value;
    uint8 timer1_register_size:1;
    uint8 timer1_Mode : 1;
    uint8 timer1_counter_mode : 1;
    uint8 timer1_osc_cfg : 1;
    uint8 reserved:2;
}timer1_t;
/* Section : Function Declarations */
Std_ReturnType TIMER1_Init(timer1_t* timer_obj);
Std_ReturnType TIMER1_DeInit(timer1_t* timer_obj);
Std_ReturnType TIMER1_Read(timer1_t* timer_obj,uint16 *value);
Std_ReturnType TIMER1_Write(timer1_t* timer_obj,uint16 value);
#endif	/* HAL_TIMER1_H */

