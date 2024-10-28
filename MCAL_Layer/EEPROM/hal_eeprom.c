/* 
 * File:   hal_eeprom.h
 * Author: fathy
 *
 * Created on July 8, 2024, 6:30 PM
 */

#include "hal_eeprom.h"

Std_ReturnType Data_EEPROM_WriteByte(uint16 address, uint8 data) {
    Std_ReturnType ret = E_OK;
    EEADRH = (uint8) ((address >> 8)&0x03);
    EEADR = (uint8) (address & 0xFF);
    EEDATA = data;
    EECON1bits.EEPGD = ACCESS_DATA_EEPROM_MEMORY;
    EECON1bits.CFGS = ACCESS_FLASH_EEPROM_MEMORY;
    EECON1bits.WREN = FLASH_EEPROM_WRITE_ENABLE;
    uint8 Interrupt_Status = INTERRUPT_GLOBAL_INTERRUPT_STATUS;
    INTERRUPT_GlobalInterruptDisable();
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = INITIATE_DATA_EEPROM_ERASE_WRITE;
    while (EECON1bits.WR);
    EECON1bits.WREN = FLASH_EEPROM_WRITE_DISABLE;
    INTERRUPT_GlobalInterruptRedefineVal(Interrupt_Status);
    return ret;
}

Std_ReturnType Data_EEPROM_ReadByte(uint16 address, uint8 *data) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == data) {
        ret = E_NOT_OK;
    } else {
        ret = E_OK;
        EEADRH = ((address >> 8)&0x03);
        EEADR = (address & 0xFF);
        EECON1bits.EEPGD = ACCESS_DATA_EEPROM_MEMORY;
        EECON1bits.CFGS = ACCESS_FLASH_EEPROM_MEMORY;
        EECON1bits.RD = INITIATE_DATA_EEPROM_READ;
        NOP();
        NOP();
        *data = EEDATA;
    }
    return ret;
}
