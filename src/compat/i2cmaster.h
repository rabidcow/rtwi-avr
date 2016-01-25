/*

Copyright (c) 2015-2016 Joshua Seagoe

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef I2CMASTER_H_INCLUDED
#define I2CMASTER_H_INCLUDED

/*

Shims for compatibility with code written for Peter Fleury's i2cmaster.

*/

#include "rtwi.h"
#include <avr/io.h>

#define I2C_READ  1
#define I2C_WRITE 0

#define i2c_init        twi_init
#define i2c_stop        twi_stop_condition
#define i2c_start       twi_start_send
#define i2c_rep_start   twi_start_send
#define i2c_write       twi_send_byte
#define i2c_readAck     twi_recv_more
#define i2c_readNak     twi_recv_last

#define i2c_start_wait(ADDR) do { } while (i2c_start(ADDR))
#define i2c_read(ACK) twi_recv_byte((ACK) ? TWI_RECV_MORE : TWI_RECV_LAST)

#endif /* I2CMASTER_H_INCLUDED */
