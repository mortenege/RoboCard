#include "slip.h"
#include <stdlib.h>
#include <stdio.h>

/* Boolean defines */
#define TRUE  1
#define FALSE  0
/* SLIP state machine defines */
#define SLIP_STATE_IDLE 	0
#define SLIP_STATE_ESC 		1
#define SLIP_STATE_DATA		2
/* SLIP unwrapping defines */
#define SLIP_WRAP_LEN		1
#define SLIP_WRAP_DATA		2
#define SLIP_WRAP_FLAGS		3
#define SLIP_WRAP_CHECK		4
#define SLIP_WRAP_DONE		5
#define SLIP_WRAP_FAIL_CS	-1

//char* slip_packet;
/* Slip pointer */
uint8_t slip_pos = 0;

// DEPRECATED FUNCTION
void init_slip(uint8_t len){
//	slip_packet = (char*)malloc(sizeof(char)*len);
}

/********************************************************************
 * SLIP_SEND_PACKET: Sends the packet pointed to by p. The packet is encapsulated in SLIP's END character
 * return: none
 */
void slip_send_packet(char* p, uint8_t len){
	uart_tx_ch(END);
	while(len--){
		switch(*p){
		case END:
			uart_tx_ch(ESC);
			uart_tx_ch(ESC_END);
			break;
		case ESC:
			uart_tx_ch(ESC);
			uart_tx_ch(ESC_ESC);
			break;
		default:
			uart_tx_ch(*p);
			break;
		}
		p++;
	}
	uart_tx_ch(END);
}
/**
 * SLIP_ADD_TO_PACKET: adds char c to packet p (if this is SLIP standard)
 * return: true/false if an entire packet has been received
 */
uint8_t slip_add_to_packet(char* p, char c, uint8_t len){
	static uint8_t state = SLIP_STATE_IDLE;

	switch (state){
	case SLIP_STATE_IDLE:
		switch(c){
		case END:
			if (slip_pos)
				return TRUE;
			else
				return FALSE;
		case ESC:
			state = SLIP_STATE_ESC;
			break;
		default:
			state = SLIP_STATE_DATA;
			break;
		}
	case SLIP_STATE_ESC:
		switch (c){
		case ESC_ESC:
			c = ESC;
			state = SLIP_STATE_DATA;
			break;
		case ESC_END:
			c = END;
			state = SLIP_STATE_DATA;
			break;
		}
	case SLIP_STATE_DATA:
		if (slip_pos < len)
			p[slip_pos++] = c;
		else{
			state = SLIP_STATE_IDLE;
			return FALSE; // In case of overflow
		}
		//uart_tx_str((uint8_t*) "ADDED CH\n");
		state = SLIP_STATE_IDLE;
		break;
	default:
		state = SLIP_STATE_IDLE;
		break;
	}

	return FALSE;
}

// DEPRECATED
uint8_t slip_retrieve_packet(){
	int return_length = slip_pos;

	return return_length;
}

/**********************************************************
 * SLIP_CLEAR_BUFFER: Clears the buffer p for length len
 */
void slip_clear_buffer(char* p, uint8_t len){
	memset((char*)p,'0', len);
	slip_pos = 0;
}
/**********************************************************
 * SLIP_UNWRAP_DATA: Unwrappes the data from packet buffer, and writes the data bytes back
 * return: number of bytes in new packet
 * ATTENTION: This function is NOT finished. More to come on flags and errors etc.
 */
int8_t slip_unwrap_data(char* buffer, uint8_t data_len){
	// This method unwraps a full length input buffer and leaves the data
	char* new_buffer = (char*)malloc(sizeof(char)*data_len);
	uint8_t ptr = 0, len = 0;
	uint8_t checksum = 0, buffer_checksum = 0;

	//uart_tx_str((uint8_t*)"slip_unwrap_data ENTERED!\n");
	// Read in the first byte and check the length
	if (buffer[ptr] <= data_len)
		len = buffer[ptr];	// TODO: else: Throw an ERROR MESSAGE - OVERFLOW EXPECTED
	else{
		// The data will not fit in the buffer. Return!
		//uart_tx_str((uint8_t*)"Length failed!\n");
		return -4;
	}
	ptr++;
	// Read len bytes from the second byte
	//uart_tx_str((uint8_t*)"slip_unwrap_data DATA ENTERED!\n");
	for (uint8_t i = 0; i < len; i++){
		new_buffer[i] = buffer[ptr++];
	}
	// Process the the flags byte
	//uart_tx_str((uint8_t*)"slip_unwrap_data FLAGS ENTERED!\n");
	// ATTENTION
	ptr++;
	// Process the checksum byte
	//uart_tx_str((uint8_t*)"slip_unwrap_data CHECKSUM ENTERED!\n");
	for (uint8_t i = 0; i < ptr; i++){
		checksum += buffer[i];
	}
	buffer_checksum = buffer[ptr];
	if (buffer_checksum != checksum){
		// Error!!!
		//uart_tx_str((uint8_t*)"CHECKSUM failed1\n");
		return -8;	// ATTENTION HERE
	}
	ptr++;
	// Copy over the new buffer to the original buffer
	//uart_tx_str((uint8_t*)"slip_unwrap_data DONE ENTERED!\n");
	memset((char*)buffer,'0',len);
	memcpy(buffer, new_buffer, len);
	free(new_buffer);
	// Return
	return len; // Returns from the only break possible
}

/********************************************************************
 * SLIP_WRAP_DATA: Packs the data in data_buffer with designed header of flags, len and checksum
 * The function sends of the packet once it is wrapped
 * return: none
 */
void slip_wrap_data(char* data_buffer, char flags, uint8_t data_len, uint8_t header_size){
	// Wrap the data in a header package
	uint8_t buffer_size = header_size + data_len;
	char* packet = (char*)malloc(sizeof(char)*buffer_size);
	memset((char*)packet, '0', buffer_size);

	uint8_t ptr = 0;
	uint8_t checksum = 0;
	packet[ptr++] = data_len;
	for (uint8_t i = 0; i < data_len; i++){
		packet[ptr++] = data_buffer[i];
		//checksum += data_buffer[i];
	}
	// Add header
	packet[ptr++] = flags;
	for (uint8_t i = 0; i < ptr; i++){
		checksum += packet[i];
	}
	packet[ptr] = checksum;
	// Send packet
	slip_send_packet(packet, buffer_size);
	free(packet);
}
