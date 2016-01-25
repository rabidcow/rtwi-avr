#include "rtwi.h"
#include "rdelay.h"
#include "demo.h"

#include <stdint.h>
#include <stdarg.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "ds3231.h"
#include "ssd1306.h"

uint8_t rng0, rng1;
uint8_t rng(void)
{
    uint8_t x = rng0;
    const uint8_t y = rng1;
    rng0 = y;
    x ^= x << 4;
    x ^= x >> 1;
    x ^= y ^ (y >> 2);
    rng1 = x + 64;
    return x;
}

#define YEAR_ZERO_BCD 0x2000
#define DAY_OF_WEEK_ZERO 0

const char day_names_blob[] PROGMEM =
    "Sunday\0"
    "Monday\0"
    "Tuesday\0"
    "Wednesday\0"
    "Thursday\0"
    "Friday\0"
    "Saturday";

const char * const day_names[] PROGMEM =
{
    day_names_blob + 0,
    day_names_blob + 7,
    day_names_blob + 14,
    day_names_blob + 22,
    day_names_blob + 32,
    day_names_blob + 41,
    day_names_blob + 48,
};

const char suffixes[] PROGMEM = "th\0st\0nd\0rd";

const char month_names_blob[] PROGMEM =
    "January\0"
    "February\0"
    "March\0"
    "April\0"
    "May\0"
    "June\0"
    "July\0"
    "August\0"
    "September\0"
    "October\0"
    "November\0"
    "December";

const char * const month_names[] PROGMEM =
{
    month_names_blob,
    month_names_blob + 0,
    month_names_blob + 8,
    month_names_blob + 17,
    month_names_blob + 23,
    month_names_blob + 29,
    month_names_blob + 33,
    month_names_blob + 38,
    month_names_blob + 43,
    month_names_blob + 50,
    month_names_blob + 60,
    month_names_blob + 68,
    month_names_blob + 77,
};

const char ampm[] PROGMEM = "  \0AM\0PM";

// Normally this would be inline, but so we can switch implementations...
void init_bus(void);

int main(void)
{
    DEMO_CLKPR_AUTO();
    DEMO_SET_UP_ALL_PORTS();
    DEMO_SET_UP_LED();

    // Hack to reset this cheap garbage SSD1306 board.
    PORTB &= ~(1 << 0);
    DDRB  |=   1 << 0 ;
    udelay(5);
    // Set to high impedence to let the external resistor pull it to +3.3v.
    DDRB  &= ~(1 << 0);

    init_bus();

    rng0 = 1;
    rng1 = 0;

    blink(0xB0, 2);

    ssd1306_init();
    ssd1306_clear();

    // Start the clock if it's not already running.
    ds3231_start();

    struct DS3231_Time now;
    while (1)
    {
        ds3231_get_time(&now);

        // 012345678901234567890
        // Wednesday, the 21st
        // of September, 2199
        //
        uint8_t suffix_index = now.day_of_month_bcd & 15;
        if (suffix_index > 3 || now.day_of_month_bcd == 0x11 || now.day_of_month_bcd == 0x12)
            suffix_index = 0;
        ssd1306_row = 0;
        ssd1306_column = 0;
        ssd1306_effect = 0;
        ssd1306_printf(PSTR("%s, the %x%s\nof %s, %x\n"),
            pgm_read_ptr(day_names + now.day_of_week),
            now.day_of_month_bcd,
            suffixes + 3*suffix_index,
            pgm_read_ptr(month_names + now.month_bcd),
            now.year_bcd);

        // 012345678901234567890
        //     ####::#### AM
        //     ####::####:##
        //
        ssd1306_row = 3;
        ssd1306_column = 0;
        ssd1306_effect = SSD1306_EFFECT_TALL_TOP | SSD1306_EFFECT_WIDE;
        ssd1306_printf(PSTR("  %02x:%02x"), now.hours_bcd, now.minutes_bcd);
        ssd1306_effect = 0;
        ssd1306_printf(PSTR(" %s\n"), ampm + 3*now.ampm);
        ssd1306_effect = SSD1306_EFFECT_TALL_BOTTOM | SSD1306_EFFECT_WIDE;
        ssd1306_printf(PSTR("  %02x:%02x"), now.hours_bcd, now.minutes_bcd);
        ssd1306_effect = 0;
        ssd1306_printf(PSTR(":%02x\n"), now.seconds_bcd);

        // 012345678901234567890
        // 2199-09-21 12:59:59AM
        ssd1306_row = 6;
        ssd1306_column = 0;
        ssd1306_effect = 0;
        ssd1306_printf(PSTR("%04x-%02x-%02x %02x:%02x:%02x%s"),
            now.year_bcd, now.month_bcd, now.day_of_month_bcd,
            now.hours_bcd, now.minutes_bcd, now.seconds_bcd,
            ampm + 3*now.ampm);
        // TODO: trigger on the interrupt output from DS3231
        mdelay(300);
    }
}
