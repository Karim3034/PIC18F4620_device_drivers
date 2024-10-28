/* 
 * File:   hal_eeprom.h
 * Author: fathy
 *
 * Created on July 8, 2024, 6:30 PM
 */

#ifndef HAL_EEPROM_H
#define	HAL_EEPROM_H

/* Section : Includes */
#include "pic18f4620.h"
#include "../MCAL_std_types.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/* Section : Macro Declarations */
#define ACCESS_FLASH_PROGRAM_MEMORY                1
#define ACCESS_CONFIGURATION_REGISTERS             1
#define ACCESS_DATA_EEPROM_MEMORY                  0
#define ACCESS_FLASH_EEPROM_MEMORY                 0
#define FLASH_EEPROM_WRITE_ENABLE                  1
#define FLASH_EEPROM_WRITE_DISABLE                 0
#define INITIATE_DATA_EEPROM_ERASE_WRITE           1
#define DATA_EEPROM_WRITE_ERASE_COMPLETED          1
#define INITIATE_DATA_EEPROM_READ                  1
#define DOES_NOT_INITIATE_DATA_EEPROM_READ         1
/* Section : Macro Functions Declarations */

/* Section : Data Types Declarations */

/* Section : Function Declarations */

Std_ReturnType Data_EEPROM_WriteByte(uint16 address,uint8 data);
Std_ReturnType Data_EEPROM_ReadByte(uint16 address,uint8 *data);
#endif	/* HAL_EEPROM_H */

