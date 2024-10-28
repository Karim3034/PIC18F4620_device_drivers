/* 
 * File:   mcal_external_interrupt.h
 * Author: fathy
 *
 * Created on July 4, 2024, 8:57 AM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H

/* Section : Includes */
#include "mcal_interrupt_config.h"

/* Section : Macro Declarations */
#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/*INT0*/
#define EXT_INT0_InterruptEnable()          (INTCONbits.INT0E=1)
#define EXT_INT0_InterruptDisable()         (INTCONbits.INT0E=0)
#define EXT_INT0_InterruptFlagClear()       (INTCONbits.INT0F=0)
#define EXT_INT0_RisingEdgeSet()            (INTCON2bits.INTEDG0=1)
#define EXT_INT0_FallingEdgeSet()           (INTCON2bits.INTEDG0=0)
/*INT1*/
#define EXT_INT1_InterruptEnable()          (INTCON3bits.INT1E=1)
#define EXT_INT1_InterruptDisable()         (INTCON3bits.INT1E=0)
#define EXT_INT1_InterruptFlagClear()       (INTCON3bits.INT1F=0)
#define EXT_INT1_RisingEdgeSet()            (INTCON2bits.INTEDG1=1)
#define EXT_INT1_FallingEdgeSet()           (INTCON2bits.INTEDG1=0)
/*INT2*/
#define EXT_INT2_InterruptEnable()          (INTCON3bits.INT2E=1)
#define EXT_INT2_InterruptDisable()         (INTCON3bits.INT2E=0)
#define EXT_INT2_InterruptFlagClear()       (INTCON3bits.INT2F=0)
#define EXT_INT2_RisingEdgeSet()            (INTCON2bits.INTEDG2=1)
#define EXT_INT2_FallingEdgeSet()           (INTCON2bits.INTEDG2=0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/*INT1*/
#define EXT_INT1_HighPrioritySet()          (INTCON3bits.INT1IP=1)
#define EXT_INT1_LowPrioritySet()           (INTCON3bits.INT1IP=0)
/*INT2*/
#define EXT_INT2_HighPrioritySet()          (INTCON3bits.INT2IP=1)
#define EXT_INT2_LowPrioritySet()           (INTCON3bits.INT2IP=0)
#endif
#endif

#if EXTERNAL_INTERRUPT_ONCHANGE_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
#define EXT_RBx_InterruptEnable()         (INTCONbits.RBIE=1)
#define EXT_RBx_InterruptDisable()        (INTCONbits.RBIE=0)
#define EXT_RBx_InterruptFlagClear()       (INTCONbits.RBIF=0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define EXT_RBx_HighPrioritySet()          (INTCON2bits.RBIP=1)
#define EXT_RBx_LowPrioritySet()           (INTCON2bits.RBIP=0)
#endif 
#endif
/* Section : Macro Functions Declarations */

/* Section : Data Types Declarations */
typedef void (*InterruptHandler)(void);
typedef enum{
    INTERRUPT_FAILING_EDGE = 0,
    INTERRUPT_RISING_EDGE
}interrupt_INTx_edge;

typedef enum{
    INTERRUPT_EXTERNAL_INT0 = 0,
    INTERRUPT_EXTERNAL_INT1,
    INTERRUPT_EXTERNAL_INT2
}interrupt_INTx_src;

typedef struct{
    void (*EXT_InterruptHandler)(void);
    pin_config_t mcu_pin;
    interrupt_INTx_edge edge;
    interrupt_INTx_src source;
    interrupt_priority priority;
}interrupt_INTx_t;

typedef struct{
    void (*EXT_InterruptHandler_RISING)(void);
    void (*EXT_InterruptHandler_FALLING)(void);
    pin_config_t mcu_pin;
    interrupt_priority priority;
}interrupt_RBx_t;

/* Section : Function Declarations */
Std_ReturnType Interrupt_INTx_Init(const interrupt_INTx_t *interrupt_obj);
Std_ReturnType Interrupt_INTx_DeInit(const interrupt_INTx_t *interrupt_obj);
Std_ReturnType Interrupt_RBx_Init(const interrupt_RBx_t *interrupt_obj);
Std_ReturnType Interrupt_RBx_DeInit(const interrupt_RBx_t *interrupt_obj);

#endif	/* MCAL_EXTERNAL_INTERRUPT_H */

