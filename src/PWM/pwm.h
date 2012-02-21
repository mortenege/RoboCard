/*
 * pwm_control.h
 *
 *  Created on: 05/08/2011
 *      Author: Morten Ege Jensen
 */

#ifndef PWM_CONTROL_H_
#define PWM_CONTROL_H_
#include <stdint.h>
void init_pwm(void);
void set_pwm_B(uint8_t);
void set_pwm_A(uint8_t);

void set_pwm_A_dir(uint8_t);
void set_pwm_B_dir(uint8_t);

#endif /* PWM_CONTROL_H_ */
