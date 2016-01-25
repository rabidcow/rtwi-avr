#include "rtwi.h"

#include <stdint.h>
#include "ds3231.h"

uint8_t ds3231_oscillator_control(uint8_t state)
{
    uint8_t ack = TWI_SEND_ACK;
    ack |= twi_start_write(DS3231_ADDRESS);
    ack |= twi_send_byte(DS3231_STATUS);
    ack |= twi_start_read(DS3231_ADDRESS);
    if (ack != TWI_SEND_ACK)
    {
        twi_stop_condition();
        return ack;
    }
    uint8_t status = twi_recv_last();
    if ((status & DS3231_OSF) != state)
    {
        ack |= twi_start_write(DS3231_ADDRESS);
        ack |= twi_send_byte(DS3231_STATUS);
        ack |= twi_send_byte((status &~ DS3231_OSF) | state);
    }
    twi_stop_condition();
    return ack;
}

uint8_t ds3231_get_time(struct DS3231_Time *p)
{
    uint8_t ack = TWI_SEND_ACK;
    ack |= twi_start_write(DS3231_ADDRESS);
    ack |= twi_send_byte(DS3231_SECONDS);
    ack |= twi_start_read(DS3231_ADDRESS);
    if (ack != TWI_SEND_ACK)
    {
        twi_stop_condition();
        return ack;
    }
    p->seconds_bcd      = twi_recv_more();
    p->minutes_bcd      = twi_recv_more();
    p->hours_bcd        = twi_recv_more();
    p->day_of_week      = twi_recv_more();
    p->day_of_month_bcd = twi_recv_more();
    p->month_bcd        = twi_recv_more();
    p->year_bcd         = twi_recv_last() + DS3231_YEAR_ZERO;
    twi_stop_condition();

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
    ack |= twi_start_write(DS3231_ADDRESS);
    ack |= twi_send_byte(DS3231_STATUS);
    ack |= twi_start_read(DS3231_ADDRESS);
    if (ack != TWI_SEND_ACK)
    {
        twi_stop_condition();
        return ack;
    }
    uint8_t status = twi_recv_last();
    if ((status & DS3231_OSF) == 0)
    {
        twi_start_condition();
        ack |= twi_start_write(DS3231_ADDRESS);
        ack |= twi_send_byte(DS3231_STATUS);
        ack |= twi_send_byte(status | DS3231_OSF);
    }
    if (ack != TWI_SEND_ACK)
    {
        twi_stop_condition();
        return ack;
    }

    // Actually set the time.
    ack |= twi_start_write(DS3231_ADDRESS);
    ack |= twi_send_byte(DS3231_SECONDS);
    ack |= twi_send_byte(p->seconds_bcd);
    ack |= twi_send_byte(p->minutes_bcd);
    uint8_t hours = p->hours_bcd;
    if (p->ampm)
        hours |= ((p->ampm - 1) << 5) | DS3231_12_24;
    ack |= twi_send_byte(hours);
    ack |= twi_send_byte(p->day_of_week);
    uint8_t day_of_month = p->day_of_month_bcd;
    uint16_t year = p->year_bcd - DS3231_YEAR_ZERO;
    if (year > 0x0100)
    {
        year &= 0xFF;
        day_of_month |= DS3231_CENTURY;
    }
    ack |= twi_send_byte(day_of_month);
    ack |= twi_send_byte(year);

    // Restore the oscillator state back to what it was.
    if ((status & DS3231_OSF) == 0)
    {
        ack |= twi_start_write(DS3231_ADDRESS);
        ack |= twi_send_byte(DS3231_STATUS);
        ack |= twi_send_byte(status);
    }

    twi_stop_condition();
    return ack;
}
