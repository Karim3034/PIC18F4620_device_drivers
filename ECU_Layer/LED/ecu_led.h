/* 
 * File:   ecu_led.h
 * Author: dell
 *
 * Created on September 3, 2023, 7:18 AM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H
/* Section : Includes */
#include "../../MCAL_Layer/hal_gpio.h"
#include "ecu_led_cfg.h"
/* Section : Macro Declarations */

/* Section : Macro Functions Declarations */

/* Section : Data Types Declarations */
typedef enum{
    LED_OFF,
    LED_ON        
}led_status;

typedef struct{
    uint8 port_name : 4;
    uint8 pin : 3;
    uint8 led_status : 1;
}led_t;

/* Section : Function Declarations */
Std_ReturnType led_init(const led_t *led);
Std_ReturnType led_turn_on(const led_t *led);
Std_ReturnType led_turn_off(const led_t *led);
Std_ReturnType led_turn_toggle(const led_t *led);

#endif	/* ECU_LED_H */

