/* 
 * File:   application.h
 * Author: dell
 *
 * Created on September 3, 2023, 7:40 AM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/*Section : Includes */
#include "ECU_Layer/ecu_layer_init.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/EEPROM/hal_eeprom.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "MCAL_Layer/Timer/hal_timer0.h"
#include "MCAL_Layer/Timer1/hal_timer1.h"
#include "MCAL_Layer/Timer2/hal_timer2.h"
#include "MCAL_Layer/Timer3/hal_timer3.h"
#include "MCAL_Layer/CCP1/hal_ccp1.h"
#include "MCAL_Layer/usart/hal_usart.h"
#include "MCAL_Layer/I2C/hal_MSSP_I2C.h"
/*Section : Macro Declarations */

/*Section : Macro Functions Declarations */

/*Data Type Declarations */

/*Section : Function Declarations */
void application_initialize(void);
#endif	/* APPLICATION_H */

