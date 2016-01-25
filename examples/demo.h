#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED

/* Some utilities to cut out boilerplate that makes the demos more hardware
   agnostic. */

/* Not including <avr/io.h> because I want that to be explicit in the source
   files. */
#include "led.h"

#define DEMO_F_CPU_SCALE (8000000LU / F_CPU)
/* Fuse should be set to use internal 8 MHz oscillator. Set clock divider based
   on F_CPU, which must be 8000000 divided by a small power of 2:
   8000000, 4000000, 2000000, 1000000, 500000, 250000, 125000, 62500, 31250 */
#if DEMO_F_CPU_SCALE == 1
#   define DEMO_CLKPR_AUTO() do{ CLKPR = 0x80; CLKPR = 0; }while(0)
#elif DEMO_F_CPU_SCALE == 2
#   define DEMO_CLKPR_AUTO() do{ CLKPR = 0x80; CLKPR = 1; }while(0)
#elif DEMO_F_CPU_SCALE == 4
#   define DEMO_CLKPR_AUTO() do{ CLKPR = 0x80; CLKPR = 2; }while(0)
#elif DEMO_F_CPU_SCALE == 8
#   define DEMO_CLKPR_AUTO() do{ CLKPR = 0x80; CLKPR = 3; }while(0)
#elif DEMO_F_CPU_SCALE == 16
#   define DEMO_CLKPR_AUTO() do{ CLKPR = 0x80; CLKPR = 4; }while(0)
#elif DEMO_F_CPU_SCALE == 32
#   define DEMO_CLKPR_AUTO() do{ CLKPR = 0x80; CLKPR = 5; }while(0)
#elif DEMO_F_CPU_SCALE == 64
#   define DEMO_CLKPR_AUTO() do{ CLKPR = 0x80; CLKPR = 6; }while(0)
#elif DEMO_F_CPU_SCALE == 128
#   define DEMO_CLKPR_AUTO() do{ CLKPR = 0x80; CLKPR = 7; }while(0)
#elif DEMO_F_CPU_SCALE == 256
#   define DEMO_CLKPR_AUTO() do{ CLKPR = 0x80; CLKPR = 8; }while(0)
#else
#   define DEMO_CLKPR_AUTO() _Pragma("error \"Unsupported F_CPU value.\"")
#endif

/* Set all unused pins to input with pull-up. */
#define DEMO_SET_UP_PORT(X) do{ DDR##X = 0x00; PORT##X = 0xFF; }while(0)

/* Automatically set up all ports for whatever device we're using. */
#ifdef PORTA
#   define DEMO_SET_UP_PORTA() DEMO_SET_UP_PORT(A)
#else
#   define DEMO_SET_UP_PORTA()
#endif
#ifdef PORTB
#   define DEMO_SET_UP_PORTB() DEMO_SET_UP_PORT(B)
#else
#   define DEMO_SET_UP_PORTB()
#endif
#ifdef PORTC
#   define DEMO_SET_UP_PORTC() DEMO_SET_UP_PORT(C)
#else
#   define DEMO_SET_UP_PORTC()
#endif
#ifdef PORTD
#   define DEMO_SET_UP_PORTD() DEMO_SET_UP_PORT(D)
#else
#   define DEMO_SET_UP_PORTD()
#endif
#ifdef PORTE
#   define DEMO_SET_UP_PORTE() DEMO_SET_UP_PORT(E)
#else
#   define DEMO_SET_UP_PORTE()
#endif
#ifdef PORTF
#   define DEMO_SET_UP_PORTF() DEMO_SET_UP_PORT(F)
#else
#   define DEMO_SET_UP_PORTF()
#endif
#ifdef PORTG
#   define DEMO_SET_UP_PORTG() DEMO_SET_UP_PORT(G)
#else
#   define DEMO_SET_UP_PORTG()
#endif
#ifdef PORTH
#   define DEMO_SET_UP_PORTH() DEMO_SET_UP_PORT(H)
#else
#   define DEMO_SET_UP_PORTH()
#endif
#ifdef PORTJ
#   define DEMO_SET_UP_PORTJ() DEMO_SET_UP_PORT(J)
#else
#   define DEMO_SET_UP_PORTJ()
#endif
#define DEMO_SET_UP_ALL_PORTS() do{ \
        DEMO_SET_UP_PORTA(); \
        DEMO_SET_UP_PORTB(); \
        DEMO_SET_UP_PORTC(); \
        DEMO_SET_UP_PORTD(); \
        DEMO_SET_UP_PORTE(); \
        DEMO_SET_UP_PORTF(); \
        DEMO_SET_UP_PORTG(); \
        DEMO_SET_UP_PORTH(); \
        DEMO_SET_UP_PORTJ(); \
    }while(0)

/* After setting all pins to the default state, set up LED pin. */
#define DEMO_SET_UP_LED() do{ LED_PORT &= ~(1 << LED_BIT); LED_DDR |= 1 << LED_BIT; }while(0)

#endif /* DEMO_H_INCLUDED */
