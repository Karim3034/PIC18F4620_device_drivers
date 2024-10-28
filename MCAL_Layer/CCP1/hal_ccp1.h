/*
 * File:   hal_ccp.h
 * Author: fathy
 *
 * Created on July 31, 2024, 6:25 PM
 */

#ifndef HAL_CCP_H
#define	HAL_CCP_H
/* Section : Includes */

#include "ccp1_cfg.h"
#include "pic18f4620.h"
#include "../MCAL_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../GPIO/../hal_gpio.h"

/* Section : Macro Declarations */

#define CCP_MODULE_DISABLED                            ((uint8)0x00)
#define CCP_MODULE_CAPTURE_MODE_1_FALLING_EDGE         ((uint8)0x04)
#define CCP_MODULE_CAPTURE_MODE_1_RISING_EDGE          ((uint8)0x05)
#define CCP_MODULE_CAPTURE_MODE_4_RISING_EDGE          ((uint8)0x06)
#define CCP_MODULE_CAPTURE_MODE_16_RISING_EDGE         ((uint8)0x07)
#define CCP_MODULE_COMPARE_MODE_SET_PIN_LOW            ((uint8)0x08)
#define CCP_MODULE_COMPARE_MODE_SET_PIN_HIGH           ((uint8)0x09)
#define CCP_MODULE_COMPARE_MODE_TOGGLE_ON_MATCH        ((uint8)0x02)
#define CCP_MODULE_COMPARE_MODE_GEN_SW_INTERRUPT       ((uint8)0x0A)
#define CCP_MODULE_COMPARE_MODE_EVENT                  ((uint8)0x0B)
#define CCP_MODULE_PWM_MODE                            ((uint8)0x0C)

#define CAPTURE_MODE_NOT_READY                          0x00
#define CAPTURE_MODE_READY                              0x01

#define COMPARE_MODE_NOT_READY                          0x00
#define COMPARE_MODE_READY                              0x01
/* Section : Macro Functions Declarations */
#define CCP1_SET_MODE(_CONFIG)                          (CCP1CONbits.CCP1M=_CONFIG)
#define CCP2_SET_MODE(_CONFIG)                          (CCP2CONbits.CCP2M=_CONFIG)

/* Section : Data Types Declarations */

typedef enum {
    CCP_CAPTURE_MODE_SELECTED = 0,
    CCP_COMPARE_MODE_SELECTED,
    CCP_PWM_MODE_SELECTED
} ccp_mode_t;

typedef union {

    struct {
        uint8 ccpr_low;
        uint8 ccpr_high;
    };

    struct {
        uint16 ccpr_16Bit;
    };
} CCP_PERIOD_REG_T;

typedef enum {
    ccp1,
    ccp2
} ccp_number;

typedef enum {
    TIMER1_CCP1_CCP2,
    TIMER1_CCP1_TIMER3_CCP2,
    TIMER3_CCP1_CCP2
}capture_t;

typedef struct {
    ccp_number ccp_num;
    ccp_mode_t ccp_mode;
    uint8 ccp_mode_variant;
    CCP_PERIOD_REG_T period;
    pin_config_t pin;
    capture_t capture_mode_variant;
#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void(*CCP1_InterruptHandler)(void);
    interrupt_priority ccp1_priority;
#endif
#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void(*CCP2_InterruptHandler)(void);
    interrupt_priority ccp2_priority;
#endif
#if (CCP_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
    uint32 pwm_frequency;
    uint8 timer2_postscaler_value;
    uint8 timer2_prescaler_value;
#endif
} ccp_t;
/* Section : Function Declarations */

Std_ReturnType CCP_Init(const ccp_t* ccp_obj);
Std_ReturnType CCP_Deinit(const ccp_t* ccp_obj);
#if (CCP_CFG_MODE_SELECTED == CCP_CFG_CAPTURE_MODE_SELECTED)
Std_ReturnType CCP_IsCapturedReady(uint16* capture_status);
Std_ReturnType CCP_Capture_Mode_Read_value(const ccp_t* ccp_obj, uint16 *captured_value);
#endif

#if (CCP_CFG_MODE_SELECTED == CCP_CFG_COMPARE_MODE_SELECTED)
Std_ReturnType CCP_IsCompareCompleted(uint16* compare_status);
Std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t* ccp_obj,uint16 compare_value);
#endif

#if (CCP_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
Std_ReturnType CCP_PWM_Set_Duty(ccp_t *ccp_obj, uint8 duty);
Std_ReturnType CCP_PWM_Start(ccp_t *ccp_obj);
Std_ReturnType CCP_PWM_Stop(ccp_t *ccp_obj);
#endif

#endif	/* HAL_CCP_H */

