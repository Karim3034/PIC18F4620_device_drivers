/* 
 * File:   hal_ccp.c
 * Author: fathy
 *
 * Created on July 31, 2024, 6:25 PM
 */
#include "hal_ccp1.h"
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void (*CCP1_InterruptHandler)(void);
#endif
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void (*CCP2_InterruptHandler)(void);
#endif
static void CCP_MODE_CONFIG(const ccp_t *ccp_obj);
static void Interrupt_Config(const ccp_t *ccp_obj);
static void CCP_Capture_Config(const ccp_t *ccp_obj);
static void CCP_Compare_Config(const ccp_t *ccp_obj);
Std_ReturnType CCP_Init(const ccp_t* ccp_obj) {
    Std_ReturnType ret = E_OK;
    if (NULL == ccp_obj) {
        ret = E_NOT_OK;
    } else {
        if (ccp1 == ccp_obj->ccp_num)
            CCP1_SET_MODE(CCP_MODULE_DISABLED);
        else if (ccp2 == ccp_obj->ccp_num) {
            CCP2_SET_MODE(CCP_MODULE_DISABLED);
        } else {
            /*nothing*/
        }
        if (CCP_CAPTURE_MODE_SELECTED == ccp_obj->ccp_mode) {
            CCP_Capture_Config(ccp_obj);
        } else if (CCP_COMPARE_MODE_SELECTED == ccp_obj->ccp_mode) {
            CCP_Compare_Config(ccp_obj);
        } else if (CCP_PWM_MODE_SELECTED == ccp_obj->ccp_mode) {
#if (CCP_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
            if (CCP_MODULE_PWM_MODE == ccp_obj->ccp_mode_variant) {
                if (ccp1 == ccp_obj->ccp_num)
                    CCP1_SET_MODE(CCP_MODULE_PWM_MODE);
                else if (ccp2 == ccp_obj->ccp_num) {
                    CCP2_SET_MODE(CCP_MODULE_PWM_MODE);
                } else {
                    /*nothing*/
                }
            } else {
                /*nothing*/
            }
            PR2 = (uint8) (((float) _XTAL_FREQ / (float) (ccp_obj->pwm_frequency * 4.0 * ccp_obj->timer2_prescaler_value * ccp_obj->timer2_postscaler_value)) - 1);
#endif
        }
        Interrupt_Config(ccp_obj);
        ret = gpio_pin_initialize(&(ccp_obj->pin));
        ret = E_OK;
    }

    return ret;
}

Std_ReturnType CCP_Deinit(const ccp_t* ccp_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == ccp_obj) {
        ret = E_NOT_OK;
    } else {
        if (ccp1 == ccp_obj->ccp_num)
            CCP1_SET_MODE(CCP_MODULE_DISABLED);
        else if (ccp2 == ccp_obj->ccp_num) {
            CCP2_SET_MODE(CCP_MODULE_DISABLED);
        }
#if CCP_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        CCP1_InterruptDisable();
        CCP2_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret;

}
#if (CCP_CFG_MODE_SELECTED == CCP_CFG_CAPTURE_MODE_SELECTED)

Std_ReturnType CCP_IsCapturedReady(uint16* capture_status) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == capture_status) {
        ret = E_NOT_OK;
    } else {
        if (CAPTURE_MODE_READY == PIR1bits.CCP1IF) {
            *capture_status = CAPTURE_MODE_READY;
            CCP1_InterruptFlagClear();
        } else {
            *capture_status = CAPTURE_MODE_NOT_READY;
        }
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType CCP_Capture_Mode_Read_value(const ccp_t* ccp_obj, uint16 *captured_value) {
    Std_ReturnType ret = E_NOT_OK;
    CCP_PERIOD_REG_T captured_val = {.ccpr_low = 0, .ccpr_high = 0};
    if (NULL == captured_value) {
        ret = E_NOT_OK;
    } else {
        if (ccp_obj->ccp_num == ccp1) {
            captured_val.ccpr_low = CCPR1L;
            captured_val.ccpr_high = CCPR1H;
            *captured_value = captured_val.ccpr_16Bit;
        } else if (ccp_obj->ccp_num == ccp2) {
            captured_val.ccpr_low = CCPR2L;
            captured_val.ccpr_high = CCPR2H;
            *captured_value = captured_val.ccpr_16Bit;
        } else {
            /*nothing*/
        }
        ret = E_OK;
    }
    return ret;
}
#endif

#if (CCP_CFG_MODE_SELECTED == CCP_CFG_COMPARE_MODE_SELECTED)

Std_ReturnType CCP_IsCompareCompleted(uint16* compare_status) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == compare_status) {
        ret = E_NOT_OK;
    } else {
        if (COMPARE_MODE_READY == PIR1bits.CCP1IF) {
            *compare_status = COMPARE_MODE_READY;
            CCP1_InterruptFlagClear();
        } else {
            *compare_status = COMPARE_MODE_NOT_READY;
        }
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t* ccp_obj, uint16 compare_value) {
    Std_ReturnType ret = E_NOT_OK;
    CCP_PERIOD_REG_T compared_value = {.ccpr_low = 0, .ccpr_high = 0};
    compared_value.ccpr_16Bit = compare_value;
    if (NULL == ccp_obj) {
        ret = E_NOT_OK;
    } else {
        if (ccp_obj->ccp_num == ccp1) {
            CCPR1L = compared_value.ccpr_low;
            CCPR1H = compared_value.ccpr_high;
        } else if (ccp_obj->ccp_num == ccp2) {
            CCPR2L = compared_value.ccpr_low;
            CCPR2H = compared_value.ccpr_high;
        } else {
            /*nothing*/
        }
        ret = E_OK;
    }
    return ret;
}

#endif

#if (CCP_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)

Std_ReturnType CCP_PWM_Set_Duty(ccp_t *ccp_obj, uint8 duty) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == ccp_obj) {
        ret = E_NOT_OK;
    } else {
        uint16 l_duty_temp = 0;
        l_duty_temp = (uint16) (4 * (PR2 + 1) * (duty / 100.0));
        if (ccp1 == ccp_obj->ccp_num) {
            CCP1CONbits.DC1B = (uint8) (l_duty_temp & 0x0003);
            CCPR1L = (uint8) (l_duty_temp >> 2);
        } else if (ccp2 == ccp_obj->ccp_num) {
            CCP2CONbits.DC2B = (uint8) (l_duty_temp & 0x0003);
            CCPR2L = (uint8) (l_duty_temp >> 2);
        } else {
            /*nothing*/
        }
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType CCP_PWM_Start(ccp_t *ccp_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == ccp_obj) {
        ret = E_NOT_OK;
    } else {
        if (ccp1 == ccp_obj->ccp_num) {
            CCP1_SET_MODE(CCP_MODULE_PWM_MODE);

        } else if (ccp2 == ccp_obj->ccp_num) {
            CCP2_SET_MODE(CCP_MODULE_PWM_MODE);
        }
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType CCP_PWM_Stop(ccp_t *ccp_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == ccp_obj) {
        ret = E_NOT_OK;
    } else {
        if (ccp1 == ccp_obj->ccp_num) {
            CCP1_SET_MODE(CCP_MODULE_DISABLED);
        } else if (ccp2 == ccp_obj->ccp_num) {
            CCP1_SET_MODE(CCP_MODULE_DISABLED);
        }
        ret = E_OK;
    }
    return ret;
}
#endif

static void CCP_MODE_CONFIG(const ccp_t *ccp_obj) {
    if (TIMER3_CCP1_CCP2 == ccp_obj->capture_mode_variant) {
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 1;
    } else if (TIMER1_CCP1_TIMER3_CCP2 == ccp_obj->ccp_mode_variant) {
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    } else if (TIMER1_CCP1_CCP2 == ccp_obj->ccp_mode_variant) {
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    } else {
        /*nothing*/
    }
}

static void Interrupt_Config(const ccp_t *ccp_obj) {
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    CCP1_InterruptEnable();
    CCP1_InterruptFlagClear();
    CCP1_InterruptHandler = ccp_obj->CCP1_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if (INTERRUPT_HIGH_PRIORITY == ccp_obj->ccp1_priority) {
        INTERRUPT_GlobalInterruptHighEnable();
        CCP1_HighPrioritySet();
    } else if (INTERRUPT_LOW_PRIORITY == ccp_obj->ccp1_priority) {
        INTERRUPT_GlobalInterruptLowEnable();
        CCP1_LowPrioritySet();
    }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    CCP2_InterruptEnable();
    CCP2_InterruptFlagClear();
    CCP2_InterruptHandler = ccp_obj->CCP2_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if (INTERRUPT_HIGH_PRIORITY == ccp_obj->ccp2_priority) {
        INTERRUPT_GlobalInterruptHighEnable();
        CCP2_HighPrioritySet();
    } else if (INTERRUPT_LOW_PRIORITY == ccp_obj->ccp2_priority) {
        INTERRUPT_GlobalInterruptLowEnable();
        CCP2_LowPrioritySet();
    }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
}

static void CCP_Capture_Config(const ccp_t *ccp_obj) {
    if (ccp1 == ccp_obj->ccp_num) {
        switch (ccp_obj->ccp_mode_variant) {
            case CCP_MODULE_CAPTURE_MODE_1_FALLING_EDGE:
                CCP1_SET_MODE(CCP_MODULE_CAPTURE_MODE_1_FALLING_EDGE);
                break;
            case CCP_MODULE_CAPTURE_MODE_1_RISING_EDGE:
                CCP1_SET_MODE(CCP_MODULE_CAPTURE_MODE_1_RISING_EDGE);
                break;
            case CCP_MODULE_CAPTURE_MODE_4_RISING_EDGE:
                CCP1_SET_MODE(CCP_MODULE_CAPTURE_MODE_4_RISING_EDGE);
                break;
            case CCP_MODULE_CAPTURE_MODE_16_RISING_EDGE:
                CCP1_SET_MODE(CCP_MODULE_CAPTURE_MODE_16_RISING_EDGE);
                break;
            default:
                break;
        }
    } else if (ccp2 == ccp_obj->ccp_num) {
        switch (ccp_obj->ccp_mode_variant) {
            case CCP_MODULE_CAPTURE_MODE_1_FALLING_EDGE:
                CCP2_SET_MODE(CCP_MODULE_CAPTURE_MODE_1_FALLING_EDGE);
                break;
            case CCP_MODULE_CAPTURE_MODE_1_RISING_EDGE:
                CCP2_SET_MODE(CCP_MODULE_CAPTURE_MODE_1_RISING_EDGE);
                break;
            case CCP_MODULE_CAPTURE_MODE_4_RISING_EDGE:
                CCP2_SET_MODE(CCP_MODULE_CAPTURE_MODE_4_RISING_EDGE);
                break;
            case CCP_MODULE_CAPTURE_MODE_16_RISING_EDGE:
                CCP2_SET_MODE(CCP_MODULE_CAPTURE_MODE_16_RISING_EDGE);
                break;
            default:
                break;
        }
    } else {
        /*nothing*/
    }
    CCP_MODE_CONFIG(ccp_obj);
}

static void CCP_Compare_Config(const ccp_t *ccp_obj) {
    if (ccp1 == ccp_obj->ccp_num) {
        switch (ccp_obj->ccp_mode_variant) {
            case CCP_MODULE_COMPARE_MODE_SET_PIN_LOW:
                CCP1_SET_MODE(CCP_MODULE_COMPARE_MODE_SET_PIN_LOW);
                break;
            case CCP_MODULE_COMPARE_MODE_SET_PIN_HIGH:
                CCP1_SET_MODE(CCP_MODULE_COMPARE_MODE_SET_PIN_HIGH);
                break;
            case CCP_MODULE_COMPARE_MODE_TOGGLE_ON_MATCH:
                CCP1_SET_MODE(CCP_MODULE_COMPARE_MODE_TOGGLE_ON_MATCH);
                break;
            case CCP_MODULE_COMPARE_MODE_GEN_SW_INTERRUPT:
                CCP1_SET_MODE(CCP_MODULE_COMPARE_MODE_GEN_SW_INTERRUPT);
                break;
            case CCP_MODULE_COMPARE_MODE_EVENT:
                CCP1_SET_MODE(CCP_MODULE_COMPARE_MODE_EVENT);
                break;
            default:
                break;
        }
    }
    else if (ccp2 == ccp_obj->ccp_num) {
        switch (ccp_obj->ccp_mode_variant) {
            case CCP_MODULE_COMPARE_MODE_SET_PIN_LOW:
                CCP2_SET_MODE(CCP_MODULE_COMPARE_MODE_SET_PIN_LOW);
                break;
            case CCP_MODULE_COMPARE_MODE_SET_PIN_HIGH:
                CCP2_SET_MODE(CCP_MODULE_COMPARE_MODE_SET_PIN_HIGH);
                break;
            case CCP_MODULE_COMPARE_MODE_TOGGLE_ON_MATCH:
                CCP2_SET_MODE(CCP_MODULE_COMPARE_MODE_TOGGLE_ON_MATCH);
                break;
            case CCP_MODULE_COMPARE_MODE_GEN_SW_INTERRUPT:
                CCP2_SET_MODE(CCP_MODULE_COMPARE_MODE_GEN_SW_INTERRUPT);
                break;
            case CCP_MODULE_COMPARE_MODE_EVENT:
                CCP2_SET_MODE(CCP_MODULE_COMPARE_MODE_EVENT);
                break;
            default:
                break;
        }
    }
    CCP_MODE_CONFIG(ccp_obj);
}

void CCP1_ISR(void) {
    CCP1_InterruptFlagClear();
    if (CCP1_InterruptHandler) {
        CCP1_InterruptHandler();
    } else {
        /*nothing*/
    }
}
#if CCP2_INTERRUPT_FEATURE_ENABLE

void CCP2_ISR(void) {
    CCP2_InterruptFlagClear();
    if (CCP2_InterruptHandler) {
        CCP2_InterruptHandler();
    } else {
        /*nothing*/
    }
}
#endif
