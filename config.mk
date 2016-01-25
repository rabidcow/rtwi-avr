
# BOARD is a shortcut for defining the MCU and LED port so you don't have to look them up.
# The main Makefile understands:
#   teensy2     Teensy 2.0
#   teensy2pp   Teensy++ 2.0
#   trinket     Adafruit Trinket
#   protrinket  Adafruit Pro Trinket
#BOARD = teensy2

# MCU should be GCC's -mmcu value for your target microcontroller.
# See https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html
MCU = attiny44a
#MCU = atmega32u4
# The demo expects a debug LED on one port.
LED = A2

# Define how your I2C bus is connected.
TWI_CONFIG = TWI_SCL='(A,1)' TWI_SDA='(A,0)' TWI_TRISE=450
I2C_CONFIG = SDA_PORT=PORTA SCL_PORT=PORTA SDA=0 SCL=1
# Hardware I2C uses the hardware SDA and SCL pins instead.
#USE_HARDWARE_TWI = YES
# Using the internal pull-up resistors makes for a simpler circuit, but aren't the best values.
#USE_INTERNAL_PULLUP = YES

# CPU frequency in Hz.
F_CPU = 8000000
# I2C bus frequency in Hz.
F_TWI = 100000

OBJDIR = obj
OUTDIR = out
