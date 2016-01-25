#include "../compare/i2cmaster/i2cmaster.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include "rdelay.h"
#include "demo.h"
#include "led.h"

// Include the header so "library" functions get extern C.
#include "ssd1306.h"

void start_send_progmem(const uint8_t * PROGMEM p, uint8_t n)
{
    i2c_start(pgm_read_byte(p++));
    while (--n)
        i2c_write(pgm_read_byte(p++));
}

const uint8_t ssd1306_init_data[] PROGMEM =
{
    SSD1306_ADDRESS | I2C_WRITE, SSD1306_MANY_COMMANDS,
    SSD1306_DISPLAY_OFF,
    SSD1306_SET_DISPLAY_CLOCK, 0x40,
    SSD1306_SET_MULTIPLEX_RATIO, SSD1306_SCREEN_HEIGHT-1,
    SSD1306_SET_CHARGE_PUMP, SSD1306_CHARGE_PUMP_ENABLE,
    SSD1306_SET_SEGMENTS_DESC,
    SSD1306_SET_COM_DESC,
    SSD1306_SET_COM_CONFIGURATION, 0x12,
    SSD1306_SET_CONTRAST, 0x0F,
    SSD1306_SET_PRECHARGE, 0x22,
    SSD1306_SET_VCOMH_DESELECT, 0x20,
    SSD1306_SCROLL_DISABLE,
    SSD1306_SET_DISPLAY_OFFSET, 0,
    SSD1306_SET_DISPLAY_START + 0x00,
    SSD1306_SET_ADDRESS_MODE, SSD1306_ADDRESS_MODE_HORIZONTAL,
    SSD1306_DISPLAY_UNBLANK,
    SSD1306_DISPLAY_NORMAL,
    SSD1306_DISPLAY_ON
};

void ssd1306_init(void)
{
    start_send_progmem(ssd1306_init_data, sizeof(ssd1306_init_data));
    i2c_stop();
}

const uint8_t ssd1306_clear_data[] PROGMEM =
{
    SSD1306_ADDRESS | I2C_WRITE, SSD1306_MANY_COMMANDS,
    SSD1306_SET_COLUMN_RANGE, 0, SSD1306_SCREEN_WIDTH-1,
    SSD1306_SET_PAGE_RANGE, 0, (SSD1306_SCREEN_HEIGHT/8)-1
};

void ssd1306_clear(void)
{
    start_send_progmem(ssd1306_clear_data, sizeof(ssd1306_clear_data));
    i2c_rep_start(SSD1306_ADDRESS | I2C_WRITE);
    i2c_write(SSD1306_DATA);
    for (uint16_t n = SSD1306_SCREEN_WIDTH * SSD1306_SCREEN_HEIGHT / 8 + 1; --n != 0;)
    {
        i2c_write(0);
    }
    i2c_stop();
}

void ssd1306_send_char_bits(char c, uint8_t effect)
{
    // chargen map and lookup
    uint8_t i = (c & 0x7F) - ' ';
    if (i >= 96)
        i = 0;
    const uint8_t * PROGMEM q = ssd1306_chargen + (SSD1306_CHAR_WIDTH-1)*i;
    const uint8_t background = (effect & SSD1306_EFFECT_INVERSE) ? 0xFF : 0x00;
    // send the char data
    for (uint8_t n = SSD1306_CHAR_WIDTH; --n != 0;)
    {
        uint8_t bits = pgm_read_byte(q++) ^ background;
        uint8_t swapped;
        switch (effect & 0x06)
        {
        case SSD1306_EFFECT_SHORT: break;
        case SSD1306_EFFECT_TALL_TOP:
            // ----3210 -> 33221100
            bits = bits << 4;
            // fall through
        case SSD1306_EFFECT_TALL_BOTTOM:
            // 7654---- -> 77665544
            swapped = bits >> 4;
            bits
                = (bits & 0x80)
                | ((bits & 0xC0) >> 1)
                | ((bits & 0x60) >> 2)
                | ((swapped & 0x03) << 1)
                | (swapped & 0x01)
                ;
            break;
        }
        i2c_write(bits);
        if (effect & SSD1306_EFFECT_WIDE)
            i2c_write(bits);
    }
    // one column space between chars
    i2c_write(background);
    if (effect & SSD1306_EFFECT_WIDE)
        i2c_write(background);
}

void ssd1306_write_char(uint8_t row, uint8_t column, char c, uint8_t effect)
{
    // Calculate pixel position.
    uint8_t pixel_column = column * SSD1306_CHAR_WIDTH;
    uint8_t pixel_page = row;
    uint8_t pixel_column_end;
    // Fill just the character box.
    pixel_column_end = pixel_column + 7;
    if (effect & SSD1306_EFFECT_WIDE)
        pixel_column_end += 8;
    i2c_start(SSD1306_ADDRESS | I2C_WRITE);
    i2c_write(SSD1306_MANY_COMMANDS);
    i2c_write(SSD1306_SET_COLUMN_RANGE);
    i2c_write(pixel_column + 1);
    i2c_write(pixel_column_end);
    i2c_write(SSD1306_SET_PAGE_RANGE);
    i2c_write(pixel_page);
    i2c_write(pixel_page);
    // Write data.
    i2c_rep_start(SSD1306_ADDRESS | I2C_WRITE);
    i2c_write(SSD1306_DATA);
    ssd1306_send_char_bits(c, effect);
    // And we're done.
    i2c_stop();
}
