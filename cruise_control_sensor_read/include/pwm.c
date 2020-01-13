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
	if(counter<1000)
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

void forward_backward_stay_PWM()
{
	static int counter = 0;
	if(counter<750)
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

void motor_backward_PWM()
{
	static int counter = 0;
	if(counter<500)
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

void left_right_stay_PWM()
{
	static int counter = 0;
	if(counter<3)
	{
		 PTD->PSOR |= 1<<LEFT_RIGHT_PORT;
	}
	else
	{
		 PTD->PCOR |= 1<<LEFT_RIGHT_PORT;
	}
	counter++;
	counter%=32;
}

void motor_left_PWM()
{
	static int counter = 0;
	if(counter<4)
	{
		 PTD->PSOR |= 1<<LEFT_RIGHT_PORT;
	}
	else
	{
		 PTD->PCOR |= 1<<LEFT_RIGHT_PORT;
	}
	counter++;
	counter%=32;
}

void motor_right_PWM()
{
	static int counter = 0;
	if(counter<2)
	{
		 PTD->PSOR |= 1<<LEFT_RIGHT_PORT;
	}
	else
	{
		 PTD->PCOR |= 1<<LEFT_RIGHT_PORT;
	}
	counter++;
	counter%=32;
}
