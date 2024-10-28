/* 
 * File:   ecu_keypad.h
 * Author: fathy
 *
 * Created on February 8, 2024, 9:33 AM
 */
#include "ecu_keypad.h"

static const uint8 keypad_values[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'#', '0', '=', '+'},
};

Std_ReturnType keypad_initialize(const keypad_t *keypad) {
    Std_ReturnType ret = E_NOT_OK;
    uint8 rows_counter = ZERO_INIT, columns_counter = ZERO_INIT;

    if (NULL == keypad) {
        ret = E_NOT_OK;
    } else {
        for (rows_counter = 0; rows_counter < KEYPAD_ROWS; rows_counter++) {
            ret = gpio_pin_initialize(&(keypad->keypad_rows_pins[rows_counter]));
        }
        for (columns_counter = 0; columns_counter < KEYPAD_COLUMNS; columns_counter++) {
            ret = gpio_pin_direction_initialize(&(keypad->keypad_columns_pins[columns_counter]));
        }
    }
    return ret;
}

Std_ReturnType keypad_get_value(keypad_t *keypad, uint8 *value) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == keypad || NULL == value) {
        ret = E_NOT_OK;
    } else {
        uint8 rows_counter = ZERO_INIT, columns_counter = ZERO_INIT, l_counter = ZERO_INIT, logic = ZERO_INIT;
        for (rows_counter = 0; rows_counter < KEYPAD_ROWS; rows_counter++) {
            for (l_counter = 0; l_counter < KEYPAD_ROWS; l_counter++) {
                gpio_pin_write_logic(&(keypad->keypad_rows_pins[l_counter]), GPIO_LOW);
            }
            gpio_pin_write_logic(&(keypad->keypad_rows_pins[rows_counter]), GPIO_HIGH);
            for (columns_counter = 0; columns_counter < KEYPAD_ROWS; columns_counter++) {
                ret = gpio_pin_read_logic(&(keypad->keypad_columns_pins[columns_counter]), &logic);
                if (logic == GPIO_HIGH) {
                    *value = keypad_values[rows_counter][columns_counter];
                }
            }
        }
    }
    return ret;
}