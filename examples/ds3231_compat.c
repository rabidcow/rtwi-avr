#include "compat/i2cmaster.h"

#include <stdint.h>
#include "ds3231.h"

uint8_t ds3231_oscillator_control(uint8_t state)
{
    uint8_t ack = 0;
    ack |= i2c_start(DS3231_ADDRESS | I2C_WRITE);
    ack |= i2c_write(DS3231_STATUS);
    ack |= i2c_rep_start(DS3231_ADDRESS | I2C_READ);
    if (ack != 0)
    {
        i2c_stop();
        return ack;
    }
    uint8_t status = i2c_readNak();
    if ((status & DS3231_OSF) != state)
    {
        ack |= i2c_rep_start(DS3231_ADDRESS | I2C_WRITE);
        ack |= i2c_write(DS3231_STATUS);
        ack |= i2c_write((status &~ DS3231_OSF) | state);
    }
    i2c_stop();
    return ack;
}

uint8_t ds3231_get_time(struct DS3231_Time *p)
{
    uint8_t ack = 0;
    ack |= i2c_start(DS3231_ADDRESS | I2C_WRITE);
    ack |= i2c_write(DS3231_SECONDS);
    ack |= i2c_rep_start(DS3231_ADDRESS | I2C_READ);
    if (ack != 0)
    {
        i2c_stop();
        return ack;
    }
    p->seconds_bcd      = i2c_readAck();
    p->minutes_bcd      = i2c_readAck();
    p->hours_bcd        = i2c_readAck();
    p->day_of_week      = i2c_readAck();
    p->day_of_month_bcd = i2c_readAck();
    p->month_bcd        = i2c_readAck();
    p->year_bcd         = i2c_readNak() + DS3231_YEAR_ZERO;
    i2c_stop();

    if ((p->month_bcd & DS3231_CENTURY) != 0)
    {
        p->year_bcd |= 0x0100;
        p->month_bcd &= ~DS3231_CENTURY;
    }

    p->ampm = 0;
    if ((p->hours_bcd & DS3231_12_24) != 0)
    {
        p->ampm = ((p->hours_bcd & DS3231_AM_PM) >> 5) + 1;
        p->hours_bcd &= 0x1F;
    }
    return ack;
}

uint8_t ds3231_set_time(const struct DS3231_Time *p)
{
    // Stop the oscillator so the time doesn't get corrupted.
    uint8_t ack = TWI_SEND_ACK;
    ack |= i2c_start(DS3231_ADDRESS | I2C_WRITE);
    ack |= i2c_write(DS3231_STATUS);
    ack |= i2c_rep_start(DS3231_ADDRESS | I2C_READ);
    if (ack != TWI_SEND_ACK)
    {
        i2c_stop();
        return ack;
    }
    uint8_t status = i2c_readNak();
    if ((status & DS3231_OSF) == 0)
    {
        ack |= i2c_rep_start(DS3231_ADDRESS | I2C_WRITE);
        ack |= i2c_write(DS3231_STATUS);
        ack |= i2c_write(status | DS3231_OSF);
    }
    if (ack != TWI_SEND_ACK)
    {
        i2c_stop();
        return ack;
    }

    // Actually set the time.
    ack |= i2c_rep_start(DS3231_ADDRESS | I2C_WRITE);
    ack |= i2c_write(DS3231_SECONDS);
    ack |= i2c_write(p->seconds_bcd);
    ack |= i2c_write(p->minutes_bcd);
    uint8_t hours = p->hours_bcd;
    if (p->ampm)
        hours |= ((p->ampm - 1) << 5) | DS3231_12_24;
    ack |= i2c_write(hours);
    ack |= i2c_write(p->day_of_week);
    uint8_t day_of_month = p->day_of_month_bcd;
    uint16_t year = p->year_bcd - DS3231_YEAR_ZERO;
    if (year > 0x0100)
    {
        year &= 0xFF;
        day_of_month |= DS3231_CENTURY;
    }
    ack |= i2c_write(day_of_month);
    ack |= i2c_write(year);

    // Restore the oscillator state back to what it was.
    if ((status & DS3231_OSF) == 0)
    {
        ack |= i2c_rep_start(DS3231_ADDRESS | I2C_WRITE);
        ack |= i2c_write(DS3231_STATUS);
        ack |= i2c_write(status);
    }

    i2c_stop();
    return ack;
}
