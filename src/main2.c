/*
 * main.c
 *
 *  Created on: Feb 7, 2012
 *      Author: ege
 */

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
//#include "CPU/CPU.h"
#include "UART/uart.h"
#include "SLIP/slip.h"
#include "include.h"

#define TIME 100000
#define SLIP_BUFFER_SIZE 8
int8_t clk_tick;


char* slip_rx_buffer;
char* slip_tx_buffer;

void init_led(){
	 DDRB |= (1<<DDB0);               //PB0 output
}

void blink_led(){
	PORTB ^= (1<<PB0);				// toggle LED
}

/**
 * This will be the deprecated version
 */
/*
void ctrl_task(){
	static int8_t state = CTRL_STATE_IDLE;
	uint8_t uart_rx = 0;
	// Check receiving status
	uint8_t uart_rxc_temp = uart_rxc();
	if (uart_rxc_temp) {			// If UART signal buffer receive
		uart_rx = uart_rx_ch();	// Run the control task with new char
	}
	switch (state){
	case CTRL_STATE_IDLE:
		if (uart_rxc_temp && uart_rx == 0x01){
			state = CTRL_STATE_SEND;
		}
		break;
	case CTRL_STATE_SEND:
		uart_tx_str((uint8_t*) "Message Received\r\n");
		state = CTRL_STATE_IDLE;
		break;
	default:
		break;
	}
}
*/

void ctrl_task(uint8_t ch){
	uint8_t motor = 0;
	if((ch & (1<<MOTOR_CHECK)))
		motor = MOTOR_RIGHT;
	else
		motor = MOTOR_LEFT;

	uint8_t value = ch & ~(1<<MOTOR_CHECK);
	switch(motor){
	case MOTOR_LEFT:
		uart_tx_ch(value);
		uart_tx_str((uint8_t*) "LEFT\r\n");
		break;
	case MOTOR_RIGHT:
		uart_tx_ch(value);
		uart_tx_str((uint8_t*) "RIGHT\r\n");
		break;
	default:
		break;
	}
}

int main_old(){
	cli();
	// Initialize System
	//init_cpu();
	init_led();
	init_uart();
	init_slip(SLIP_BUFFER_SIZE);
	sei();
	// Transmit construct confirmation
	uart_tx_str((uint8_t*) "\r\nProgram initialized...\r\n");
	long cnt = 0;
	// SLIP buffers
	slip_rx_buffer = (char*)malloc(sizeof(char)*SLIP_BUFFER_SIZE);
	slip_tx_buffer = (char*)malloc(sizeof(char)*SLIP_BUFFER_SIZE);
	// Super Loop
	while(1){
			// Blink LED loop
			if (cnt>=TIME){
				cnt = 0;
				blink_led();
			}
			cnt++;
			// Uart task
			if (uart_rxc()){			// If uart buffer is full
				char c = uart_rx_ch();
				if (slip_add_to_packet(slip_rx_buffer, c, SLIP_BUFFER_SIZE)){
					uint8_t len = slip_retrieve_packet();
					uart_tx_ch(len);
					slip_send_packet(slip_rx_buffer, len);
					slip_clear_buffer(slip_rx_buffer, len);
				}
			}
	}

	return 0;
}
