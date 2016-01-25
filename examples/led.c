#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "led.h"
#include "rdelay.h"

/*
Blink a pattern on the debug LED.

pattern: a bit sequence to indicate the blink pattern (MSB first)
times: number of times to repeat the pattern
*/
void blink(uint8_t pattern, int8_t times)
{
    while (times != 0)
    {
        uint8_t x = pattern;
        // Start with the debug LED on.
        LED_PORT |= 1 << LED_BIT;
        while (1)
        {
            mdelay(50);

            LED_PORT &= ~(1 << LED_BIT);
            if (x & 0x80)
                LED_PORT |= 1 << LED_BIT;
            else if (x == 0)
                break;
            x <<= 1;
        }
        // End with the debug LED off.
        LED_PORT &= ~(1 << LED_BIT);

        if (times >= 1)
        {
            mdelay(600);
            times -= 1;
        }
    }
}

#define MORSE_BITS(CODE) \
    ( ((0x20000000&0x##CODE) ? 0x8000 : 0) \
    | ((0x10000000&0x##CODE) ? 0x4000 : 0) \
    | ((0x02000000&0x##CODE) ? 0x2000 : 0) \
    | ((0x01000000&0x##CODE) ? 0x1000 : 0) \
    | ((0x00200000&0x##CODE) ? 0x0800 : 0) \
    | ((0x00100000&0x##CODE) ? 0x0400 : 0) \
    | ((0x00020000&0x##CODE) ? 0x0200 : 0) \
    | ((0x00010000&0x##CODE) ? 0x0100 : 0) \
    | ((0x00002000&0x##CODE) ? 0x0080 : 0) \
    | ((0x00001000&0x##CODE) ? 0x0040 : 0) \
    | ((0x00000200&0x##CODE) ? 0x0020 : 0) \
    | ((0x00000100&0x##CODE) ? 0x0010 : 0) \
    | ((0x00000020&0x##CODE) ? 0x0008 : 0) \
    | ((0x00000010&0x##CODE) ? 0x0004 : 0) \
    | ((0x00000002&0x##CODE) ? 0x0002 : 0) \
    | ((0x00000001&0x##CODE) ? 0x0001 : 0) \
    )
#define MORSE(CODE) \
    ( MORSE_BITS(CODE) << \
        ( (0x##CODE >= 0x10000000) ?  0 \
        : (0x##CODE >= 0x01000000) ?  2 \
        : (0x##CODE >= 0x00100000) ?  4 \
        : (0x##CODE >= 0x00010000) ?  6 \
        : (0x##CODE >= 0x00001000) ?  8 \
        : (0x##CODE >= 0x00000100) ? 10 \
        : (0x##CODE >= 0x00000010) ? 12 \
        : (0x##CODE >= 0x00000001) ? 14 \
        : 0 \
        ) \
    )

static const uint16_t PROGMEM morse_code[] =
{
    0,              // space
    MORSE(313133),  // !
    MORSE(131131),  // "
    0,              // #
    MORSE(1113113), // $
    0,              // %
    MORSE(13111),   // &
    MORSE(133331),  // '
    MORSE(31331),   // (
    MORSE(313313),  // )
    0,              // *
    MORSE(13131),   // +
    MORSE(331133),  // ,
    MORSE(311113),  // -
    MORSE(131313),  // .
    MORSE(31131),   // /
    MORSE(33333),   // 0
    MORSE(13333),   // 1
    MORSE(11333),   // 2
    MORSE(11133),   // 3
    MORSE(11113),   // 4
    MORSE(11111),   // 5
    MORSE(31111),   // 6
    MORSE(33111),   // 7
    MORSE(33311),   // 8
    MORSE(33331),   // 9
    MORSE(333111),  // :
    MORSE(313131),  // ;
    0,              // <
    MORSE(31113),   // =
    0,              // >
    MORSE(113311),  // ?
    MORSE(133131),  // @
    MORSE(13),      // A
    MORSE(3111),    // B
    MORSE(3131),    // C
    MORSE(311),     // D
    MORSE(1),       // E
    MORSE(1131),    // F
    MORSE(331),     // G
    MORSE(1111),    // H
    MORSE(11),      // I
    MORSE(1333),    // J
    MORSE(313),     // K
    MORSE(1311),    // L
    MORSE(33),      // M
    MORSE(31),      // N
    MORSE(333),     // O
    MORSE(1331),    // P
    MORSE(3313),    // Q
    MORSE(131),     // R
    MORSE(111),     // S
    MORSE(3),       // T
    MORSE(113),     // U
    MORSE(1113),    // V
    MORSE(133),     // W
    MORSE(3113),    // X
    MORSE(3133),    // Y
    MORSE(3311),    // Z
};

#define MORSE_UNIT 50

void blink_morse(char letter)
{
    if (letter >= 'a' && letter <= 'z')
        letter -= 'a' - 'A';
    if (letter < ' ' || letter > 'Z')
        return;

    uint16_t code = pgm_read_word(morse_code + letter - ' ');
    if (code == 0) // space
        mdelay(MORSE_UNIT);
    while (code != 0)
    {
        LED_PORT |= 1 << LED_BIT;
        while (code >= 0x4000)
        {
            code -= 0x4000;
            mdelay(MORSE_UNIT);
        }
        code <<= 2;
        LED_PORT &= ~(1 << LED_BIT);
        mdelay(MORSE_UNIT);
    }
    mdelay(3*MORSE_UNIT);
}
