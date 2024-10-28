
/* 
 * File:   hal_timer0.c
 * Author: fathy
 *
 * Created on July 16, 2024, 8:01 PM
 */
#include "hal_timer0.h"
static void inline Timer0_timer_mode_config(timer0_t* timer_obj);
static void inline Timer0_register_size_config(timer0_t* timer_obj);
static void inline Timer0_increment_type_config(timer0_t* timer_obj);
static void inline Timer0_prescaler_config(timer0_t* timer_obj);
void TIMER0_ISR(void);
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(*TIMER0_InterruptHandler)(void);
#endif
static uint16 preloaded_value = ZERO_INIT;

Std_ReturnType TIMER0_Init(timer0_t* timer_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TIMER0_Disable();
        Timer0_timer_mode_config(timer_obj);
        Timer0_register_size_config(timer_obj);
        Timer0_prescaler_config(timer_obj);
        TIMER0_Write(timer_obj, timer_obj->preload_value);
        preloaded_value = timer_obj->preload_value;
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER0_InterruptEnable();
        TIMER0_InterruptFlagClear();
        TIMER0_InterruptHandler = timer_obj->TIMER0_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if (INTERRUPT_HIGH_PRIORITY == timer_obj->priority) {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER0_HighPrioritySet();
        } else if (INTERRUPT_LOW_PRIORITY) {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER0_LowPrioritySet();
        }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        TIMER0_Enable();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER0_DeInit(timer0_t* timer_obj) {
    Std_ReturnType ret = E_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TIMER0_Disable();
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER0_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER0_Read(timer0_t* timer_obj, uint16 *value) {
    Std_ReturnType ret = E_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        uint8 l_tmr0l = TMR0L;
        uint8 l_tmr0h = TMR0H;
        *value = (uint16) ((l_tmr0h << 8) + l_tmr0l);
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType TIMER0_Write(timer0_t* timer_obj, uint16 value) {
    Std_ReturnType ret = E_OK;
    if (NULL == timer_obj) {
        ret = E_NOT_OK;
    } else {
        TMR0H = value >> 8;
        TMR0L = (uint8) (value);
        ret = E_OK;
    }
    return ret;
}

static void inline Timer0_timer_mode_config(timer0_t* timer_obj) {
    if (timer_obj->timer0_Mode == TIMER0_TIMER_MODE) {
        TIMER0_Set_Timer_Mode();
    } else if (timer_obj->timer0_Mode == TIMER0_COUNTER_MODE) {
        TIMER0_Set_Counter_Mode();
        Timer0_increment_type_config(timer_obj);
    } else {
        /*nothing*/
    }
}

static void inline Timer0_register_size_config(timer0_t* timer_obj) {
    if (timer_obj->timer0_register_size == TIMER0_8BIT_REGISTER_SIZE) {
        TIMER0_Set_8bit_Register_Size();
    } else if (timer_obj->timer0_register_size == TIMER0_16BIT_REGISTER_SIZE) {
        TIMER0_Set_16bit_Register_Size();
    } else {
        /*nothing*/
    }
}

static void inline Timer0_increment_type_config(timer0_t* timer_obj) {
    if (timer_obj->timer0_increment_type == TIMER0_INCREMENT_ON_RISING_EDGE) {
        TIMER0_Set_Increment_On_Rising_Edge();
    } else if (timer_obj->timer0_register_size == TIMER0_INCREMENT_ON_FALLING_EDGE) {
        TIMER0_Set_Increment_On_Falling_Edge();
    } else {
        /*nothing*/
    }
}

static void inline Timer0_prescaler_config(timer0_t* timer_obj) {
    if (timer_obj->timer0_prescaler_status == TIMER0_PRESCALER_Enable) {
        TIMER0_Set_Prescaler_Enable();
        TIMER0_Set_Prescaler_Value(timer_obj->prescaler_value);
    } else if (timer_obj->timer0_prescaler_status == TIMER0_PRESCALER_Disable) {
        TIMER0_Set_Prescaler_Disable();
    } else {
        /*nothing*/
    }
}

void TIMER0_ISR(void) {
    TIMER0_InterruptFlagClear();
    TMR0H = preloaded_value >> 8;
    TMR0L = (uint8) (preloaded_value);
    if (TIMER0_InterruptHandler) {
        TIMER0_InterruptHandler();
    } else {
        /*nothing*/
    }
}