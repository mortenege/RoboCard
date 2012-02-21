/****************************************************************************
# RoboCard Simple Scheduler example
# Copyright (c) 2010 Kjeld Jensen <kjeld@robocard.dk>
# 
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved. This file is offered as-is,
# without any warranty.
*****************************************************************************
# File: main.c                                              
# Project: RoboCard Simple Scheduler                                        
# Platform: RoboCard v1.0 http://www.robocard.dk                                                
# Microcontroller: ATmega88PA                                                      
# Author: Kjeld Jensen <kjen@robocard.dk>
# Created: 2010-08-09                                               
# Last modified: 2010-08-09                                          
****************************************************************************/
/* includes */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "SLIP/slip.h"
#include "UART/uart.h"
#include <stdlib.h>
#include "PWM/pwm.h"
#include "motorCTRL/motorCTRL.h"

/***************************************************************************/
/* defines */

/* defines for the timer0 interrupt */
/* calculation example
   INT_20MS_CNT = 16000000 [osc/s] / 256 [osc/cnt] / 50 [1/s] = 1250 [cnt] */

#define F_OSC 			16000000
#define INT0_CNT		256
#define F_20MS			50 /* frequency at 20ms period */
#define INT_20MS_CNT	(F_OSC / INT0_CNT / F_20MS) 	
/* SLIP Communication defines */
#define SLIP_HEADER_SIZE 3 	// length, status/error/flags msg, checksum value
#define SLIP_DATA_SIZE 3	// This is the maximum size
#define SLIP_BUFFER_SIZE (SLIP_HEADER_SIZE + SLIP_DATA_SIZE)
/***************************************************************************/
/* global and static variables */

/* timer0 and scheduler variables */
unsigned short timer0_count;
volatile unsigned char t20ms;
unsigned char t100ms;
unsigned char t500ms;

/* user interface variables*/
char led_stat = 0;
/* SLIP buffers and booleans */
char* slip_rx_buffer;
char* slip_tx_buffer;
uint8_t slip_received = 0;

/***************************************************************************/
void timer_init(void)
{
	/* timer 0 interrupt init */
	timer0_count = 0;
	t20ms = 0;
	t100ms = 0;
	t500ms = 0;
	TCCR0A = 0; /* normal mode (count to 255) */
	TCCR0B = (1<<CS00); /* clk_io (no prescaling) */
	TIMSK0 = (1<<OCIE0A); /* enable the Timer/Counter0 compare match A interrupt */
}
/***************************************************************************/
ISR (TIMER0_COMPA_vect)
{
	/* update timer0 cycle counter */
	if (timer0_count != INT_20MS_CNT)
		timer0_count++;
	else
	{
		timer0_count = 1;
		t20ms ++;
	}
}
/***************************************************************************/
void led(char on)
{
	if (on)
		PORTB &= ~(1<<PB0); /* set PB0 output to GND turning led on */
	else
		PORTB |= (1<<PB0); /* set PB0 output to VCC turning led off */
}
/***************************************************************************/
void led_init(void)
{
	led (0); /* make sure the led is off */
	DDRB |= (1<<DDB0); /* enable PB0 (led pin) as output */
}
/***************************************************************************
 * SLIP_TASK: Is run as often as possible to collect all incoming UART chars
 */
void slip_task(void){
	if (uart_rxc()){			// If uart chart is received
		char c = uart_rx_ch();
		if (slip_add_to_packet(slip_rx_buffer, c, SLIP_BUFFER_SIZE)){
			// SLIP package received
			// THIS COULD BE SET TO RUN IN A SEPERATE TIMING!!!
			int8_t len = slip_unwrap_data(slip_rx_buffer, SLIP_DATA_SIZE);
			if (len > 0 && len <= SLIP_BUFFER_SIZE){
				// This is the sending command
				slip_wrap_data(slip_rx_buffer, 0x01, len, SLIP_HEADER_SIZE);
				// Control the PWM signals
				motion_control((int8_t*)slip_rx_buffer, len);
			}
			slip_clear_buffer(slip_rx_buffer, SLIP_BUFFER_SIZE);
			// END OF THIS COULD BE SE... ...IMING!!!
			slip_received = 1;
		}
	}
}
/***************************************************************************
 * ctrl_task: Checks the data from the buffer and acts accordingly. Controls the PWM signals
 */
void ctrl_task(void){
	if (slip_received){				// This should be at another frequency
		slip_received = 0;
		// Unwrap the input msg
//		int8_t len = slip_unwrap_data(slip_rx_buffer, SLIP_DATA_SIZE);
//		uart_tx_str((uint8_t*)"len = slip_unwrap:");
//		uart_tx_ch(len);
//		if (len > 0 && len <= SLIP_BUFFER_SIZE){
//			//uart_tx_str((uint8_t*)"SENDING BACK...\n");
//			// This is the sending command
//			//slip_wrap_data(slip_rx_buffer, 0x01, len, SLIP_HEADER_SIZE);
//			motion_control((int8_t*)slip_rx_buffer, len);
//		}
//		slip_clear_buffer(slip_rx_buffer, SLIP_BUFFER_SIZE);

	}else{
		// Post a timeout message
		slip_received = 0;
		//slip_wrap_data("ERR", 0xFF, SLIP_DATA_SIZE, SLIP_HEADER_SIZE);
	}
}
/***************************************************************************/
void task_500ms (void)
{
	/* insert stuff to do each 500ms here */

	/* update the led */
	if (led_stat == 0) /* flip led state */
		led_stat = 1;
	else
		led_stat = 0;
	led(led_stat); /* update led state */
}
/***************************************************************************/
void task_100ms (void)
{

	/* insert stuff to do each 100ms here */
	ctrl_task();
	/* task timer housekeeping */
	t500ms++;
	if (t500ms == 5) /* if it is 500ms since last time */
	{
		task_500ms (); /* run the 500ms task */
		t500ms = 0;
	}
}
/***************************************************************************/
void task_20ms (void)
{
	
	/* insert stuff to do each 20ms here */
	//slip_task();
	
	/* task timer housekeeping */
	t100ms++;
	if (t100ms == 5) /* if it is 100ms since last time */
	{
		task_100ms(); /* run the 100ms task */
		t100ms = 0;
	}
}
/***************************************************************************/
int main(void)
{
	timer_init(); 	/* initialize the scheduler timer */
	led_init(); 	/* initialize led */
	init_uart();	/* initialize uart*/
	init_pwm();
	init_m_one();
	init_m_two();
	//init_slip(SLIP_BUFFER_SIZE); /* initialize slip protocol*/// Deprecated!!!

	/* Initialize two buffers */
	slip_rx_buffer = (char*)malloc(sizeof(char)*SLIP_BUFFER_SIZE);
	slip_tx_buffer = (char*)malloc(sizeof(char)*SLIP_BUFFER_SIZE);
	sei();
	for (;;) /* go into an endless loop */
	{
		//uart_tx_str((uint8_t*) "THIS IS SPARTA!!");
		/* handle 20ms tasks */
		if (t20ms != 0) /* if the interrupt has timed out after 20ms */
		{
			t20ms --; 
			task_20ms(); /* run the 20ms task */
		}
		slip_task();
	}
	return 0; /* just for the principle as we never get here */
}
/***************************************************************************/
