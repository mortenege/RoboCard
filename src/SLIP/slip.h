/*
 * slip.h
 *
 *  Created on: Feb 8, 2012
 *      Author: ege
 */

#ifndef SLIP_H_
#define SLIP_H_
#include "../UART/uart.h"
#include <stdint.h>

/* SLIP special character codes - in Decimal    */
#define END             192    /* indicates end of packet 			0300*/
#define ESC             219    /* indicates byte stuffing 			0333*/
#define ESC_END         220    /* ESC ESC_END means END data byte 	0334*/
#define ESC_ESC         221    /* ESC ESC_ESC means ESC data byte 	0335*/
// Functions
void slip_send_packet(char* p, uint8_t len); /* This function should be private */
uint8_t slip_add_to_packet(char* p,char c, uint8_t len);
uint8_t slip_retrieve_packet();
void slip_clear_buffer(char* p, uint8_t len);
void init_slip(uint8_t len);
int8_t slip_unwrap_data(char* buffer, uint8_t data_len);
void slip_wrap_data(char* data_buffer, char flags, uint8_t data_len, uint8_t header_size);
/**
 * STRUCT: NOT IN USE YET
 */
typedef struct{
	char len;
	char* data;
	char flags;
	char checksum;
} slip_msg;

#endif /* SLIP_H_ */
