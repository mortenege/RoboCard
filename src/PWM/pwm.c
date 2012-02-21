/*
 * pwm_control.c
 *
 *  Created on: 05/08/2011
 *      Author: Morten Ege Jensen
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "../PWM/pwm.h"

#define PWM_COUNT 1000

/**
 * DEPRECATED
 */
void init_pwm_isr(void){
	//TIMSK1 = (1<<OCIE1A)|(1<<TOIE1);
}

void init_pwm(void){
	//init_pwm_isr();
	TCCR1A = (1<<WGM11); /* fast 10-bit PWM top at ICR1*/
	TCCR1B = (1<<WGM13)|(1<<WGM12);
	ICR1 = PWM_COUNT;
	set_pwm_A_dir(0);
	set_pwm_B_dir(0);

	TCCR1B |= (1<<CS10);	/* Prescale 1/1 */
	set_pwm_A(50);
	set_pwm_B(25);

	DDRB |= (1<<PB1); /* Remember to set PIN as OUTPUT */
	DDRB |= (1<<PB2); /* Remember to set PIN as OUTPUT */
}

void set_pwm_A(uint8_t pwm){
	uint16_t temp = 0;
	if (pwm < 0){
		pwm = 0;
	}
	else if (pwm > 100){
		pwm = 100;
	}
	temp = PWM_COUNT/100*pwm;
	OCR1A = temp;
	//OCR1A = pwm;
}

void set_pwm_B(uint8_t pwm){
	uint16_t temp = 0;
	if (pwm < 0){
		pwm = 0;
	}
	else if (pwm > 100){
		pwm = 100;
	}
	temp = PWM_COUNT/100*pwm;
	OCR1B = temp;
	//OCR1B = pwm;
}

void set_pwm_A_dir(uint8_t dir){
	if (dir==0){
		/* Clear OC1A on compare match, set OC1A on BOTTOM */
		TCCR1A |=(1<<COM1A1);
		TCCR1A &= ~(1<<COM1A0);
	}else if(dir==1){
		/* Set OC1A on compare match, CLEAR OC1A on BOTTOM */ //INVERTED
		TCCR1A |=(1<<COM1A0);
		TCCR1A |=(1<<COM1A1);
	}else{
		// STOP MOTOR
	}
}

void set_pwm_B_dir(uint8_t dir){
	if (dir==0){
		/* Clear OC1B on compare match, set OC1B on BOTTOM */
		TCCR1A |=(1<<COM1B1);
		TCCR1A &= ~(1<<COM1B0);
	}else if (dir==1){
		/* Set OC1B on compare match, CLEAR OC1B on BOTTOM */ //INVERTED
		TCCR1A |=(1<<COM1B0);
		TCCR1A |=(1<<COM1B1);
	}else{
		//STOP MOTOR
	}
}


/*
ISR (TIMER0_COMPA_vect){
	//PORTB |= (1 << PB0);
}

ISR (TIMER0_OVF_vect){
	//PORTB &= ~(1 << PB0);
}
*/

