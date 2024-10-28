/* 
 * File:   ecu_seven_segment.h
 * Author: fathy
 *
 * Created on February 5, 2024, 8:52 PM
 */

#ifndef ECU_SEVEN_SEGMENT_H
#define	ECU_SEVEN_SEGMENT_H

#include "../../MCAL_Layer//hal_gpio.h"
#include "ecu_seven_segment_cfg.h"

#define SEGMENT_PIN0   0
#define SEGMENT_PIN1   1
#define SEGMENT_PIN2   2
#define SEGMENT_PIN3   3



typedef enum{
    SEGMENT_COMMON_ANODE,
    SEGMENT_COMMON_CATHODE,
}segment_type_t;

typedef struct{
    pin_config_t segment_pins[4];
    segment_type_t segment_type;
}segment_t;

Std_ReturnType seven_segment_initialize(const segment_t *segment);
Std_ReturnType seven_segment_write_number(const segment_t *segment,uint8 number);

#endif	/* ECU_SEVEN_SEGMENT_H */

