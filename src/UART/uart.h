/*
 * uart.h
 *
 *  Created on: 08/08/2011
 *      Author: Morten Ege Jensen
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define FCPU 16000000
#define BAUD_RATE(desired) (FCPU/(16*desired))-1
/* 1200
 * 2400
 * 4800
 * 9600
 * 19200
 * 38400
 * 57600
 * 115200
 */

void init_uart(void);
void uart_tx_ch(char);
void uart_tx_str(uint8_t*);
uint8_t uart_rx_ch();
uint8_t uart_rxc();

#endif /* UART_H_ */
