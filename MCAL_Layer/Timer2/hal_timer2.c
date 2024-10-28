
#include "hal_timer2.h"

/* 
 * File:   hal_timer2.c
 * Author: fathy
 *
 * Created on July 19, 2024, 5:48 AM
 */
void TIMER2_ISR(void);
void (*TIMER2_InterruptHandler) (void);
static uint8 preloaded_value;

Std_ReturnType TIMER2_Init(timer2_t* timer_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TIMER2_Disable();
        TIMER2_Set_Prescaler_Value(timer_obj->prescaler_value);
        TIMER2_Set_Postscaler_Value(timer_obj->postscaler_value);
        //TIMER2_Write(timer_obj, timer_obj->preload_value);
        TMR2 = timer_obj->preload_value;
        preloaded_value = timer_obj->preload_value;
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptEnable();
        TIMER2_InterruptFlagClear();
        TIMER2_InterruptHandler = timer_obj->TIMER2_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if (INTERRUPT_HIGH_PRIORITY == timer_obj->priority) {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER2_HighPrioritySet();
        } else if (INTERRUPT_LOW_PRIORITY) {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER2_LowPrioritySet();
        }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        TIMER2_Enable();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER2_DeInit(timer2_t* timer_obj) {
    Std_ReturnType ret = E_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TIMER2_Disable();
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER2_Read(timer2_t* timer_obj, uint8 *value) {
    Std_ReturnType ret = E_OK;
    if ((NULL == timer_obj) || (NULL == value)) {
        ret = E_NOT_OK;
    } else {
        *value = TMR2;
         ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER2_Write(timer2_t* timer_obj, uint8 value) {
    Std_ReturnType ret = E_OK;
    if ((NULL == timer_obj) || (NULL == value)) {
        ret = E_NOT_OK;
    } else {
        TMR2 = value;
        ret = E_OK;
    }
    return ret;
}

void TIMER2_ISR(void) {
    TIMER2_InterruptFlagClear();
    TMR2 = preloaded_value;
    if (TIMER2_InterruptHandler) {
        TIMER2_InterruptHandler();
    } else {
        /*nothing*/
    }
}