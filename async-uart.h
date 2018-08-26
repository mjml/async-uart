/** 
    A little library that allows for asynchronous serial communication over an arduino's TX0/RX0 uart interface.
		
		Note that the inclusion of the library implies ISR vector handlers for UDRE_vect and RX_vect.
		
 */

#define RX_TOKEN_DELIM '\n'

void init_async_uart (int baud);
void async_uart_puts (char* buf, int n);
void async_uart_gets (char* buf, int n);

char is_uart_sending ();
char is_uart_recv_ready ();
void wait_uart_send_ready();
void wait_uart_recv_ready();
