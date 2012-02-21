/*
 * state_machine.c
 *
 *  Created on: Feb 13, 2012
 *      Author: ege
 */

#include "motorCTRL.h"
#include "../UART/uart.h"

#define ST_IDLE		0
#define ST_ONE		1
#define ST_TWO		2
#define ST_POS1		-1
#define ST_POS2		-2

#define MOTOR_ONE	1
#define MOTOR_TWO	2

void state_machine(int8_t ch){
	static int8_t state = ST_IDLE;
	switch(state){
	case ST_IDLE:
		uart_tx_str((uint8_t*)"STATE_IDLE ENTERED\n");
		if (ch == MOTOR_ONE)
			state = ST_ONE;
		else if (ch == MOTOR_TWO)
			state = ST_TWO;
		else
			state = ST_POS1;
		break;
	case ST_ONE:
		uart_tx_str((uint8_t*)"STATE_MOTOR_ONE ENTERED\n");
		if (ch == 0)
			m_one_ctrl(M_ONE_STOP, 0);
		else if (ch > 0)
			m_one_ctrl(M_ONE_FWD, ch);
		else if (ch < 0)
			m_one_ctrl(M_ONE_REV, ch);
		state = ST_IDLE;
		break;
	case ST_TWO:
		uart_tx_str((uint8_t*)"STATE_MOTOR_TWO ENTERED\n");
		if (ch == 0)
			m_two_ctrl(M_TWO_STOP, 0);
		else if (ch > 0)
			m_two_ctrl(M_TWO_FWD, ch);
		else if (ch < 0)
			m_two_ctrl(M_TWO_REV, ch);
		else
			state = ST_POS2;
		state = ST_IDLE;
		break;
	case ST_POS1:
		uart_tx_str((uint8_t*)"STATE_FAIL_POS1\n");
		state = ST_IDLE;
		break;
	case ST_POS2:
		uart_tx_str((uint8_t*)"STATE_FAIL_POS2\n");
		state = ST_IDLE;
		break;
	}
}
void motion_control(int8_t* data, uint8_t len){
	//for (uint8_t i = 0; i < len; i++){
		//state_machine(data[i]);
	//}
	int8_t left = data[0];
	int8_t right = data[1];
	m_one_drive(right);
	m_two_drive(left);
}

