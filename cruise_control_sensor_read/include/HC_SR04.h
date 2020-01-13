/*
 * HC_SR04.h
 *
 *  Created on: Dec 18, 2019
 *      Author: egon.osvath
 */

#ifndef HC_SR04_H_
#define HC_SR04_H_

short positive(short x);


	int TRIG_PORT;
	int ECHO_PORT;
	short last_distance_in_CMs;

void init_ultrasonic_sensor(int, int);

void read_distance();

#endif /* HC_SR04_H_ */
