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

#ifndef RTWI_H_INCLUDED
#define RTWI_H_INCLUDED

#ifndef F_TWI
/* Bus clock frequency. (Target; actual frequency may be lower.) */
#   define F_TWI 100000
#endif

#include <avr/io.h>

#if defined(TWI_HARDWARE)

#   if !defined(TWBR) || !defined(TWSR) || !defined(TWCR) || !defined(TWDR) || !defined(TWINT) || !defined(TWSTA) || !defined(TWSTO) || !defined(TWEN)
#       error "Can't find TWI hardware definitions."
#   endif

#   define TWI_FREQ_SCALE ((F_CPU / F_TWI) - 16) / 2
#   if TWI_FREQ_SCALE < 256
#       define TWI_TWBR TWI_FREQ_SCALE
#       define TWI_TWPS 0
#   elif TWI_FREQ_SCALE < 4 * 256 && defined(TWPS0)
#       define TWI_TWBR (TWI_FREQ_SCALE / 4)
#       define TWI_TWPS (1<<TWPS0)
#   elif TWI_FREQ_SCALE < 4 * 4 * 256 && defined(TWPS1)
#       define TWI_TWBR (TWI_FREQ_SCALE / 16)
#       define TWI_TWPS ((1<<TWPS1) | (0<<TWPS0))
#   elif TWI_FREQ_SCALE < 4 * 4 * 4 * 256 && defined(TWPS1)
#       define TWI_TWBR (TWI_FREQ_SCALE / 64)
#       define TWI_TWPS ((1<<TWPS1) | (1<<TWPS0))
#   else
#       error "F_TWI too slow for F_CPU."
#   endif

#   define TWI_IF_HARDWARE(...) __VA_ARGS__
#   define TWI_IF_SOFTWARE(...)

#else /* TWI_HARDWARE */

#   if F_TWI <= 100000
/* Standard-mode */
#       define TWI_THDSTA_MIN 4000 /* 32.0 clocks @ 8 MHz */
#       define TWI_TLOW_MIN   4700 /* 37.6 */
#       define TWI_THIGH_MIN  4000 /* 32.0 */
#       define TWI_TSUSTA_MIN 4700 /* 37.6 */
#       define TWI_THDDAT_MIN  300 /*  2.4 */
#       define TWI_TSUDAT_MIN  250 /*  2.0 */
#       define TWI_TSUSTO_MIN 4000 /* 32.0 */
#       define TWI_TBUF_MIN   4700 /* 37.6 */
#   elif F_TWI <= 400000
/* Fast-mode */
#       define TWI_THDSTA_MIN  600 /*  4.8 clocks @ 8 MHz */
#       define TWI_TLOW_MIN   1300 /* 10.4 */
#       define TWI_THIGH_MIN   600 /*  4.8 */
#       define TWI_TSUSTA_MIN  600 /*  4.8 */
#       define TWI_THDDAT_MIN  300 /*  2.4 */
#       define TWI_TSUDAT_MIN  100 /*  0.8 */
#       define TWI_TSUSTO_MIN  600 /*  4.8 */
#       define TWI_TBUF_MIN   1300 /* 10.4 */
#   elif F_TWI <= 1000000
/* Fast-mode Plus */
#       define TWI_THDSTA_MIN  260
#       define TWI_TLOW_MIN    500
#       define TWI_THIGH_MIN   260
#       define TWI_TSUSTA_MIN  260
#       define TWI_THDDAT_MIN    0
#       define TWI_TSUDAT_MIN   50
#       define TWI_TSUSTO_MIN  260
#       define TWI_TBUF_MIN    500
#   else
#       error "F_TWI too large."
#   endif

#   define TWI_TGLITCH (2 * 1000000000 / F_CPU) /* 2 CPU cycles to set output bit. */
#   define TWI_TLOW_EXTRA (TWI_TLOW_MIN - (TWI_THDDAT_MIN + TWI_TSUDAT_MIN + TWI_TGLITCH))
#   if TWI_TLOW_EXTRA < 0
#       undef TWI_TLOW_EXTRA
#       define TWI_TLOW_EXTRA 0
#   endif
#   ifndef TWI_TRISE
#       ifdef TWI_INTERNAL_PULLUP
            // conservative estimate for internal pull-up at 5V
#           define TWI_TRISE 3000
#       else
#           define TWI_TRISE 0
#       endif
#   endif
#   ifndef TWI_TFALL
#       define TWI_TFALL 6
#   endif

/* 100 kHz = 10000 ns = 80 clocks @ 8 MHz */
/* 400 kHz =  2500 ns = 20 clocks @ 8 MHz */
/*   1 MHz =  1000 ns                     */
#   define TWI_TCYCLE (1000000000 / F_TWI)
// Rise time is from 30% to 70% of Vbus, but we start at zero, which takes 42% longer.
#   define TWI_TRISE_FROM_ZERO (TWI_TRISE + TWI_TRISE*42/100)
#   define TWI_TCYCLE_EXTRA (TWI_TCYCLE - (TWI_TSUSTA_MIN + TWI_TLOW_MIN) - (TWI_TRISE_FROM_ZERO + TWI_TFALL))
#   if TWI_TCYCLE_EXTRA < 0
#       undef TWI_TCYCLE_EXTRA
#       define TWI_TCYCLE_EXTRA 0
#   endif
#   if TWI_TCYCLE < TWI_THIGH_MIN + TWI_TLOW_MIN
#       error "Bad timing constants."
#   endif

// Preprocessor stuff to make pin configuration less tedious.
#   define TWI_PASTE_(A, B) A ## B
#   define TWI_PASTE(A, B) TWI_PASTE_(A, B)
#   define TWI_EXPAND(X) X
#   define TWI_PORT_(LETTER, POSITION) TWI_PASTE(PORT, LETTER)
#   define TWI_PORT(SPEC) TWI_EXPAND(TWI_PORT_)SPEC
#   define TWI_DDR_(LETTER, POSITION) TWI_PASTE(DDR, LETTER)
#   define TWI_DDR(SPEC) TWI_EXPAND(TWI_DDR_)SPEC
#   define TWI_PIN_(LETTER, POSITION) TWI_PASTE(PIN, LETTER)
#   define TWI_PIN(SPEC) TWI_EXPAND(TWI_PIN_)SPEC
#   define TWI_BIT_(LETTER, POSITION) POSITION
#   define TWI_BIT(SPEC) TWI_EXPAND(TWI_BIT_)SPEC
#   define TWI_MASK(SPEC) (1 << (TWI_BIT(SPEC)))
#   define TWI_SAME_PORT(SPEC1, SPEC2) (_SFR_ADDR(TWI_PORT(SPEC1)) == _SFR_ADDR(TWI_PORT(SPEC2)))
#   define TWI_SAME_PIN(SPEC1, SPEC2) (TWI_BIT(SPEC1) == TWI_BIT(SPEC2) && TWI_SAME_PORT(SPEC1, SPEC2))

// Default values for SCL/SDA.
#   ifndef TWI_SCL
#       define TWI_SCL (B, 0)
#   endif
#   ifndef TWI_SDA
#       define TWI_SDA (B, 1)
#   endif

// We support split input/output on the data line, but we also support the standard combined SDA.
#   ifndef TWI_SDO
#       define TWI_SDO TWI_SDA
#   endif
#   ifndef TWI_SDI
#       define TWI_SDI TWI_SDA
#   endif

// Split clock/busy? Why not.
#   ifndef TWI_BUSY
#       define TWI_BUSY TWI_SCL
#   endif

#   define TWI_IF_HARDWARE(...)
#   define TWI_IF_SOFTWARE(...) __VA_ARGS__

#endif /* TWI_HARDWARE */

#define TWI_7BIT(ADDRESS) ((ADDRESS)<<1)
#define TWI_8BIT(ADDRESS) (ADDRESS)
#define TWI_7BIT_BINARY(BITS) \
    ( ((07000000&0##BITS) ? 0x80 : 0) \
    | ((00700000&0##BITS) ? 0x40 : 0) \
    | ((00070000&0##BITS) ? 0x20 : 0) \
    | ((00007000&0##BITS) ? 0x10 : 0) \
    | ((00000700&0##BITS) ? 0x08 : 0) \
    | ((00000070&0##BITS) ? 0x04 : 0) \
    | ((00000007&0##BITS) ? 0x02 : 0) \
    )
#define TWI_10BIT_BINARY(BITS) \
    ( ((07000000000&0##BITS) ? 0x0400 : 0) \
    | ((00700000000&0##BITS) ? 0x0200 : 0) \
    | ((00070000000&0##BITS) ? 0x0080 : 0) \
    | ((00007000000&0##BITS) ? 0x0040 : 0) \
    | ((00000700000&0##BITS) ? 0x0020 : 0) \
    | ((00000070000&0##BITS) ? 0x0010 : 0) \
    | ((00000007000&0##BITS) ? 0x0008 : 0) \
    | ((00000000700&0##BITS) ? 0x0004 : 0) \
    | ((00000000070&0##BITS) ? 0x0002 : 0) \
    | ((00000000007&0##BITS) ? 0x0001 : 0) \
    | 0xF800)
/*  7-bit addresses: abcdefg -> abcdefg0
   10-bit addresses: abcdefghij -> 11111ab0 cdefghij
*/
#define TWI_ADDRESS(BITS) (sizeof(#BITS) > 8 ? TWI_10BIT_BINARY(BITS) : TWI_7BIT_BINARY(BITS))


#define TWI_WRITE_TO(ADDRESS)  (((ADDRESS) &~ 1) + 0x00)
#define TWI_READ_FROM(ADDRESS) (((ADDRESS) &~ 1) + 0x01)
#define TWI_GENERAL_CALL 0x00
#define TWI_START_BYTE   0x01

#if defined(TWI_HARDWARE)
#   define TWI_RECV_MORE ((1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN))
#   define TWI_RECV_LAST ((1<<TWINT) | (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN))
#   define TWI_SEND_ACK  0x00
#   define TWI_SEND_NACK 0x80
#else
#   define TWI_RECV_MORE 0x00
#   define TWI_RECV_LAST 0x80
#   define TWI_SEND_ACK  0x00
#   define TWI_SEND_NACK 0xFF
#endif

#ifndef __ASSEMBLER__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(TWI_NO_EXPLICIT_CLOBBER)

void twi_start_condition(void);
void twi_stop_condition(void);

uint8_t twi_send_byte(uint8_t x);
uint8_t twi_recv_byte(uint8_t ack);

uint8_t twi_start_send(uint8_t x);
uint8_t twi_recv_more(void);
uint8_t twi_recv_last(void);

#else /* TWI_NO_EXPLICIT_CLOBBER */

/* Calling via inline assembly allows us to specify which registers each
   function clobbers. That allows GCC to do much better register allocation
   around these calls. */

#   define twi_start_condition twi_inline_start_condition
#   define twi_stop_condition twi_inline_stop_condition
#   define twi_send_byte twi_inline_send_byte
#   define twi_recv_byte twi_inline_recv_byte

#   define twi_start_send twi_inline_start_send
#   define twi_recv_more twi_inline_recv_more
#   define twi_recv_last twi_inline_recv_last

static inline void twi_start_condition(void) __attribute__((always_inline));
void twi_start_condition(void)
{
    __asm__ volatile (
        "rcall twi_start_condition\n\t"
        : : : TWI_IF_HARDWARE("r24") TWI_IF_SOFTWARE("r26", "r27")
    );
}

static inline void twi_stop_condition(void) __attribute__((always_inline));
void twi_stop_condition(void)
{
    __asm__ volatile (
        "rcall twi_stop_condition\n\t"
        : : : TWI_IF_HARDWARE("r24") TWI_IF_SOFTWARE("r26", "r27")
    );
}

static inline uint8_t twi_send_byte(uint8_t x) __attribute__((always_inline));
uint8_t twi_send_byte(uint8_t x)
{
    register uint8_t ack __asm__("r24") = x;
    __asm__ volatile (
        "rcall twi_send_byte\n\t"
        : "=r" (ack)
        : "r" (ack)
        : TWI_IF_SOFTWARE("r1", "r25", "r26", "r27")
    );
    return ack;
}

static inline uint8_t twi_recv_byte(uint8_t ack) __attribute__((always_inline));
uint8_t twi_recv_byte(uint8_t ack)
{
    register uint8_t x __asm__("r24") = ack;
    __asm__ volatile (
        "rcall twi_recv_byte\n\t"
        : "=r" (x)
        : "r" (x)
        : TWI_IF_SOFTWARE("r1", "r25", "r26", "r27")
    );
    return x;
}

static inline uint8_t twi_start_send(uint8_t x) __attribute__((always_inline));
uint8_t twi_start_send(uint8_t x)
{
    register uint8_t ack __asm__("r24") = x;
    __asm__ volatile (
        "rcall twi_start_send\n\t"
        : "=r" (ack)
        : "r" (ack)
        : TWI_IF_SOFTWARE("r1", "r25", "r26", "r27")
    );
    return ack;
}

static inline uint8_t twi_recv_more(void) __attribute__((always_inline));
uint8_t twi_recv_more(void)
{
    register uint8_t x __asm__("r24");
    __asm__ volatile (
        "rcall twi_recv_more\n\t"
        : "=r" (x)
        :
        : TWI_IF_SOFTWARE("r1", "r25", "r26", "r27")
    );
    return x;
}

static inline uint8_t twi_recv_last(void) __attribute__((always_inline));
uint8_t twi_recv_last(void)
{
    register uint8_t x __asm__("r24");
    __asm__ volatile (
        "rcall twi_recv_last\n\t"
        : "=r" (x)
        :
        : TWI_IF_SOFTWARE("r1", "r25", "r26", "r27")
    );
    return x;
}

#endif /* TWI_NO_EXPLICIT_CLOBBER */

// Try to clear a stuck SDA line by sending nine clock pulses.
#define twi_clear_bus() do { twi_send_byte(0xFF); twi_stop_condition(); } while(0)

#define twi_start_write(ADDR) twi_start_send(TWI_WRITE_TO(ADDR))
#define twi_start_read(ADDR) twi_start_send(TWI_READ_FROM(ADDR))

static inline void twi_init(void) __attribute__((always_inline));
void twi_init(void)
{
#if defined(TWI_HARDWARE)
    /* You can set these manually to adjust bus speed at runtime, but
       twi_send_byte won't report status correctly if you use a different
       TWPS. */
    TWBR = TWI_TWBR;
    TWSR = TWI_TWPS;
#else /* TWI_HARDWARE */
    /* It's generally better to set this up with all your other pins, but this
       function is provided for convenience. */
#   if !defined(TWI_INTERNAL_PULLUP)
        /* LOW/no pull-up and INPUT */
#       define TWI_INIT_LINE(SPEC) do { \
            TWI_DDR(SPEC)  &= ~TWI_MASK(SPEC); \
            TWI_PORT(SPEC) &= ~TWI_MASK(SPEC); } while(0)
#   else
        /* HIGH/pull-up and INPUT */
#       define TWI_INIT_LINE(SPEC) do { \
            TWI_DDR(SPEC)  &= ~TWI_MASK(SPEC); \
            TWI_PORT(SPEC) |=  TWI_MASK(SPEC); } while(0)
#   endif
    TWI_INIT_LINE(TWI_SCL);
    if (!TWI_SAME_PIN(TWI_SCL, TWI_BUSY))
        TWI_INIT_LINE(TWI_BUSY);
    TWI_INIT_LINE(TWI_SDO);
    if (!TWI_SAME_PIN(TWI_SDI, TWI_SDO))
        TWI_INIT_LINE(TWI_SDI);
#   undef TWI_INIT_LINE
#endif /* TWI_HARDWARE */
}

#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLER__ */

#endif /* RTWI_H_INCLUDED */
