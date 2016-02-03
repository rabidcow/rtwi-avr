# rtwi
##### A tiny I²C master library for AVR.

## What is it?

rtwi is a software (bit-bang) implementation of the I²C interface, operating as
a bus master, for AVR microcontrollers.

An implementation using dedicated TWI hardware is also included as an option.

[More documentationy and stuff.](doc/rtwi.md)

## Why is it interesting?

* It's *very* **small**. The code itself is about a hundred bytes and inline
  assembly in the header file allows the compiler to avoid unnecessarily saving
  and restore registers around it.
* It's **easy to configure**. Define a constant for your clock rate, all the
  appropriate delays are computed for you.
* It supports some weird configurations. Not only can you assign SCL and SDA to
  any two pins, you can split SDA into SDI and SDO and separate BUSY from SCL.
* It's **MIT licensed**. Want to use this in a closed-source product so you can
  shave a few cents off by using a part with less flash? Please do.

## How do I use it?

Add `rtwi.S`, `rtwi.h`, `rdelay.h`, and `cpu.h` to your project. If you want to
use it as a drop-in replacement for i2cmaster.S, also add `i2cmaster.h` from
the [compat](src/compat) directory. `#include` either `rtwi.h` or `i2cmaster.h` in
source files that will be using the library.

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

Want API docs? [Here's some API docs.](doc/rtwi.md#api)
