#include "application.h"

mssp_i2c_t obj;
uint8 ack = 0;

void send_1Byte(uint8 address, uint8 data) {
    MSSP_I2C_Master_Send_Start(&obj);
    MSSP_I2C_Master_Write(&obj, address, &ack);
    MSSP_I2C_Master_Write(&obj, data, &ack);
    MSSP_I2C_Master_Send_Stop(&obj);
}
volatile uint8 counter = 0,slave2_data=0;
void APP_receive_ISR(void){
    I2C_CLOCK_STRETCH_CFG();
    if((SSPSTATbits.R_nW == 0) && (SSPSTATbits.D_nA == 0)){   /*Master needs to write Data... Slave Will read it*/
        uint8 dummy_buff = SSPBUF;
        while(!SSPSTATbits.BF);
        slave2_data = SSPBUF;
    }
    I2C_CLOCK_RELEASE_CFG();
}
led_t led2 = {.port_name = PORTD_INDEX,.pin = GPIO_PIN0,.led_status = GPIO_LOW};
int main() {
    Std_ReturnType ret = E_NOT_OK;
    obj.i2c_cfg.i2c_slew_rate = SLEW_RATE_DISABLE;
    obj.i2c_cfg.i2c_SMBus = SMBUS_DISABLE;
    obj.i2c_cfg.i2c_slave_address = 0x62;
    obj.i2c_cfg.i2c_general_call = GENERAL_CALL_DISABLE;
    obj.i2c_cfg.i2c_mode = I2C_SLAVE_MODE_SELECTED;
    obj.i2c_cfg.mssp_i2c_mode_cfg = I2C_SLAVE_MODE_7BIT_ADDRESS;
    obj.i2c_cfg.i2c_SMBus = SMBUS_DISABLE;
    obj.I2C_Default_Interrupt_Handler = APP_receive_ISR;
    obj.I2C_RECEIVE_OVERFLOW_Interrupt_Handler = NULL;
    obj.I2C_Write_Collision_Interrupt_Handler = NULL;
    MSSP_I2C_Init(&obj);
    led_init(&led2);
    while (1) {
        if('b' == slave2_data){
           led_turn_on(&led2);
        }
        else if('d' == slave2_data){
           led_turn_off(&led2);
        }
    }
    return (EXIT_SUCCESS);
}

void application_initialize(void) {
    Std_ReturnType ret = E_NOT_OK;
    ecu_layer_initalize();
}