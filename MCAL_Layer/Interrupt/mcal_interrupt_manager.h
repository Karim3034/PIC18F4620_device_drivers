/* 
 * File:   mcal_interrupt_manager.h
 * Author: fathy
 *
 * Created on July 4, 2024, 8:56 AM
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H

/* Section : Includes */
#include "mcal_interrupt_config.h"
/* Section : Macro Declarations */

/* Section : Macro Functions Declarations */

/* Section : Data Types Declarations */

/* Section : Function Declarations */
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);
void RB4_ISR(uint8 src);
void RB5_ISR(uint8 src);
void RB6_ISR(uint8 src);
void RB7_ISR(uint8 src);
void ADC_ISR(void);
void TIMER0_ISR(void);
void TIMER1_ISR(void);
void TIMER2_ISR(void);
void TIMER3_ISR(void);
void CCP1_ISR(void);
void EUSART_TX_ISR(void);
void EUSART_RX_ISR(void);
void EUSART_Ferr_ISR(void);
void EUSART_Oerr_ISR(void);
void I2C_Default_ISR(void);
void I2C_Write_Collision_ISR(void);
#if CCP2_INTERRUPT_FEATURE_ENABLE
void CCP2_ISR(void);
#endif
#endif	/* MCAL_INTERRUPT_MANAGER_H */

