/*
 * motorCTRL.c
 *
 *  Created on: 08/08/2011
 *      Author: Morten Ege Jensen
 */


#include <avr/io.h>
#include <stdint.h>
#include "motorCTRL.h"


/* MOTOR ONE CTRL FUNCTIONS*/
void init_m_one(void){
	// Enable PWM PIN
	M_ONE_DDR |= (1<<M_ONE_PIN);
	// Enable Enable PIN
	M_ONE_EN_DDR |= (1<<M_ONE_EN_PIN);
	// Set Enable PIN
	m_one_en(1);
	M_ONE_PWM(0);
	M_ONE_DIR(0);
}

void m_one_ctrl(uint8_t dir, int8_t speed){
	if (dir == M_ONE_FWD){
		m_one_en(1);
		M_ONE_PORT &= ~(1<<M_ONE_PIN);
		M_ONE_DIR(dir);
		M_ONE_PWM(speed);
	}else if (dir == M_ONE_REV){
		m_one_en(1);
		M_ONE_PORT |= (1<<M_ONE_PIN);
		M_ONE_DIR(dir);
		M_ONE_PWM(speed);
	}else{
		m_one_en(0);
		M_ONE_PORT &= ~(1<<M_ONE_PIN);
		M_ONE_PWM(speed);
	}
}

void m_one_en(uint8_t on){
	if (on){
		M_ONE_EN_PORT |= (1<<M_ONE_EN_PIN);
	}else{
		M_ONE_EN_PORT &= ~(1<<M_ONE_EN_PIN);
	}
}

/* MOTOR TWO CTRL FUNCTIONS*/
void init_m_two(void){
	// Enable PWM PIN
	M_TWO_DDR |= (1<<M_TWO_PIN);
	// Enable Enable PIN
	M_TWO_EN_DDR |= (1<<M_TWO_EN_PIN);
	// Set Enable PIN
	m_two_en(1);
	M_TWO_PWM(0);
	M_TWO_DIR(0);
}

void m_two_ctrl(uint8_t dir, int8_t speed){
	if (dir == M_TWO_FWD){
		m_two_en(1);
		M_TWO_PORT &= ~(1<<M_TWO_PIN);
		M_TWO_DIR(dir);
		M_TWO_PWM(speed);
	}else if (dir == M_TWO_REV){
		m_two_en(1);
		M_TWO_PORT |= (1<<M_TWO_PIN);
		M_TWO_DIR(dir);
		M_TWO_PWM(speed);
	}else{
		m_two_en(0);
		M_TWO_PORT &= ~(1<<M_TWO_PIN);
		M_TWO_PWM(speed);
	}
}

void m_two_en(uint8_t on){
	if (on){
		M_TWO_EN_PORT |= (1<<M_TWO_EN_PIN);
	}else{
		M_TWO_EN_PORT &= ~(1<<M_TWO_EN_PIN);
	}
}

// NEW COMMANDS
void m_one_drive(int8_t speed){
	if (speed > 0){
		//FWD_DRIVE
		m_one_en(1);
		M_ONE_PORT &= ~(1<<M_ONE_PIN);
		M_ONE_DIR(M_ONE_FWD);
		M_ONE_PWM(speed);
	}else if (speed < 0){
		//REV DRIVE
		m_one_en(1);
		// Flip for two's compliment
		speed = ~speed;
		speed++;
		M_ONE_PORT |= (1<<M_ONE_PIN);
		M_ONE_DIR(M_ONE_REV);
		M_ONE_PWM(speed);
	}else{
		m_one_en(0);
		M_ONE_PORT &= ~(1<<M_ONE_PIN);
		M_ONE_PWM(speed);
	}
}
void m_two_drive(int8_t speed){
	if (speed > 0){
		//FWD_DRIVE
		m_two_en(1);
		M_TWO_PORT &= ~(1<<M_TWO_PIN);
		M_TWO_DIR(M_TWO_FWD);
		M_TWO_PWM(speed);
	}else if (speed < 0){
		//REV DRIVE
		m_two_en(1);
		// Flip for two's compliment
		speed = ~speed;
		speed++;
		M_TWO_PORT |= (1<<M_TWO_PIN);
		M_TWO_DIR(M_TWO_REV);
		M_TWO_PWM(speed);
	}else{
		m_two_en(0);
		M_TWO_PORT &= ~(1<<M_TWO_PIN);
		M_TWO_PWM(speed);
	}
}
