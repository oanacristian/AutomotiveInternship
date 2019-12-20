/*
 * pwm.c
 *
 *  Created on: Dec 19, 2019
 *      Author: egon.osvath
 */

#include "S32K144.h"
#include "pwm.h"

void pwm_forward_backward_init(int port)
{
	FORWARD_BACKWARD_PORT = port;
}

void pwm_left_right_init(int port)
{
	LEFT_RIGHT_PORT = port;
}

void motor_forward_PWM()
{
	static int counter = 0;
	if(counter<350)
	{
		 PTD->PSOR |= 1<<FORWARD_BACKWARD_PORT;
	}
	else
	{
		 PTD->PCOR |= 1<<FORWARD_BACKWARD_PORT;
	}
	counter+=10;
	counter%=5000;
}

void motor_backward_PWM()
{
	static int counter = 0;
	if(counter<650)
	{
		 PTD->PSOR |= 1<<FORWARD_BACKWARD_PORT;
	}
	else
	{
		 PTD->PCOR |= 1<<FORWARD_BACKWARD_PORT;
	}
	counter+=10;
	counter%=5000;
}

void motor_left_PWM()
{
	static int counter = 0;
	if(counter<280)
	{
		 PTD->PSOR |= 1<<LEFT_RIGHT_PORT;
	}
	else
	{
		 PTD->PCOR |= 1<<LEFT_RIGHT_PORT;
	}
	counter+=10;
	counter%=5000;
}

void motor_right_PWM()
{
	static int counter = 0;
	if(counter<650)
	{
		 PTD->PSOR |= 1<<LEFT_RIGHT_PORT;
	}
	else
	{
		 PTD->PCOR |= 1<<LEFT_RIGHT_PORT;
	}
	counter+=10;
	counter%=5000;
}
