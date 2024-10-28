/* 
 * File:   ecu_dc_motor.h
 * Author: fathy
 *
 * Created on January 25, 2024, 9:31 PM
 */

#include "ecu_dc_motor.h"

Std_ReturnType dc_motor_initialize(const dc_motor_t *dc_motor){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == dc_motor){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_initialize(&(dc_motor->dc_motor_pin[DC_MOTOR_PIN0]));
        ret = gpio_pin_initialize(&(dc_motor->dc_motor_pin[DC_MOTOR_PIN1]));
    }
    return ret;
}
Std_ReturnType dc_motor_move_right(const dc_motor_t *dc_motor){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == dc_motor){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(dc_motor->dc_motor_pin[DC_MOTOR_PIN0]),GPIO_HIGH);
        ret = gpio_pin_write_logic(&(dc_motor->dc_motor_pin[DC_MOTOR_PIN1]),GPIO_LOW);
    }
    return ret;
}
Std_ReturnType dc_motor_move_left(const dc_motor_t *dc_motor){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == dc_motor){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(dc_motor->dc_motor_pin[DC_MOTOR_PIN0]),GPIO_LOW);
        ret = gpio_pin_write_logic(&(dc_motor->dc_motor_pin[DC_MOTOR_PIN1]),GPIO_HIGH);
    }
    return ret;
}
Std_ReturnType dc_motor_stop(const dc_motor_t *dc_motor){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == dc_motor){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(dc_motor->dc_motor_pin[DC_MOTOR_PIN0]),GPIO_LOW);
        ret = gpio_pin_write_logic(&(dc_motor->dc_motor_pin[DC_MOTOR_PIN1]),GPIO_LOW);
    }
    return ret;
}
