/*

Copyright (c) 2015 Joshua Seagoe

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

#ifndef RDELAY_H_INCLUDED
#define RDELAY_H_INCLUDED

#ifndef F_CPU
#   error "F_CPU not defined."
#endif

/*
    F_CPU   ns/cycle
    10  MHz 100
    12  MHz 83.3
    12.8MHz 78.1
    15  MHz 66.6
    16  MHz 62.5
    16.5MHz 60.6
    18  MHz 55.5
    20  MHz 50.0
    25  MHz 40.0
*/
#define CPU_NS_PER_CYCLE (1000000000 / (F_CPU))

#ifndef __ASSEMBLER__

#include <avr/builtins.h>
#include <stdint.h>

// Delay as close as possible (rounded up) to exactly the specified delay.
// Note that as a minimum granularity, each CPU cycle will be 40 - 100+ ns.

static inline void cycle_delay_exact(uint32_t cycles) __attribute__((always_inline));
static inline void ndelay_exact(double ns, uint32_t cycles_already) __attribute__((always_inline));
static inline void udelay_exact(double us, uint32_t cycles_already) __attribute__((always_inline));
static inline void mdelay_exact(double ms, uint32_t cycles_already) __attribute__((always_inline));

// Possibly delay a bit longer than requested to get smaller code.

static inline void cycle_delay_small(uint32_t cycles) __attribute__((always_inline));
static inline void ndelay_small(double ns, uint32_t cycles_already) __attribute__((always_inline));
static inline void udelay_small(double us, uint32_t cycles_already) __attribute__((always_inline));
static inline void mdelay_small(double ns, uint32_t cycles_already) __attribute__((always_inline));

static inline void ndelay(double ns) __attribute__((always_inline));
static inline void udelay(double us) __attribute__((always_inline));
static inline void mdelay(double ms) __attribute__((always_inline));

static inline void ndelay(double ns) { ndelay_exact(ns, 0); }
static inline void udelay(double us) { udelay_exact(us, 0); }
static inline void mdelay(double ms) { mdelay_small(ms, 0); }

void cycle_delay_exact(uint32_t cycles)
{
    __builtin_avr_delay_cycles(cycles);
}

void cycle_delay_small(uint32_t cycles)
{
    if (cycles < 3)
    {
        __builtin_avr_delay_cycles(cycles);
    }
    else if (cycles <= 3*0x100)
    {
        uint8_t i;
        __asm__ volatile (
            "ldi %0,%1\n\t"
            "1:dec %0\n\t"
            "brne 1b\n\t"
            : "=r" (i) : "M" ((cycles + 1) / 3)
        );
    }
    else if (cycles <= 4*0x100)
    {
        // slightly longer loops to avoid using an additional register, but not at the expense of space
        uint8_t i;
        __asm__ volatile (
            "ldi %0,%1\n\t"
            "1:dec %0\n\t"
            "nop\n\t"
            "brne 1b\n\t"
            : "=r" (i) : "M" ((cycles + 2) / 4)
        );
    }
    else if (cycles <= 5*0x100)
    {
        uint8_t i;
        __asm__ volatile (
            "ldi %0,%1\n\t"
            "1:dec %0\n\t"
            "rjmp .\n\t"
            "brne 1b\n\t"
            : "=r" (i) : "M" ((cycles + 3) / 5)
        );
    }
    else if (cycles <= 4*0x10000)
    {
        uint16_t i;
        __asm__ volatile (
            "ldi %A0,lo8(%1)\n\t"
            "ldi %B0,hi8(%1)\n\t"
            "1:sbiw %0,1\n\t"
            "brne 1b\n\t"
            : "=w" (i) : "n" ((cycles + 2) / 4)
        );
    }
    else if (cycles <= 5*0x10000)
    {
        uint16_t i;
        __asm__ volatile (
            "ldi %A0,lo8(%1)\n\t"
            "ldi %B0,hi8(%1)\n\t"
            "1:sbiw %0,1\n\t"
            "nop\n\t"
            "brne 1b\n\t"
            : "=w" (i) : "n" ((cycles + 3) / 5)
        );
    }
    else if (cycles <= 6*0x10000)
    {
        uint16_t i;
        __asm__ volatile (
            "ldi %A0,lo8(%1)\n\t"
            "ldi %B0,hi8(%1)\n\t"
            "1:sbiw %0,1\n\t"
            "rjmp .\n\t"
            "brne 1b\n\t"
            : "=w" (i) : "n" ((cycles + 4) / 6)
        );
    }
    else if (cycles <= 7*0x10000)
    {
        uint16_t i;
        __asm__ volatile (
            "ldi %A0,lo8(%1)\n\t"
            "ldi %B0,hi8(%1)\n\t"
            "1:sbiw %0,1\n\t"
            "rjmp .\n\t"
            "nop\n\t"
            "brne 1b\n\t"
            : "=w" (i) : "n" ((cycles + 5) / 7)
        );
    }
    else if (cycles <= 8*0x10000)
    {
        uint16_t i;
        __asm__ volatile (
            "ldi %A0,lo8(%1)\n\t"
            "ldi %B0,hi8(%1)\n\t"
            "1:sbiw %0,1\n\t"
            "rjmp .\n\t"
            "rjmp .\n\t"
            "brne 1b\n\t"
            : "=w" (i) : "n" ((cycles + 6) / 8)
        );
    }
    else if (cycles <= 5*0x1000000)
    {
        const uint32_t count = (cycles + 2) / 5;
        uint16_t i;
        uint8_t j;
        __asm__ volatile (
            "ldi %A0,lo8(%2)\n\t"
            "ldi %B0,hi8(%2)\n\t"
            "ldi %1,%3\n\t"
            "1:sbiw %0,1\n\t"
            "sbci %1,0\n\t"
            "brne 1b\n\t"
            : "=w" (i), "=r" (j) : "n" (count & 0xFFFF), "M" (count >> 16)
        );
    }
    else
    {
        __builtin_avr_delay_cycles(cycles);
    }
}

void ndelay_exact(double ns, uint32_t cycles_already)
{
    if (ns <= CPU_NS_PER_CYCLE)
        return;

    const uint32_t cycles = (uint32_t) (((F_CPU) * ns + 999999999) / 1e9) - cycles_already;
    cycle_delay_exact(cycles);
}

void ndelay_small(double ns, uint32_t cycles_already)
{
    if (ns <= CPU_NS_PER_CYCLE)
        return;

    const uint32_t cycles = (uint32_t) (((F_CPU) * ns + 999999999) / 1e9) - cycles_already;
    cycle_delay_small(cycles);
}

void udelay_exact(double us, uint32_t cycles_already)
{
    if (us <= 0)
        return;

    const uint32_t cycles = (uint32_t) (((F_CPU) * us + 999999) / 1e6) - cycles_already;
    cycle_delay_exact(cycles);
}

void udelay_small(double us, uint32_t cycles_already)
{
    ndelay_small(1000 * us, cycles_already);
}

void mdelay_exact(double ms, uint32_t cycles_already)
{
    udelay_exact(1000 * ms, cycles_already);
}

void mdelay_small(double ms, uint32_t cycles_already)
{
    udelay_small(1000 * ms, cycles_already);
}

#else /* __ASSEMBLER__ */

#ifndef DELAY_RLO
#   define DELAY_RLO r26
#endif
#ifndef DELAY_RHI
#   define DELAY_RHI r27
#endif

.macro cycle_delay_small cycles:req, rlo=DELAY_RLO, rhi=DELAY_RHI
    .if \cycles <= 0
    .elseif \cycles == 1
        nop
    .elseif \cycles == 2
        rjmp .
    .elseif \cycles == 3
        nop
        rjmp .
    .elseif \cycles == 4
        rjmp .
        rjmp .
    .elseif \cycles == 5
        nop
        rjmp .
        rjmp .
    .elseif \cycles == 6
        rjmp .
        rjmp .
        rjmp .
    .elseif \cycles <= 3*256
        ldi \rlo, lo8((\cycles + 2) / 3)
1:      dec \rlo
        brne 1b
    .elseif \cycles <= 4*256
        ldi \rlo, lo8((\cycles + 3) / 4)
1:      dec \rlo
        nop
        brne 1b
    .elseif \cycles <= 5*256
        ldi \rlo, lo8((\cycles + 4) / 5)
1:      dec \rlo
        rjmp .
        brne 1b
    .elseif \cycles <= 4*65536
        ldi \rlo, lo8((\cycles + 3) / 4)
        ldi \rhi, hi8((\cycles + 3) / 4)
1:      subi \rlo, 1
        sbci \rhi, 0
        brne 1b
    .else
        .error "cycle_delay_small: Delay too large."
    .endif
.endm

// Number of CPU cycles needed for the specified delay.
#define NS_DELAY_CYCLES(NS, CYCLES_ALREADY) (((((F_CPU) / 1000) * (NS) + 999999) / 1000000) - (CYCLES_ALREADY))

.macro ndelay ns:req, cycles_already=0
    // Sacrifice accuracy with weird F_CPU values for really long delays.
    // For all values, assume CPU frequency is an integral multiple of 1 kHz.
    // 0x7FF0BDC1 = 0x80000000 - 999999
    .if \ns < (0x7FF0BDC1 / ((F_CPU) / 1000))
        // To avoid overflowing signed 32-bit math in NS_DELAY_CYCLES:
        //     F_CPU =  1MHz: NS <= 2147483
        //     F_CPU =  8MHz: NS <=  268310
        //     F_CPU = 12MHz: NS <=  178873
        //     F_CPU = 16MHz: NS <=  134155
        //     F_CPU = 20MHz: NS <=  107324
        cycle_delay_small NS_DELAY_CYCLES(\ns, \cycles_already)
    .elseif \ns < (0x7FFE7961 / ((F_CPU) / 10000))
        cycle_delay_small (((((F_CPU) / 10000) * \ns + 99999) / 100000) - \cycles_already)
    .elseif \ns < (0x7FFFD8F1 / ((F_CPU) / 100000))
        cycle_delay_small (((((F_CPU) / 100000) * \ns + 9999) / 10000) - \cycles_already)
    .else
        // More than this requires additional blocks in cycle_delay_small.
        .error "ndelay: Delay too large."
    .endif
.endm

#endif /* __ASSEMBLER__ */

#endif /* RDELAY_H_INCLUDED */
