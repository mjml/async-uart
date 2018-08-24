CC=avr-gcc
OBJCOPY=avr-objcopy
CCPARAMS=-Os -DF_CPU=8000000L -mmcu=atmega168p -c -std=gnu99 -Iavr-softuart
DUDE=avrdude
MCU=atmega168p
RM=rm

default: test1

clean:
	$(RM) -f test1 *.o test1.hex

test1.o: test1.c
	$(CC) $(CCPARAMS) -o $@ $^

async-uart.o: async-uart.c
	$(CC) $(CCPARAMS) -o $@ $^

test1: test1.o
	$(CC) -mmcu=$(MCU) $^ -o $@

test1.hex: test1
	$(OBJCOPY) -O ihex -R .eeprom $< $@

@upload: test1.hex
	$(DUDE) -F -V -c arduino -p $(MCU) -P /dev/ttyUSB0 -b 57600 -U flash:w:test1.hex

@run: @upload
	sleep 2
	minicom -8 -b 9600 -D /dev/ttyUSB0 
