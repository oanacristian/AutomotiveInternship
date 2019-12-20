/*
 * clock_time.h
 *
 *  Created on: Dec 19, 2019
 *      Author: egon.osvath
 */

#ifndef CLOCK_TIME_H_
#define CLOCK_TIME_H_

#define get_clocks_in_milliseconds_80MHZ(milliseconds) ((milliseconds)*(40000U))
#define get_clocks_in_microseconds_80MHZ(microseconds) ((microseconds)*(40U))

#define get_clocks_in_milliseconds_8MHZ(milliseconds) ((milliseconds)*(4000U))
#define get_clocks_in_microseconds_8MHZ(microseconds) ((microseconds)*(4U))


#endif /* CLOCK_TIME_H_ */
