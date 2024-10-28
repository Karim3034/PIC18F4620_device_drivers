/* 
 * File:   mcal_interrupt_config.h
 * Author: fathy
 *
 * Created on July 4, 2024, 8:56 AM
 */

#ifndef MCAL_INTERRUPT_CONFIG_H
#define	MCAL_INTERRUPT_CONFIG_H
/* Section : Includes */
#include "pic18f4620.h"
#include "../MCAL_std_types.h"
#include "../hal_gpio.h"
#include "mcal_interrupt_gen_config.h"
/* Section : Macro Declarations */
#define INTERRUPT_ENABLE                1
#define INTERRUPT_DISABLE               1
#define INTERRUPT_OCCUR                 1
#define INTERRUPT_NOT_OCCUR             0
#define INTERRUPT_ENABLE                1
#define INTERRUPT_PRIORITY_ENABLE       1
#define INTERRUPT_PRIORITY_DISABLE      1
#define RISING                          1
#define FALLING                         0
#define INTERRUPT_GLOBAL_INTERRUPT_STATUS       (INTCONbits.GIE)
/* Section : Macro Functions Declarations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
//#define INTERRUPT_PriorityLevelsEnable()        (RCONbits.IPEN=1)
#define INTERRUPT_PriorityLevelsDisable()       (RCONbits.IPEN=0)
#define INTERRUPT_GlobalInterruptHighEnable()   (INTCONbits.GIEH=1) 
#define INTERRUPT_GlobalInterruptHighDisable()  (INTCONbits.GIEH=0) 
#define INTERRUPT_GlobalInterruptLowEnable()    (INTCONbits.GIEL=1) 
#define INTERRUPT_GlobalInterruptLowDisable()  (INTCONbits.GIEL=0) 
#else
#define INTERRUPT_GlobalInterruptEnable()       (INTCONbits.GIE=1) 
#define INTERRUPT_GlobalInterruptDisable()      (INTCONbits.GIE=0) 
#define INTERRUPT_PeripheralInterruptEnable()   (INTCONbits.PEIE=1)
#define INTERRUPT_PeripheralInterruptDisable()  (INTCONbits.PEIE=0)
#define INTERRUPT_GlobalInterruptRedefineVal(val) (INTCONbits.GIE=val)
#endif
/* Section : Data Types Declarations */
typedef enum{
    INTERRUPT_LOW_PRIORITY = 0,
    INTERRUPT_HIGH_PRIORITY
}interrupt_priority;

/* Section : Function Declarations */

#endif	/* MCAL_INTERRUPT_CONFIG_H */

