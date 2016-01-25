# rtwi
##### A tiny I²C master library for AVR.

## What is it?

rtwi is a software (bit-bang) implementation of the I²C interface, operating as
a bus master, for AVR microcontrollers.

An implementation using dedicated TWI hardware is also included as an option.

## Comparison with other libraries

Peter Fleury's i2cmaster.S (v. 1.12) and twimaster.c (v. 1.3) fit the exact
same role, so they make a good comparison. There are also both software and
hardware implementations (including slave support) in avrlib, but I couldn't
get them to compile with avr-gcc 4.9.2.

Feature                       | rtwi (SW) | rtwi (HW) | i2cmaster.S | twimaster.c
------------------------------|-----------|-----------|-------------|-------------
supported hardware            | any       | TWI       | any         | TWI
license                       | MIT       | MIT       | GPL         | GPL
I²C master                    | YES       | YES       | YES         | YES
I²C slave                     | no        | no        | no          | no
any two IO pins               | YES       | no        | YES         | no
can split SDI and SDO         | YES       | no        | no          | no
simple clock speed selection¹ | YES       | YES       | no          | YES
can change speed at run time  | no        | no        | no          | YES
clock stretching (as master)  | YES       | YES       | YES         | YES
multiple master               | no        | no        | no          | YES
implementation language       | asm       | asm       | asm         | C
code size² (bytes)            | 92        | 58        | 180         | 172

1. i2cmaster.S requires you to adjust a half-cycle delay function to set the
clock speed. rtwi and twimaster.c convert the desired clock frequency (eg, 100
kHz) into the appropriate delay automatically.

2. With default options, CPU clock at 4 MHz, and I²C clock targetting 100 kHz.
`i2c_start_wait` not included.

## Basic Usage

A typical "write 1-byte register" command looks like this:

    twi_start_write(SLAVE_ADDRESS);
    twi_send_byte(SLAVE_REGISTER);
    twi_send_byte(value);
    twi_stop_condition();

And a typical "read 1-byte register" looks like this:

    twi_start_write(SLAVE_ADDRESS);
    twi_send_byte(SLAVE_REGISTER);
    twi_start_read(SLAVE_ADDRESS);
    value = twi_recv_last();
    twi_stop_condition();

## API

### twi_start_condition
> `void twi_start_condition(void);`

Set the start or repeated start condition on the bus and take SCL low in
preparation for the first data bit. This must be called at the beginning of
each packet sent or recieved to alert slave devices and take control of the
bus. You also need to send a repeated start condition when switching from
sending to receiving.

### twi_send_byte
> `uint8_t twi_send_byte(uint8_t value);`

Sends a data byte on the bus. The return value indicates whether or not a slave
acknowledged this byte; zero for ACK, nonzero for NACK. Exact return values are
available as `TWI_SEND_ACK` and `TWI_SEND_NACK`.

### twi_start_send / twi_start_write / twi_start_read
> `uint8_t twi_start_send(uint8_t value);`  
> `uint8_t twi_start_write(uint8_t address);`  
> `uint8_t twi_start_read(uint8_t address);`

`twi_start_condition` followed by `twi_send_byte`, optionally with
`TWI_WRITE_TO` or `TWI_READ_FROM` mixed in.

### twi_recv_byte
> `uint8_t twi_recv_byte(uint8_t ack);`

Receives a data byte on the bus and acknowledge. The `ack` parameter is either
`TWI_RECV_MORE` (ACK) or `TWI_RECV_LAST` (NACK) to indicate whether or not this
will be the last byte requested in the packet.

### twi_recv_more / twi_recv_last
> `uint8_t twi_recv_more(void);`  
> `uint8_t twi_recv_last(void);`

Saving you the trouble of passing a parameter.

### twi_stop_condition
> `void twi_stop_condition(void);`

Set the stop condition on the bus and release control of it. You should do this
whenever you're done talking to a slave device.

### twi_init
> `void twi_init(void)`

For software mode, this is a macro that sets up the output and direction
registers for the configured IO pins. Ideally you would take the logic for this
and roll it into whatever constants you use to initialize other pins on the
same port, but this provides a handy shortcut.

For hardware mode, this is a function to configure the TWI hardware. You could
do it in your own code, but there's not much benefit. The header file
calculates the relevant constants to use as `TWI_TWBR` and `TWI_TWPS`, if you
really want.

### TWI_ADDRESS
> `TWI_ADDRESS(BITS)`

This macro converts a 7-bit address in binary into an 8-bit constant. This way
there's no ambiguity as to whether the address includes the R/W flag for anyone
reading your code.

It will also convert a 10-bit address in binary into a 16-bit constant, which
would need to be split and sent as two bytes. (Big-endian, so that no 10-bit
address constant will be equal to any 7-bit address constant.)

For example:

    #define ADDRESS TWI_ADDRESS(0101101)
    twi_start_write(ADDRESS);

will send the byte `0x5A`. Alternatively, you could
`#define ADDRESS TWI_7BIT(0x2D)` or `TWI_8BIT(0x5A)` if you want clarity
without binary.

    #define ADDRESS TWI_ADDRESS(1000111001)
    twi_start_write(ADDRESS >> 8);
    twi_send_byte(ADDRESS & 0xFF);
    twi_start_read(ADDRESS >> 8);

will send `0xFC` followed by `0x39`, then a repeated start and `0xFD`. (Reads
with 10-bit addressing are especially awkward.)

## Compilation Options

These are macros that you can define in your Makefile to configure the library.

### Basic

#### F_TWI

Set the frequency that the I²C bus should operate at.

The achieved bus speed will be lower than your requested `F_TWI` value,
primarily due to CPU cycles that can't be completely accounted for, but a
little comes from clock-stretching code waiting for the clock to rise to Vih.

You can attempt to correct for unaccounted CPU cycles by increasing `F_TWI`
above the bus speed you actually want, but if you set it above 100 kHz then
minimum timing requirements for standard-mode I²C may not be honored. And this
can only remove delay cycles; at some point the only cycles left are those
needed to drive the protocol. Expect a maximum clock rate of about `F_CPU`/40.

Default is `100000` &mdash; 100 kHz.

#### TWI_SCL

Set the location of the clock line. To make it easier to configure, this should
be defined as the port letter and number, as if they're separate parameters to
a function, like `(C, 4)`. The space is optional, but you need the comma and
parentheses. If you define it in a Makefile, you'll probably need to quote it.

Default is `(B, 0)` &mdash; pin B0.

#### TWI_SDA

Set the location of the data line.

Default is `(B, 1)` &mdash; pin B1.

#### TWI_HARDWARE

If defined, use the dedicated TWI hardware. This won't compile if your target
microcontroller has no such hardware. Hardware mode ignores all other
configuration except for `F_TWI`.

It's theoretically possible to use the USI hardware, but it's pretty awkward
for I²C, to the point that it'd only make sense if you really need speed
approaching half the CPU clock. I don't, so I didn't bother.

### Advanced

The I²C bus is composed of two bi-directional lines, but in rare cases (eg,
AN10658) it can be useful to split it into uni-directional lines. This is
trivial to do in software, so why not?

#### TWI_SDO

Set the location of the uni-directional output line.

Default is to use SDA.

#### TWI_SDI

Set the location of the uni-directional input line.

Default is to use SDA.

#### TWI_BUSY

Specify a separate busy line instead of using clock stretching to detect when
a slave is busy.

Default is to use SCL.

#### TWI_TRISE

This allows you to specify the expected rise time (in nanoseconds) to get
slightly more accurate bus timing. If you know your bus capacitance and pull-up
resistor values then you can just calculate it, but you probably don't know
your bus capacitance, so you'd need to measure it. Or you could wildly guess
`400` for 4.7 kΩ pull-up resistors.

What you'd want to measure is the time it takes for the signal to rise from
30% to 70% of Vcc, partly because that's a value that's limited by the I²C
spec (1000 ns for standard mode aka 100 kHz and 300 ns in fast mode.), but also
because the clock stretching code will wait for the signal to rise to Vih,
which is typically 70% of Vcc.

Default is `0`, because setting this too high will make delays shorter than the
required minimums.

#### TWI_INTERNAL_PULLUP

Don't use this. I didn't know what to expect when writing this code and it
turns out that the resulting rise times are *way* out of spec. It's not going
to work very well (maybe not at all) and I'll probably remove it in the future.

Use external pull-up resistors. 4.7 kΩ is usually a decent value.

## Extras

In order to get more precise timing, `rdelay.h` includes delay functions that
allow a number of "already spent" CPU cycles to be subtracted, with exact and
size-optimized variants. To go with this, `cpu.h` defines macros for the number
of cycles taken by each instruction supported on the target CPU.

There's an exact equivalent to everything in i2cmaster except for
`i2c_start_wait`, so you could just change all the names in code written for
that and it'll just work (on rare occasion, the real API can be more
efficient). A header file is provided that defines everything for this.
