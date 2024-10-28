/* 
 * File:   hal_timer3.c
 * Author: fathy
 *
 * Created on July 19, 2024, 8:28 AM
 */
#include "hal_timer3.h"
static void inline Timer3_timer_mode_config(timer3_t* timer_obj);
void TIMER3_ISR(void);
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(*TIMER3_InterruptHandler)(void);
#endif
static uint16 preloaded_value = ZERO_INIT;

Std_ReturnType TIMER3_Init(timer3_t* timer_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TIMER3_Disable();
        TIMER3_Set_Prescaler_Value(timer_obj->prescaler_value);
        Timer3_timer_mode_config(timer_obj);
        TIMER3_Write(timer_obj, timer_obj->preload_value);
        preloaded_value = timer_obj->preload_value;
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptEnable();
        TIMER3_InterruptFlagClear();
        TIMER3_InterruptHandler = timer_obj->TIMER3_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if (INTERRUPT_HIGH_PRIORITY == timer_obj->priority) {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER3_HighPrioritySet();
        } else if (INTERRUPT_LOW_PRIORITY) {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER3_LowPrioritySet();
        }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        TIMER3_Enable();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER3_DeInit(timer3_t* timer_obj) {
    Std_ReturnType ret = E_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TIMER3_Disable();
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER3_Read(timer3_t* timer_obj, uint16 *value) {
    Std_ReturnType ret = E_OK;
    if ((NULL == timer_obj) || (NULL == value)) {
        ret = E_NOT_OK;
    } else {
        uint8 l_tmr3l = TMR3L;
        uint8 l_tmr3h = TMR3H;
        *value = (uint16) ((l_tmr3h << 8) + l_tmr3l);
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER3_Write(timer3_t* timer_obj, uint16 value) {
    Std_ReturnType ret = E_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TMR3H = value >> 8;
        TMR3L = (uint8) (value);
        ret = E_OK;
    }
    return ret;
}

static void inline Timer3_timer_mode_config(timer3_t* timer_obj) {
    if (timer_obj->timer3_Mode == TIMER3_TIMER_MODE) {
        TIMER3_Set_Timer_Mode();
    } else if (timer_obj->timer3_Mode == TIMER3_COUNTER_MODE) {
        TIMER3_Set_Counter_Mode();
        if (timer_obj->timer3_counter_mode == TIMER3_SYNCHRONOUS_EXTERNAL_CLOCK_INPUT_CFG) {
            TIMER3_Synchronous_External_Clock_Input();
        } else if (timer_obj->timer3_counter_mode == TIMER3_ASYNCHRONOUS_EXTERNAL_CLOCK_INPUT_CFG) {
            TIMER3_Asynchronouse_External_Clock_Input();
        } else {
            /*nothing*/
        }
    } else {
        /*nothing*/
    }
}

void TIMER3_ISR(void) {
    TIMER3_InterruptFlagClear();
    TMR3H = preloaded_value >> 8;
    TMR3L = (uint8) (preloaded_value);
    if (TIMER3_InterruptHandler) {
        TIMER3_InterruptHandler();
    } else {
        /*nothing*/
    }
}