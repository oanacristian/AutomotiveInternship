/* FTM.h              (c) 2016 NXP Semiconductor, Inc.
 * Descriptions: FTM example code.
 * 03 Jun 2016  Osvaldo Romero: Initial version
 */

#ifndef FTM_H_
#define FTM_H_

void FTM0_init(void);
void FTM0_CH0_OC_init(void);
void FTM0_CH1_PWM_init(void);
void FTM0_CH6_IC_init(void);
void FTM0_CH0_output_compare(void);
void FTM0_CH6_input_capture(void);
void start_FTM0_counter (void);
#endif /* FTM_H_ */
