/*
 * motorCTRL.h
 *
 *  Created on: 08/08/2011
 *      Author: Morten Ege Jensen
 */

#ifndef MOTORCTRL_H_
#define MOTORCTRL_H_

#include <avr/io.h>
#include <stdint.h>
#include "../PWM/pwm.h"

#define M_STATE_IDLE 	0
#define M_STATE_REC 	5
#define M_STATE_M1 		1
#define M_STATE_M1_RX 	2
#define M_STATE_M2 		3
#define M_STATE_M2_RX 	4

/* Defines for MOTOR ONE*/
#define M_ONE_FWD	0
#define M_ONE_REV	1
#define M_ONE_STOP	2

#define M_ONE_PWM(sp) set_pwm_B(sp)
#define M_ONE_DIR(dir) set_pwm_B_dir(dir)

#define M_ONE_PORT PORTB
#define M_ONE_DDR DDRB
#define M_ONE_PIN PB3

#define M_ONE_EN_PORT PORTB
#define M_ONE_EN_DDR DDRB
#define M_ONE_EN_PIN PB4

/* Functions for MOTOR ONE*/
void init_m_one(void);
void m_one_ctrl(uint8_t, int8_t);
void m_one_en(uint8_t);

/* Defines for MOTOR TWO*/
#define M_TWO_FWD	0
#define M_TWO_REV	1
#define M_TWO_STOP	2

#define M_TWO_PWM(sp) set_pwm_A(sp)
#define M_TWO_DIR(dir) set_pwm_A_dir(dir)

#define M_TWO_PORT PORTC
#define M_TWO_DDR DDRC
#define M_TWO_PIN PC0

#define M_TWO_EN_PORT PORTC
#define M_TWO_EN_DDR DDRC
#define M_TWO_EN_PIN PC1

/* Functions for MOTOR TWO*/
void init_m_two(void);
void m_two_ctrl(uint8_t, int8_t);
void m_two_en(uint8_t);

//NEW COMMANDS
void m_one_drive(int8_t);
void m_two_drive(int8_t);
// This is the one thats working
void motion_control(int8_t* data, uint8_t len);
#endif /* MOTORCTRL_H_ */
