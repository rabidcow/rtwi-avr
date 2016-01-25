
# Define programs and commands.
CC = avr-gcc
CXX = $(CC)
AS = $(CC)
LD = $(CC)
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
NM = avr-nm
AVRDUDE = avrdude
CKSUM = cksum

OPTIMIZATION_FLAGS = -Os -fmerge-all-constants
# -fpack-struct generates spurious warnings for non-POD types and GCC seems to
# know that byte alignment is enough anyway.
COMMON_FLAGS = -ffunction-sections -fdata-sections -fshort-enums \
    -freg-struct-return
COMMON_WARNINGS = -Wall -Wpedantic -Wundef -Wtype-limits -Wwrite-strings \
    -Wempty-body -Wsign-compare -Wmissing-field-initializers
ASM_LISTING_FLAG = -Wa,-adhlns=$(addsuffix .lst,$(basename $@))

CFLAGS = -gdwarf-2 -DF_CPU=$(F_CPU)UL $(addprefix -D,$(DEFINE)) \
    $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS) -std=gnu99 $(COMMON_WARNINGS) \
    -Wstrict-prototypes $(ASM_LISTING_FLAG) $(addprefix -I,$(INCLUDE))

CXXFLAGS = -gdwarf-2 -DF_CPU=$(F_CPU)UL $(addprefix -D,$(DEFINE)) \
    $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS) -std=gnu++11 -fno-exceptions \
    -fno-rtti -fstrict-enums $(COMMON_WARNINGS) $(ASM_LISTING_FLAG) \
    $(addprefix -I,$(INCLUDE))

ASFLAGS = -DF_CPU=$(F_CPU) $(addprefix -D,$(DEFINE)) \
    $(ASM_LISTING_FLAG),-gstabs,--listing-cont-lines=100

LDFLAGS = -Wl,--relax -Wl,--gc-sections $(addprefix -l,$(LIBRARY))

# -j = include, -R = exclude
PROGRAM_SECTIONS = -R .eeprom -R .fuse -R .lock -R .signature
EEPROM_SECTIONS = -j .eeprom

EEPFLAGS = --set-section-flags=.eeprom="alloc,load" \
    --change-section-lma .eeprom=0 --no-change-warnings

GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d

ALL_CFLAGS   = -mmcu=$(MCU) $(CFLAGS) $(GENDEPFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) $(CXXFLAGS) $(GENDEPFLAGS)
# Need to specify language because make on Win32 ignores the case from the
# file system.
ALL_ASFLAGS  = -mmcu=$(MCU) $(ASFLAGS) $(GENDEPFLAGS) -x assembler-with-cpp

AVRDUDE_FLASH = $(AVRDUDE) -c usbtiny -p $(MCU) -U flash:w:out/$(1).bin
AVRDUDE_EEPROM = $(AVRDUDE) -c usbtiny -p $(MCU) -U eeprom:w:out/$(1).eep.bin

# Asm source from C.
%.s : %.c
	@echo $@ : $^
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Asm source from C++.
%.s : %.cpp
	@echo $@ : $^
	$(CXX) -S $(ALL_CXXFLAGS) $< -o $@

# Object file from C.
$(OBJDIR)/%.o : %.c
	@echo $@ : $^
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 

# Object file from C++.
$(OBJDIR)/%.o : %.cpp
	@echo $@ : $^
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

# Object file from asm with CPP.
$(OBJDIR)/%.o : %.S
	@echo $@ : $^
	$(AS) -c $(ALL_ASFLAGS) $< -o $@

# Intel hex version of the final program.
%.hex : %.elf
	@echo $@ : $^
	$(OBJCOPY) $(PROGRAM_SECTIONS) -O ihex $< $@

# Raw binary version of the final program.
%.bin : %.elf
	@echo $@ : $^
	$(OBJCOPY) $(PROGRAM_SECTIONS) -O binary $< $@
	$(CKSUM) $@

# Intel hex version of EEPROM section.
%.eep : %.elf
	@echo $@ : $^
	-$(OBJCOPY) $(EEPROM_SECTIONS) $(EEPFLAGS) -O ihex $< $@ || exit 0

# Raw binary version of EEPROM section.
%.eep.bin : %.elf
	@echo $@ : $^
	-$(OBJCOPY) $(EEPROM_SECTIONS) $(EEPFLAGS) -O binary $< $@ && || exit 0
	$(CKSUM) $@

# Asm listing of the final program.
%.lss : %.elf
	@echo $@ : $^
	$(OBJDUMP) -h -d -z $< > $@

# Symbol listing.
%.sym : %.elf
	@echo $@ : $^
	$(NM) -l $< > $@

# We generate an ELF file that contains everything split into sections,
# then pack sections together as necessary for burning.
# The dependencies for the .elf file determine what is actually built and
# linked for each example.
%.elf :
	@echo $@ : $^
	$(LD) $(ALL_CFLAGS) $^ -o $@ -Wl,-Map=$(@:%.elf=%.map),--cref $(LDFLAGS)

# Create binary files directory.
$(shell mkdir $(OUTDIR) 2>/dev/null)

# Create object files directory.
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
