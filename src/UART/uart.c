/*
 * uart.c
 *
 *  Created on: 08/08/2011
 *      Author: Morten Ege Jensen
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

uint8_t rx_tick = 0;
int8_t rx_data = 0;
uint8_t rx_buffer[10];
uint8_t rx_buffer_ptr = 0;

void init_uart(void){
	/* 8 bit, no parity, 1 stop bit BDRate 115200*/
	DDRD |= (1<<PD0)|(1<<PD1);
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0); // Enable RX and TX
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	UBRR0 = 25; // 16MHz, 38400 bdRate. For any other config use scale

	// Setup for interrupt
	UCSR0B |= (1<<RXCIE0);	// Enable interrupt on Receive complete

}

void uart_tx_ch(char ch){
	while (!(UCSR0A & (1<<UDRE0)));	// Wait for empty buffer
	UDR0 = ch;
}

void uart_tx_str(uint8_t* str){
	while (*str){
		uart_tx_ch(*str);
		str++;
	}
	//uart_tx_ch('\r');
	//uart_tx_ch('\n');
}

uint8_t uart_rx_ch(){
	rx_tick = 0;
	return rx_data; // return the content of the buffer
}

uint8_t uart_rxc(){
	uint8_t return_value = 0;
	if (rx_tick){
		return_value = 1;
	}
	return return_value;
}
/*	//NOT COMPLETED
void uart_task(){
	uint8_t rx_temp = uart_rx_ch();
	if (rx_temp){
		rx_buffer[rx_buffer_ptr++] = rx_temp;
	}
}
*/

ISR(USART_RX_vect){
	rx_tick = 1;
	rx_data = UDR0;
}



