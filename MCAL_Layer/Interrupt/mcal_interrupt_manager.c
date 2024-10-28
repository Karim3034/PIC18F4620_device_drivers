/* 
 * File:   mcal_interrupt_manager.c
 * Author: fathy
 *
 * Created on July 4, 2024, 8:56 AM
 */
#include "mcal_interrupt_manager.h"

static volatile uint8 RB4_Flag = 0, RB5_Flag = 0, RB6_Flag = 0, RB7_Flag = 0;

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE

void __interrupt() InterruptManagerHigh(void) {
    if ((INTERRUPT_ENABLE == INTCONbits.INT0E) && (INTERRUPT_OCCUR == INTCONbits.INT0IF)) {
        INT0_ISR();
    }
    if ((INTERRUPT_ENABLE == INTCON3bits.INT2E) && (INTERRUPT_OCCUR == INTCON3bits.INT2F)) {
        INT2_ISR();
    }
}

void __interrupt(low_priority) InterruptManagerLow(void) {
    if ((INTERRUPT_ENABLE == INTCON3bits.INT1E) && (INTERRUPT_OCCUR == INTCON3bits.INT1F)) {
        INT1_ISR();
    }
}

#else

void __interrupt() InterruptManagerHigh(void) {
    if((INTERRUPT_ENABLE == PIE1bits.CCP1IE)&&(INTERRUPT_OCCUR == PIR1bits.CCP1IF)){
        CCP1_ISR();
    }
#if CCP2_INTERRUPT_FEATURE_ENABLE
    if((INTERRUPT_ENABLE == PIE2bits.CCP2IE)&&(INTERRUPT_OCCUR == PIR2bits.CCP2IF)){
        CCP2_ISR();
    }
#endif
    if ((INTERRUPT_ENABLE == PIE1bits.ADIE) && (INTERRUPT_OCCUR == PIR1bits.ADIF)) {
        ADC_ISR();
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == PIE1bits.TXIE) && (INTERRUPT_OCCUR == PIR1bits.TXIF)) {
        EUSART_TX_ISR();
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == PIE1bits.RCIE) && (INTERRUPT_OCCUR == PIR1bits.RCIF)) {
        EUSART_RX_ISR();
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCONbits.TMR0IE) && (INTERRUPT_OCCUR == INTCONbits.TMR0IF)) {
        TIMER0_ISR();
    } else {
        /*nothing*/
    }
    
    if ((INTERRUPT_ENABLE == PIE1bits.TMR1IE) && (INTERRUPT_OCCUR == PIR1bits.TMR1IF)) {
        TIMER1_ISR();
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == PIE1bits.TMR2IE) && (INTERRUPT_OCCUR == PIR1bits.TMR2IF)) {
        TIMER2_ISR();
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == PIE2bits.TMR3IE) && (INTERRUPT_OCCUR == PIR2bits.TMR3IF)) {
        TIMER3_ISR();
    } else {
        /*nothing*/
    }
    
    if ((INTERRUPT_ENABLE == INTCONbits.INT0E) && (INTERRUPT_OCCUR == INTCONbits.INT0F)) {
        INT0_ISR();
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCON3bits.INT1E) && (INTERRUPT_OCCUR == INTCON3bits.INT1F)) {
        INT1_ISR();
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCON3bits.INT2E) && (INTERRUPT_OCCUR == INTCON3bits.INT2F)) {
        INT2_ISR();
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) &&
            (PORTBbits.RB4 == GPIO_HIGH) && (RB4_Flag == 0)) {
        RB4_Flag = 1;
        RB4_ISR(RISING);
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)&&
            (PORTBbits.RB4 == GPIO_LOW) && (RB4_Flag == 1)) {
        RB4_Flag = 0;
        RB4_ISR(FALLING);
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)&&
            (PORTBbits.RB5 == GPIO_HIGH) && (RB5_Flag == 0)) {
        RB5_Flag = 1;
        RB5_ISR(RISING);
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)&&
            (PORTBbits.RB5 == GPIO_LOW) && (RB5_Flag == 1)) {
        RB5_Flag = 0;
        RB5_ISR(FALLING);
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)&&
            (PORTBbits.RB6 == GPIO_HIGH) && (RB6_Flag == 0)) {
        RB6_Flag = 1;
        RB6_ISR(RISING);
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)&&
            (PORTBbits.RB6 == GPIO_LOW) && (RB6_Flag == 1)) {
        RB6_Flag = 0;
        RB6_ISR(FALLING);
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)&&
            (PORTBbits.RB7 == GPIO_HIGH) && (RB7_Flag == 0)) {
        RB7_Flag = 1;
        RB7_ISR(RISING);
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)&&
            (PORTBbits.RB7 == GPIO_LOW) && (RB7_Flag == 1)) {
        RB7_Flag = 0;
        RB7_ISR(FALLING);
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == PIE1bits.SSPIE) && (INTERRUPT_OCCUR ==PIR1bits.SSPIF)){
        I2C_Default_ISR();
    } else {
        /*nothing*/
    }
    if ((INTERRUPT_ENABLE == PIE2bits.BCLIE) && (INTERRUPT_OCCUR == PIR2bits.BCLIF)){
        I2C_Write_Collision_ISR();
    } else {
        /*nothing*/
    }
    
}
#endif