CC=avr-gcc
OBJCOPY=avr-objcopy
CCPARAMS=-O1 -DF_CPU=8000000L -mmcu=atmega168
DUDE=avrdude
RM=rm

# The 3v3 pro minis are atmega168pa but the device codes on them suggest that they are atmega168
MCU=atmega168
PROGRAMMER_BAUD=19200
PROGRAMMER_TYPE=arduino #This works for my little Baite-009 ch340 device

CONSOLE_BAUD=9600

default: test2 async-uart.s

.PHONY: upload run clean

clean:
	$(RM) -f test1 test2 *.o test1.hex

async-uart.o: async-uart.c
	$(CC) $(CCPARAMS) -c -o $@ $^

async-uart.s: async-uart.c
	$(CC) $(CCPARAMS) -fverbose-asm -S -o $@ $^

test1.o: test1.c
	$(CC) $(CCPARAMS) -c -o $@ $^

test1: test1.o async-uart.o
	$(CC) -mmcu=$(MCU) $^ -o $@

test1.hex: test1
	$(OBJCOPY) -O ihex -R .eeprom $< $@

test2.o: test2.c
	$(CC) $(CCPARAMS) -c -o $@ $^

test2: test2.o async-uart.o
	$(CC) -mmcu=$(MCU) $^ -o $@

test2.hex: test2
	$(OBJCOPY) -O ihex -R .eeprom $< $@

upload: test2.hex
	$(DUDE) -F -V -c $(PROGRAMMER_TYPE) -p $(MCU) -P /dev/ttyUSB0 -b $(PROGRAMMER_BAUD) -U flash:w:$<

run: upload
	sleep 2
	minicom -8 -b $(CONSOLE_BAUD) -D /dev/ttyUSB0 
