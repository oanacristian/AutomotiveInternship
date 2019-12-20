/*
 * HC_SR04.c
 *
 *  Created on: Dec 18, 2019
 *      Author: egon.osvath
 */
#include "S32K144.h"
#include "HC_SR04.h"
#include "clock_time.h"

void init_ultrasonic_sensor(int trigger, int echo)
{
	TRIG_PORT = trigger;
	ECHO_PORT = echo;
}

short positive(short x)
{
	if(x<0)
	{
		return -x;
	}
	return x;
}

void read_distance()
{
	unsigned long start_clock = 0;
	unsigned long end_clock = 0;
	unsigned long long time = 0;

	PTD->PSOR |= 1<<TRIG_PORT;
	start_clock = FTM0->CNT;
	while(FTM0->CNT < start_clock + get_clocks_in_microseconds_8MHZ(10))
	{
		if( FTM0->CNT < start_clock || FTM0->CNT > start_clock + get_clocks_in_milliseconds_8MHZ(65))
		{
			return;
		}
	}
	PTD->PCOR |= 1<<TRIG_PORT;
	start_clock = FTM0->CNT;
	while(!((PTD->PDIR & (1<<ECHO_PORT))?1:0))
	{
		if(FTM0->CNT < start_clock || FTM0->CNT > start_clock + get_clocks_in_milliseconds_8MHZ(65))
		{
			return;
		}
	}
	start_clock = FTM0->CNT;
	while((PTD->PDIR & (1<<ECHO_PORT))?1:0)
	{
		if(FTM0->CNT < start_clock || FTM0->CNT > start_clock + get_clocks_in_milliseconds_8MHZ(65))
		{
			return;
		}
	}
	end_clock = FTM0->CNT;
	time = end_clock - start_clock;
	time /= get_clocks_in_microseconds_8MHZ(1); // get time in microseconds
	last_distance_in_CMs = time;

}
