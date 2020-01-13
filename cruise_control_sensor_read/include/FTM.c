/* FTM.c              (c) 2016 NXP Semiconductor, Inc.
 * Descriptions: FTM example code.
 * 2016 Jun 03  Osvaldo Romero: Initial version
 * 2016 Oct 31 SM: Updated for new header symbols for PCCn
 */

#include "S32K144.h" /* include peripheral declarations S32K144 */
#include "FTM.h"

uint16_t CurrentCaptureVal = 0;
uint16_t PriorCaptureVal = 0;
uint16_t DeltaCapture = 0;

void FTM0_init(void) {
  PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config */
  PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(0b001)	/* Clock Src=1, 8 MHz SOSCDIV1_CLK */
                                |  PCC_PCCn_CGC_MASK;   /* Enable clock for FTM regs */
  FTM0->MODE |= FTM_MODE_WPDIS_MASK;  /* Write protect to registers disabled (default) */
  FTM0->SC = 0x00030007;     /* Enable PWM channel 0 output*/
                             /* Enable PWM channel 1 output*/
                             /* TOIE (Timer Overflow Interrupt Ena) = 0 (default) */
                             /* CPWMS (Center aligned PWM Select) = 0 (default, up count) */
                             /* CLKS (Clock source) = 0 (default, no clock; FTM disabled) */
                             /* PS (Prescaler factor) = 7. Prescaler = 128 */
  FTM0->COMBINE = 0x00000000;/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0  */
  FTM0->POL = 0x00000000;    /* Polarity for all channels is active high (default) */
  FTM0->MOD = 62500 -1 ;     /* FTM1 counter final value (used for PWM mode) */
                             /* FTM1 Period = MOD-CNTIN+0x0001 ~= 62500 ctr clks  */
                             /* 8MHz /128 = 62.5kHz ->  ticks -> 1Hz */
}

void FTM0_CH0_OC_init(void) {
  FTM0->CONTROLS[0].CnSC = 0x00000014; /* FTM0 ch0: Output Compare, toggle output on match */
                                       /* CHIE (Chan Interrupt Ena)= 0 (default) */
                                       /* MSB:MSA (chan Mode Select)= 0b01, Output Compare */
                                       /* ELSB:ELSA (chan Edge or Level Select)= 0b01, toggle*/
  FTM0->CONTROLS[0].CnV= 6250;         /* FTM0 ch 0 Compare Value= 6250  clks, 100ms toggle*/
  FTM0->POL &= ~FTM_POL_POL0_MASK;     /* FTM0 ch 0 polarity = 0 (Default, active high) */
}

void FTM0_CH1_PWM_init(void) {
  FTM0->CONTROLS[1].CnSC = 0x00000028;  /* FTM0 ch1: edge-aligned PWM, low true pulses */
                                        /* CHIE (Chan Interrupt Ena) = 0 (default) */
                                        /* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM*/
                                        /* ELSB:ELSA (chan Edge/Level Select)=0b10, low true */
  FTM0->CONTROLS[1].CnV =  46875;       /* FTM0 ch1 compare value (~75% duty cycle) */
}

void FTM0_CH6_IC_init(void) {
  FTM0->CONTROLS[6].CnSC = 0x0000000C;  /* FTM0 ch6: Input Capture rising or falling edge */
                                        /* CHIE (Chan Interrupt Ena) = 0 (default) */
                                        /* MSB:MSA (chan Mode Select)=0b00, Input Capture */
                                        /* ELSB:ELSA (ch Edge/Level Select)=0b11, rise or fall*/
}

void FTM0_CH0_output_compare(void) {
  if (1==((FTM0->CONTROLS[0].CnSC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT)) {
	                                             /* If chan flag is set */
  FTM0->CONTROLS[0].CnSC &= ~FTM_CnSC_CHF_MASK;  /* Clear flag: read reg then set CHF=0 */
    if(  FTM0->CONTROLS[0].CnV==56250)  {        /* If count at last value before end, */
      FTM0->CONTROLS[0].CnV= 0 ;                 /* Update compare value: to 0*/
    }
    else {
      FTM0->CONTROLS[0].CnV= FTM0->CONTROLS[0].CnV + 6250 ;
                                      /* Update compare value: add 6250 to current value*/
      }
  }
}

void FTM0_CH6_input_capture(void) {
  if (1==((FTM0->CONTROLS[6].CnSC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT)) {
	                                               /* If chan flag is set */
  FTM0->CONTROLS[6].CnSC &= ~FTM_CnSC_CHF_MASK;    /* Clear flag: read reg then set CHF=0 */
  PriorCaptureVal = CurrentCaptureVal;             /* Record value of prior capture */
  CurrentCaptureVal = FTM0->CONTROLS[6].CnV;       /* Record value of current capture */
  DeltaCapture = CurrentCaptureVal - PriorCaptureVal;
                                  /* Will be 6250 clocks (100 msec) if connected to FTM0 CH0 */
  }
}

void start_FTM0_counter (void) {
  FTM0->SC |= FTM_SC_CLKS(3);
                      /* Start FTM0 counter with clk source = external clock (SOSCDIV1_CLK)*/
}


