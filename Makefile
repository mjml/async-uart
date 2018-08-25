CC=avr-g++
OBJCOPY=avr-objcopy
CCPARAMS=-Os -DF_CPU=8000000L -mmcu=atmega168p -c -std=gnu99 -Iavr-softuart
DUDE=avrdude
RM=rm

# The 3v3 pro minis are atmega168pa but the device codes on them suggest that they are atmega168
MCU=atmega168
PROGRAMMER_BAUD=19200
PROGRAMMER_TYPE=arduino #This works for my little Baite-009 ch340 device

CONSOLE_BAUD=9600

default: test1

clean:
	$(RM) -f test1 *.o test1.hex

test1.o: test1.c
	$(CC) $(CCPARAMS) -o $@ $^

async-uart.o: async-uart.c
	$(CC) $(CCPARAMS) -o $@ $^

test1: test1.o async-uart.o
	$(CC) -mmcu=$(MCU) $^ -o $@

test1.hex: test1
	$(OBJCOPY) -O ihex -R .eeprom $< $@

@upload: test1.hex
	$(DUDE) -F -V -c $(PROGRAMMER_TYPE) -p $(MCU) -P /dev/ttyUSB0 -b $(PROGRAMMER_BAUD) -U flash:w:test1.hex

@run: @upload
	sleep 2
	minicom -8 -b $(CONSOLE_BAUD) -D /dev/ttyUSB0 
