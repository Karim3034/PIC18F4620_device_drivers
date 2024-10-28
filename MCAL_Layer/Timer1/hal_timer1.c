/* 
 * File:   hal_timer1.c
 * Author: fathy
 *
 * Created on July 17, 2024, 6:01 PM
 */
#include "hal_timer1.h"
static void inline Timer1_timer_mode_config(timer1_t* timer_obj);
static void inline Timer1_osc_config(timer1_t* timer_obj);
void TIMER1_ISR(void);
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(*TIMER1_InterruptHandler)(void);
#endif
static uint16 preloaded_value = ZERO_INIT;

Std_ReturnType TIMER1_Init(timer1_t* timer_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TIMER1_Disable();
        TIMER1_Set_Prescaler_Value(timer_obj->prescaler_value);
        Timer1_timer_mode_config(timer_obj);
        TIMER1_Write(timer_obj, timer_obj->preload_value);
        preloaded_value = timer_obj->preload_value;
        Timer1_osc_config;
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptEnable();
        TIMER1_InterruptFlagClear();
        TIMER1_InterruptHandler = timer_obj->TIMER1_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if (INTERRUPT_HIGH_PRIORITY == timer_obj->priority) {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER1_HighPrioritySet();
        } else if (INTERRUPT_LOW_PRIORITY) {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER1_LowPrioritySet();
        }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        TIMER1_Enable();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER1_DeInit(timer1_t* timer_obj) {
    Std_ReturnType ret = E_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TIMER1_Disable();
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER1_Read(timer1_t* timer_obj, uint16 *value) {
    Std_ReturnType ret = E_OK;
    if ((NULL == timer_obj) || (NULL == value)) {
        ret = E_NOT_OK;
    } else {
        uint8 l_tmr0l = TMR1L;
        uint8 l_tmr0h = TMR1H;
        *value = (uint16) ((l_tmr0h << 8) + l_tmr0l);
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER1_Write(timer1_t* timer_obj, uint16 value) {
    Std_ReturnType ret = E_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TMR1H = value >> 8;
        TMR1L = (uint8) (value);
        ret = E_OK;
    }
    return ret;
}

static void inline Timer1_timer_mode_config(timer1_t* timer_obj) {
    if (timer_obj->timer1_Mode == TIMER1_TIMER_MODE) {
        TIMER1_Set_Timer_Mode();
    } else if (timer_obj->timer1_Mode == TIMER1_COUNTER_MODE) {
        TIMER1_Set_Counter_Mode();
        if (timer_obj->timer1_counter_mode == TIMER1_SYNCHRONOUS_EXTERNAL_CLOCK_INPUT_CFG) {
            TIMER1_Synchronous_External_Clock_Input();
        } else if (timer_obj->timer1_counter_mode == TIMER1_ASYNCHRONOUS_EXTERNAL_CLOCK_INPUT_CFG) {
            TIMER1_Asynchronouse_External_Clock_Input();
        } else {
            /*nothing*/
        }
    } else {
        /*nothing*/
    }
}

static void inline Timer1_osc_config(timer1_t* timer_obj) {
    if (timer_obj->timer1_osc_cfg == TIMER1_OSCILLATOR_ENABLE_CFG) {
        Timer1_Oscillator_Enable();
    } else if (timer_obj->timer1_register_size == TIMER1_OSCILLATOR_DISABLE_CFG) {
        Timer1_Oscillator_Disable();
    } else {
        /*nothing*/
    }
}

void TIMER1_ISR(void) {
    TIMER1_InterruptFlagClear();
    TMR1H = preloaded_value >> 8;
    TMR1L = (uint8) (preloaded_value);
    if (TIMER1_InterruptHandler) {
        TIMER1_InterruptHandler();
    } else {
        /*nothing*/
    }
}