/* 
 * File:   mcal_external_interrupt.c
 * Author: fathy
 *
 * Created on July 4, 2024, 8:57 AM
 */
#include "mcal_external_interrupt.h"

static InterruptHandler INT0_InterruptHandler = NULL;
static InterruptHandler INT1_InterruptHandler = NULL;
static InterruptHandler INT2_InterruptHandler = NULL;
static InterruptHandler RB4_InterruptHandler_RISING = NULL;
static InterruptHandler RB4_InterruptHandler_FALLING = NULL;
static InterruptHandler RB5_InterruptHandler_RISING = NULL;
static InterruptHandler RB5_InterruptHandler_FALLING = NULL;
static InterruptHandler RB6_InterruptHandler_RISING = NULL;
static InterruptHandler RB6_InterruptHandler_FALLING = NULL;
static InterruptHandler RB7_InterruptHandler_RISING = NULL;
static InterruptHandler RB7_InterruptHandler_FALLING = NULL;


static Std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *interrupt_obj);
static Std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t *interrupt_obj);
static Std_ReturnType Interrupt_INTx_Pin_Init(const interrupt_INTx_t *interrupt_obj);
static Std_ReturnType Interrupt_INTx_Edge_Init(const interrupt_INTx_t *interrupt_obj);
static Std_ReturnType Interrupt_INTx_Priority_Init(const interrupt_INTx_t *interrupt_obj);
static Std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t * interrupt_obj);
static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t *interrupt_obj);

static Std_ReturnType Interrupt_RBx_Enable(const interrupt_RBx_t *interrupt_obj);
static Std_ReturnType Interrupt_RBx_Disable(const interrupt_RBx_t *interrupt_obj);
static Std_ReturnType Interrupt_RBx_Pin_Init(const interrupt_RBx_t * interrupt_obj);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
static Std_ReturnType Interrupt_RBx_Priority_Init(const interrupt_RBx_t * interrupt_obj);
#endif
static Std_ReturnType Interrupt_RBx_Clear_Flag(const interrupt_RBx_t * interrupt_obj);
static Std_ReturnType Interrupt_RBx_SetInterruptHandler(const interrupt_RBx_t *interrupt_obj);

Std_ReturnType Interrupt_INTx_Init(const interrupt_INTx_t *interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        /*Disable External Interrupt*/
        ret = Interrupt_INTx_Disable(interrupt_obj);
        /*Clear Interrupt flag*/
        ret = Interrupt_INTx_Clear_Flag(interrupt_obj);
        /*configure External Interrupt i/o pin*/
        ret = Interrupt_INTx_Pin_Init(interrupt_obj);
        /*configure External Interrupt Edge*/
        ret = Interrupt_INTx_Edge_Init(interrupt_obj);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        /*configure External Interrupt priority*/
        ret = Interrupt_INTx_Priority_Init(interrupt_obj);
#endif
        /*configure default Interrupt CallBack*/
        ret = Interrupt_INTx_SetInterruptHandler(interrupt_obj);
        /*Enable External Interrupt*/
        Interrupt_INTx_Enable(interrupt_obj);
    }
    return ret;
}

void INT0_ISR(void) {
    EXT_INT0_InterruptFlagClear();
    if (INT0_InterruptHandler) {
        INT0_InterruptHandler();
    }
}

void INT1_ISR(void) {
    EXT_INT1_InterruptFlagClear();
    if (INT1_InterruptHandler) {
        INT1_InterruptHandler();
    }
}

void INT2_ISR(void) {
    EXT_INT2_InterruptFlagClear();
    if (INT2_InterruptHandler) {
        INT2_InterruptHandler();
    }
}

void RB4_ISR(uint8 src) {
    EXT_RBx_InterruptFlagClear();
    if (RISING == src) {
        if (RB4_InterruptHandler_RISING) {
            RB4_InterruptHandler_RISING();
        }
    } else if (FALLING == src) {
        if (RB4_InterruptHandler_FALLING) {
            RB4_InterruptHandler_FALLING();
        }
    }
}
void RB5_ISR(uint8 src) {
    EXT_RBx_InterruptFlagClear();
    if (RISING == src) {
        if (RB5_InterruptHandler_RISING) {
            RB5_InterruptHandler_RISING();
        }
    } else if (FALLING == src) {
        if (RB5_InterruptHandler_FALLING) {
            RB5_InterruptHandler_FALLING();
        }
    }
}
void RB6_ISR(uint8 src) {
    EXT_RBx_InterruptFlagClear();
    if (RISING == src) {
        if (RB6_InterruptHandler_RISING) {
            RB6_InterruptHandler_RISING();
        }
    } else if (FALLING == src) {
        if (RB6_InterruptHandler_FALLING) {
            RB6_InterruptHandler_FALLING();
        }
    }
}
void RB7_ISR(uint8 src) {
    EXT_RBx_InterruptFlagClear();
    if (RISING == src) {
        if (RB7_InterruptHandler_RISING) {
            RB7_InterruptHandler_RISING();
        }
    } else if (FALLING == src) {
        if (RB7_InterruptHandler_FALLING) {
            RB7_InterruptHandler_FALLING();
        }
    }
}

Std_ReturnType Interrupt_INTx_DeInit(const interrupt_INTx_t *interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        Interrupt_INTx_Disable(interrupt_obj);
    }
    return ret;
}

Std_ReturnType Interrupt_RBx_Init(const interrupt_RBx_t *interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = Interrupt_RBx_Disable(interrupt_obj);
        ret = Interrupt_RBx_Clear_Flag(interrupt_obj);
        ret = Interrupt_RBx_Pin_Init(interrupt_obj);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if (INTERRUPT_LOW_PRIORITY == interrupt_obj->priority) {
            INTERRUPT_GlobalInterruptLowEnable();
        } else if (INTERRUPT_HIGH_PRIORITY == interrupt_obj->priority) {
            INTERRUPT_GlobalInterruptHighEnable();
        } else {
            ret = E_NOT_OK;
        }
        Interrupt_RBx_Priority_Init(interrupt_obj);
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        Interrupt_RBx_SetInterruptHandler(interrupt_obj);
        Interrupt_RBx_Enable(interrupt_obj);
    }
    return ret;
}

Std_ReturnType Interrupt_RBx_DeInit(const interrupt_RBx_t *interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        Interrupt_RBx_Disable(interrupt_obj);
    }
    return ret;
}

static Std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = E_OK;
        switch (interrupt_obj->source) {
            case INTERRUPT_EXTERNAL_INT0:
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                INTERRUPT_GlobalInterruptHighEnable();
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_INT0_InterruptEnable();
                break;
            case INTERRUPT_EXTERNAL_INT1:
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if (INTERRUPT_LOW_PRIORITY == interrupt_obj->priority) {
                    INTERRUPT_GlobalInterruptLowEnable();
                } else if (INTERRUPT_HIGH_PRIORITY == interrupt_obj->priority) {
                    INTERRUPT_GlobalInterruptHighEnable();
                } else {
                    ret = E_NOT_OK;
                }
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_INT1_InterruptEnable();
                break;
            case INTERRUPT_EXTERNAL_INT2:
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if (INTERRUPT_LOW_PRIORITY == interrupt_obj->priority) {
                    INTERRUPT_GlobalInterruptLowEnable();
                } else if (INTERRUPT_HIGH_PRIORITY == interrupt_obj->priority) {
                    INTERRUPT_GlobalInterruptHighEnable();
                } else {
                    ret = E_NOT_OK;
                }
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_INT2_InterruptEnable();
                break;
            default:
                ret = E_NOT_OK;
        }
    }
    return ret;
}

static Std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t * interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = E_OK;
        switch (interrupt_obj->source) {
            case INTERRUPT_EXTERNAL_INT0:
                EXT_INT0_InterruptDisable();
                break;
            case INTERRUPT_EXTERNAL_INT1:
                EXT_INT1_InterruptDisable();
                break;
            case INTERRUPT_EXTERNAL_INT2:
                EXT_INT2_InterruptDisable();
                break;
            default:
                ret = E_NOT_OK;
        }
    }
    return ret;
}

static Std_ReturnType Interrupt_INTx_Pin_Init(const interrupt_INTx_t * interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = gpio_pin_initialize(&(interrupt_obj->mcu_pin));
    }
    return ret;
}

static Std_ReturnType Interrupt_INTx_Edge_Init(const interrupt_INTx_t * interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = E_OK;
        switch (interrupt_obj->source) {
            case INTERRUPT_EXTERNAL_INT0:
                if (INTERRUPT_FAILING_EDGE == interrupt_obj->edge) {
                    EXT_INT0_FallingEdgeSet();
                } else if (INTERRUPT_RISING_EDGE == interrupt_obj->edge) {
                    EXT_INT0_RisingEdgeSet();
                } else {
                    ret = E_NOT_OK;
                }
                break;
            case INTERRUPT_EXTERNAL_INT1:
                if (INTERRUPT_FAILING_EDGE == interrupt_obj->edge) {
                    EXT_INT1_FallingEdgeSet();
                } else if (INTERRUPT_RISING_EDGE == interrupt_obj->edge) {
                    EXT_INT1_RisingEdgeSet();
                } else {
                    ret = E_NOT_OK;
                }
                break;
            case INTERRUPT_EXTERNAL_INT2:
                if (INTERRUPT_FAILING_EDGE == interrupt_obj->edge) {
                    EXT_INT2_FallingEdgeSet();
                } else if (INTERRUPT_RISING_EDGE == interrupt_obj->edge) {
                    EXT_INT2_RisingEdgeSet();
                } else {
                    ret = E_NOT_OK;
                }
                break;
            default:
                ret = E_NOT_OK;
        }
    }
    return ret;
}

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

static Std_ReturnType Interrupt_INTx_Priority_Init(const interrupt_INTx_t * interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = E_OK;
        switch (interrupt_obj->source) {
            case INTERRUPT_EXTERNAL_INT1:
                if (INTERRUPT_LOW_PRIORITY == interrupt_obj->priority) {
                    EXT_INT1_LowPrioritySet();
                } else if (INTERRUPT_HIGH_PRIORITY == interrupt_obj->priority) {
                    EXT_INT1_HighPrioritySet();
                } else {
                    ret = E_NOT_OK;
                }
                break;
            case INTERRUPT_EXTERNAL_INT2:
                if (INTERRUPT_LOW_PRIORITY == interrupt_obj->priority) {
                    EXT_INT2_LowPrioritySet();
                } else if (interrupt_obj->priority == INTERRUPT_HIGH_PRIORITY) {
                    EXT_INT2_HighPrioritySet();
                } else {
                    ret = E_NOT_OK;
                }
                break;
            default:
                ret = E_NOT_OK;
        }
    }
    return ret;
}
#endif

static Std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t * interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = E_OK;
        switch (interrupt_obj->source) {
            case INTERRUPT_EXTERNAL_INT0:
                EXT_INT0_InterruptFlagClear();
                break;
            case INTERRUPT_EXTERNAL_INT1:
                EXT_INT1_InterruptFlagClear();
                break;
            case INTERRUPT_EXTERNAL_INT2:
                EXT_INT2_InterruptFlagClear();
                break;
            default:
                ret = E_NOT_OK;
        }
    }
    return ret;
}

static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void)) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == InterruptHandler) {
        ret = E_NOT_OK;
    } else {
        INT0_InterruptHandler = InterruptHandler;
        ret = E_OK;
    }
    return ret;
}

static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void)) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == InterruptHandler) {
        ret = E_NOT_OK;
    } else {
        INT1_InterruptHandler = InterruptHandler;
        ret = E_OK;
    }
    return ret;
}

static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void)) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == InterruptHandler) {
        ret = E_NOT_OK;
    } else {
        INT2_InterruptHandler = InterruptHandler;
        ret = E_OK;
    }
    return ret;
}

static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t *interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = E_OK;
        switch (interrupt_obj->source) {
            case INTERRUPT_EXTERNAL_INT0:
                ret = INT0_SetInterruptHandler(interrupt_obj->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT1:
                ret = INT1_SetInterruptHandler(interrupt_obj->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT2:
                ret = INT2_SetInterruptHandler(interrupt_obj->EXT_InterruptHandler);
                break;
            default:
                ret = E_NOT_OK;
        }
    }
    return ret;
}

static Std_ReturnType Interrupt_RBx_Enable(const interrupt_RBx_t *interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        EXT_RBx_InterruptEnable();
        ret = E_OK;
    }
    return ret;
}

static Std_ReturnType Interrupt_RBx_Disable(const interrupt_RBx_t *interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        EXT_RBx_InterruptDisable();
    }
    return ret;
}

static Std_ReturnType Interrupt_RBx_Pin_Init(const interrupt_RBx_t * interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = gpio_pin_initialize(&interrupt_obj->mcu_pin);
    }
    return ret;
}
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

static Std_ReturnType Interrupt_RBx_Priority_Init(const interrupt_RBx_t * interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = E_OK;
        switch (interrupt_obj->priority) {
            case INTERRUPT_LOW_PRIORITY:
                EXT_RBx_LowPrioritySet();
                break;
            case INTERRUPT_HIGH_PRIORITY:
                EXT_RBx_HighPrioritySet();
                break;
            default:
                ret = E_NOT_OK;
        }
    }
    return ret;
}
#endif

static Std_ReturnType Interrupt_RBx_Clear_Flag(const interrupt_RBx_t * interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        EXT_RBx_InterruptFlagClear();
        ret = E_OK;
    }
    return ret;
}

static Std_ReturnType Interrupt_RBx_SetInterruptHandler(const interrupt_RBx_t *interrupt_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == interrupt_obj) {
        ret = E_NOT_OK;
    } else {
        ret = E_OK;
        switch (interrupt_obj->mcu_pin.pin) {
            case GPIO_PIN4:
                RB4_InterruptHandler_RISING = interrupt_obj->EXT_InterruptHandler_RISING;
                RB4_InterruptHandler_FALLING = interrupt_obj->EXT_InterruptHandler_FALLING;
                break;
            case GPIO_PIN5:
                RB5_InterruptHandler_RISING = interrupt_obj->EXT_InterruptHandler_RISING;
                RB5_InterruptHandler_FALLING = interrupt_obj->EXT_InterruptHandler_FALLING;
                break;
            case GPIO_PIN6:
                RB6_InterruptHandler_RISING = interrupt_obj->EXT_InterruptHandler_RISING;
                RB6_InterruptHandler_FALLING = interrupt_obj->EXT_InterruptHandler_FALLING;
                break;
            case GPIO_PIN7:
                RB7_InterruptHandler_RISING = interrupt_obj->EXT_InterruptHandler_RISING;
                RB7_InterruptHandler_FALLING = interrupt_obj->EXT_InterruptHandler_FALLING;
                break;
        }
    }
    return ret;
}