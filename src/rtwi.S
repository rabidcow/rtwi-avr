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

#include <avr/io.h>
#include "rtwi.h"
#include "cpu.h"
// move the delay registers so ndelay preserves r24/r25
#define DELAY_RLO r26
#define DELAY_RHI r27
#include "rdelay.h"

#define MIN(A, B) (((A) < (B)) * (A) + ((A) >= (B)) * (B))
#define MAX(A, B) (((A) < (B)) * (B) + ((A) >= (B)) * (A))

/*
    Bus Volatage
    Vcc Min Max
    5.5 3.3 6.0
    5.0 3.0 5.5
    4.5 2.7 5.0
    3.3 2.0 3.8
    3.0 1.8 3.5
*/

#if defined(TWI_HARDWARE)
/*

Implementation of TWI using dedicated TWI hardware.

*/

// Having to _SFR_IO_ADDR all the time is annoying.
.macro stsfr d, s
    .if _SFR_IO_REG_P(\d)
        out _SFR_IO_ADDR(\d), \s
    .else
        sts _SFR_MEM_ADDR(\d), \s
    .endif
.endm
.macro ldsfr d, s
    .if _SFR_IO_REG_P(\s)
        in \d, _SFR_IO_ADDR(\s)
    .else
        lds \d, _SFR_MEM_ADDR(\s)
    .endif
.endm

.section .text.twi_start_stop_condition
.global twi_start_condition
twi_start_condition:
    ldi r24, (1<<TWINT) | (1<<TWSTA) | (0<<TWSTO) | (1<<TWEN)
twi_control_wait:
    stsfr TWCR, r24
twi_wait:
    ldsfr r24, TWCR
    sbrs r24, TWINT
    rjmp twi_wait
    ret

.global twi_stop_condition
twi_stop_condition:
    ldi r24, (1<<TWINT) | (0<<TWSTA) | (1<<TWSTO) | (1<<TWEN)
    rjmp twi_control_wait

.section .text.twi_send_byte
.global twi_start_send
twi_start_send:
    rcall twi_start_condition
.global twi_send_byte
twi_send_byte:                          ; uint8_t twi_send_byte(uint8_t data);
    stsfr TWDR, r24
    ldi r24, (1<<TWINT) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN)
    rcall twi_control_wait
    ldsfr r24, TWSR
    // TWSR 000110xx: + SLA+W, ACK
    // TWSR 001000xx: - SLA+W, NACK
    // TWSR 001010xx: + data, ACK
    // TWSR 001100xx: - data, NACK
    // --------------------------- polarity change
    // TWSR 001110xx: - arbitration lost
    // TWSR 010000xx: + SLA+R, ACK
    // TWSR 010010xx: - SLA+R, NACK
    subi r24, 0xF8 | TWI_TWPS           ; remove TWPS and get bit 6 to indicate polarity of bit 3
    swap r24                            ; move bit 6 down to where it can carry to bit 3
    subi r24, 0x84                      ; toggle bit 7 (was 3) if bit 2 (was 6) is set
    andi r24, 0x80                      ; 00=ACK, 80=NACK/arbitration lost
    ret

.section .text.twi_recv_byte
.global twi_recv_last
twi_recv_last:
    ldi r24, TWI_RECV_LAST
    rjmp twi_recv_byte
.global twi_recv_more
twi_recv_more:
    ldi r24, TWI_RECV_MORE
.global twi_recv_byte
twi_recv_byte:                          ; uint8_t twi_recv_byte(uint8_t ack);
    ; TWI_RECV_MORE and TWI_RECV_LAST are set to the appropriate constants in rtwi.h
    rcall twi_control_wait
    ldsfr r24, TWDR
    ret

#else /* TWI_HARDWARE */
/*

Implementation of TWI in software.

*/

#if !defined(TWI_INTERNAL_PULLUP)
// Levels are set by toggling the DDR pin and letting the external pull-up resister do its job.
// This is because slaves are allowed to stretch the clock by holding clock low.
// The resulting code seems backwards because DDR=0 sets the IO line to a logic HIGH.
.macro strong_low port, ddr, bit
    sbi _SFR_IO_ADDR(\ddr), \bit
.endm
.macro weak_high port, ddr, bit
    cbi _SFR_IO_ADDR(\ddr), \bit
.endm
#else
// For the internal pull-ups, we have to toggle two bits to change line status:
// high = input, high; low = output, low
// Always go via input, low to avoid driving a strong high against a bus that might be strong low.
.macro strong_low port, ddr, bit
    cbi _SFR_IO_ADDR(\port), \bit
    sbi _SFR_IO_ADDR(\ddr), \bit
.endm
.macro weak_high port, ddr, bit
    cbi _SFR_IO_ADDR(\ddr), \bit
    sbi _SFR_IO_ADDR(\port), \bit
.endm
#endif

#define SET_SCL_HIGH      weak_high  TWI_PORT(TWI_SCL), TWI_DDR(TWI_SCL), TWI_BIT(TWI_SCL)
#define SET_SCL_LOW       strong_low TWI_PORT(TWI_SCL), TWI_DDR(TWI_SCL), TWI_BIT(TWI_SCL)
#define SET_SDO_HIGH      weak_high  TWI_PORT(TWI_SDO), TWI_DDR(TWI_SDO), TWI_BIT(TWI_SDO)
#define SET_SDO_LOW       strong_low TWI_PORT(TWI_SDO), TWI_DDR(TWI_SDO), TWI_BIT(TWI_SDO)

#define SKIP_IF_SCL_HIGH  sbis _SFR_IO_ADDR(TWI_PIN(TWI_SCL)), TWI_BIT(TWI_SCL)
#define SKIP_IF_BUSY_HIGH sbis _SFR_IO_ADDR(TWI_PIN(TWI_BUSY)), TWI_BIT(TWI_BUSY)
#define SKIP_IF_SDI_LOW   sbic _SFR_IO_ADDR(TWI_PIN(TWI_SDI)), TWI_BIT(TWI_SDI)

/*

    first byte
        ___    ____ ____ ____ ____ ____ ____ ____ ____ ____________
    SDA    \__/____X____X____X____X____X____X____X____X____/
        S/Sr    7    6    5    4    3    2    1    0   ACK
        _____    _    _    _    _    _    _    _    _    _
    SCL      \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \________


    last byte
        ___________ ____ ____ ____ ____ ____ ____ ____ ____ ___ ___
    SDA       \____X____X____X____X____X____X____X____X____X___X___
                7    6    5    4    3    2    1    0   ACK   Sr/P
                 _    _    _    _    _    _    _    _    _    _____
    SCL ________/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ 

*/

// Shave off some time for the rcall into twi_begin_clock_pulse, tuned for the rounding that will happen in ndelay.
// It gets the smallest delay because it has the least work to hide.
#define TWI_TLOW_SLIVER (TWI_TLOW_EXTRA/19)
#define TWI_TCYCLE_SLIVER (TWI_TCYCLE_EXTRA/3)

.section .text.twi_sw

.global twi_start_condition
twi_start_condition:                    ; void twi_start_condition(void);
    SET_SDO_HIGH                        ; Take data high in case this is a repeated start.
    SKIP_IF_SCL_HIGH                    ; Same with clock, but in that case we need to add some delays.
    rcall twi_begin_clock_pulse         ; Specifically, the same delays as if beginning a regular clock pulse.
    SET_SDO_LOW                         ; Start condition is a transition to low on SDA while SCL is high.
                                        ; Hold clock high long enough for start condition to be recognized.
    ndelay TWI_THDSTA_MIN, CPU_CYCLES_SBI
                                        ; Start condition will be followed by data,
                                        ; so take the clock low in preparation.
; ... by falling through to twi_end_clock_pulse

twi_end_clock_pulse:
    SET_SCL_LOW
                                        ; wait data hold time + half of low time
    ndelay (TWI_THDDAT_MIN + TWI_TLOW_EXTRA-TWI_TLOW_SLIVER), (CPU_CYCLES_SBI + CPU_CYCLES_RET + CPU_CYCLES_RCALL + CPU_CYCLES_DEC + 2 + CPU_CYCLES_LSL + CPU_CYCLES_ROL)
    ret

.global twi_stop_condition
twi_stop_condition:                     ; void twi_stop_condition(void);
    SET_SDO_LOW                         ; Stop condition is a low to high transition, so start low.
    rcall twi_begin_clock_pulse         ; Make sure the clock high long enough.
    SET_SDO_HIGH                        ; Then go high.
    ret

twi_begin_clock_pulse:
                                        ; We just had a transition, so we need to wait for the data set-up time.
    ndelay (TWI_TSUDAT_MIN + TWI_TLOW_SLIVER + TWI_TCYCLE_SLIVER), CPU_CYCLES_RCALL
    SET_SCL_HIGH                        ; Float the clock high.
1:  SKIP_IF_BUSY_HIGH                   ; Wait for slaves to release the clock as well.
    rjmp 1b
                                        ; Hold clock high for the minimum time.
                                        ; This also works for stop condition, because it's the same length.
    ; Wait the extra time required to meet our target clock rate during the high pules, because that consumes less power.
    // need MAX(TWI_TSUSTA_MIN, TWI_THIGH_MIN, TWI_TSUSTO_MIN)
    ndelay (TWI_TSUSTA_MIN + TWI_TCYCLE_EXTRA-TWI_TCYCLE_SLIVER), (2 + CPU_CYCLES_RET + CPU_CYCLES_SBI + 2 + CPU_CYCLES_INC)
    ret

/*

    Before loop:
     ___      ___ ___ ___ ___ ___ ___ ___ ___      ___ ___ ___ ___ ___ ___ ___ ___
    |___| <- |_7_|_6_|_5_|_4_|_3_|_2_|_1_|_0_| <- |_A_|___|___|___|___|___|___|___| <- SDA
    CARRY                DATA_REG                              ACK_REG

    After loop:
     ___      ___ ___ ___ ___ ___ ___ ___ ___      ___ ___ ___ ___ ___ ___ ___ ___
    |___| <- |___|___|___|___|___|___|___|_7_| <- |_6_|_5_|_4_|_3_|_2_|_1_|_0_|_A_| <- SDA
    CARRY                DATA_REG                              ACK_REG

    Return:
              ___ ___ ___ ___ ___ ___ ___ ___      ___ ___ ___ ___ ___ ___ ___ ___      ___
             |___|___|___|___|___|___|___|___| -> |_7_|_6_|_5_|_4_|_3_|_2_|_1_|_0_| -> |_A_|
                         DATA_REG                              ACK_REG                 CARRY

    Each loop iteration:
         ______
    SDA X______    1: set SDA to data/floating
        1   3      2: begin clock pulse
           __      3: read data/ack
    SCL __/  \_    4: end clock pulse
          2  4
*/

#define TWI_DATA_BITS 8
#define TWI_ACK_BITS 1

#define BIT_COUNTER  r25
#define ACK_REG  r24
#define DATA_REG r1

.global twi_recv_last
twi_recv_last:
    ldi r24, TWI_RECV_LAST
    rjmp twi_recv_byte
.global twi_recv_more
twi_recv_more:
    ldi r24, TWI_RECV_MORE
.global twi_recv_byte
twi_recv_byte:                          ; uint8_t twi_recv_byte(uint8_t ack);
    dec DATA_REG                        ; send high for all data bits so sender can drive them
; ... fall through to twi_send_or_recv_byte (assumes ACK_REG == r24 to call from C)

twi_send_or_recv_byte:
    ldi BIT_COUNTER, TWI_DATA_BITS + TWI_ACK_BITS
1:  lsl ACK_REG                         ; shift out the next bit to send
    rol DATA_REG
    SET_SDO_HIGH                        ; (1) set SDA to value of carry flag
    brcs 2f                             ; This will glitch for 2 clocks if we're setting SDA
    SET_SDO_LOW                         ; low, but since SCL is low, that should be fine.
2:  rcall twi_begin_clock_pulse         ; (2) do the clock pulse
    SKIP_IF_SDI_LOW                     ; (3) check data level at the end
    inc ACK_REG
    rcall twi_end_clock_pulse           ; (4) end clock pulse
    dec BIT_COUNTER
    brne 1b
    lsr DATA_REG                        ; shift back one so ack is in carry and data in ACK_REG
    ror ACK_REG
    ret

.global twi_start_send
twi_start_send:
    rcall twi_start_condition
.global twi_send_byte
twi_send_byte:                          ; uint8_t twi_send_byte(uint8_t data);
    mov DATA_REG, r24
    ldi ACK_REG, 0x80                   ; send high for ack so reciever can drive it
    rcall twi_send_or_recv_byte
    sbc r24, r24                        ; set result from carry flag: 0x00 = ACK, 0xFF = NACK
    ret

#endif /* TWI_HARDWARE */
