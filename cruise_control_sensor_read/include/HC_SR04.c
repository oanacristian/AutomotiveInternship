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


void read_distance()
{
	unsigned long start_clock = 0;
	unsigned long end_clock = 0;
	unsigned long long time = 0;

	PTD->PSOR |= 1<<TRIG_PORT;
	start_clock = LPIT0->TMR[0].CVAL;
	while(LPIT0->TMR[0].CVAL > start_clock - get_clocks_in_microseconds_80MHZ(10))
	{
		if( LPIT0->TMR[0].CVAL > start_clock)
		{
			return;
		}
	}
	PTD->PCOR |= 1<<TRIG_PORT;
	start_clock = LPIT0->TMR[0].CVAL;
	while(!((PTD->PDIR & (1<<ECHO_PORT))?1:0) ||  LPIT0->TMR[0].CVAL > start_clock);
	start_clock = LPIT0->TMR[0].CVAL;
	while((PTD->PDIR & (1<<ECHO_PORT))?1:0 ||  LPIT0->TMR[0].CVAL > start_clock);
	end_clock = LPIT0->TMR[0].CVAL;
	time = start_clock - end_clock;
	time /= get_clocks_in_microseconds_80MHZ(58); // get time in microseconds
	last_distance_in_CMs = time;

}
