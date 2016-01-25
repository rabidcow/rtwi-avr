#ifndef DS3231_H_INCLUDED
#define DS3231_H_INCLUDED

#include "rtwi.h" // for TWI_ADDRESS

#define DS3231_ADDRESS TWI_ADDRESS(1101000)

#define DS3231_SECONDS      0x00
#define DS3231_MINUTES      0x01
#define DS3231_HOURS        0x02
#define DS3231_DAY          0x03
#define DS3231_DATE         0x04
#define DS3231_MONTH        0x05
#define DS3231_YEAR         0x06
#define DS3231_A1_SECONDS   0x07
#define DS3231_A1_MINUTES   0x08
#define DS3231_A1_HOURS     0x09
#define DS3231_A1_DAY_DATE  0x0A
#define DS3231_A2_MINUTES   0x0B
#define DS3231_A2_HOURS     0x0C
#define DS3231_A2_DAY_DATE  0x0D
#define DS3231_CONTROL      0x0E
#define DS3231_STATUS       0x0F
#define DS3231_AGING_OFFSET 0x10
#define DS3231_TEMP_MSB     0x11
#define DS3231_TEMP_LSB     0x12

#define DS3231_12_24 0x40
#define DS3231_AM_PM 0x20
#define DS3231_CENTURY  0x80
#define DS3231_DAY_DATE 0x40
// CONTROL
#define DS3231_EOSC  0x80
#define DS3231_BBSQW 0x40
#define DS3231_CONV  0x20
#define DS3231_RS2   0x10
#define DS3231_RS1   0x08
#define DS3231_FREQ_1_HZ 0
#define DS3231_FREQ_1024_HZ DS3231_RS1
#define DS3231_FREQ_4096_HZ DS3231_RS2
#define DS3231_FREQ_8192_HZ (DS3231_RS2|DS3231_RS1)
#define DS3231_INTCN 0x04
#define DS3231_A2IE  0x02
#define DS3231_A1IE  0x01
// STATUS
#define DS3231_OSF     0x80
#define DS3231_EN32KHZ 0x08
#define DS3231_BSY     0x04
#define DS3231_A2F     0x02
#define DS3231_A1F     0x01

#define DS3231_YEAR_ZERO 0x2000

struct DS3231_Time
{
    uint8_t seconds_bcd;
    uint8_t minutes_bcd;
    uint8_t hours_bcd;
    uint8_t ampm;
    uint8_t day_of_week;
    uint8_t day_of_month_bcd;
    uint8_t month_bcd;
    uint16_t year_bcd;
};

#ifdef __cplusplus
extern "C"
{
#endif

uint8_t ds3231_oscillator_control(uint8_t state);
#define ds3231_stop()  ds3231_oscillator_control(DS3231_OSF)
#define ds3231_start() ds3231_oscillator_control(0)
uint8_t ds3231_get_time(struct DS3231_Time *p);
uint8_t ds3231_set_time(const struct DS3231_Time *p);

#ifdef __cplusplus
}
#endif

#endif /* DS3231_H_INCLUDED */
