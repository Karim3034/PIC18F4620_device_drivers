/* 
 * File:   ecu_keypad.h
 * Author: fathy
 *
 * Created on February 8, 2024, 9:33 AM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

#include "../../MCAL_Layer/hal_gpio.h"
#include "ecu_keypad_cfg.h"

#define KEYPAD_ROWS  4
#define KEYPAD_COLUMNS  4

typedef struct {
    pin_config_t keypad_rows_pins[KEYPAD_ROWS];
    pin_config_t keypad_columns_pins[KEYPAD_COLUMNS];
} keypad_t;

Std_ReturnType keypad_initialize(const keypad_t *keypad);
Std_ReturnType keypad_get_value(keypad_t *keypad, uint8 *value);

#endif	/* ECU_KEYPAD_H */

