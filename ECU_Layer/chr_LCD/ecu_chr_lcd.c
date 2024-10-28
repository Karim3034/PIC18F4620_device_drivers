#include "ecu_chr_lcd.h"
static Std_ReturnType send_4bit_data(const chr_lcd_4bit_t* lcd, uint8 data);
static Std_ReturnType send_4bit_enable_signal(const chr_lcd_4bit_t* lcd);
static Std_ReturnType send_8bit_data(const chr_lcd_8bit_t* lcd, uint8 data);
static Std_ReturnType send_8bit_enable_signal(const chr_lcd_8bit_t* lcd);
static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t* lcd, uint8 row, uint8 column);
static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_8bit_t* lcd, uint8 row, uint8 column);

Std_ReturnType lcd_4bit_intialize(const chr_lcd_4bit_t* lcd) {
    Std_ReturnType ret = E_OK;
    uint8 l_data_pins_counter = ZERO_INIT;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = gpio_pin_initialize(&(lcd->lcd_rs));
        ret = gpio_pin_initialize(&(lcd->lcd_en));
        for (l_data_pins_counter = 0; l_data_pins_counter < 8; l_data_pins_counter++) {
            ret = gpio_pin_initialize(&(lcd->lcd_data[l_data_pins_counter]));
        }
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(20);
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_us(150);
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        ret = lcd_4bit_send_command(lcd, _LCD_CLEAR);
        ret = lcd_4bit_send_command(lcd, _LCD_RETURN_HOME);
        ret = lcd_4bit_send_command(lcd, _LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_4bit_send_command(lcd, _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_4bit_send_command(lcd, _LCD_4BIT_MODE_2_LINE);
        ret = lcd_4bit_send_command(lcd, 0x80);
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t* lcd, uint8 command) {
    Std_ReturnType ret = E_OK;
    uint8 l_counter = ZERO_INIT;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);
        ret = send_4bit_data(lcd, command >> 4);
        ret = send_4bit_enable_signal(lcd);
        ret = send_4bit_data(lcd, command);
        ret = send_4bit_enable_signal(lcd);
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t* lcd, uint8 data) {
    Std_ReturnType ret = E_OK;
    uint8 l_counter = ZERO_INIT;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);
        ret = send_4bit_data(lcd, data >> 4);
        ret = send_4bit_enable_signal(lcd);
        ret = send_4bit_data(lcd, data);
        ret = send_4bit_enable_signal(lcd);
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t* lcd, uint8 row, uint8 column, uint8 data) {
    Std_ReturnType ret = E_OK;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        lcd_4bit_set_cursor(lcd, row, column);
        lcd_4bit_send_char_data(lcd, data);
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t* lcd, uint8* str) {
    Std_ReturnType ret = E_OK;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        while (*str) {
            ret = lcd_4bit_send_char_data(lcd, *str++);
        }
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t* lcd, uint8 row, uint8 column, uint8* str) {
    Std_ReturnType ret = E_OK;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = lcd_4bit_set_cursor(lcd, row, column);
        while (*str) {
            ret = lcd_4bit_send_char_data(lcd, *str++);
        }
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t* lcd, uint8 row, uint8 column, const uint8 custom_char[],uint8 mem_pos) {
   Std_ReturnType ret = E_OK;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = lcd_4bit_send_command(lcd,_LCD_CGRAM_START);
        for(uint8 counter = 0;counter<8;counter++)
            ret = lcd_4bit_send_char_data(lcd,custom_char[counter]);
        ret = lcd_4bit_send_char_data_pos(lcd,row,column,mem_pos);
    }
    return ret;
}

Std_ReturnType lcd_8bit_intialize(const chr_lcd_8bit_t* lcd) {
    Std_ReturnType ret = E_OK;
    uint8 l_data_pins_counter = ZERO_INIT;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = gpio_pin_initialize(&(lcd->lcd_rs));
        ret = gpio_pin_initialize(&(lcd->lcd_en));
        for (l_data_pins_counter = 0; l_data_pins_counter < 8; l_data_pins_counter++) {
            ret = gpio_pin_initialize(&(lcd->lcd_data[l_data_pins_counter]));
        }
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(20);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_us(150);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        ret = lcd_8bit_send_command(lcd, _LCD_CLEAR);
        ret = lcd_8bit_send_command(lcd, _LCD_RETURN_HOME);
        ret = lcd_8bit_send_command(lcd, _LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_8bit_send_command(lcd, _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_8bit_send_command(lcd, 0x80);
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t* lcd, uint8 command) {
    Std_ReturnType ret = E_OK;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);
        ret = send_8bit_data(lcd, command);
        ret = send_8bit_enable_signal(lcd);
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t* lcd, uint8 data) {
    Std_ReturnType ret = E_OK;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);
        ret = send_8bit_data(lcd, data);
        ret = send_8bit_enable_signal(lcd);
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t* lcd, uint8 row, uint8 column, uint8 data) {
    Std_ReturnType ret = E_OK;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = lcd_8bit_set_cursor(lcd, row, column);
        ret = lcd_8bit_send_char_data(lcd, data);
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t* lcd, uint8* str) {
    Std_ReturnType ret = E_OK;
    uint8 l_char_counter = ZERO_INIT;
    if (NULL == lcd || NULL == str) {
        ret = E_NOT_OK;
    } else {
        while (*str) {
            lcd_8bit_send_char_data(lcd, *str++);
        }
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t* lcd, uint8 row, uint8 column, uint8* str) {
    Std_ReturnType ret = E_OK;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = lcd_8bit_set_cursor(lcd, row, column);
        while (*str) {
            lcd_8bit_send_char_data(lcd, *str++);
        }
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t* lcd, uint8 row, uint8 column, const uint8 custom_char[],uint8 mem_pos) {
    Std_ReturnType ret = E_OK;
    if (NULL == lcd) {
        ret = E_NOT_OK;
    } else {
        ret = lcd_8bit_send_command(lcd,_LCD_CGRAM_START);
        for(uint8 counter = 0;counter<8;counter++)
            ret = lcd_8bit_send_char_data(lcd,custom_char[counter]);
        ret = lcd_8bit_send_char_data_pos(lcd,row,column,mem_pos);
    }
    return ret;
}

Std_ReturnType convert_byte_to_string(uint8 val, uint8 *str) {
    Std_ReturnType ret = E_OK;
    uint8 l_counter = ZERO_INIT;
    if (NULL == str) {
        ret = E_NOT_OK;
    } else {
        memset(str,'\0',4);
        sprintf(str,"%i",val);
    }
    return ret;
}

Std_ReturnType convert_short_to_string(uint16 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    uint8 Temp_String[6] = {0};
    uint8 DataCounter = 0;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{    
        memset(str, ' ', 5);
        str[5] = '\0';
        sprintf((char *)Temp_String, "%i", value);
        while(Temp_String[DataCounter] != '\0'){
            str[DataCounter] = Temp_String[DataCounter]; 
            DataCounter++;
        }
    }
    return ret;
}

Std_ReturnType convert_int_to_string(uint32 val, uint8 *str) {
    Std_ReturnType ret = E_OK;
    uint8 l_counter = ZERO_INIT;
    if (NULL == str) {
        ret = E_NOT_OK;
    } else {
        memset(str,'\0',11);
        sprintf(str,'%i',val);
    }
    return ret;
}

static Std_ReturnType send_4bit_data(const chr_lcd_4bit_t* lcd, uint8 data) {
    Std_ReturnType ret = E_NOT_OK;
    uint8 l_counter = ZERO_INIT;
    for (l_counter = 0; l_counter < 4; l_counter++) {
        ret = gpio_pin_write_logic(&(lcd->lcd_data[l_counter]), data >> l_counter & 0x01);
    }
    return ret;
}

static Std_ReturnType send_8bit_data(const chr_lcd_8bit_t* lcd, uint8 data) {
    Std_ReturnType ret = E_NOT_OK;
    uint8 l_counter = ZERO_INIT;
    for (l_counter = 0; l_counter < 8; l_counter++) {
        ret = gpio_pin_write_logic(&(lcd->lcd_data[l_counter]), data >> l_counter & 0x01);
    }
    return ret;
}

static Std_ReturnType send_4bit_enable_signal(const chr_lcd_4bit_t* lcd) {
    Std_ReturnType ret = E_NOT_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
    return ret;
}

static Std_ReturnType send_8bit_enable_signal(const chr_lcd_8bit_t* lcd) {
    Std_ReturnType ret = E_NOT_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
    return ret;
}

static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t* lcd, uint8 row, uint8 column) {
    Std_ReturnType ret = E_NOT_OK;
    column--;
    switch (row) {
        case ROW1: ret = lcd_8bit_send_command(lcd, (0x80 + column));
            break;
        case ROW2: ret = lcd_8bit_send_command(lcd, (0xc0 + column));
            break;
        case ROW3: ret = lcd_8bit_send_command(lcd, (0x94 + column));
            break;
        case ROW4: ret = lcd_8bit_send_command(lcd, (0xd4 + column));
            break;
    }
    return ret;
}

static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_8bit_t* lcd, uint8 row, uint8 column) {
    Std_ReturnType ret = E_NOT_OK;
    column--;
    switch (row) {
        case ROW1: ret = lcd_4bit_send_command(lcd, (0x80 + column));
            break;
        case ROW2: ret = lcd_4bit_send_command(lcd, (0xc0 + column));
            break;
        case ROW3: ret = lcd_4bit_send_command(lcd, (0x94 + column));
            break;
        case ROW4: ret = lcd_4bit_send_command(lcd, (0xd4 + column));
            break;
    }
    return ret;
}