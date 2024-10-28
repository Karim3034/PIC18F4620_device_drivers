/* 
 * File:   ecu_led.c
 * Author: dell
 *
 * Created on September 3, 2023, 7:18 AM
 */
#include "../ECU_Layer/LED/ecu_led.h"

Std_ReturnType led_init(const led_t *led){
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }
    else{
        pin_config_t pin_obj = {.port = led->port_name,.pin = led->pin,
        .direction = GPIO_DIRECTION_OUTPUT,.logic = led->led_status};
        gpio_pin_initialize(&pin_obj);
    }
    return ret;
}
Std_ReturnType led_turn_on(const led_t *led){
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }
    else{
        pin_config_t pin_obj = {.port = led->port_name,.pin = led->pin,
        .direction = GPIO_DIRECTION_OUTPUT,.logic = GPIO_HIGH};
        gpio_pin_write_logic(&pin_obj,GPIO_HIGH);
    }
    return ret;
}
Std_ReturnType led_turn_off(const led_t *led){
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }
    else{
        pin_config_t pin_obj = {.port = led->port_name,.pin = led->pin,
        .direction = GPIO_DIRECTION_OUTPUT,.logic = led->led_status};
        gpio_pin_write_logic(&pin_obj,GPIO_LOW);
    }
    return ret;
}
Std_ReturnType led_turn_toggle(const led_t *led){
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }
    else{
         pin_config_t pin_obj = {.port = led->port_name,.pin = led->pin,
        .direction = GPIO_DIRECTION_OUTPUT,.logic = led->led_status};
        gpio_pin_toggle_logic(&pin_obj);
    }
    return ret;
}