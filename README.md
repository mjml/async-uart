# async-uart.c

A little module that provides asynchronous serial i/o over an atmel chip's USART device.

This is tested with an ATmega168 chip on an Arduino Pro Mini 3v3 clone.

Note that using the module implies ISR vector handlers for UDRE_vect and RX_vect.

# Usage

Usage is simple: call async_uart_puts() and/or async_uart_gets().

Each of these methods asks for a character buffer that they perform asynchronous i/o with.
When you need to block on the result, call wait_uart_send_ready() or wait_uart_recv_ready(),
  or you can poll with is_uart_sending() and is_uart_recv_ready().
After the wait function returns, you can call the async function again.

The gets/recv version will place characters in a buffer you provide, up until CR,LF,or EOF.
The resulting string will contain the CR or LF char, and will be null-terminated.
So if you provide a buffer of length 128, the most you get is a string of length 127.

If you want different terminators, you really have to just go in and alter the interrupt service routine
  by hand and have it respond to those, sorry -- this was sufficient for my purposes and I don't want
	to make it more general than it needs to be.

# Examples

Take a look at test1.c for a send-only example, or at test2.c for a simple text-echoing example.

# Caveats

Doesn't check for frame error, parity error, or data overrun.
So get things right the first time!

@author: Michael Joya [mike@michaeljoya.com]
