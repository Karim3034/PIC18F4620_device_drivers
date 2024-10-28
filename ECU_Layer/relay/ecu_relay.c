/* 
 * File:   ecu_relay.h
 * Author: fathy
 *
 * Created on January 23, 2024, 6:48 PM
 */

#include "ecu_relay.h"

Std_ReturnType relay_initialize(relay_t *relay){
    Std_ReturnType ret = E_OK;
    if(NULL == relay){
        ret = E_NOT_OK;
    }
    else{
        pin_config_t pin_obj = {.port = relay->relay_port,.pin = relay->relay_pin,
        .direction = GPIO_DIRECTION_OUTPUT,.logic = relay->relay_status};
        gpio_pin_write_logic(&pin_obj,GPIO_LOW);
        gpio_pin_initialize(&pin_obj);
    }
    return ret;
}
Std_ReturnType relay_turn_on(relay_t *relay){
    Std_ReturnType ret = E_OK;
    if(NULL == relay){
        ret = E_NOT_OK;
    }
    else{
        pin_config_t pin_obj = {.port = relay->relay_port,.pin = relay->relay_pin,
        .direction = GPIO_DIRECTION_OUTPUT,.logic = relay->relay_status};
        gpio_pin_write_logic(&pin_obj,GPIO_HIGH);
    }
    return ret;
}
Std_ReturnType relay_turn_off(relay_t *relay){
    Std_ReturnType ret = E_OK;
    if(NULL == relay){
        ret = E_NOT_OK;
    }
    else{
       pin_config_t pin_obj = {.port = relay->relay_port,.pin = relay->relay_pin,
        .direction = GPIO_DIRECTION_OUTPUT,.logic = relay->relay_status};
        gpio_pin_write_logic(&pin_obj,GPIO_LOW);
    }
    return ret;
}
