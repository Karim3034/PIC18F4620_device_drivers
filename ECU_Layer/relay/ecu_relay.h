/* 
 * File:   ecu_relay.h
 * Author: fathy
 *
 * Created on January 23, 2024, 6:48 PM
 */

#ifndef ECU_RELAY_H
#define	ECU_RELAY_H

#include "ecu_relay_cfg.h"
#include "../../MCAL_Layer//hal_gpio.h"
#define RELAY_OFF  0x00U
#define RELAY_ON   0x01U

typedef struct {
    uint8 relay_port : 4;
    uint8 relay_pin : 3;
    uint8 relay_status : 1;
}relay_t;
Std_ReturnType relay_initialize(relay_t *relay);
Std_ReturnType relay_turn_on(relay_t *relay);
Std_ReturnType relay_turn_off(relay_t *relay);



#endif	/* ECU_RELAY_H */

