/* 
 * File:   hal_adc.h
 * Author: fathy
 *
 * Created on July 13, 2024, 9:12 AM
 */

#ifndef HAL_ADC_H
#define	HAL_ADC_H

/* Section : Includes */
#include "pic18f4620.h"
#include "hal_adc_cfg.h"
#include "../MCAL_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/* Section : Macro Declarations */
#define ADC_VOLTAGE_REFERENCE_INTERNAL              0
#define ADC_VOLTAGE_REFERENCE_EXTERNAL              1
#define ADC_ALL_CHANNELS_ANALOG                     0x02
#define ADC_FROM_AN0_TO_AN11_ANALOG                 0x03
#define ADC_FROM_AN0_TO_AN10_ANALOG                 0x04
#define ADC_FROM_AN0_TO_AN9_ANALOG                  0x05
#define ADC_FROM_AN0_TO_AN8_ANALOG                  0x06
#define ADC_FROM_AN0_TO_AN7_ANALOG                  0x07
#define ADC_FROM_AN0_TO_AN6_ANALOG                  0x08
#define ADC_FROM_AN0_TO_AN5_ANALOG                  0x09
#define ADC_FROM_AN0_TO_AN4_ANALOG                  0x0A
#define ADC_FROM_AN0_TO_AN3_ANALOG                  0x0B
#define ADC_FROM_AN0_TO_AN2_ANALOG                  0x0C
#define ADC_FROM_AN0_TO_AN1_ANALOG                  0x0D
#define ADC_FROM_AN0_TO_AN0_ANALOG                  0x0E
#define ADC_ALL_CHANNELS_DIGITAL                    0x0F
#define ADC_RESULT_FORMAT_RIGHT_JUSTIFIED           1
#define ADC_RESULT_FORMAT_LEFT_JUSTIFIED            0
#define ADC_CONVERSION_COMPLETED                    0x00U
#define ADC_CONVERSION_IN_PROGRESS                  0x01U
/* Section : Macro Functions Declarations */
#define ADC_CONVERSION_STATUS()                   (ADCON0bits.GO_nDONE)
#define ADC_START_CONVERSION()                    (ADCON0bits.GODONE=1)
#define ADC_ENABLE()                              (ADCON0bits.ADON = 1)
#define ADC_DISABLE()                             (ADCON0bits.ADON = 0)
#define ADC_Set_Result_Format_Right()             (ADCON2bits.ADFM=1)
#define ADC_Set_Result_Format_Left()              (ADCON2bits.ADFM=0)

#define ADC_Set_Voltage_Reference_Internal()    do{ADCON1bits.VCFG0 = 0;\
                                                   ADCON1bits.VCFG1 = 0;\
                                                   }while(0);
                                                  
#define ADC_Set_Voltage_Reference_External()        do{ADCON1bits.VCFG0 = 1;\
                                                       ADCON1bits.VCFG1 = 1;\
                                                   }while(0);
                                                   
#define ADC_Analog_DIGITAL_PORT_CONFIG(_CONFIG)   (ADCON1bits.PCFG=_CONFIG)
/* Section : Data Types Declarations */

typedef enum {
    ADC_CHANNEL_AN0=0,
    ADC_CHANNEL_AN1,
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,
    ADC_CHANNEL_AN5,
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12            
}adc_channel_select_t;

typedef enum{
    ADC_0_TAD,
    ADC_2_TAD,
    ADC_4_TAD,
    ADC_6_TAD,
    ADC_8_TAD,
    ADC_12_TAD,
    ADC_16_TAD,
    ADC_20_TAD,
}adc_acquisition_time_t;
typedef enum{
    ADC_CONVERSION_CLOCK_FOSC_DIV_2=0,
    ADC_CONVERSION_CLOCK_FOSC_DIV_8,
    ADC_CONVERSION_CLOCK_FOSC_DIV_32,
    ADC_CONVERSION_CLOCK_FOSC_FRC,
    ADC_CONVERSION_CLOCK_FOSC_DIV_4,
    ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    ADC_CONVERSION_CLOCK_FOSC_DIV_64
}adc_conversion_clock_t;


typedef struct{
#if INTERRUPT_FEATURE_ENABLE == 1
    void(*ADC_Interrupt_Handler)(void);
#endif
    adc_channel_select_t adc_channel;
    adc_acquisition_time_t adc_acquisition_time;
    adc_conversion_clock_t adc_conversion_clock;
    uint8 voltage_reference : 2;
    uint8 result_format : 4;
    uint8  reserved: 2;
    
}adc_conf_t;
typedef uint16 adc_result;
/* Section : Function Declarations */
Std_ReturnType ADC_Init(adc_conf_t * adc);
Std_ReturnType ADC_DeInit(adc_conf_t * adc);
Std_ReturnType ADC_Select_Channel(adc_conf_t * adc,adc_channel_select_t channel);
Std_ReturnType ADC_Start_Conversion(adc_conf_t * adc);
Std_ReturnType ADC_Get_Conversion_Status(adc_conf_t *adc,uint8* status);
Std_ReturnType ADC_Get_Conversion_Result(adc_conf_t *adc,adc_result* result);
Std_ReturnType ADC_Get_Conversion_Blocking(adc_conf_t *adc,adc_channel_select_t channel,adc_result* res);
Std_ReturnType ADC_Get_Conversion_Interrupt(adc_conf_t *adc,adc_channel_select_t channel);

#endif	/* HAL_ADC_H */

