#include "rtwi_cpp.h"

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

static struct : TwiDevice<0xD0>
{
    Reg <0x00> SECONDS;
    Reg <0x01> MINUTES;
    Reg <0x02> HOURS;
    Reg <0x03> DAY;
    Reg <0x04> DATE;
    Reg <0x05> MONTH_CENTURY;
    Reg <0x06> YEAR;
    Reg <0x07> ALARM_1_SECONDS;
    Reg <0x08> ALARM_1_MINUTES;
    Reg <0x09> ALARM_1_HOURS;
    Reg <0x0A> ALARM_1_DAY_DATE;
    //Block <0x07, 4> ALARM_1;
    Reg <0x0B> ALARM_2_MINUTES;
    Reg <0x0C> ALARM_2_HOURS;
    Reg <0x0D> ALARM_2_DAY_DATE;
    //Block <0x0B, 3> ALARM_2;
    Reg <0x0E> CONTROL;
    Reg <0x0F> STATUS;
    Reg <0x10, int8_t> AGING_OFFSET;
    RegR<0x11> MSB_OF_TEMP;
    RegR<0x12> LSB_OF_TEMP;
    RegR<0x11, BE_int16_t> TEMP;

    void alarm1_secondly()
    {
        ALARM_1_SECONDS  = 0x80;
        ALARM_1_MINUTES  = 0x80;
        ALARM_1_HOURS    = 0x80;
        ALARM_1_DAY_DATE = 0x80;
    }
    void alarm1_minutely(uint8_t seconds_bcd)
    {
        ALARM_1_SECONDS  = seconds_bcd;
        ALARM_1_MINUTES  = 0x80;
        ALARM_1_HOURS    = 0x80;
        ALARM_1_DAY_DATE = 0x80;
    }
    void alarm1_hourly(uint8_t minutes_bcd, uint8_t seconds_bcd)
    {
        ALARM_1_SECONDS  = seconds_bcd;
        ALARM_1_MINUTES  = minutes_bcd;
        ALARM_1_HOURS    = 0x80;
        ALARM_1_DAY_DATE = 0x80;
    }
    void alarm1_daily(uint8_t hours_bcd, uint8_t minutes_bcd, uint8_t seconds_bcd)
    {
        ALARM_1_SECONDS  = seconds_bcd;
        ALARM_1_MINUTES  = minutes_bcd;
        ALARM_1_HOURS    = hours_bcd;
        ALARM_1_DAY_DATE = 0x80;
    }
    void alarm1_weekly(uint8_t day, uint8_t hours_bcd, uint8_t minutes_bcd, uint8_t seconds_bcd)
    {
        ALARM_1_SECONDS  = seconds_bcd;
        ALARM_1_MINUTES  = minutes_bcd;
        ALARM_1_HOURS    = hours_bcd;
        ALARM_1_DAY_DATE = 0x40 | day;
    }
    void alarm1_monthly(uint8_t date_bcd, uint8_t hours_bcd, uint8_t minutes_bcd, uint8_t seconds_bcd)
    {
        ALARM_1_SECONDS  = seconds_bcd;
        ALARM_1_MINUTES  = minutes_bcd;
        ALARM_1_HOURS    = hours_bcd;
        ALARM_1_DAY_DATE = date_bcd;
    }

    void alarm2_minutely()
    {
        ALARM_2_MINUTES  = 0x80;
        ALARM_2_HOURS    = 0x80;
        ALARM_2_DAY_DATE = 0x80;
    }
    void alarm2_hourly(uint8_t minutes_bcd)
    {
        ALARM_2_MINUTES  = minutes_bcd;
        ALARM_2_HOURS    = 0x80;
        ALARM_2_DAY_DATE = 0x80;
    }
    void alarm2_daily(uint8_t hours_bcd, uint8_t minutes_bcd)
    {
        ALARM_2_MINUTES  = minutes_bcd;
        ALARM_2_HOURS    = hours_bcd;
        ALARM_2_DAY_DATE = 0x80;
    }
    void alarm2_weekly(uint8_t day, uint8_t hours_bcd, uint8_t minutes_bcd)
    {
        ALARM_2_MINUTES  = minutes_bcd;
        ALARM_2_HOURS    = hours_bcd;
        ALARM_2_DAY_DATE = 0x40 | day;
    }
    void alarm2_monthly(uint8_t date_bcd, uint8_t hours_bcd, uint8_t minutes_bcd)
    {
        ALARM_2_MINUTES  = minutes_bcd;
        ALARM_2_HOURS    = hours_bcd;
        ALARM_2_DAY_DATE = date_bcd;
    }

} DS3231;
