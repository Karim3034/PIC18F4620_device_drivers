/* 
 * File:   ecu_dc_motor.h
 * Author: fathy
 *
 * Created on January 25, 2024, 9:30 PM
 */

#ifndef ECU_DC_MOTOR_H
#define	ECU_DC_MOTOR_H

#include "ecu_dc_motor_cfj.h"
#include "../../MCAL_Layer/hal_gpio.h"

#define DC_MOTOR_OFF     0x00U
#define DC_MOTOR_ON      0x01U

#define DC_MOTOR_PIN0    0x00U
#define DC_MOTOR_PIN1    0x01U

typedef struct{
    pin_config_t dc_motor_pin[2];
}dc_motor_t;

Std_ReturnType dc_motor_initialize(const dc_motor_t *dc_motor);
Std_ReturnType dc_motor_move_right(const dc_motor_t *dc_motor);
Std_ReturnType dc_motor_move_left(const dc_motor_t *dc_motor);
Std_ReturnType dc_motor_stop(const dc_motor_t *dc_motor);

#endif	/* ECU_DC_MOTOR_H */

