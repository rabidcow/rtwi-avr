#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/*
Blink a pattern on the debug LED.

pattern: a bit sequence to indicate the blink pattern (MSB first)
    The pattern always starts with the LED on and ends with it off. You can
    think of the pattern as having an extra 1 bit at the beginning.
    For example:
        0x00 = (1)00000000: one 20 ms flash
        0x80 = (1)10000000: one 40 ms flash
        0xC0 = (1)11000000: one 60 ms flash
        0xE0 = (1)11100000: one 80 ms flash
        0xFF = (1)11111111: one 180 ms flash
        0x40 = (1)01000000: two 20 ms flashes
        0xB0 = (1)10110000: two 40 ms flashes
        0xDC = (1)11011100: two 60 ms flashes
        0xEF = (1)11101111: two 80 ms flashes
        0x50 = (1)01010000: three 20 ms flashes
        0xB6 = (1)10110110: three 40 ms flashes
        0x54 = (1)01010100: four 20 ms flashes
        0x55 = (1)01010101: five 20 ms flashes
        0xEA = (1)11101010: one 80 ms flash followed by two 20 ms flashes
        0x77 = (1)01110111: one 20 ms flash followed by two 60 ms flashes

times: number of times to repeat the pattern: 0 to 127, or -1 to loop forever
*/
void blink(uint8_t pattern, int8_t times);

void blink_morse(char letter);

#ifdef __cplusplus
}
#endif

#endif /* LED_H_INCLUDED */
