#include <stdint.h>
#include <stdarg.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

// Include the header so "library" functions get extern C.
#include "ssd1306.h"

#define CHAR_COLUMN(BITS) \
    ( ((0xF0000000&0x##BITS) ? 0x01 : 0) \
    | ((0x0F000000&0x##BITS) ? 0x02 : 0) \
    | ((0x00F00000&0x##BITS) ? 0x04 : 0) \
    | ((0x000F0000&0x##BITS) ? 0x08 : 0) \
    | ((0x0000F000&0x##BITS) ? 0x10 : 0) \
    | ((0x00000F00&0x##BITS) ? 0x20 : 0) \
    | ((0x000000F0&0x##BITS) ? 0x40 : 0) \
    | ((0x0000000F&0x##BITS) ? 0x80 : 0) \
    )
#define CHAR(A,B,C,D,E) \
    CHAR_COLUMN(A), CHAR_COLUMN(B), CHAR_COLUMN(C), CHAR_COLUMN(D), CHAR_COLUMN(E)

const uint8_t PROGMEM ssd1306_chargen[96*(SSD1306_CHAR_WIDTH-1)] =
{
    CHAR(0000000, 0000000, 0000000, 0000000, 0000000), // space
    CHAR(0000000, 1111010, 0000000, 0000000, 0000000), // !
    CHAR(0000000, 1100000, 0000000, 1100000, 0000000), // "
    CHAR(0101000, 1111100, 0101000, 1111100, 0101000), // #
    CHAR(0100110, 1010010, 1111111, 1010010, 1001100), // $
    CHAR(1100000, 1100110, 0011000, 1100110, 0000110), // %
    CHAR(0101100, 1010010, 0101010, 0000100, 0001010), // &
    CHAR(0000000, 0000000, 1100000, 0000000, 0000000), // '
    CHAR(0000000, 0000000, 0111110, 1000001, 0000000), // (
    CHAR(0000000, 1000001, 0111110, 0000000, 0000000), // )
    CHAR(0000000, 0011100, 0011100, 0011100, 0000000), // *
    CHAR(0000000, 0001000, 0011100, 0001000, 0000000), // +
    CHAR(0000001, 0000010, 0000000, 0000000, 0000000), // ,
    CHAR(0000000, 0001000, 0001000, 0001000, 0000000), // -
    CHAR(0000000, 0000010, 0000000, 0000000, 0000000), // .
    CHAR(0000010, 0001100, 0110000, 1000000, 0000000), // /
    CHAR(0111100, 1000010, 1011010, 1000010, 0111100), // 0
    CHAR(0000000, 0100010, 1111110, 0000010, 0000000), // 1
    CHAR(0100010, 1000110, 1001010, 1010010, 0100110), // 2
    CHAR(0000100, 1000010, 1010010, 1110010, 1001100), // 3
    CHAR(0011000, 0101000, 1001010, 1111110, 0001010), // 4
    CHAR(0010100, 1110010, 1010010, 1010010, 1001100), // 5
    CHAR(0011100, 0110010, 1010010, 1010010, 0001100), // 6
    CHAR(1100000, 1001000, 1001110, 1111000, 1001000), // 7
    CHAR(0101100, 1010010, 1010010, 1010010, 0101100), // 8
    CHAR(0110000, 1001010, 1001010, 1001100, 0111000), // 9
    CHAR(0000000, 0100010, 0000000, 0000000, 0000000), // :
    CHAR(0000001, 0100010, 0000000, 0000000, 0000000), // ;
    CHAR(0000000, 0001000, 0010100, 0010100, 0100010), // <
    CHAR(0000000, 0010100, 0010100, 0010100, 0000000), // =
    CHAR(0100010, 0010100, 0010100, 0001000, 0000000), // >
    CHAR(0100000, 1000000, 1001010, 1010000, 0100000), // ?
    CHAR(0111100, 1000010, 1011010, 1001010, 0111000), // @
    CHAR(0000110, 0011010, 1101000, 0011010, 0000110), // A
    CHAR(1000010, 1111110, 1010010, 1010010, 0101100), // B
    CHAR(0111100, 1000010, 1000010, 1000010, 1100100), // C
    CHAR(1000010, 1111110, 1000010, 1000010, 0111100), // D
    CHAR(1000010, 1111110, 1010010, 1010010, 1000110), // E
    CHAR(1000010, 1111110, 1010010, 1010000, 1000000), // F
    CHAR(0111100, 1000010, 1010010, 1010100, 0011110), // G
    CHAR(1111110, 0010000, 0010000, 0010000, 1111110), // H
    CHAR(0000000, 1000010, 1111110, 1000010, 0000000), // I
    CHAR(0001100, 0000010, 0000010, 1000010, 1111100), // J
    CHAR(1000010, 1111110, 0010000, 1101110, 1000010), // K
    CHAR(1000010, 1111110, 1000010, 0000010, 0000110), // L
    CHAR(1111110, 0110000, 0001100, 0110000, 1111110), // M
    CHAR(1111110, 0100010, 0011000, 1000100, 1111110), // N
    CHAR(0111100, 1000010, 1000010, 1000010, 0111100), // O
    CHAR(1000010, 1111110, 1001010, 1001000, 0110000), // P
    CHAR(0111100, 1000010, 1001010, 1000100, 0111011), // Q
    CHAR(1000010, 1111110, 1001000, 1001110, 0110010), // R
    CHAR(0100110, 1010010, 1010010, 1010010, 0001100), // S
    CHAR(1100000, 1000010, 1111110, 1000010, 1100000), // T
    CHAR(1111100, 1000010, 0000010, 1000010, 1111100), // U
    CHAR(1100000, 1011000, 0000110, 1011000, 1100000), // V
    CHAR(1111000, 1000110, 0011000, 1000110, 1111000), // W
    CHAR(1000010, 1100110, 0011000, 1100110, 1000010), // X
    CHAR(1100000, 1010000, 0001110, 1010000, 1100000), // Y
    CHAR(1100010, 1000110, 1011010, 1100010, 1000110), // Z
    CHAR(0000000, 0000000, 1111111, 1000001, 0000000), // [
    CHAR(1000000, 0110000, 0001100, 0000010, 0000000), // backslash
    CHAR(0000000, 1000001, 1111111, 0000000, 0000000), // ]
    CHAR(0010000, 0100000, 1000000, 0100000, 0010000), // ^
    CHAR(0000001, 0000001, 0000001, 0000001, 0000001), // _
    CHAR(0000000, 1000000, 0100000, 0000000, 0000000), // `
    CHAR(0000100, 0101010, 0101010, 0011100, 0000010), // a
    CHAR(1000000, 1111110, 0010010, 0100010, 0011100), // b
    CHAR(0011100, 0100010, 0100010, 0100010, 0010100), // c
    CHAR(0001100, 0010010, 1010010, 1111110, 0000010), // d
    CHAR(0011100, 0101010, 0101010, 0101010, 0011000), // e
    CHAR(0010010, 0111110, 1010010, 1010000, 1000000), // f
    CHAR(0011000, 0100101, 0100101, 0010101, 0111110), // g
    CHAR(1000010, 1111110, 0010000, 0100000, 0011110), // h
    CHAR(0000000, 0010010, 1011110, 0000010, 0000000), // i
    CHAR(0000010, 0000001, 0010001, 1011110, 0000000), // j
    CHAR(1000010, 1111110, 0001000, 0010110, 0100010), // k
    CHAR(0000000, 1000010, 1111110, 0000010, 0000000), // l
    CHAR(0111110, 0100000, 0011110, 0100000, 0011110), // m
    CHAR(0100010, 0111110, 0010000, 0100000, 0011110), // n
    CHAR(0011100, 0100010, 0100010, 0100010, 0011100), // o
    CHAR(0000001, 0111111, 0100100, 0100100, 0011000), // p
    CHAR(0011000, 0100100, 0100100, 0111111, 0000001), // q
    CHAR(0100010, 0111110, 0010010, 0100000, 0110000), // r
    CHAR(0010000, 0101010, 0101010, 0101010, 0000100), // s
    CHAR(0100000, 1111100, 0100010, 0100010, 0000010), // t
    CHAR(0111100, 0000010, 0100100, 0111110, 0000010), // u
    CHAR(0110000, 0101100, 0000010, 0101100, 0110000), // v
    CHAR(0111000, 0000110, 0111000, 0000110, 0111000), // w
    CHAR(0100010, 0010100, 0001000, 0010100, 0100010), // x
    CHAR(0110001, 0101101, 0000010, 0101100, 0110000), // y
    CHAR(0110010, 0100110, 0101010, 0110010, 0100110), // z
    CHAR(0000000, 0001000, 0110110, 1000001, 0000000), // {
    CHAR(0000000, 0000000, 1111110, 0000000, 0000000), // |
    CHAR(0000000, 1000001, 0110110, 0001000, 0000000), // }
    CHAR(0010000, 0100000, 0110000, 0010000, 0100000), // ~
    CHAR(0000010, 0000000, 0000010, 0000000, 0000010), // DEL
};

uint8_t ssd1306_row, ssd1306_column, ssd1306_effect;

int ssd1306_putce(char c, uint8_t effect)
{
    ssd1306_write_char(ssd1306_row, ssd1306_column, c, effect);
//    if (effect & SSD1306_EFFECT_TALL)
//        ssd1306_write_char(ssd1306_row + 1, ssd1306_column, c, effect ^ SSD1306_EFFECT_TALL_ROW);
    ssd1306_column += 1;
    if (effect & SSD1306_EFFECT_WIDE)
        ssd1306_column += 1;
    if (ssd1306_column >= SSD1306_COLUMNS)
    {
        ssd1306_column = 0;
        ssd1306_row += 1;
//        if (effect & SSD1306_EFFECT_TALL)
//            ssd1306_row += 1;
        if (ssd1306_row >= SSD1306_ROWS)
            ssd1306_row = 0;
    }
    return c;
}

int ssd1306_putc(char c)
{
    return ssd1306_putce(c, ssd1306_effect);
}

void ssd1306_printf(const char * PROGMEM format, ...)
{
    va_list args;
    va_start(args, format);
    while (1)
    {
        char c = pgm_read_byte(format++);
        switch (c)
        {
        case 0:
            va_end(args);
            return;
#if 1
        case '\n':
            do
                ssd1306_putce(' ', ssd1306_effect &~ SSD1306_EFFECT_WIDE);
            while (ssd1306_column != 0);
            continue;
        case '\t':
            do
                ssd1306_putce(' ', ssd1306_effect &~ SSD1306_EFFECT_WIDE);
            while ((ssd1306_column & 3) != 0);
            continue;
        case '%':
            {
                uint8_t width = 0, precision = 0;
                uint8_t flags = 0;
                const uint8_t LEFT_JUSTIFY = 0x01;
                const uint8_t PAD_WITH_ZERO = 0x02;
                const uint8_t SIGN_ALWAYS = 0x04;
                const uint8_t SIGN_WITH_PLUS = 0x08;
                const uint8_t ALTERNATIVE_FORM = 0x10;
                const uint8_t SET_PRECISION = 0x20;
                const uint8_t LONG_DATA = 0x40;
            another_format_char:
                c = pgm_read_byte(format++);
                switch (c)
                {
                case '%':
                    break;
                case '-':
                    flags |= LEFT_JUSTIFY;
                    goto another_format_char;
                case '+':
                    flags |= SIGN_WITH_PLUS;
                    // fall through
                case ' ':
                    flags |= SIGN_ALWAYS;
                    goto another_format_char;
                case '0':
                    if (width == 0 && (flags & SET_PRECISION) == 0)
                    {
                        flags |= PAD_WITH_ZERO;
                        goto another_format_char;
                    }
                    // fall through
                case '1': case '2': case '3':
                case '4': case '5': case '6':
                case '7': case '8': case '9':
                    if (flags & SET_PRECISION)
                        precision = precision * 10 + (c - '0');
                    else
                        width = width * 10 + (c - '0');
                    goto another_format_char;
                case 'l': case 'z': case 'j': case 't':
                    flags |= LONG_DATA;
                    goto another_format_char;
                case 'h':
                    goto another_format_char;
                case '#':
                    flags |= ALTERNATIVE_FORM;
                    goto another_format_char;
                case '*':
                    if (flags & SET_PRECISION)
                        precision = va_arg(args, unsigned);
                    else
                        width = va_arg(args, unsigned);
                    goto another_format_char;
                case '.':
                    flags |= SET_PRECISION;
                    goto another_format_char;

                case 'c':
                    c = va_arg(args, int);
                    break;
                case 's': // string in program space
                    {
                        const char * PROGMEM p = va_arg(args, const char *);
                        const char * PROGMEM q = p;
                        while (width != 0 && (c = pgm_read_byte(q++)) != 0)
                            --width;
                        for (uint8_t side = 2*LEFT_JUSTIFY; side;)
                        {
                            side -= LEFT_JUSTIFY;
                            if ((flags & LEFT_JUSTIFY) != side)
                                while (width-- != 0)
                                    ssd1306_putc(' ');
                            else
                                while ((c = pgm_read_byte(p++)) != 0)
                                {
                                    if ((flags & SET_PRECISION) && !precision--)
                                        break;
                                    ssd1306_putc(c);
                                }
                        }
                    }
                    continue;
                case 'S': // string in data space
                    {
                        const char *p = va_arg(args, const char *);
                        const char * PROGMEM q = p;
                        while (width != 0 && (c = *q++) != 0)
                            --width;
                        for (uint8_t side = 2*LEFT_JUSTIFY; side;)
                        {
                            side -= LEFT_JUSTIFY;
                            if ((flags & LEFT_JUSTIFY) != side)
                                while (width-- != 0)
                                    ssd1306_putc(' ');
                            else
                                while ((c = *p++) != 0)
                                {
                                    if ((flags & SET_PRECISION) && !precision--)
                                        break;
                                    ssd1306_putc(c);
                                }
                        }
                    }
                    continue;
                case 'd': case 'i': // 16-bit signed integer
                case 'u': // 16-bit unsigned integer
                    {
                        uint16_t x = va_arg(args, unsigned);
                        uint8_t n = 0;
                        char buffer[6], *p = buffer;
                        if ((flags & SET_PRECISION) && precision == 0 && x == 0)
                            continue;
                        if (c != 'u')
                        {
                            if ((x & 0x8000) != 0)
                            {
                                x = -x;
                                ssd1306_putc('-');
                            }
                            else if (flags & SIGN_ALWAYS)
                            {
                                ssd1306_putc((flags & SIGN_WITH_PLUS) ? '+' : ' ');
                            }
                        }
                        while (x >= 10)
                        {
                            ++n;
                            if (width != 0)
                                --width;
                            *p++ = x % 10;
                            x = x / 10;
                        }
                        uint8_t d = x + '0';
                        for (uint8_t side = 2*LEFT_JUSTIFY; side;)
                        {
                            side -= LEFT_JUSTIFY;
                            if ((flags & LEFT_JUSTIFY) != side)
                                while (width-- > 1)
                                    ssd1306_putc((flags & PAD_WITH_ZERO) ? '0' : ' ');
                            else
                                while (1)
                                {
                                    ssd1306_putc(d);
                                    if (!n--)
                                        break;
                                    d = *--p + '0';
                                }
                        }
                    }
                    continue;
                case 'p':
                    c = 'X';
                    width = 2 * sizeof(void *);
                    if (sizeof(void *) > sizeof(unsigned))
                        flags |= LONG_DATA;
                    flags |= PAD_WITH_ZERO;
                case 'x':
                case 'X':
                    {
                        uint32_t x;
                        uint8_t n = 0;
                        char buffer[9], *p = buffer;
                        if (flags & LONG_DATA)
                            x = va_arg(args, unsigned long);
                        else
                            x = va_arg(args, unsigned);
                        if ((flags & SET_PRECISION) && precision == 0 && x == 0)
                            continue;
                        if (x != 0 && (flags & ALTERNATIVE_FORM))
                        {
                            ssd1306_putc('0');
                            ssd1306_putc(c);
                        }
                        while (x >= 16)
                        {
                            ++n;
                            if (width != 0)
                                --width;
                            *p++ = x % 16;
                            x = x / 16;
                        }
                        c = c - ('X' - 'A') - (10 + '0');
                        uint8_t d = x + '0';
                        for (uint8_t side = 2*LEFT_JUSTIFY; side;)
                        {
                            side -= LEFT_JUSTIFY;
                            if ((flags & LEFT_JUSTIFY) != side)
                                while (width-- > 1)
                                    ssd1306_putc((flags & PAD_WITH_ZERO) ? '0' : ' ');
                            else
                                while (1)
                                {
                                    if (d > '9')
                                        d += c;
                                    ssd1306_putc(d);
                                    if (!n--)
                                        break;
                                    d = *--p + '0';
                                }
                        }
                    }
                    continue;
                }
            }
            break;
#endif
        }
        ssd1306_putc(c);
    }
}
