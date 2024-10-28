/* 
 * File:   hal_adc.h
 * Author: fathy
 *
 * Created on July 13, 2024, 9:12 AM
 */

#include "hal_adc.h"
static void inline adc_input_channel_port_configure(adc_channel_select_t channel);
static void inline adc_set_result_format(adc_conf_t *adc);
static void inline adc_set_voltage_ref(adc_conf_t *adc);
static void(*ADC_InterruptHandler)(void);

Std_ReturnType ADC_Init(adc_conf_t * adc) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == adc) {
        ret = E_NOT_OK;
    } else {
        /*Disable the ADC*/
        ADC_DISABLE();
        /*Configure the Port (Analog/Digital)*/
        ADC_Analog_DIGITAL_PORT_CONFIG(ADC_ALL_CHANNELS_ANALOG);
        /*Configure the voltage reference*/
        adc_set_voltage_ref(adc);
        /*Configure the default channel*/
        ADCON0bits.CHS = adc->adc_channel;
        adc_input_channel_port_configure(adc->adc_channel);
        /*Configure acquisition time*/
        ADCON2bits.ACQT = adc->adc_acquisition_time;
        /*Configure the conversion clock*/
        ADCON2bits.ADCS = adc->adc_conversion_clock;
        /*Configure Interrupts*/
#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        ADC_InterruptEnable();
        ADC_InterruptFlagClear();
        ADC_InterruptHandler = adc->ADC_Interrupt_Handler;
#endif
        /*Result Justification*/
        ADC_Set_Result_Format_Right();
        /*Enable the ADC*/
        ADC_ENABLE();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType ADC_DeInit(adc_conf_t * adc) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == adc) {
        ret = E_NOT_OK;
    } else {
        /*ADC Disable*/
        ADC_DISABLE();
        /*Configure Interrupt*/
#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptDisable();        
#endif
    }
    return ret;
}

Std_ReturnType ADC_Select_Channel(adc_conf_t * adc, adc_channel_select_t channel) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == adc) {
        ret = E_NOT_OK;
    } else {
        ADCON0bits.CHS = channel;
        adc_input_channel_port_configure(channel);
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType ADC_Start_Conversion(adc_conf_t * adc) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == adc) {
        ret = E_NOT_OK;
    } else {
        ADC_START_CONVERSION();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType ADC_IsConversionDone(adc_conf_t *adc, uint8* status) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == adc) {
        ret = E_NOT_OK;
    } else {
        *status = !ADC_CONVERSION_STATUS();
    }
    return ret;
}

Std_ReturnType ADC_Get_Conversion_Result(adc_conf_t *adc, adc_result* result) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == adc) {
        ret = E_NOT_OK;
    } else {
        if (adc->result_format == ADC_RESULT_FORMAT_RIGHT_JUSTIFIED) {
            *result = (adc_result) ((ADRESH << 8) + ADRESL);
        } else if (adc->result_format == ADC_RESULT_FORMAT_LEFT_JUSTIFIED) {
            *result = (adc_result) ((ADRESL >> 6) + (ADRESH<<2));
        } else {
            *result = (adc_result) ((ADRESH << 8) + ADRESL);
        }
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType ADC_Get_Conversion_Blocking(adc_conf_t *adc, adc_channel_select_t channel, uint16* result) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == adc) {
        ret = E_NOT_OK;
    } else {
        ADC_Select_Channel(adc, channel);
        ADC_Start_Conversion(adc);
        while (ADC_CONVERSION_STATUS());
        ret = ADC_Get_Conversion_Result(adc,result);
        ret = E_OK;
    }
    return ret;
}
Std_ReturnType ADC_Get_Conversion_Interrupt(adc_conf_t *adc,adc_channel_select_t channel){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == adc) {
        ret = E_NOT_OK;
    } else {
        ADC_Select_Channel(adc, channel);
        ADC_Start_Conversion(adc);
        ret = E_OK;
    }
    return ret;
}
static void inline adc_input_channel_port_configure(adc_channel_select_t channel) {
    switch (channel) {
        case ADC_CHANNEL_AN0:
            SET_BIT(TRISA, _TRISA_RA0_POSN);
            break;
        case ADC_CHANNEL_AN1:
            SET_BIT(TRISA, _TRISA_RA1_POSN);
            break;
        case ADC_CHANNEL_AN2:
            SET_BIT(TRISA, _TRISA_RA2_POSN);
            break;
        case ADC_CHANNEL_AN3:
            SET_BIT(TRISA, _TRISA_RA3_POSN);
            break;
        case ADC_CHANNEL_AN4:
            SET_BIT(TRISA, _TRISA_RA4_POSN);
            break;
        case ADC_CHANNEL_AN5:
            SET_BIT(TRISE, _TRISE_RE0_POSN);
            break;
        case ADC_CHANNEL_AN6:
            SET_BIT(TRISE, _TRISE_RE1_POSN);
            break;
        case ADC_CHANNEL_AN7:
            SET_BIT(TRISE, _TRISE_RE2_POSN);
            break;
        case ADC_CHANNEL_AN8:
            SET_BIT(TRISB, _TRISB_RB2_POSN);
            break;
        case ADC_CHANNEL_AN9:
            SET_BIT(TRISB, _TRISB_RB3_POSN);
            break;
        case ADC_CHANNEL_AN10:
            SET_BIT(TRISB, _TRISB_RB1_POSN);
            break;
        case ADC_CHANNEL_AN11:
            SET_BIT(TRISB, _TRISB_RB4_POSN);
            break;
        case ADC_CHANNEL_AN12:
            SET_BIT(TRISB, _TRISB_RB0_POSN);
            break;

    }
}

static void inline adc_set_result_format(adc_conf_t *adc) {
    if (ADC_RESULT_FORMAT_RIGHT_JUSTIFIED == adc->result_format) {
        ADC_Set_Result_Format_Right();
    } else if (ADC_RESULT_FORMAT_LEFT_JUSTIFIED == adc->result_format) {
        ADC_Set_Result_Format_Left();
    } else {
        ADC_Set_Result_Format_Right();
    }
}

static void inline adc_set_voltage_ref(adc_conf_t *adc) {
    if (ADC_VOLTAGE_REFERENCE_INTERNAL == adc->voltage_reference) {
        ADC_Set_Voltage_Reference_Internal();
    } else if (ADC_VOLTAGE_REFERENCE_EXTERNAL == adc->voltage_reference) {
        ADC_Set_Voltage_Reference_External();
    } else {
        ADC_Set_Voltage_Reference_Internal();
    }
}


void ADC_ISR(void){
    ADC_InterruptFlagClear();
    if(ADC_InterruptHandler){
        ADC_InterruptHandler();
    }
}