
include config.mk

SRC_RTWI = src/rtwi.S
ifdef USE_HARDWARE_TWI
SRC_I2CMASTER = compare/i2cmaster/twimaster.c
else
SRC_I2CMASTER = compare/i2cmaster/i2cmaster.S
endif

SRC_CLOCK_ANY = examples/clock.c examples/ssd1306_common.c examples/led.c

.PHONY : clock_rtwi
clock_rtwi : summary $(OUTDIR)/clock_rtwi.bin
	@echo ""
	@cksum $(OUTDIR)/clock_rtwi.bin
SRC_CLOCK_RTWI = $(SRC_CLOCK_ANY) examples/clock_rtwi.c examples/ds3231_rtwi.c examples/ssd1306_rtwi.c $(SRC_RTWI)
$(OUTDIR)/clock_rtwi.elf : $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SRC_CLOCK_RTWI))))

.PHONY : clock_i2cmaster
clock_i2cmaster : summary $(OUTDIR)/clock_i2cmaster.bin
	@echo ""
	@cksum $(OUTDIR)/clock_i2cmaster.bin
SRC_CLOCK_I2CMASTER = $(SRC_CLOCK_ANY) examples/clock_i2cmaster.c examples/ds3231_i2cmaster.c examples/ssd1306_i2cmaster.c $(SRC_I2CMASTER)
$(OUTDIR)/clock_i2cmaster.elf : $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SRC_CLOCK_I2CMASTER))))

.PHONY : clock_compat
clock_compat : summary $(OUTDIR)/clock_compat.bin
	@echo ""
	@cksum $(OUTDIR)/clock_compat.bin
SRC_CLOCK_COMPAT = $(SRC_CLOCK_ANY) examples/clock_compat.c examples/ds3231_compat.c examples/ssd1306_compat.c $(SRC_RTWI)
$(OUTDIR)/clock_compat.elf : $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SRC_CLOCK_COMPAT))))

.PHONY : clock
clock : clock_rtwi clock_i2cmaster clock_compat

# Configuration based on commercial development boards.
# If you're not using one of these, be sure to define MCU and LED_CONFIG.
ifeq ($(BOARD),teensy2)
# Teensy 2.0
MCU = atmega32u4
LED = D6
else ifeq ($(BOARD),teensy2pp)
# Teensy++ 2.0
MCU = at90usb1286
LED = D6
else ifeq ($(BOARD),trinket)
# Adafruit Trinket
MCU = atmega328
LED = B1
else ifeq ($(BOARD),protrinket)
# Adafruit Pro Trinket
MCU = attiny85
LED = B5
endif

SPLIT_PIN_DEF = $(subst 7, 7,$(subst 6, 6,$(subst 5, 5,$(subst 4, 4,$(subst 3, 3,$(subst 2, 2,$(subst 1, 1,$(subst 0, 0,$(1)))))))))
PIN_CONFIG_1 = $(1)_PORT=PORT$(2) $(1)_DDR=DDR$(2) $(1)_PIN=PIN$(2) $(1)_BIT=$(3)
# $(call PIN_CONFIG,BLAH,A0) -> BLAH_PORT=PORTA BLAH_DDR=DDRA BLAH_PIN=PINA BLAH_BIT=0
PIN_CONFIG = $(call PIN_CONFIG_1,$(1),$(word 1,$(call SPLIT_PIN_DEF,$(2))),$(word 2,$(call SPLIT_PIN_DEF,$(2))))

LED_CONFIG = $(call PIN_CONFIG,LED,$(LED))

ifdef USE_HARDWARE_TWI
GPIO_CONFIG = $(LED_CONFIG) TWI_HARDWARE
else
GPIO_CONFIG = $(LED_CONFIG) $(TWI_CONFIG) $(I2C_CONFIG)
endif

ifdef USE_INTERNAL_PULLUP
PULLUP_CONFIG = TWI_INTERNAL_PULLUP
else
PULLUP_CONFIG =
endif

DEFINE  := $(GPIO_CONFIG) $(PULLUP_CONFIG) F_TWI=$(F_TWI)
INCLUDE := src
LIBRARY := m

include avr.mk

# Create object files directories.
$(shell mkdir $(OBJDIR)/src 2>/dev/null)
$(shell mkdir $(OBJDIR)/examples 2>/dev/null)
$(shell mkdir $(OBJDIR)/compare 2>/dev/null)
$(shell mkdir $(OBJDIR)/compare/i2cmaster 2>/dev/null)

.PHONY : summary

summary :
	@echo "Configuration:"
	@echo "    MCU: $(MCU)"
	@echo "    F_CPU: $(F_CPU)"
	@echo "    Debug: LED=$(LED)"
ifdef USE_HARDWARE_TWI
	@echo "    TWI: hardware (TWI)"
else
	@echo "    TWI: $(TWI_CONFIG)"
endif
	@echo ""
