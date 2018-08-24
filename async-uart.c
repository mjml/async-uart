
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

static char* uart_tx_fifo;
static char* uart_tx_fifo_end;
static char* uart_rx_fifo;
static char* uart_rx_fifo_end;

static bool rx_ln = false;

void init_async_uart(int baud, FILE* pstream)
{
	cli();
	
	unsigned int ubrr = (F_CPU / 16 / baud) - 1;
	
	// baud rate selector
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// frame configuration: one stop bit, no parity (this could be parameterized)
	UCSROC = (1<<USBS0)
	// rx interrupt enable, tx data register read interrupt enable, rx enable, tx enable
	UCSR0B = (1<<RXCIE0) | (1<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0);
	
  sei();
}

void one_tx (char c)
{
	TXB0 = c;
}

char one_rx ()
{
	return RXB0;
}


void async_uart_puts (const char* str, int n)
{
	uart_tx_fifo = str;
	uart_tx_fifo_end = str + n;
	if (UCSR0A & (1<<UDRE0)) {
		one_tx(str[0]);
		uart_tx_fifo++;
	}
}


void async_uart_gets (const char* fifo, int n)
{
	uart_rx_fifo = fifo;
	uart_rx_fifo_end = fifo + n;
	rx_ln = false;
}

ISR(USART_UDRE_vect)
{
	one_tx(*uart_tx_fifo++);
}

ISR(USART_RX_vect)
{
	
}
