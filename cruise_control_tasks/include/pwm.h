/*
 * pwm.h
 *
 *  Created on: Dec 19, 2019
 *      Author: egon.osvath
 */

#ifndef PWM_H_
#define PWM_H_

int FORWARD_BACKWARD_PORT;
int LEFT_RIGHT_PORT;

void pwm_forward_backward_init(int);

void pwm_left_right_init(int);

void motor_forward_PWM();

void motor_backward_PWM();

void motor_left_PWM();

void motor_right_PWM();

void forward_backward_stay_PWM();

void  left_right_stay_PWM();


#endif /* PWM_H_ */
