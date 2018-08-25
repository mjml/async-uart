
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

static char* uart_tx_fifo = nullptr;
static char* uart_tx_fifo_end = nullptr;
static char* uart_rx_fifo = nullptr;
static char* uart_rx_fifo_end = nullptr;

bool rx_rd = false;

void init_async_uart (int baud)
{
	cli();
	
	unsigned int ubrr = (F_CPU / 16 / baud) - 1;
	
	// baud rate selector
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// frame configuration: one stop bit, no parity (this could be parameterized)
	// rx enable, tx enable
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
  sei();
}

void one_tx (char c)
{
	UDR0 = c;
}

char one_rx ()
{
	return UDR0;
}

bool is_uart_sending () {
	// For some reason, we need to read from UDRE0 in this loop otherwise
	// the interrupt vector handler never seems to properly reset it.
	char chh = (UCSR0A & (1 << UDRE0));
	return (uart_tx_fifo < uart_tx_fifo_end);
}

bool is_uart_receiving () {
	return uart_rx_fifo < uart_rx_fifo_end;
}

bool is_uart_recv_ready () {
	return rx_rd;
}

void wait_uart_send_ready()
{
	bool led = true;
	while (is_uart_sending());
}

void wait_uart_recv_ready()
{
	while (is_uart_receiving() && rx_rd) {}
}

void async_uart_puts (char* buf, int n)
{
	uart_tx_fifo = buf;
	uart_tx_fifo_end = buf + strlen(buf);
	UCSR0B |= (1<<UDRIE0);
}

void async_uart_gets (char* buf, int n)
{
	uart_rx_fifo = buf;
	uart_rx_fifo_end = buf + n;
	rx_rd = false;
	UCSR0B |= (1<<RXCIE0);
}

ISR(USART_UDRE_vect)
{
	char c = 0;
	if (uart_tx_fifo < uart_tx_fifo_end) {
		c = *uart_tx_fifo++;
		UDR0 = c;
	} else {
		UCSR0B &= ~(1<<UDRIE0);
	}
}

ISR(USART_RX_vect)
{
	char c = 0;
	c = UDR0;
	if (uart_rx_fifo < uart_rx_fifo_end && c != 0) {
		*(uart_rx_fifo++) = c;
		rx_rd = (c != '\n');
	}
	if (c==0 || !rx_rd) {
		UCSR0B &= ~(1<<RXCIE0);
	}
}

