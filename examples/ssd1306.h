#ifndef SSD1306_H_INCLUDED
#define SSD1306_H_INCLUDED

#include <stdint.h>
#include "rtwi.h" // for TWI_ADDRESS

#define SSD1306_ADDRESS TWI_ADDRESS(0111100)
//#define SSD1306_ADDRESS TWI_ADDRESS(0111101)

// Control Byte
#define SSD1306_MANY_COMMANDS 0x00
#define SSD1306_ONE_COMMAND   0x80
#define SSD1306_DATA          0x40

// "Fundamental" Commands
#define SSD1306_SET_CONTRAST       0x81
#define SSD1306_DISPLAY_ON         0xAF
#define SSD1306_DISPLAY_OFF        0xAE
#define SSD1306_DISPLAY_BLANK      0xA5
#define SSD1306_DISPLAY_UNBLANK    0xA4
#define SSD1306_DISPLAY_NORMAL     0xA6
#define SSD1306_DISPLAY_INVERSE    0xA7

// Scrolling Commands
#define SSD1306_SCROLL_RIGHT           0x26
#define SSD1306_SCROLL_LEFT            0x27
#define SSD1306_SCROLL_VERTICAL_RIGHT  0x29
#define SSD1306_SCROLL_VERTICAL_LEFT   0x2A
#define SSD1306_SCROLL_DISABLE         0x2E
#define SSD1306_SCROLL_ENABLE          0x2F
#define SSD1306_SCROLL_VERTICAL_AREA   0xA3

// Addressing Commands
#define SSD1306_SET_COLUMN_LOW     0x00 /* 0000xxxx */
#define SSD1306_SET_COLUMN_HIGH    0x10 /* 0001xxxx */
#define SSD1306_SET_ADDRESS_MODE   0x20
#define     SSD1306_ADDRESS_MODE_HORIZONTAL 0x00
#define     SSD1306_ADDRESS_MODE_VERTICAL   0x01
#define     SSD1306_ADDRESS_MODE_PAGE       0x02
#define SSD1306_SET_COLUMN_RANGE   0x21
#define SSD1306_SET_PAGE_RANGE     0x22
#define SSD1306_SET_PAGE           0xB0 /* 10110xxx */

// Hardware Configuration Commands
#define SSD1306_SET_DISPLAY_START      0x40 /* 01xxxxxx */
#define SSD1306_SET_SEGMENTS_ASC       0xA0
#define SSD1306_SET_SEGMENTS_DESC      0xA1
#define SSD1306_SET_MULTIPLEX_RATIO    0xA8
#define SSD1306_SET_COM_ASC            0xC0
#define SSD1306_SET_COM_DESC           0xC8
#define SSD1306_SET_DISPLAY_OFFSET     0xD3
#define SSD1306_SET_COM_CONFIGURATION  0xDA
#define SSD1306_SET_DISPLAY_CLOCK      0xD5
#define SSD1306_SET_PRECHARGE          0xD9
#define SSD1306_SET_VCOMH_DESELECT     0xDB
#define SSD1306_NOP    0xE3

// Charge Pump Commands
#define SSD1306_SET_CHARGE_PUMP        0x8D
#define     SSD1306_CHARGE_PUMP_DISABLE 0x10
#define     SSD1306_CHARGE_PUMP_ENABLE  0x14


#define SSD1306_SCREEN_WIDTH  128
#define SSD1306_SCREEN_HEIGHT 64

#define SSD1306_CHAR_WIDTH  6
#define SSD1306_CHAR_HEIGHT 8

// 128/6=21 columns and 64/8=8 rows
#define SSD1306_COLUMNS (SSD1306_SCREEN_WIDTH / SSD1306_CHAR_WIDTH)
#define SSD1306_ROWS    (SSD1306_SCREEN_HEIGHT / SSD1306_CHAR_HEIGHT)

#define SSD1306_EFFECT_NARROW       0x00
#define SSD1306_EFFECT_WIDE         0x01
#define SSD1306_EFFECT_SHORT        0x00
#define SSD1306_EFFECT_TALL         0x04
#define SSD1306_EFFECT_TALL_ROW     0x02
#define SSD1306_EFFECT_TALL_TOP     (SSD1306_EFFECT_TALL)
#define SSD1306_EFFECT_TALL_BOTTOM  (SSD1306_EFFECT_TALL | SSD1306_EFFECT_TALL_ROW)
#define SSD1306_EFFECT_INVERSE      0x08

#ifdef __cplusplus
extern "C"
{
#endif

extern const uint8_t PROGMEM ssd1306_chargen[96*(SSD1306_CHAR_WIDTH-1)];

void ssd1306_init(void);
void ssd1306_clear(void);
void ssd1306_write_char(uint8_t row, uint8_t column, char c, uint8_t effect);
uint8_t ssd1306_row, ssd1306_column, ssd1306_effect;
int ssd1306_putce(char c, uint8_t effect);
int ssd1306_putc(char c);
void ssd1306_printf(const char * PROGMEM format, ...);

#ifdef __cplusplus
}
#endif

#endif /* SSD1306_H_INCLUDED */
