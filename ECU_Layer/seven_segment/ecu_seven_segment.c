/* 
 * File:   ecu_seven_segment.c
 * Author: fathy
 *
 * Created on February 5, 2024, 8:52 PM
 */
#include "ecu_seven_segment.h"

Std_ReturnType seven_segment_initialize(const segment_t *segment) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == segment) {
        ret = E_NOT_OK;
    } else {
        for (uint8 i = 0; i < 4; i++) {
            gpio_pin_direction_initialize(&(segment->segment_pins[i]));
        }
    }
    return ret;
}

Std_ReturnType seven_segment_write_number(const segment_t *segment, uint8 number) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == segment || number > 9) {
        ret = E_NOT_OK;
    } else {
        for (uint8 i = 0; i < 4; i++) {
            gpio_pin_write_logic(&(segment->segment_pins[SEGMENT_PIN0 + i]), (number>>i) & 0x01);
        }
    }
    return ret;
}