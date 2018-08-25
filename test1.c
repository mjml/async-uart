#define F_CPU 8000000L

#include <util/delay.h>
#include <stdio.h>
#include "async-uart.h"
#include <avr/io.h>

int main ()
{
	init_async_uart(9600);

	int x = 0;
	char bufr[128];

	DDRB |= (1<<DDB5);
	
	
	while (1) {
		snprintf(bufr, 128, "I can count! Look: %d\r\n", ++x);
		async_uart_puts(bufr,128);
		wait_uart_send_ready();
	}
	
}
