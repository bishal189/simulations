MCU     = atmega32
F_CPU   = 8000000
TARGET  = digital_thermometer

CC      = avr-gcc
OBJCOPY = avr-objcopy
SIZE    = avr-size

CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os -Wall
LDFLAGS = -mmcu=$(MCU)

all: $(TARGET).hex

$(TARGET).elf: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	$(SIZE) --format=avr $<

clean:
	rm -f $(TARGET).elf $(TARGET).hex

.PHONY: all clean
